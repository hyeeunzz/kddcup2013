#include "command.h"

bool paperAuthorCompare1(const PaperAuthor *a, const PaperAuthor *b) { return a->author_id < b->author_id; }
bool paperAuthorCompare2(const PaperAuthor *a, const PaperAuthor *b) { return a->paper_id < b->paper_id; }

void printPaper(DB *db, int paper_id, bool print_coauthor)
{
	Paper *paper = db->getPaperById(paper_id);
	if (paper == NULL){
		printf("No such Paper with id %d\n", paper_id);
	}
	else {
		if (print_coauthor) puts("==== Paper ====");
		printf("pid     : %d\n", paper->id);
		printf("title   : %s\n", paper->title.c_str());
		printf("year    : %d\n", paper->year);
		printf("keyword : %s\n", paper->keywords.c_str());

		if (paper->conference_id != 0) {
			Conference *conference = db->getConferenceById(paper->conference_id);
			printf("c_id    : %d\n", paper->conference_id);
			printf("c_short : %s\n", conference->shortname.c_str());
			printf("c_full  : %s\n", conference->fullname.c_str());
			//printf("c_home  : %s\n", conference->homepage.c_str());
		}
		if (paper->journal_id != 0){
			Journal *journal = db->getJournalById(paper->journal_id);
			printf("j_id    : %d\n", paper->journal_id);
			printf("j_short : %s\n", journal->shortname.c_str());
			printf("j_full  : %s\n", journal->fullname.c_str());
			//printf("j_home  : %s\n", journal->homepage.c_str());
		}

		if (print_coauthor) {
			char buf[128];
			printf("\n> Want to print out coauthors of this paper? (y/n) ");
			fgets(buf, 128, stdin);
			if (buf[0] == 'y' || buf[0] == 'Y'){
				puts("");
				puts("==== Coauthors ====");
				std::vector<PaperAuthor*> paper_authors;
				db->getPaperAuthorsByPaperId(paper_authors, paper_id);
				std::sort(paper_authors.begin(), paper_authors.end(), paperAuthorCompare1);
				for (int i = 0; i < paper_authors.size(); i++){
					printAuthor(db, paper_authors[i]->author_id, false);
					puts("==============================");
				}
			}
		}
	}
}

void printAuthor(DB *db, int author_id, bool print_paper)
{
	Author *author = db->getAuthorById(author_id);
	if (author == NULL){
		printf("No such Author with id %d\n", author_id);
	}
	else {
		if (print_paper) puts("==== Author ====");
		printf("aid     : %d\n", author->id);
		printf("name    : %s\n", author->name.c_str());
		printf("affilat : %s\n", author->affiliation.c_str());

		if (print_paper) {
			char buf[128];
			printf("\n> Want to print out papers of this author? (y/n) ");
			fgets(buf, 128, stdin);
			if (buf[0] == 'y' || buf[0] == 'Y'){
				puts("");
				puts("==== Papers ====");
				std::vector<PaperAuthor*> paper_authors;
				db->getPaperAuthorsByAuthorId(paper_authors, author_id);
				std::sort(paper_authors.begin(), paper_authors.end(), paperAuthorCompare2);
				for (int i = 0; i < paper_authors.size(); i++){
					printPaper(db, paper_authors[i]->paper_id, false);
					puts("==============================");
				}
			}
		}
	}
}

void printPaperAuthor(DB *db, int paper_id, int author_id){
	std::vector<PaperAuthor *> paper_authors;
	db->getPaperAuthorsById(paper_authors, paper_id, author_id);
	if (paper_authors.size() == 0){
		printf("No such PaperAuthor with paper_id %d and author_id %d\n", paper_id, author_id);
	}
	else {
		puts("==== PaperAuthors ====");
		for (int i = 0; i < paper_authors.size(); i++){
			printf("pid     : %d\n", paper_authors[i]->paper_id);
			printf("aid     : %d\n", paper_authors[i]->author_id);
			printf("name    : %s\n", paper_authors[i]->name.c_str());
			printf("affilat : %s\n", paper_authors[i]->name.c_str());
			puts("==============================");
		}
		puts("");
		puts("==== Paper ====");
		printPaper(db, paper_id, false);
		puts("");
		puts("==== Author ====");
		printAuthor(db, author_id, false);
	}
}

void executeInterpreter(DB *db)
{
	puts("\nSearch interpreter...");
	puts("examples)");
	puts("paper by id : p {id}");
	puts("author by id : a {id}");
	puts("paper_author by id : pa {pid} {aid}");

	char buf[256];
	puts("");
	while (true) {
		printf("> ");
		fgets(buf, 256, stdin);
		if (buf[0] == 'p' && buf[1] == ' ') {
			// paper
			int paper_id;
			sscanf_s(buf + 2, "%d", &paper_id);
			printPaper(db, paper_id, true);
		}
		else if (buf[0] == 'a' && buf[1] == ' ') {
			// author
			int author_id;
			sscanf_s(buf + 2, "%d", &author_id);
			printAuthor(db, author_id, true);
		}
		else if (buf[0] == 'p' && buf[1] == 'a' && buf[2] == ' '){
			// paper author
			int paper_id, author_id;
			sscanf_s(buf + 3, "%d%d", &paper_id, &author_id);
			printPaperAuthor(db, paper_id, author_id);
		}
		else if (buf[0] == '\n') {

		}
		else if (strcmp(buf, "exit")){
			puts("Bye!");
			break;
		}
		else {
			printf("Unknown command.\n");
		}
	}
}