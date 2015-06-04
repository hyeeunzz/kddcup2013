#ifndef _FEATURE_PAPER_H_
#include "feature.h"

void generatePaperDuplicateFeature(DB *db, const std::vector<Example *> &examples);
void generatePaperFeatures(FeatureList &f, DB *db, int author_id, int paper_id);

#endif
