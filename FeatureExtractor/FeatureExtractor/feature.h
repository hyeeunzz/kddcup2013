#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <vector>
#include "db.h"
#include "util.h"

#define MAGIC_NUMBER 856362

typedef struct _Feature {
	int id;
	double value;
	_Feature(int _id, int _value){
		id = _id;
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
	void save();
};

Dataset* loadDataset(char *filename, DB *db);

#endif
