#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "db.h"

void executeInterpreter(DB *db);
void printPaper(DB *db, int id, bool print);
void printAuthor(DB *db, int id, bool print);

#endif