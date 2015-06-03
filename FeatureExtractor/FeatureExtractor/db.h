#ifndef _DB_H_
#define _DB_H_

#include <vector>
#include <algorithm>
#include <ctime>
#include <string>

typedef struct _Author {
	int id;
	std::string name;
	std::string affiliation;
} Author;

typedef struct _Paper {
	int id;
	std::string title;
	int year;
	int conference_id;
	int journal_id;
	std::string keywords;
} Paper;

typedef struct _PaperAuthor {
	int paper_id;
	int author_id;
	std::string name;
	std::string affiliation;
} PaperAuthor;

typedef struct _Conference {
	int id;
	std::string shortname;
	std::string fullname;
	std::string homepage;
} Conference;

typedef struct _Journal {
	int id;
	std::string shortname;
	std::string fullname;
	std::string homepage;
} Journal;

// to support binary search of author_id in PaperAuthor
typedef struct _PaperAuthorIndex {
public:
	int author_id;
	size_t paper_author_index;
} PaperAuthorIndex;

class DB {
public:
	char datapath[128];
	std::vector<Author*> authors;
	std::vector<Paper*> papers;
	std::vector<PaperAuthor*> paper_authors;
	std::vector<Conference*> conferences;
	std::vector<Journal*> journals;
	std::vector<PaperAuthorIndex*> paper_author_index; // author_id to index
	Author* getAuthorById(int id);
	Paper* getPaperById(int id);
	Conference* getConferenceById(int id);
	Journal* getJournalById(int id);
	void getPaperAuthorsByPaperId(std::vector<PaperAuthor*> &result, int paper_id);
	void getPaperAuthorsByAuthorId(std::vector<PaperAuthor*> &result, int author_id);
	void getPaperAuthorsById(std::vector<PaperAuthor*> &result, int paper_id, int author_id);
};

FILE *getFile(char *datapath, char *filename);
DB *loadDB(char *datapath);

#endif