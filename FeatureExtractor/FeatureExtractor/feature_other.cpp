// Feature.id : 3xx
#include "feature_other.h"

using std::vector;
using std::string;

// 301. Count of pair of author id and paper id in paper_author
Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id) {
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	return paper_authors.size();
}

void generateOtherFeatures(DB *db, Dataset *dataset)
{
	generateSingleFeature(db, dataset, 301, generatePaperAuthorCountFeature);
}
