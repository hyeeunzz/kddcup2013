#ifndef _DB_H_
#define _DB_H_

#include <vector>
#include <algorithm>
#include <ctime>

typedef struct _Author {
	int id;
	char name[96];
	char affiliation[144];
} Author;

typedef struct _Paper {
	int id;
	char title[144];
	int year;
	int conference_id;
	int journal_id;
	char keywords[144];
} Paper;

typedef struct _PaperAuthor {
	int paper_id;
	int author_id;
	char name[96];
	char affiliation[144];
} PaperAuthor;

typedef struct _Conference {
	int id;
	char shortname[64];
	char fullname[144];
	char homepage[144];
} Conference;

typedef struct _Journal {
	int id;
	char shortname[64];
	char fullname[144];
	char homepage[144];
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