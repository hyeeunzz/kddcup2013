#include "util.h"

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2)
{
	const std::size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);

	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i + 1;
		for (unsigned int j = 0; j < len2; j++)
			// note that std::min({arg1, arg2, arg3}) works only in C++11,
			// for C++98 use std::min(std::min(arg1, arg2), arg3)
			// col[j + 1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1) });
			col[j + 1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1), prevCol[j] + (s1[i] == s2[j] ? 0 : 1) );
		col.swap(prevCol);
	}
	return prevCol[len2];
}

void stringToLower(std::string& s){
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}


void generateSmallDomain(DB *db)
{
	srand(0);
	int *author_mark = (int*)calloc(3000000, sizeof(int));
	int *paper_mark = (int*)calloc(3000000, sizeof(int));
	int *train_mark = (int*)calloc(3000000, sizeof(int));

	FILE *author_fp, *paper_fp, *paper_author_fp, *train_fp;
	fopen_s(&author_fp, "../../data_small/Author.csv", "w");
	fopen_s(&paper_fp, "../../data_small/Paper.csv", "w");
	fopen_s(&paper_author_fp, "../../data_small/PaperAuthor.csv", "w");
	fopen_s(&train_fp, "../../data_small/Train.csv", "w");

	fprintf(author_fp, "Id,Name,Affiliation\n");
	fprintf(paper_fp, "Id,Title,Year,ConferenceId,JournalId,Keyword\n");
	fprintf(paper_author_fp, "PaperId,AuthorId,Name,Affiliation\n");
	fprintf(train_fp, "AuthorId, ConfirmedPaperIds, DeletedPaperIds\n");

	// start
	FILE *fp;
	char buf[512];
	fopen_s(&fp, "../../data/Train.csv", "r");
	fgets(buf, 128, fp);
	while (!feof(fp)){
		int author_id = 0;
		int paper_id = 0;
		fscanf_s(fp, "%d,", &author_id);
		bool valid_author = false;
		if (rand() % 10 == 0){
			valid_author = true;
		}

		char c = 0;

		while (true) {
			c = getc(fp);
			if (paper_id > 0 && (c == ',' || c == ' ' || c == '\n' || c == EOF)){
				// completed paper_id!
				if (valid_author){
					author_mark[author_id] = 1;
					paper_mark[paper_id] = 1;
					train_mark[author_id] = 1;
				}
				paper_id = 0;
			}
			else if (c >= '0' && c <= '9') {
				paper_id = paper_id * 10 + (c - '0');
			}
			if (c == '\n' || c == EOF){
				break;
			}
		}
		if (c == EOF) {
			break;
		}
	}

	// Generate sampled CSV files...
	std::vector<PaperAuthor*> paper_authors;

	// PaperAuthor.csv
	for (int i = 0; i < db->paper_authors.size(); i++){
		int author_id = db->paper_authors[i]->author_id;
		int paper_id = db->paper_authors[i]->paper_id;
		if (paper_mark[paper_id] == 1 || author_mark[author_id] == 1){
			if (author_mark[author_id] == 0) author_mark[author_id] = 2;
			if (paper_mark[paper_id] == 0)	 paper_mark[paper_id] = 2;
			// add papers that is written by the current author
			db->getPaperAuthorsByAuthorId(paper_authors, author_id);
			for (int j = 0; j < paper_authors.size(); j++){
				if (paper_mark[paper_authors[j]->paper_id] == 0) paper_mark[paper_authors[j]->paper_id] = 2;
			}
			// add authors that are author of the current paper (coauthers)
			db->getPaperAuthorsByPaperId(paper_authors, paper_id);
			for (int j = 0; j < paper_authors.size(); j++){
				if (author_mark[paper_authors[j]->author_id] == 0) author_mark[paper_authors[j]->author_id] = 2;
			}
			fprintf(paper_author_fp, "%d,%d,\"%s\",\"%s\"\n", db->paper_authors[i]->paper_id, db->paper_authors[i]->author_id, db->paper_authors[i]->name, db->paper_authors[i]->affiliation);
		}
	}
	for (int i = 0; i < 3000000; i++){
		author_mark[i] = (author_mark[i] == 2) ? 1 : author_mark[i];
		paper_mark[i] = (paper_mark[i] == 2) ? 1 : paper_mark[i];
	}
	// Author.csv
	for (int i = 0; i < db->authors.size(); i++){
		if (author_mark[db->authors[i]->id]){
			fprintf(author_fp, "%d,\"%s\",\"%s\"\n", db->authors[i]->id, db->authors[i]->name, db->authors[i]->affiliation);
			db->getPaperAuthorsByAuthorId(paper_authors, db->authors[i]->id);
			for (int j = 0; j < paper_authors.size(); j++){
				paper_mark[paper_authors[j]->paper_id] = 1;
			}
		}
	}
	// Paper.csv
	for (int i = 0; i < db->papers.size(); i++){
		if (paper_mark[db->papers[i]->id]){
			fprintf(paper_fp, "%d,\"%s\",%d,%d,%d,\"%s\"\n", db->papers[i]->id, db->papers[i]->title, db->papers[i]->year, db->papers[i]->conference_id, db->papers[i]->journal_id, db->papers[i]->keywords);
		}
	}
	// Train.csv
	rewind(fp);
	fgets(buf, 128, fp);
	char line[50000];
	while (!feof(fp)){
		int author_id = 0;
		fscanf_s(fp, "%d,", &author_id);
		if (author_id == 0 || feof(fp)) break;
		fgets(line, 50000, fp);
		/*printf("%d # %s\n\n", author_id, line);*/
		if (train_mark[author_id]){
			fprintf(train_fp, "%d,%s", author_id, line);
		}
	}

	fclose(author_fp);
	fclose(paper_fp);
	fclose(paper_author_fp);
	fclose(train_fp);
}