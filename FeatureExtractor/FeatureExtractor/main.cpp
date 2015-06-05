#include <stdio.h>
#include <stdlib.h>
#include "db.h"
#include "feature.h"
#include "feature_paper.h"
#include <iostream>
#include "command.h"

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
		generatePaperDuplicateFeature(db, train->examples);
		generatePaperDuplicateFeature(db, valid->examples);
		printf("# of features : %d\n", train->examples[0]->X.size());
		train->save();
		valid->save();
	}
	else if (argc == 3 && strcmp(argv[2], "-cj") == 0){
		printf("Generate Conference&Journal Similarity matrix...");
		generateConferenceSimilarityMatrix(db);
		generateJournalSimilarityMatrix(db);
	}
	else if (argc == 3 && strcmp(argv[2], "-s") == 0){
		executeInterpreter(db);
	}

	//generateSmallDomain(db);

	return 0;
}
