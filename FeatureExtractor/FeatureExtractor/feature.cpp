#include "feature.h"
#include "feature_author.h"
#include "feature_paper.h"
#include "feature_other.h"
#include <iostream>
using namespace std;

bool file_exists(const char *filepath)
{
	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filepath, "r");
	if (err == 0) {
		fclose(fp);
	}
	if (err == 2) return false;
	return true;
}

//void feature_generator(DB *db, Dataset *dataset)
void generateSingleFeature(DB *db, Dataset *dataset, int feature_id, void(*feature_generator)(DB *, Dataset *)) {
	// Check whether the feature file is already generated or not
	char filepath[256];
	sprintf_s(filepath, "%s/%s.feature.%d.csv", db->datapath, dataset->filename, feature_id);
	if (file_exists(filepath)){
		printf("Feature #%d : Already exists\n", feature_id);
	}
	else {
		clock_t start_time = std::clock();
		printf("Feature #%d... ", feature_id);
		errno_t err;
		FILE *fp;
		if ((err = fopen_s(&fp, filepath, "w")) != 0){
			fprintf(stderr, "Error occured while opening %s.feature.%d.csv!\n", dataset->filename);
			exit(1);
		}
		fprintf(fp, "author_id,paper_id,feature\n");
		feature_generator(db, dataset);
		// Handle magic numbers...
		int index = dataset->examples[0]->X.size() - 1;
		double mean = 0.0;
		int count = 0;
		for (size_t j = 0; j < dataset->examples.size(); j++){
			Example *example = dataset->examples[j];
			if (example->X[index].value != MAGIC_NUMBER) {
				mean += example->X[index].value;
				count++;
			}
		}
		mean /= (double)count;
		for (size_t j = 0; j < dataset->examples.size(); j++){
			Example *example = dataset->examples[j];
			if (example->X[index].value == MAGIC_NUMBER) {
				example->X[index].value = mean;
			}
		}
		//////////////////////
		for (int i = 0; i < dataset->examples.size(); i++){
			Example *example = dataset->examples[i];
			fprintf(fp, "%d,%d,%f\n", example->author_id, example->paper_id, example->X[index].value);
		}
		fclose(fp);
		printf("ok (%d ms)\n", std::clock() - start_time);
	}
}

//Feature feature_generator(DB *db, int author_id, int paper_id)
void generateSingleFeature(DB *db, Dataset *dataset, int feature_id, Feature (*feature_generator)(DB *,int,int) ) {
	// Check whether the feature file is already generated or not
	char filepath[256];
	sprintf_s(filepath, "%s/%s.feature.%d.csv", db->datapath, dataset->filename, feature_id);
	if (file_exists(filepath)){
		printf("Feature #%d : Already exists\n", feature_id);
	}
	else {
		clock_t start_time = std::clock();
		printf("Feature #%d... ", feature_id);
		errno_t err;
		FILE *fp;
		if ((err = fopen_s(&fp, filepath, "w")) != 0){
			fprintf(stderr, "Error occured while opening %s.feature.%d.csv!\n", dataset->filename);
			exit(1);
		}
		fprintf(fp, "author_id,paper_id,feature\n");
		for (int i = 0; i < dataset->examples.size(); i++){
			Example *example = dataset->examples[i];
			example->X.push_back(feature_generator(db, example->author_id, example->paper_id));
		}
		// Handle magic numbers...
		int i = dataset->examples[0]->X.size() - 1;
		double mean = 0.0;
		int count = 0;
		for (size_t j = 0; j < dataset->examples.size(); j++){
			Example *example = dataset->examples[j];
			if (example->X[i].value != MAGIC_NUMBER) {
				mean += example->X[i].value;
				count++;
			}
		}
		mean /= (double) count;
		for (size_t j = 0; j < dataset->examples.size(); j++){
			Example *example = dataset->examples[j];
			if (example->X[i].value == MAGIC_NUMBER) {
				example->X[i].value = mean;
			}
		}
		//////////////////////
		for (int i = 0; i < dataset->examples.size(); i++){
			Example *example = dataset->examples[i];
			fprintf(fp, "%d,%d,%f\n", example->author_id, example->paper_id, example->X[example->X.size() - 1].value);
		}
		fclose(fp);
		printf("ok (%d ms)\n", std::clock() - start_time);
	}
}

void generateFeatures(DB *db, Dataset *dataset)
{
	generateAuthorFeatures(db, dataset);
	generatePaperFeatures(db, dataset);
	generateOtherFeatures(db, dataset);
}

Dataset* loadDataset(char *filename, DB *db)
{
	// Load train.csv and make dataset: (X,y) pairs
	Dataset* result = new Dataset();
	result->db = db;
	clock_t start_time = std::clock();
	FILE *fp = getFile(db->datapath, filename);
	sprintf_s(result->filename, filename);

	while (!feof(fp)){
		int author_id = 0;
		int paper_id = 0;
		fscanf_s(fp, "%d,", &author_id);

		char c = 0;
		int state = 0; // 0: confirmed, 1: deleted

		while (true) {
			c = getc(fp);
			if (c == ','){
				if (state == 0){
					state = 1;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", author_id);
					exit(1);
				}
			}
			if (paper_id > 0 && (c == ',' || c == ' ' || c == '\n' || c == EOF)){
				// Generate example
				Example example;
				example.author_id = author_id;
				example.paper_id = paper_id;
				example.y = (state == 0) ? 1 : -1;

				result->examples.push_back(new Example(example));
				paper_id = 0;
			}
			else if (c >= '0' && c <= '9') {
				paper_id = paper_id * 10 + (c - '0');
			}
			if (c == '\n' || c == EOF){
				break;
			}
		}
		if (c == EOF) {
			break;
		}
	}

	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);

	return result;
}
