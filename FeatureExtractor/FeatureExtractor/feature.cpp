#include "feature.h"
#include "feature_author.h"
#include "feature_paper.h"
#include "feature_other.h"
#include <iostream>
using namespace std;

FeatureList generateFeatures(DB *db, int author_id, int paper_id)
{
	FeatureList f;
	generateAuthorFeatures(f, db, author_id, paper_id);
	generatePaperFeatures(f, db, author_id, paper_id);
	generateOtherFeatures(f, db, author_id, paper_id);

	return f;
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
				// Generate feature and example
				FeatureList feature_list = generateFeatures(db, author_id, paper_id);
				Example example;
				example.author_id = author_id;
				example.paper_id = paper_id;
				example.X = feature_list;
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

	// Handle magic numbers...
	int D = result->examples[0]->X.size(); // feature dimension
	for (size_t i = 0; i < D; i++){
		bool magic = false;
		double mean = 0.0;
		int count = 0;
		for (size_t j = 0; j < result->examples.size(); j++){
			Example *example = result->examples[j];
			if (example->X[i].value == MAGIC_NUMBER) {
				magic = true;
			}
			else {
				mean += example->X[i].value;
				count++;
			}
		}
		mean /= (double) count;
		if (magic){
			for (size_t j = 0; j < result->examples.size(); j++){
				Example *example = result->examples[j];
				if (example->X[i].value == MAGIC_NUMBER) {
					example->X[i].value = mean;
				}
			}
		}
	}

	// Check existence of duplicated feature id
	for (int i = 0; i < D; i++){
		for (int j = i + 1; j < D; j++){
			if (result->examples[0]->X[i].id == result->examples[0]->X[j].id) {
				fprintf(stderr, "ERROR - duplicated feature id (%d)!\n", result->examples[0]->X[i].id);
				exit(10);
			}
		}
	}

	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);

	return result;
}

void Dataset::save()
{
	clock_t start_time = std::clock();
	fprintf(stderr, "save %s.feature.csv... ", filename);
	char buf[256];
	errno_t err;

	// {filename}.feature.csv
	sprintf_s(buf, "%s/%s.feature.csv", db->datapath, filename);
	FILE *fp;
	if ((err = fopen_s(&fp, buf, "w")) != 0){
		fprintf(stderr, "Error occured while writing %s.feature.csv!\n", filename);
		exit(1);
	}

	fprintf(fp, "author_id,paper_id");
	for (size_t i = 0; i < examples[0]->X.size(); i++){
		fprintf(fp, ",f%d", examples[0]->X[i].id);
	}
	fprintf(fp, ",target\n");
	for (size_t i = 0; i < examples.size(); i++){
		Example* example = examples[i];
		fprintf(fp, "%d,%d", example->author_id, example->paper_id);
		for (size_t j = 0; j < example->X.size(); j++){
			fprintf(fp, ",%lf", example->X[j].value);
		}
		fprintf(fp, ",%d\n", example->y);
	}
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
}
