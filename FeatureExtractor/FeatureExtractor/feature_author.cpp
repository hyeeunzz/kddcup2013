// Feature.id : 1xx
#include "feature_author.h"

using std::vector;
using std::string;

// Levenshtetin distance between author's affiliation and paper_author's affiliation
Feature generateAuthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		PaperAuthor *author2 = paper_authors[i];
		if (strlen(author->affiliation) > 0 && strlen(author2->affiliation) > 0) {
			string author_affiliation(author->affiliation);
			string author2_affiliation(author2->affiliation);
			stringToLower(author_affiliation);
			stringToLower(author2_affiliation);
			distances.push_back(levenshteinDistance(author_affiliation, author2_affiliation));
			count++;
		}
	}

	if (count > 0){
		return Feature(101, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(101, MAGIC_NUMBER);
	}
}

// Levenshtetin distance between author's affiliation and coauthor's affiliation
Feature generateCoauthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id){
	vector<PaperAuthor*> paper_authors;
	Author *author = db->getAuthorById(author_id);

	vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (strlen(author->affiliation) > 0 && strlen(coauthor->affiliation) > 0) {
			string author_affiliation(author->affiliation);
			string coauthor_affiliation(coauthor->affiliation);
			stringToLower(author_affiliation);
			stringToLower(coauthor_affiliation);
			distances.push_back(levenshteinDistance(author_affiliation, coauthor_affiliation));
			count++;
		}
	}

	if (count > 0) {
		return Feature(102, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(102, MAGIC_NUMBER);
	}
}

// Levenshtetin distance between author's name and paper_author's name
Feature generateAuthorNameLevenshteinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	vector<double> distances;
	int count = 0;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	for (size_t i = 0; i < paper_authors.size(); i++){
		PaperAuthor *author2 = paper_authors[i];
		if (strlen(author->name) > 0 && strlen(author2->name) > 0) {
			string name(author->name);
			string name2(author2->name);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(levenshteinDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(103, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(103, MAGIC_NUMBER);
	}
}

Feature generateAuthorNameLevenstheinDistanceFeature2(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	vector<double> distances;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++){
		string name(author->name);
		string name2(paper_authors[i]->name);
		// Compare Abbreviation
	}

	return Feature(104, 0);
}

void generateAuthorFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generateAuthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateCoauthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenstheinDistanceFeature2(db, author_id, paper_id));
}
