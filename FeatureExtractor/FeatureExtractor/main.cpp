#include <stdio.h>
#include <stdlib.h>
#include "db.h"
#include "feature.h"

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
	loadDataset("Train", db)->save();
	loadDataset("Valid", db)->save();

	//generateSmallDomain(db);
	//for (int i = 0; i < train->examples.size(); i++){
	//	Author *author = db->getAuthorById(train->examples[i]->author_id);
	//	printf("%10d %10d : %3d  %.1f  %.1f\n",
	//		train->examples[i]->author_id, train->examples[i]->paper_id, train->examples[i]->y,
	//		train->examples[i]->X[0], train->examples[i]->X[1]);
	//}

	return 0;
}
