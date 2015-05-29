#ifndef _FEATURE_AUTHOR_H_
#include "feature.h"

void generateAuthorFeatures(FeatureList &f, DB *db, int author_id, int paper_id);
Feature generateCoauthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id);
Feature generateAuthorAffiliationLevenshteinDistanceFeature(DB *db, int author_id, int paper_id);
Feature generateAuthorNameLevenshteinDistanceFeature(DB *db, int author_id, int paper_id);


#endif