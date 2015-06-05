#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <vector>
#include "db.h"
#include "util.h"

#define MAGIC_NUMBER 856362

typedef struct _Feature {
	double value;
	_Feature(double _value){
		value = _value;
	}
} Feature;
typedef std::vector<Feature> FeatureList;
typedef int Label;

typedef struct _Example {
	int author_id;
	int paper_id;
	FeatureList X; // feature
	Label y; // +1: positive, -1: negative
} Example;

class Dataset {
public:
	DB *db;
	char filename[128];
	std::vector<Example *> examples;
};

bool file_exists(const char *filepath);
Dataset* loadDataset(char *filename, DB *db);
void generateFeatures(DB *db, Dataset *dataset);
void generateSingleFeature(DB *db, Dataset *dataset, int feature_id, void(*feature_generator)(DB *, Dataset *));
void generateSingleFeature(DB *db, Dataset *dataset, int feature_id, Feature(*feature_generator)(DB *, int, int));

#endif
