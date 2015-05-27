#include "data.h"

#define DATA_PATH "../../data"

bool authorCompare(const Author &a, const Author &b) { return a.id < b.id; }
bool paperCompare(const Paper &a, const Paper &b) { return a.id < b.id; }
bool paperAuthorCompare(const PaperAuthor &a, const PaperAuthor &b) { return a.paper_id < b.paper_id; }
bool conferenceCompare(const Conference &a, const Conference &b) { return a.id < b.id; }
bool JournalCompare(const Journal &a, const Journal &b) { return a.id < b.id; }
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
		db->authors.push_back(author);
	}
	sort(db->authors.begin(), db->authors.end(), authorCompare);
	fprintf(stderr, "ok\n");
	fclose(fp);
}

void parsePaper(DB *db)
{
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
		db->papers.push_back(paper);
	}
	std::sort(db->papers.begin(), db->papers.end(), paperCompare);
	fprintf(stderr, "ok\n");
	fclose(fp);
}

void parsePaperAuthor(DB *db)
{
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
		db->paper_authors.push_back(paper_author);
	}
	std::sort(db->paper_authors.begin(), db->paper_authors.end(), paperAuthorCompare);
	fprintf(stderr, "ok\n");
	fclose(fp);
}

void parseConference(DB *db)
{
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
		db->conferences.push_back(conference);
	}
	std::sort(db->conferences.begin(), db->conferences.end(), conferenceCompare);
	fprintf(stderr, "ok\n");
	fclose(fp);
}

void parseJournal(DB *db)
{
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
		db->journals.push_back(journal);
	}
	std::sort(db->journals.begin(), db->journals.end(), JournalCompare);
	fprintf(stderr, "ok\n");
	fclose(fp);
}

DB *loadDB()
{
	DB *db = new DB();
	parseAuthor(db);
	parsePaper(db);
	parsePaperAuthor(db);
	parseConference(db);
	parseJournal(db);

	return db;
}
