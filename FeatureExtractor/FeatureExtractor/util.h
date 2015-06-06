#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>
#include <algorithm>
#include "db.h"

double levenshteinDistance(const std::string& s1, const std::string& s2);
double jaroDistance(const std::string &s1, const std::string &s2);
void generateSmallDomain(DB *db);
void stringToLower(std::string& s);
std::vector<std::string> split(const std::string &str, const char *wschars);
void generateConferenceSimilarityMatrix(DB *db);
void generateJournalSimilarityMatrix(DB *db);

#endif