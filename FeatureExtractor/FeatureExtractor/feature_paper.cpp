// Feature.id : 2xx
#include "feature_paper.h"

using std::vector;
using std::string;

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
			return Feature(201, 1);
		else
			return Feature(201, -1);
	}
	return Feature(201, MAGIC_NUMBER);
	
}

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
						return Feature(202, 1);
					}
				}
			}
		}
		return Feature(202, -1);
	}
	return Feature(202, MAGIC_NUMBER);

}

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
						return Feature(203, 1);
					}
				}
			}
		}
		return Feature(203, -1);
	}
	return Feature(203, MAGIC_NUMBER);

}

Feature generateMeanPaperPublicationYearofAuthor(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByAuthorId(paper_authors, author_id);

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
		return Feature(204, abs(paper->year - avg_year));
	}
	else {
		return Feature(204, MAGIC_NUMBER);
	}
}

Feature generateMeanPaperPublicationYearofCoauthor(DB *db, int author_id, int paper_id)
{
	Paper *paper = db->getPaperById(paper_id);
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsByPaperId(paper_authors, paper_id);

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
		return Feature(205, abs(paper->year - avg_year));
	}
	else {
		return Feature(205, MAGIC_NUMBER);
	}
}

// Distance between major conference cluster and paper's conference cluster
Feature generateConferenceClusterFeature(DB *db, int author_id, int paper_id)
{
	Paper *this_paper = db->getPaperById(paper_id);
	if (this_paper->conference_id == 0){
		return Feature(206, MAGIC_NUMBER);
	}
	Conference *this_conference = db->getConferenceById(this_paper->conference_id);
	if (this_conference == NULL) {
		return Feature(206, MAGIC_NUMBER);
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

	return Feature(206, result);
}

// Distance between major journal cluster and paper's journal cluster
Feature generateJournalClusterFeature(DB *db, int author_id, int paper_id)
{
	Paper *this_paper = db->getPaperById(paper_id);
	if (this_paper->journal_id == 0){
		return Feature(207, MAGIC_NUMBER);
	}
	Journal *this_journal = db->getJournalById(this_paper->journal_id);
	if (this_journal == NULL) {
		return Feature(207, MAGIC_NUMBER);
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

	return Feature(207, result);
}

void generatePaperFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generatePaperPublicationTimeFeature(db, author_id, paper_id));
	f.push_back(generatePaperConferenceFeature(db, author_id, paper_id));
	f.push_back(generatePaperJournalFeature(db, author_id, paper_id));
	f.push_back(generateMeanPaperPublicationYearofAuthor(db, author_id, paper_id));
	f.push_back(generateMeanPaperPublicationYearofCoauthor(db, author_id, paper_id));
	f.push_back(generateConferenceClusterFeature(db, author_id, paper_id));
	f.push_back(generateJournalClusterFeature(db, author_id, paper_id));
}
