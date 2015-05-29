#ifndef _FEATURE_OTHER_H_
#include "feature.h"

void generateOtherFeatures(FeatureList &f, DB *db, int author_id, int paper_id);
Feature generatePaperAuthorCountFeature(DB *db, int author_id, int paper_id);

#endif
