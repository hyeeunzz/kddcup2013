#ifndef _EVAL_H_
#define _EVAL_H_

#include<stdio.h>

typedef struct _Tested {
	int author_id;
	int paper_id[144];
} Tested;

typedef struct _Answer {
	int author_id;
	int confirmed_paper_id[144];
	int deleted_paper_id[144];
} Answer;

#endif