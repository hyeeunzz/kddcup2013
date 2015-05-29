#include "feature_other.h"

Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id) {
	std::vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	return paper_authors.size();
}

void generateOtherFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generatePaperAuthorCountFeature(db, author_id, paper_id));
}
