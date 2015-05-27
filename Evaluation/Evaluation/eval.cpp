#include "eval.h"
#include<stdlib.h>
#include<ctype.h>
/*
#define MAX_A 2293837
#define MAX_P 2259021
#define MAX_C 5222
#define MAX_J 22228
*/
#define MAX_Tested 1496
#define MAX_Answer 3739

#define DATA_PATH "../../data"

/* global variable*/
Tested *tested[MAX_Tested];
Answer *answer[MAX_Answer];

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

int openTested(Tested *tested[])
{	
	FILE *fp = getFile("Tested");
	
	int tested_index  = 0;

	while (!feof(fp)) {
		fscanf_s(fp, "%d,", &tested[tested_index]->author_id);

		char c = 0;
		int id_index = 0;
		bool check = true;

		while (true) {
			c = getc(fp);
			if (c == ' ') {
				id_index ++;
				check = true;
			}
			else if (isprint(c)) {
				if ( c >= '0' && c <= '9') {
					if (check) {
						tested[tested_index]->paper_id[id_index] = c - '0';
						check = false;
					}
					else {
						tested[tested_index]->paper_id[id_index] = tested[tested_index]->paper_id[id_index] * 10 + (c - '0');
					}
				}
			}
			else if (c == '\n' || c == EOF) {
				tested[tested_index]->paper_id[id_index + 1] = 0;
				break;
			}
		}

		tested_index ++;

		if (c == EOF) break;
	}
	
	fprintf(stderr, "ok\n");
	fclose(fp);

	return tested_index;
}

int openAnswer(Answer *answer[])
{
	FILE *fp = getFile("Answer");
	
	int answer_index  = 0;

	while (!feof(fp)) {
		fscanf_s(fp, "%d,", &answer[answer_index]->author_id);

		char c = 0;
		int id_index = 0;
		int state = 0; // 0: confirmed, 1: deleted
		bool check = true;

		while (true) {
			c = getc(fp);
			if (c == ',') {
				if (state == 0) {
					state = 1;
					answer[answer_index]->confirmed_paper_id[id_index + 1] = 0;
					id_index = 0;
				}
				else {
					fprintf(stderr, "Parsing error - %d!\n", answer[answer_index]->author_id);
					exit(1);
				}
			}
			else if (c == ' ') {
				id_index ++;
				check = true;
			}
			else if (isprint(c)) {
				if (state == 0) {
					if ( c >= '0' && c <= '9') {
						if (check) {
							answer[answer_index]->confirmed_paper_id[id_index] = c - '0';
							check = false;
						}
						else {
							answer[answer_index]->confirmed_paper_id[id_index] = answer[answer_index]->confirmed_paper_id[id_index] * 10 + (c - '0');
						}
					}
				}
				else {
					if ( c >= '0' && c <= '9') {
						if (check) {
							answer[answer_index]->deleted_paper_id[id_index] = c - '0';
							check = false;
						}
						else {
							answer[answer_index]->deleted_paper_id[id_index] = answer[answer_index]->deleted_paper_id[id_index] * 10 + (c - '0');
						}
					}
				}
			}
			else if (c == '\n' || c == EOF) break;
		}

		answer_index ++;

		if (c == EOF) break;
	}
	
	fprintf(stderr, "ok\n");
	fclose(fp);
	
	return answer_index;
}

int getAnswer(int author_id)
{
	size_t left = 0;
	size_t right = MAX_Answer - 1;

	while (left <= right){
		size_t mid = (left + right) / 2;
		if (answer[mid]->author_id == author_id){
			return mid;
		}
		else if (answer[mid]->author_id < author_id){
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
}

void evaluation(float score[])
{
	int num_of_tested = openTested(tested);
	int num_of_answer = openAnswer(answer);

	int num_of_confirm =0;

	for (int i = 0; i < num_of_tested; i ++) {
		int answer_id = getAnswer(tested[i]->author_id);
		for (int j = 0; tested[i]->paper_id[j] != 0; j ++) {
			for (int k = 0; answer[answer_id]->confirmed_paper_id[k] != 0; k ++) {
				if (tested[i]->paper_id[j] == answer[answer_id]->confirmed_paper_id[k]) {
					num_of_confirm ++;
					score[i] += (float) num_of_confirm / (j + 1);
				}
				else if (tested[i]->paper_id[j] == answer[answer_id]->deleted_paper_id[k]) {
					break;
				}
			}
		}

		score[i] /= (float) num_of_confirm;
	}
}

void main()
{
	float score[MAX_Tested] = {0.0,};
	FILE *fp;
	errno_t err;

	if ((err = fopen_s(&fp, "score.txt", "w")) != 0){
		fprintf(stderr, "Error occured!\n");
		exit(1);
	}

	evaluation(score);

	for (int i = 0; i < MAX_Tested; i ++) {
		fprintf_s(fp, "%.5f\n", score[i]);
	}

	fclose(fp);
}