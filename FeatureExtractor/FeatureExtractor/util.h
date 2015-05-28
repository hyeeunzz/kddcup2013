#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>
#include <algorithm>
#include "db.h"

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2);
void generateSmallDomain(DB *db);

#endif