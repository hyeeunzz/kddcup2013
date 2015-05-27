#include "data.h"

#define DATA_PATH "../../data"

bool authorCompare(const Author *a, const Author *b) { return a->id < b->id; }
bool paperCompare(const Paper *a, const Paper *b) { return a->id < b->id; }
bool paperAuthorCompare(const PaperAuthor *a, const PaperAuthor *b) { return a->paper_id < b->paper_id; }
bool paperAuthorIndexCompare(const PaperAuthorIndex *a, const PaperAuthorIndex *b) { return a->author_id < b->author_id; }
bool conferenceCompare(const Conference *a, const Conference *b) { return a->id < b->id; }
bool JournalCompare(const Journal *a, const Journal *b) { return a->id < b->id; }
bool isPrintable(char c) { return c >= 32 && c <= 126; }

FILE *getFile(char *filename)
{
	fprintf(stderr, "parse%s... ", filename);
	char buf[128];
	errno_t err;

	// {filename}.csv
	sprintf_s(buf, "%s/%s.csv", DATA_PATH, filename);
	FILE *fp;
	if ((err = fopen_s(&fp, buf, "r")) != 0){
		fprintf(stderr, "Error occured while loading %s.csv!\n", filename);
		exit(1);
	}
	// Skip head line
	fgets(buf, 128, fp);
	return fp;
}

void parseAuthor(DB *db)
{
	clock_t start_time = std::clock();
	FILE *fp = getFile("Author");

	while (!feof(fp)){
		Author author;
		fscanf_s(fp, "%d,", &author.id);

		char c = 0;
		int name_index = 0, affiliation_index = 0;
		int state = 0; // 0: name, 1: affiliation
		bool quote = false;

		while (true) {
			c = getc(fp);
			if (!quote && c == ','){
				if (state == 0){
					state = 1;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", author.id);
					exit(1);
				}
			}
			else if (c == '\"'){
				quote = !quote;
			}
			else if (isPrintable(c)) { //printable ascii
				if (state == 0){
					author.name[name_index++] = c;
				}
				else {
					author.affiliation[affiliation_index++] = c;
				}
			}
			else if ((!quote && c == '\n') || c == EOF){
				break;
			}
		}
		author.name[name_index] = 0;
		author.affiliation[affiliation_index] = 0;
		if (c == EOF) {
			break;
		}
		db->authors.push_back(new Author(author));
	}
	sort(db->authors.begin(), db->authors.end(), authorCompare);
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);
}

void parsePaper(DB *db)
{
	clock_t start_time = std::clock();
	FILE *fp = getFile("Paper");

	while (!feof(fp)){
		Paper paper;
		fscanf_s(fp, "%d,", &paper.id);

		char c = 0;
		int title_index = 0, keyword_index = 0;
		int state = 0; // 0: title, 1: year, 2: ConferenceId, 3: JournalId, 4: Keyword
		bool quote = false;
		paper.year = 0;
		paper.conference_id = 0;
		paper.journal_id = 0;

		while (true) {
			c = getc(fp);
			if (!quote && c == ','){
				if (state < 4){
					state++;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", paper.id);
					exit(1);
				}
			}
			else if (c == '\"'){
				quote = !quote;
			}
			else if (isPrintable(c)) { //printable ascii
				if (state == 0){ // title
					paper.title[title_index++] = c;
				}
				else if (state == 1){ // year
					if (c >= '0' && c <= '9'){
						paper.year = paper.year * 10 + (c - '0');
					}
				}
				else if (state == 2){ // conference id
					if (c >= '0' && c <= '9'){
						paper.conference_id = paper.conference_id * 10 + (c - '0');
					}
				}
				else if (state == 3){ // journal id
					if (c >= '0' && c <= '9'){
						paper.journal_id = paper.journal_id * 10 + (c - '0');
					}
				}
				else if (state == 4){
					paper.keywords[keyword_index++] = c;
				}
			}
			else if ((!quote && c == '\n') || c == EOF){
				break;
			}
		}
		paper.title[title_index] = 0;
		paper.keywords[keyword_index] = 0;
		if (c == EOF) {
			break;
		}
		db->papers.push_back(new Paper(paper));
	}
	std::sort(db->papers.begin(), db->papers.end(), paperCompare);
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);
}

