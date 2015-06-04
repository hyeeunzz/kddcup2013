#include <stdio.h>
#include <stdlib.h>
#include "db.h"
#include "feature.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "Not enough input arguments.\n");
		fprintf(stderr, "ex) FeatureExtracter.exe {datapath}\n");
		exit(1);
	}
	char *datapath = argv[1];
	DB *db = loadDB(datapath);

	if (argc == 2){
		Dataset *train = loadDataset("Train", db);
		Dataset *valid = loadDataset("Valid", db);
		printf("# of features : %d\n", train->examples[0]->X.size());
		train->save();
		valid->save();
	}
	else if (argc == 3 && strcmp(argv[2], "-cj") == 0){
		printf("Generate Conference&Journal Similarity matrix...");
		generateConferenceSimilarityMatrix(db);
		generateJournalSimilarityMatrix(db);
	}

	//generateSmallDomain(db);

	return 0;
}
