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

char *TrouverMatches(const char *txt, const int *bl)
{
	int i, j;
	char *res = (char*) malloc(256 * sizeof(char));
	char ctmp = 'a';
	for (i = 0; i<256; i++)
	{
		res[i] = 0;
	}
	i = 0, j = 0;
	while (ctmp != 0)
	{
		ctmp = txt[i];
		if (bl[i] == true)
		{
			res[j] = ctmp;
			j++;
		}
		i++;
	}
	return res;
}

double jaroDistance(const std::string &s1, const std::string &s2)
{
	const char *t1 = s1.c_str();
	const char *t2 = s2.c_str();

	int ecartMax, l1, l2, compteMatching, compteTransposition, longueurPrefix, i, j;
	char *t1Matche, *t2Matche;
	int *b1, *b2;
	double distanceJaro;
	if (t1[0] == 0 || t2[0] == 0)
		return 0.0;
	l1 = strlen(t1);
	l2 = strlen(t2);
	ecartMax = (std::max(l1, l2) / 2) - 1;
	compteMatching = 0;
	b1 = (int*) malloc((l1 + 2)*sizeof(int));
	b2 = (int*) malloc((l2 + 2)*sizeof(int));
	for (i = 0; i<l1; i++)
		b1[i] = false;
	for (i = 0; i<l2; i++)
		b2[i] = false;

	for (i = 0; i<l1; i++)
	{
		for (j = std::max(i - ecartMax, 0); j <= std::min(i + ecartMax, l2); j++)
		{
			if (t1[i] == t2[j])
			{
				b1[i] = true;
				b2[j] = true;
				compteMatching++;
				break;
			}
		}
	}

	if (compteMatching == 0)
		return 0.0;

	t1Matche = TrouverMatches(t1, b1);
	t2Matche = TrouverMatches(t2, b2);

	compteTransposition = 0;
	if (strcmp(t1Matche, t2Matche) != 0)
	{
		for (i = 0; i<strlen(t1Matche); i++)
		if (t1Matche[i] != t2Matche[i])
			compteTransposition++;
	}
	else
		compteTransposition = 0;

	free(t1Matche);
	free(t2Matche);
	free(b1);
	free(b2);

	distanceJaro = (((double)compteMatching / l1) + ((double)compteMatching / l2) + ((compteMatching - compteTransposition / 2.0) / compteMatching)) / 3.0;

	longueurPrefix = 0;
	for (i = 0; i<std::min(3, std::min(l1, l2)) + 1; i++)
	{
		if (t1[i] == t2[i])
			longueurPrefix++;
		else
			break;

	}
	return distanceJaro + (longueurPrefix*0.1*(1 - distanceJaro));
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

std::vector<std::string> split(const std::string &str, const char *wschars)
{
	const char *cstr = str.c_str();
	std::vector<std::string> out;

	while (*cstr) {                     // while remaining string not empty
		size_t toklen;
		cstr += strspn(cstr, wschars);    // skip leading whitespace
		toklen = strcspn(cstr, wschars);  // figure out token length
		if (toklen)                       // if we have a token, add to list
			out.push_back(std::string(cstr, toklen));
		cstr += toklen;                   // skip over token
	}

	// ran out of string; return list
	return out;
}

void generateConferenceSimilarityMatrix(DB *db) {
	const int MAX_CONF_COUNT = 5500;
	int n = db->conferences.size(); // should be larger than MAX conference_id
	int **result = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++){
		result[i] = (int*)calloc(n, sizeof(int));
	}
	int conference_index[MAX_CONF_COUNT] = { 0, };
	for (int i = 0; i < n; i++){
		conference_index[db->conferences[i]->id] = i;
	}

	int num_papers;
	for (int i = 0; i < db->paper_author_index.size(); i += num_papers) {
		PaperAuthorIndex *index = db->paper_author_index[i];
		int author_id = index->author_id;
		for (num_papers = 0; i + num_papers < db->paper_author_index.size() && db->paper_author_index[i]->author_id == db->paper_author_index[i + num_papers]->author_id; num_papers++);
		// start counting
		std::vector<int> conference_ids;
		int conference_count[MAX_CONF_COUNT] = { 0, };
		for (int j = i; j < i + num_papers; j++){
			PaperAuthor *paper_author = db->paper_authors[db->paper_author_index[j]->paper_author_index];
			Paper *paper = db->getPaperById(paper_author->paper_id);
			if (paper == NULL || paper->conference_id == 0) continue;

			if (std::find(conference_ids.begin(), conference_ids.end(), paper->conference_id) == conference_ids.end()){
				conference_ids.push_back(paper->conference_id);
			}
			conference_count[paper->conference_id]++;
		}
		for (int j = 0; j < conference_ids.size(); j++){
			for (int k = j+1; k < conference_ids.size(); k++){
				int min_count = std::min(conference_count[conference_ids[j]], conference_count[conference_ids[k]]);
				result[conference_index[conference_ids[j]]][conference_index[conference_ids[k]]] += min_count;
				result[conference_index[conference_ids[k]]][conference_index[conference_ids[j]]] += min_count;
			}
		}
	}

	char path[512];
	sprintf_s(path, "%s/ConferenceSimilarity.dat", db->datapath);
	errno_t err;
	FILE *fp;
	if ((err = fopen_s(&fp, path, "wb")) != 0){
		fprintf(stderr, "Cannot write %s\n", path);
		exit(1);
	}

	fwrite(&n, sizeof(int), 1, fp);
	for (int i = 0; i < n; i++){
		int id = db->conferences[i]->id;
		fwrite(&id, sizeof(int), 1, fp);
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			fwrite(&result[i][j], sizeof(int), 1, fp);
		}
	}
	fclose(fp);
}

