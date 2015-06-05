// Feature.id : 2xx
#include "feature_paper.h"

using std::vector;
using std::string;

// 200
void generatePaperDuplicateFeature(DB *db, Dataset *dataset)
{
	vector<Example *> examples = dataset->examples;

	int start_index = -1;
	int end_index = -1;
	int current_author_id = -1;
	for (size_t i = 0; i < examples.size(); i++) {
		if (examples[i]->author_id != current_author_id) {
			start_index = i;
			current_author_id = examples[start_index]->author_id;
			for (end_index = i; end_index < examples.size() && examples[end_index]->author_id == current_author_id; end_index++);
		}
		int count = 0;
		int current_paper_id = examples[i]->paper_id;
		for (int j = start_index; j < end_index; j++){
			if (examples[j]->paper_id == current_paper_id) count++;
			if (count >= 2) break;
		}
		examples[i]->X.push_back(Feature((count >= 2) ? 1 : 0));
	}
}

// 201
Feature generatePaperPublicationTimeFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	int p_year = paper->year;
	if (p_year > 0){
		int min_year = 2014;
		int max_year = 0;
		int p_year2;
		for (size_t i = 0; i < paper_authors.size(); i++){
			PaperAuthor *author2 = paper_authors[i];
			Paper *paper2 = db->getPaperById(author2->paper_id);
			if (paper2 != NULL){
				p_year2 = paper2->year;
				if (1500 < p_year2 && p_year2 < 2014){
					if (p_year2 < min_year)
						min_year = p_year2;
					if (max_year < p_year2)
						max_year = p_year2;
				}
			}
		}

		if (min_year < p_year && p_year < max_year)
			return Feature(1);
		else
			return Feature(-1);
	}
	return Feature(MAGIC_NUMBER);
	
}


//202
Feature generatePaperConferenceFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	int p_con = paper->conference_id;
	if (p_con > 0){
		int p_con2;
		for (size_t i = 0; i < paper_authors.size(); i++){
			PaperAuthor *author2 = paper_authors[i];
			if (author2->paper_id != paper_id){
				Paper *paper2 = db->getPaperById(author2->paper_id);
				if (paper2 != NULL){
					p_con2 = paper2->conference_id;
					if (p_con == p_con2){
						return Feature(1);
					}
				}
			}
		}
		return Feature(-1);
	}
	return Feature(MAGIC_NUMBER);

}

//203
Feature generatePaperJournalFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	int p_jrn = paper->journal_id;
	if (p_jrn > 0){
		int p_jrn2;
		for (size_t i = 0; i < paper_authors.size(); i++){
			PaperAuthor *author2 = paper_authors[i];
			if (author2->paper_id != paper_id){
				Paper *paper2 = db->getPaperById(author2->paper_id);
				if (paper2 != NULL){
					p_jrn2 = paper2->journal_id;
					if (p_jrn == p_jrn2){
						return Feature(1);
					}
				}
			}
		}
		return Feature(-1);
	}
	return Feature(MAGIC_NUMBER);

}

//204
Feature generateMeanPaperPublicationYearofAuthorFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	if (paper->year <= 1500 || paper->year >= 2014) {
		return Feature(-1);
	}
	int year = 0;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++) {
		Paper *paper2 = db->getPaperById(paper_authors[i]->paper_id);
		if (paper2 != NULL) {
			if (paper2->year > 1500 && paper2->year < 2014) {
				year += paper2->year;
				count++;
			}
		}
	}

	if (count > 0) {
		double avg_year = (double)year / count;
		if (abs(paper->year - avg_year) < 5) {
			return Feature(1);
		}
		else {
			return Feature(-1);
		}
	}
	else {
		return Feature(-1);
	}
}

//205
Feature generateMeanPaperPublicationYearofCoauthorFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

	if (paper->year <= 1500 || paper->year >= 2014) {
		return Feature(-1);
	}
	int year = 0;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++) {
		Author *coauthor = db->getAuthorById(paper_authors[i]->author_id);
		vector<PaperAuthor*> paper_authors2;
		if (coauthor != NULL) {
			db->getPaperAuthorsByAuthorId(paper_authors2, coauthor->id);
			for (size_t j = 0; j < paper_authors2.size(); j++) {
				Paper *paper2 = db->getPaperById(paper_authors2[j]->paper_id);
				if (paper2 != NULL) {
					if (paper2->year > 1500 && paper2->year < 2014) {
						year += paper2->year;
						count++;
					}
				}
			}
		}
	}

	if (count > 0) {
		double avg_year = (double)year / count;
		if (abs(paper->year - avg_year) < 5) {
			return Feature(1);
		}
		else {
			return Feature(-1);
		}
	}
	else {
		return Feature(-1);
	}
}

// 206. Distance between major conference cluster and paper's conference cluster
Feature generateConferenceClusterFeature(DB *db, int author_id, int paper_id)
{
	Paper *this_paper = db->getPaperById(paper_id);
	if (this_paper->conference_id == 0){
		return Feature(-1);
	}
	Conference *this_conference = db->getConferenceById(this_paper->conference_id);
	if (this_conference == NULL) {
		return Feature(-1);
	}

	// Determine author's major conference cluster
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);
	int cluster_count[100] = { 0, };

	for (int i = 0; i < paper_authors.size(); i++){
		PaperAuthor* paper_author = paper_authors[i];
		Paper *paper = db->getPaperById(paper_author->paper_id);
		if (paper == NULL || paper->conference_id == 0) continue;
		Conference *conference = db->getConferenceById(paper->conference_id);
		if (conference == NULL) continue;
		cluster_count[conference->cluster]++;
	}
	int result = 0;
	int this_conference_count = cluster_count[this_conference->cluster];
	for (int i = 0; i < 100; i++){
		if (cluster_count[i] > this_conference_count){
			result++;
		}
	}

	if (result < 2) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
}

