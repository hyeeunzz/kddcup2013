#include "feature.h"
#include <iostream>
using namespace std;

#define MAGIC_NUMBER 856362

// Levenshtetin distance between author's affiliation and coauthor's affiliation
Feature generateCoauthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id){
	std::vector<PaperAuthor*> paper_authors;
	Author *author = db->getAuthorById(author_id);

	std::vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (strlen(author->affiliation) > 0 && strlen(coauthor->affiliation) > 0) {
			std::string author_affiliation(author->affiliation);
			std::string coauthor_affiliation(coauthor->affiliation);
			stringToLower(author_affiliation);
			stringToLower(coauthor_affiliation);
			distances.push_back(levenshteinDistance(author_affiliation, coauthor_affiliation));
			count++;
		}
	}

	if (count > 0) {
		return *std::max_element(distances.begin(), distances.end());
	}
	else {
		return MAGIC_NUMBER;
	}
}

// Levenshtetin distance between author's affiliation and paper_author's affiliation
Feature generateAuthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id){
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	std::vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		PaperAuthor *author2 = paper_authors[i];
		if (strlen(author->affiliation) > 0 && strlen(author2->affiliation) > 0) {
			std::string author_affiliation(author->affiliation);
			std::string author2_affiliation(author2->affiliation);
			stringToLower(author_affiliation);
			stringToLower(author2_affiliation);
			distances.push_back(levenshteinDistance(author_affiliation, author2_affiliation));
			count++;
		}
	}

	if (count > 0){
		return *std::max_element(distances.begin(), distances.end());
	}
	else {
		return MAGIC_NUMBER;
	}
}

// Levenshtetin distance between author's name and paper_author's name
Feature generateAuthorNameLevenshteinDistanceFeature(DB *db, int author_id, int paper_id){
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	std::vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		PaperAuthor *author2 = paper_authors[i];
		if (strlen(author->affiliation) > 0 && strlen(author2->affiliation) > 0) {
			std::string name(author->name);
			std::string name2(author2->name);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(levenshteinDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return *std::max_element(distances.begin(), distances.end());
	}
	else {
		return MAGIC_NUMBER;
	}
}

Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id) {
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	return paper_authors.size();
}

FeatureList generateFeatures(DB *db, int author_id, int paper_id)
{
	FeatureList f;
	f.push_back(generateAuthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	//f.push_back(generateCoauthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generatePaperAuthorCountFeature(db, author_id, paper_id));

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
			if (example->X[i] == MAGIC_NUMBER) {
				magic = true;
			}
			else {
				mean += example->X[i];
				count++;
			}
		}
		mean /= (double) count;
		if (magic){
			for (size_t j = 0; j < result->examples.size(); j++){
				Example *example = result->examples[j];
				if (example->X[i] == MAGIC_NUMBER) {
					example->X[i] = mean;
				}
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

	for (size_t i = 0; i < examples.size(); i++){
		Example* example = examples[i];
		fprintf(fp, "%d,%d", example->author_id, example->paper_id);
		for (size_t j = 0; j < example->X.size(); j++){
			fprintf(fp, ",%lf", example->X[j]);
		}
		fprintf(fp, ",%d\n", example->y);
	}
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
}
