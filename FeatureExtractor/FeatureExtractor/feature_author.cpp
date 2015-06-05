// Feature.id : 1xx
#include "feature_author.h"

using std::vector;
using std::string;

// 101. Levenshtein distance between author's affiliation and paper_author's affiliation
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
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

// 102. Levenshtein distance between author's affiliation and coauthor's affiliation
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
		return Feature(*min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
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
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

// Levenshtein distance between abbrevation of author's name and paper_author's name
Feature generateAuthorAbbreviatedNameLevenstheinDistanceFeature(DB *db, int author_id, int paper_id)
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
				abbname2[count2] = author->name[j];
				count2++;
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
		if (author->name.length() > 0 && author2->name.length() > 0) {
			count2 = 0;
			first = true;
			for (size_t j = 0; j < author2->name.length(); j++){
				if (author2->name[j] == NULL) {
					abbname2[count2] = NULL;
					break;
				}
				else if (author2->name[j] == ' ') {
					abbname2[count2] = author2->name[j];
					count2++;
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
		return Feature(*min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
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
		return Feature(*min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorCoauthorAbbreviatedNameLevstheinDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
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
				abbname2[count2] = author->name[j];
				count2++;
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
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (author->name.length() > 0 && coauthor->name.length() > 0) {
			count2 = 0;
			first = true;
			for (size_t j = 0; j < coauthor->name.length(); j++){
				if (coauthor->name[j] == NULL) {
					abbname2[count2] = NULL;
					break;
				}
				else if (coauthor->name[j] == ' ') {
					abbname2[count2] = coauthor->name[j];
					count2++;
					first = true;
				}
				else {
					if (first) {
						abbname2[count2] = coauthor->name[j];
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
		return Feature(*min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
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
		if (author->name.length() > 0 && coauthor->name.length() > 0) {
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
		return Feature(*min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateTotalNumberofCoauthors(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	if (paper_authors.size() > 4) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
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

	if (count / paper_authors.size() > 40) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
}

Feature generateNumberofPapersofAuthor(DB *db, int author_id, int paper_id)
{
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, author_id);

	if (paper_authors.size() > 1) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
}

Feature generateAuthorAffiliationJaroDistanceFeature(DB *db, int author_id, int paper_id)
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
			distances.push_back(jaroDistance(author_affiliation, author2_affiliation));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateCoauthorAffiliationJaroDistanceFeature(DB *db, int author_id, int paper_id){
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
			distances.push_back(jaroDistance(author_affiliation, coauthor_affiliation));
			count++;
		}
	}

	if (count > 0) {
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorNameJaroDistanceFeature(DB *db, int author_id, int paper_id)
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
			distances.push_back(jaroDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorAbbreviatedNameJaroDistanceFeature(DB *db, int author_id, int paper_id)
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
				abbname2[count2] = author->name[j];
				count2++;
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
		if (author->name.length() > 0 && author2->name.length() > 0) {
			count2 = 0;
			first = true;
			for (size_t j = 0; j < author2->name.length(); j++){
				if (author2->name[j] == NULL) {
					abbname2[count2] = NULL;
					break;
				}
				else if (author2->name[j] == ' ') {
					abbname2[count2] = author2->name[j];
					count2++;
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
			distances.push_back(jaroDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorCoauthorNameJaroDistanceFeature(DB *db, int author_id, int paper_id)
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
			distances.push_back(jaroDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorCoauthorAbbreviatedNameJaroDistanceFeature(DB *db, int author_id, int paper_id)
{
	Author *author = db->getAuthorById(author_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);
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
				abbname2[count2] = author->name[j];
				count2++;
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
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		if (coauthor == NULL || author->id == coauthor->id){
			continue;
		}
		if (author->name.length() > 0 && coauthor->name.length() > 0) {
			count2 = 0;
			first = true;
			for (size_t j = 0; j < coauthor->name.length(); j++){
				if (coauthor->name[j] == NULL) {
					abbname2[count2] = NULL;
					break;
				}
				else if (coauthor->name[j] == ' ') {
					abbname2[count2] = coauthor->name[j];
					count2++;
					first = true;
				}
				else {
					if (first) {
						abbname2[count2] = coauthor->name[j];
						count2++;
						first = false;
					}
				}
			}
			string name2(abbname2);
			stringToLower(name);
			stringToLower(name2);
			distances.push_back(jaroDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

Feature generateAuthorCoauthorLastNameJaroDistanceFeature(DB *db, int author_id, int paper_id)
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
		if (author->name.length() > 0 && coauthor->name.length() > 0) {
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
			distances.push_back(jaroDistance(name, name2));
			count++;
		}
	}

	if (count > 0){
		return Feature(*max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

void generateAuthorFeatures(DB *db, Dataset *dataset)
{
	generateSingleFeature(db, dataset, 101, generateAuthorAffiliationLevenshteinDistanceFeature);
	generateSingleFeature(db, dataset, 102, generateCoauthorAffiliationLevenshteinDistanceFeature);
	generateSingleFeature(db, dataset, 103, generateAuthorNameLevenshteinDistanceFeature);
	generateSingleFeature(db, dataset, 104, generateAuthorAbbreviatedNameLevenstheinDistanceFeature);
	generateSingleFeature(db, dataset, 105, generateAuthorCoauthorNameLevstheinDistanceFeature);
	generateSingleFeature(db, dataset, 106, generateAuthorCoauthorAbbreviatedNameLevstheinDistanceFeature);
	generateSingleFeature(db, dataset, 107, generateAuthorCoauthorLastNameLevestheinDistanceFeature);
	generateSingleFeature(db, dataset, 108, generateTotalNumberofCoauthors);
	generateSingleFeature(db, dataset, 109, generateAverageNumberofPapersofCoauthor);
	generateSingleFeature(db, dataset, 110, generateNumberofPapersofAuthor);
	generateSingleFeature(db, dataset, 111, generateAuthorAffiliationJaroDistanceFeature);
	generateSingleFeature(db, dataset, 112, generateCoauthorAffiliationJaroDistanceFeature);
	generateSingleFeature(db, dataset, 113, generateAuthorNameJaroDistanceFeature);
	generateSingleFeature(db, dataset, 114, generateAuthorAbbreviatedNameJaroDistanceFeature);
	generateSingleFeature(db, dataset, 115, generateAuthorCoauthorNameJaroDistanceFeature);
	generateSingleFeature(db, dataset, 116, generateAuthorCoauthorAbbreviatedNameJaroDistanceFeature);
	generateSingleFeature(db, dataset, 117, generateAuthorCoauthorLastNameJaroDistanceFeature);
}
