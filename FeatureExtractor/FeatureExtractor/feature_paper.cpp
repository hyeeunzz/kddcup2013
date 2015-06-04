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

Feature generateMeanPaperPublicationYearofAuthorFeature(DB *db, int author_id, int paper_id)
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

Feature generateMeanPaperPublicationYearofCoauthorFeature(DB *db, int author_id, int paper_id)
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

void generatePaperFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	//f.push_back(generatePaperPublicationTimeFeature(db, author_id, paper_id));
	//f.push_back(generatePaperConferenceFeature(db, author_id, paper_id));
	//f.push_back(generatePaperJournalFeature(db, author_id, paper_id));
	f.push_back(generateMeanPaperPublicationYearofAuthorFeature(db, author_id, paper_id));
	//f.push_back(generateMeanPaperPublicationYearofCoauthorFeature(db, author_id, paper_id));
}