// 207. Distance between major journal cluster and paper's journal cluster
Feature generateJournalClusterFeature(DB *db, int author_id, int paper_id)
{
	Paper *this_paper = db->getPaperById(paper_id);
	if (this_paper->journal_id == 0){
		return Feature(-1);
	}
	Journal *this_journal = db->getJournalById(this_paper->journal_id);
	if (this_journal == NULL) {
		return Feature(-1);
	}

	// Determine author's major journal cluster
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);
	int cluster_count[100] = { 0, };

	for (int i = 0; i < paper_authors.size(); i++){
		PaperAuthor* paper_author = paper_authors[i];
		Paper *paper = db->getPaperById(paper_author->paper_id);
		if (paper == NULL || paper->journal_id == 0) continue;
		Journal *journal = db->getJournalById(paper->journal_id);
		if (journal == NULL) continue;
		cluster_count[journal->cluster]++;
	}
	int result = 0;
	int this_journal_count = cluster_count[this_journal->cluster];
	for (int i = 0; i < 100; i++){
		if (cluster_count[i] > this_journal_count){
			result++;
		}
	}

	if (result < 25) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
}

//208
Feature generatePaperTitleLevenshteinDstanceFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	vector<double> distances;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++){
		Paper *paper2 = db->getPaperById(paper_authors[i]->paper_id);
		if (paper2 == NULL || paper->id == paper2->id) continue;
		if (paper->title.length() > 0 && paper2->title.length() > 0) {
			string paper_title(paper->title);
			string paper2_title(paper2->title);
			stringToLower(paper_title);
			stringToLower(paper2_title);
			distances.push_back(levenshteinDistance(paper_title, paper2_title));
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

//209
Feature generatePaperTitleJaroDstanceFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	vector<double> distances;
	int count = 0;
	for (size_t i = 0; i < paper_authors.size(); i++){
		Paper *paper2 = db->getPaperById(paper_authors[i]->paper_id);
		if (paper2 == NULL || paper->id == paper2->id) continue;
		if (paper->title.length() > 0 && paper2->title.length() > 0) {
			string paper_title(paper->title);
			string paper2_title(paper2->title);
			stringToLower(paper_title);
			stringToLower(paper2_title);
			distances.push_back(jaroDistance(paper_title, paper2_title));
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

//210
Feature generatePublicationYearDifferenceofAuthorPapersFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	vector<double> differences;
	int count = 0;
	int diff = 0;
	if (paper->year < 1500 || paper->year > 2014) {
		return Feature(MAGIC_NUMBER);
	}
	for (size_t i = 0; i < paper_authors.size(); i++){
		Paper *paper2 = db->getPaperById(paper_authors[i]->paper_id);
		if (paper2 == NULL || paper->id == paper2->id) continue;
		if (paper2->year > 1500 && paper2->year < 2014) {
			diff = abs(paper->year - paper2->year);
			differences.push_back(diff);
			count++;
		}
	}

	if (count > 0){
		return Feature(*min_element(differences.begin(), differences.end()));
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

//211
Feature generatePaperPublicationTimeGapFeature(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

	int min_year = 2014;
	int max_year = 1500;
	for (size_t i = 0; i < paper_authors.size(); i++){
		Paper *paper2 = db->getPaperById(paper_authors[i]->paper_id);
		if (paper2 != NULL){
			if (paper2->year > 1500 && paper2->year < 2014){
				if (paper2->year < min_year)
					min_year = paper2->year;
				if (max_year < paper2->year)
					max_year = paper2->year;
			}
		}
	}

	if (min_year < max_year) {
		return Feature(max_year - min_year);
	}
	else {
		return Feature(MAGIC_NUMBER);
	}
}

void generatePaperFeatures(DB *db, Dataset *dataset)
{
	generateSingleFeature(db, dataset, 200, generatePaperDuplicateFeature);
	generateSingleFeature(db, dataset, 201, generatePaperPublicationTimeFeature);
	generateSingleFeature(db, dataset, 202, generatePaperConferenceFeature);
	generateSingleFeature(db, dataset, 203, generatePaperJournalFeature);
	generateSingleFeature(db, dataset, 204, generateMeanPaperPublicationYearofAuthorFeature);
	generateSingleFeature(db, dataset, 205, generateMeanPaperPublicationYearofCoauthorFeature);
	generateSingleFeature(db, dataset, 206, generateConferenceClusterFeature);
	generateSingleFeature(db, dataset, 207, generateJournalClusterFeature);
	//generateSingleFeature(db, dataset, 208, generatePaperTitleLevenshteinDstanceFeature);
	//generateSingleFeature(db, dataset, 209, generatePaperTitleJaroDstanceFeature);
	generateSingleFeature(db, dataset, 210, generatePublicationYearDifferenceofAuthorPapersFeature);
	generateSingleFeature(db, dataset, 211, generatePaperPublicationTimeGapFeature);
}