void parsePaperAuthor(DB *db)
{
	clock_t start_time = std::clock();
	FILE *fp = getFile("PaperAuthor");

	while (!feof(fp)){
		PaperAuthor paper_author;
		fscanf_s(fp, "%d,%d,", &paper_author.paper_id, &paper_author.author_id);

		char c = 0;
		int name_index = 0, affiliation_index = 0;
		int state = 0; // 0: name, 1: affiliation
		bool quote = false;

		while (true) {
			c = getc(fp);
			if (!quote && c == ','){
				if (state == 0){
					state = 1;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", paper_author.paper_id);
					exit(1);
				}
			}
			else if (c == '\"'){
				quote = !quote;
			}
			else if (isPrintable(c)) { //printable ascii
				if (state == 0){
					paper_author.name[name_index++] = c;
				}
				else {
					paper_author.affiliation[affiliation_index++] = c;
				}
			}
			else if ((!quote && c == '\n') || c == EOF){
				break;
			}
		}
		paper_author.name[name_index] = 0;
		paper_author.affiliation[affiliation_index] = 0;
		if (c == EOF) {
			break;
		}
		db->paper_authors.push_back(new PaperAuthor(paper_author));
	}
	std::sort(db->paper_authors.begin(), db->paper_authors.end(), paperAuthorCompare);

	// make index...
	PaperAuthorIndex paper_author_index;
	for (size_t i = 0; i < db->paper_authors.size(); i++){
		paper_author_index.author_id = db->paper_authors[i]->author_id;
		paper_author_index.paper_author_index = i;
		db->paper_author_index.push_back(new PaperAuthorIndex(paper_author_index));
	}
	std::sort(db->paper_author_index.begin(), db->paper_author_index.end(), paperAuthorIndexCompare);

	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);
}

void parseConference(DB *db)
{
	clock_t start_time = std::clock();
	FILE *fp = getFile("Conference");

	while (!feof(fp)){
		Conference conference;
		fscanf_s(fp, "%d,", &conference.id);

		char c = 0;
		int shortname_index = 0, fullname_index = 0, homepage_index = 0;
		int state = 0; // 0: shortname, 1: fullname, 2: homepage
		bool quote = false;

		while (true) {
			c = getc(fp);
			if (!quote && c == ','){
				if (state < 2){
					state++;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", conference.id);
					exit(1);
				}
			}
			else if (c == '\"'){
				quote = !quote;
			}
			else if (isPrintable(c)) { //printable ascii
				if (state == 0){ // shortname
					conference.shortname[shortname_index++] = c;
				}
				else if (state == 1){ // fullname
					conference.fullname[fullname_index++] = c;
				}
				else if (state == 2){ // homepage
					conference.homepage[homepage_index++] = c;
				}
			}
			else if ((!quote && c == '\n') || c == EOF){
				break;
			}
		}
		conference.shortname[shortname_index] = 0;
		conference.fullname[fullname_index] = 0;
		conference.homepage[homepage_index] = 0;
		if (c == EOF) {
			break;
		}
		db->conferences.push_back(new Conference(conference));
	}
	std::sort(db->conferences.begin(), db->conferences.end(), conferenceCompare);
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);
}

