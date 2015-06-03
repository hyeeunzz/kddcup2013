// Feature.id : 1xx
#include "feature_author.h"

using std::vector;
using std::string;

// Levenshtein distance between author's affiliation and paper_author's affiliation
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
		if (author->affiliation.length() > 0 && author2->affiliation.length() > 0) {
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

// Levenshtein distance between author's affiliation and coauthor's affiliation
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
		if (author->affiliation.length() > 0 && coauthor->affiliation.length() > 0) {
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

// Levenshtein distance between author's name and paper_author's name
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
		if (author->name.length() > 0 && author2->name.length() > 0) {
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

// Levenshtein distance between abbrevation of author's name and paper_author's name
Feature generateAuthorNameLevenstheinDistanceFeature2(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);
	vector<double> distances;

	int count = 0;
	int count2 = 0;
	char abbname[96];
	char abbname2[96];
	bool first = true;

	if (author->name.length() > 0) {
		for (size_t j = 0; j < author->name.length(); j++){
			if (author->name[j] == NULL) {
				abbname[count2] = NULL;
				break;
			}
			else if (author->name[j] == ' ') {
				count++;
				first = true;
			}
			else {
				if (first) {
					abbname[count2] = author->name[j];
					count2++;
					first = false;
				}
			}
		}
	}
	string name(abbname);
	
	for (size_t i = 0; i < paper_authors.size(); i++){
		PaperAuthor *author2 = paper_authors[i];
		if (author2->name.length() > 0) {
			count2 = 0;
			for (size_t j = 0; j < author2->name.length(); j++){
				if (author2->name[j] == NULL) {
					abbname2[count2] = NULL;
					break;
				}
				else if (author2->name[j] == ' ') {
					count++;
					first = true;
				}
				else {
					if (first) {
						abbname2[count2] = author2->name[j];
						count2++;
						first = false;
					}
				}
			}
			string name2(abbname2);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(levenshteinDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(104, *min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(104, MAGIC_NUMBER);
	}
}

// Levenshtein distance between author's name and coauthor's name
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
		if (author->name.length() > 0 && coauthor->name.length() > 0) {
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

// Levenshtein distance between author's last name and coauthor's last name
Feature generateAuthorCoauthorLastNameLevestheinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
	vector<double> distances;

	int count = 0;
	int count2 = 0;
	char lastname[96];
	char lastname2[96];
	if (author->name.length() > 0) {
		for (size_t j = 0; j < author->name.length(); j++){
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
	}
	for (size_t i = 0; i < paper_authors.size(); i++){
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (coauthor->name.length() > 0) {
			count2 = 0;
			for (size_t j = 0; j < coauthor->name.length(); j++){
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
			distances.push_back(levenshteinDistance(name, name2));
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

// The number of authors having the same name of the author's name
Feature generateNumberofSameNameAuthors(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;

	int count = 0;
	for (size_t i = 0; i < db->authors.size(); i++) {
		if (author->name == db->authors[i]->name) count++;
	}
	for (size_t i = 0; i < db->paper_authors.size(); i++) {
		if (author->name == paper_authors[i]->name) count++;
	}
	
	return Feature(107, count);
}

Feature generateTotalNumberofCoauthors(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	return Feature(108, paper_authors.size());
}

Feature generateAverageNumberofPapersofCoauthor(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++) {
		vector<PaperAuthor*> paper_authors2;
		db->getPaperAuthorsByAuthorId(paper_authors2, paper_authors[i]->author_id);
		count += paper_authors2.size();
	}

	return Feature(109, count/paper_authors.size());
}

Feature generateNumberofPapersofAuthor(DB *db, int author_id, int paper_id)
{
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, author_id);

	return Feature(110, paper_authors.size());
}

void generateAuthorFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	//f.push_back(generateAuthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateCoauthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	//f.push_back(generateAuthorNameLevenshteinDistanceFeature(db, author_id, paper_id));
	//f.push_back(generateAuthorNameLevenstheinDistanceFeature2(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorNameLevstheinDistanceFeature(db, author_id, paper_id));
	//f.push_back(generateAuthorCoauthorLastNameLevestheinDistanceFeature(db, author_id, paper_id));
	//f.push_back(generateNumberofSameNameAuthors(db, author_id, paper_id));
	//f.push_back(generateTotalNumberofCoauthors(db, author_id, paper_id));
	f.push_back(generateAverageNumberofPapersofCoauthor(db, author_id, paper_id));
	//f.push_back(generateNumberofPapersofAuthor(db, author_id, paper_id));
}