void generateJournalSimilarityMatrix(DB *db) {
	const int MAX_JOUR_COUNT = 25600;
	int n = db->journals.size(); // should be larger than MAX journal_id
	int **result = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++){
		result[i] = (int*)calloc(n, sizeof(int));
	}
	int journal_index[MAX_JOUR_COUNT] = { 0, };
	for (int i = 0; i < n; i++){
		journal_index[db->journals[i]->id] = i;
	}

	int num_papers;
	for (int i = 0; i < db->paper_author_index.size(); i += num_papers) {
		PaperAuthorIndex *index = db->paper_author_index[i];
		int author_id = index->author_id;
		for (num_papers = 0; i + num_papers < db->paper_author_index.size() && db->paper_author_index[i]->author_id == db->paper_author_index[i + num_papers]->author_id; num_papers++);
		// start counting
		std::vector<int> journal_ids;
		int journal_count[MAX_JOUR_COUNT] = { 0, };
		for (int j = i; j < i + num_papers; j++){
			PaperAuthor *paper_author = db->paper_authors[db->paper_author_index[j]->paper_author_index];
			Paper *paper = db->getPaperById(paper_author->paper_id);
			if (paper == NULL || paper->journal_id == 0) continue;

			if (std::find(journal_ids.begin(), journal_ids.end(), paper->journal_id) == journal_ids.end()){
				journal_ids.push_back(paper->journal_id);
			}
			journal_count[paper->journal_id]++;
		}
		for (int j = 0; j < journal_ids.size(); j++){
			for (int k = j + 1; k < journal_ids.size(); k++){
				int min_count = std::min(journal_count[journal_ids[j]], journal_count[journal_ids[k]]);
				result[journal_index[journal_ids[j]]][journal_index[journal_ids[k]]] += min_count;
				result[journal_index[journal_ids[k]]][journal_index[journal_ids[j]]] += min_count;
			}
		}
	}

	char path[512];
	sprintf_s(path, "%s/JournalSimilarity.dat", db->datapath);
	errno_t err;
	FILE *fp;
	if ((err = fopen_s(&fp, path, "wb")) != 0){
		fprintf(stderr, "Cannot write %s\n", path);
		exit(1);
	}

	fwrite(&n, sizeof(int), 1, fp);
	for (int i = 0; i < n; i++){
		int id = db->journals[i]->id;
		fwrite(&id, sizeof(int), 1, fp);
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			fwrite(&result[i][j], sizeof(int), 1, fp);
		}
	}
	fclose(fp);
}

