// Feature.id : 3xx
#include "feature_other.h"

using std::vector;
using std::string;

// 301
Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id) {
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	if (paper_authors.size() > 1) {
		return Feature(1);
	}
	else {
		return Feature(-1);
	}
}

void generateOtherFeatures(DB *db, Dataset *dataset)
{
	generateSingleFeature(db, dataset, 301, generatePaperAuthorCountFeature);
}
