#include <vector>
#include <algorithm>

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

class DB {
public:
	std::vector<Author> authors;
	std::vector<Paper> papers;
	std::vector<PaperAuthor> paper_authors;
	std::vector<Conference> conferences;
	std::vector<Journal> journals;
};

void parseAuthor(DB *db);
void parsePaper(DB *db);
void parsePaperAuthor(DB *db);
void parseConference(DB *db);
void parseJournal(DB *db);
DB *loadDB();