void parseJournal(DB *db)
{
	clock_t start_time = std::clock();
	FILE *fp = getFile("Journal");

	while (!feof(fp)){
		Journal journal;
		fscanf_s(fp, "%d,", &journal.id);

		char c = 0;
		int shortname_index = 0, fullname_index = 0, homepage_index = 0;
		int state = 0; // 0: shortname, 1: fullname, 2: homepage
		bool quote = false;

		while (true) {
			c = getc(fp);
			if (!quote && c == ','){
				if (state < 2){
					state++;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", journal.id);
					exit(1);
				}
			}
			else if (c == '\"'){
				quote = !quote;
			}
			else if (isPrintable(c)) { //printable ascii
				if (state == 0){ // shortname
					journal.shortname[shortname_index++] = c;
				}
				else if (state == 1){ // fullname
					journal.fullname[fullname_index++] = c;
				}
				else if (state == 2){ // homepage
					journal.homepage[homepage_index++] = c;
				}
			}
			else if ((!quote && c == '\n') || c == EOF){
				break;
			}
		}
		journal.shortname[shortname_index] = 0;
		journal.fullname[fullname_index] = 0;
		journal.homepage[homepage_index] = 0;
		if (c == EOF) {
			break;
		}
		db->journals.push_back(new Journal(journal));
	}
	std::sort(db->journals.begin(), db->journals.end(), JournalCompare);
	fprintf(stderr, "ok (%d ms)\n", std::clock() - start_time);
	fclose(fp);
}

DB *loadDB()
{
	DB *db = new DB();
	parseConference(db);
	parseJournal(db);
	parseAuthor(db);
	parsePaper(db);
	parsePaperAuthor(db);

	return db;
}

Author* DB::getAuthorById(int id)
{
	// do binary search
	size_t left = 0;
	size_t right = authors.size() - 1;

	while (left <= right){
		size_t mid = (left + right) / 2;
		if (authors[mid]->id == id){
			return authors[mid];
		}
		else if (authors[mid]->id < id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return NULL;
}

Paper* DB::getPaperById(int id)
{
	// do binary search
	size_t left = 0;
	size_t right = papers.size() - 1;

	while (left <= right){
		size_t mid = (left + right) / 2;
		if (papers[mid]->id == id){
			return papers[mid];
		}
		else if (papers[mid]->id < id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return NULL;
}

Conference* DB::getConferenceById(int id)
{
	// do binary search
	size_t left = 0;
	size_t right = conferences.size() - 1;

	while (left <= right){
		size_t mid = (left + right) / 2;
		if (conferences[mid]->id == id){
			return conferences[mid];
		}
		else if (conferences[mid]->id < id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return NULL;
}

Journal* DB::getJournalById(int id)
{
	// do binary search
	size_t left = 0;
	size_t right = journals.size() - 1;

	while (left <= right){
		size_t mid = (left + right) / 2;
		if (journals[mid]->id == id){
			return journals[mid];
		}
		else if (journals[mid]->id < id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return NULL;
}

void DB::getPaperAuthorsByPaperId(std::vector<PaperAuthor*> &result, int paper_id)
{
	// do binary search
	size_t left = 0;
	size_t right = paper_authors.size() - 1;

	size_t start;
	while (left <= right){
		size_t mid = (left + right) / 2;
		if (paper_authors[mid]->paper_id == paper_id){
			start = mid;
			break;
		}
		else if (paper_authors[mid]->paper_id < paper_id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	while (start > 0 && paper_authors[start-1]->paper_id == paper_id) start--;

	// Put the paper authors into result
	result.clear();
	for (size_t i = start; i < paper_authors.size() && paper_authors[i]->paper_id == paper_id; i++){
		result.push_back(paper_authors[i]);
	}
}

void DB::getPaperAuthorsByAuthorId(std::vector<PaperAuthor*> &result, int author_id)
{
	// do binary search
	size_t left = 0;
	size_t right = paper_author_index.size() - 1;

	size_t start;
	while (left <= right){
		size_t mid = (left + right) / 2;
		if (paper_author_index[mid]->author_id == author_id){
			start = mid;
			break;
		}
		else if (paper_author_index[mid]->author_id < author_id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	while (start > 0 && paper_author_index[start - 1]->author_id == author_id) start--;

	// Put the paper_authors into result
	result.clear();
	for (size_t i = start; i < paper_author_index.size() && paper_author_index[i]->author_id == author_id; i++){
		result.push_back(paper_authors[ paper_author_index[i]->paper_author_index ]);
	}
	std::sort(result.begin(), result.end(), paperAuthorCompare);
}

