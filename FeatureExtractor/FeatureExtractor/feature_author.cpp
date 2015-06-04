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
		return Feature(101, *max_element(distances.begin(), distances.end()));
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
		return Feature(103, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(103, MAGIC_NUMBER);
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
		return Feature(106, *min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(106, MAGIC_NUMBER);
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
		return Feature(107, *min_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(107, MAGIC_NUMBER);
	}
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

	return Feature(109, (double)count/paper_authors.size());
}

Feature generateNumberofPapersofAuthor(DB *db, int author_id, int paper_id)
{
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, author_id);

	return Feature(110, paper_authors.size());
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
		return Feature(111, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(111, MAGIC_NUMBER);
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
<<<<<<< HEAD
		return Feature(113, *max_element(distances.begin(), distances.end()));
=======
		return Feature(112, *max_element(distances.begin(), distances.end()));
>>>>>>> daf3fb5f8c3ad8affadc65083cf617f8df70a1ba
	}
	else {
		return Feature(112, MAGIC_NUMBER);
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
		return Feature(113, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(113, MAGIC_NUMBER);
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
		return Feature(114, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(114, MAGIC_NUMBER);
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
		return Feature(115, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(115, MAGIC_NUMBER);
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
		return Feature(116, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(116, MAGIC_NUMBER);
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
		return Feature(117, *max_element(distances.begin(), distances.end()));
	}
	else {
		return Feature(117, MAGIC_NUMBER);
	}
}

void generateAuthorFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generateAuthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateCoauthorAffiliationLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameLevenshteinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorAbbreviatedNameLevenstheinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorNameLevstheinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorAbbreviatedNameLevstheinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorLastNameLevestheinDistanceFeature(db, author_id, paper_id));
	f.push_back(generateTotalNumberofCoauthors(db, author_id, paper_id));
	f.push_back(generateAverageNumberofPapersofCoauthor(db, author_id, paper_id));
	f.push_back(generateNumberofPapersofAuthor(db, author_id, paper_id));
	f.push_back(generateAuthorAffiliationJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateCoauthorAffiliationJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorNameJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorAbbreviatedNameJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorNameJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorAbbreviatedNameJaroDistanceFeature(db, author_id, paper_id));
	f.push_back(generateAuthorCoauthorLastNameJaroDistanceFeature(db, author_id, paper_id));
}
