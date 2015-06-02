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
		return Feature(101, *min_element(distances.begin(), distances.end()));
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
		return Feature(102, *min_element(distances.begin(), distances.end()));
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
		return Feature(103, *min_element(distances.begin(), distances.end()));
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

// Levenshtetin distance between author's name and coauthor's name
Feature generateAuthorCoauthorNameLevstheinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	vector<double> distances;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++){
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (strlen(author->name) > 0 && strlen(coauthor->name) > 0) {
			string name(author->name);
			string name2(coauthor->name);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(levenshteinDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(105, *min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(105, MAGIC_NUMBER);
	}
}

Feature generateAuthorCoauthorLastNameLevestheinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	vector<double> distances;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++){
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (strlen(author->name) > 0 && strlen(coauthor->name) > 0) {
			// Compare Lastname
			int count2 = 0;
			char lastname[96];
			char lastname2[96];
			for (size_t j = 0; j < strlen(author->name); j++){
				if (author->name[j] == NULL) {
					lastname[count2] = NULL;
					break;
				}
				else if (author->name[j] == ' ') count2 = 0;
				else {
					lastname[count2] = author->name[j];
					count2++;
				}
			}
			for (size_t j = 0; j < strlen(coauthor->name); j++){
				if (coauthor->name[j] == NULL) {
					lastname2[count2] = NULL;
					break;
				}
				else if (coauthor->name[j] == ' ') count2 = 0;
				else {
					lastname2[count2] = coauthor->name[j];
					count2++;
				}
			}
			string name(lastname);
			string name2(lastname2);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(levenshteinDistance(lastname, lastname2));
			count++;
		}
	}

	if (count > 0){
		return Feature(106, *min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(106, MAGIC_NUMBER);
	}
}

void generateAuthorFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generateAuthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateCoauthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenstheinDistanceFeature2(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorNameLevstheinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorLastNameLevestheinDistanceFeature(db, author_id, paper_id));
}
