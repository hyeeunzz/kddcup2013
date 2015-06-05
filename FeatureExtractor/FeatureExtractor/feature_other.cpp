// Feature.id : 3xx
#include "feature_other.h"

using std::vector;
using std::string;

Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id) {
	vector<PaperAuthor*> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);

	if (paper_authors.size() > 1) {
		return Feature(301, 1);
	}
	else {
		return Feature(301, -1);
	}
}

void generateOtherFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	//f.push_back(generatePaperAuthorCountFeature(db, author_id, paper_id));
}
