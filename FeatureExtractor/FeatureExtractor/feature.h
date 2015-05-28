#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <vector>
#include "db.h"
#include "util.h"

typedef double Feature;
typedef std::vector<double> FeatureList;
typedef int Label;

typedef struct _Example {
	int author_id;
	int paper_id;
	FeatureList X; // feature
	Label y; // +1: positive, -1: negative
} Example;

//typedef std::vector<Example*> Dataset;
class Dataset {
public:
	DB *db;
	char filename[128];
	std::vector<Example *> examples;
	void save();
};

Dataset* loadDataset(char *filename, DB *db);

#endif
