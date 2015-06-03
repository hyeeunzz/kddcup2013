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
	if (p_year != 0){
		int min_year = 2014;
		int max_year = 0;
		for (size_t i = 0; i < paper_authors.size(); i++){
			PaperAuthor *author2 = paper_authors[i];
			Paper *paper2 = db->getPaperById(author2->paper_id);
			int p_year2 = paper2->year;
			if (1500 < p_year2 && p_year2 < 2014){
				if (p_year2 < min_year)
					min_year = p_year2;
				if (max_year < p_year2)
					max_year = p_year2;
			}
		}

		if (min_year < p_year && p_year < max_year){
			return Feature(201, 1);
		}
	}
	return Feature(201, 0);
	
}

void generatePaperFeatures(FeatureList &f, DB *db, int author_id, int paper_id)
{
	f.push_back(generatePaperPublicationTimeFeature(db, author_id, paper_id));
}
