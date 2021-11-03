#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>
#include <stdbool.h>

#define DEBUG_INPUT	0
#define DEBUG_TABLE	0
#define DEBUG_JUMP	0
#define DEBUG_LADDER	0
#define DEBUG_MM	0
#define DEBUG_MM2	0
#define DEBUG_MMQUERY	0
#define DEBUG_MMTABLE	0
#define DEBUG_GFQUERY 0

#ifndef DEBUG_RESULTS
  #define DEBUG_RESULTS 0
#endif

// Prototype functions

void *alloc(size_t size);

unsigned int log_base2(unsigned int x);

int max(int a, int b);

int min(int a, int b);

// The highest power of 2 that divides n with no remainder 
unsigned int highest_pwrOf2_divides(unsigned int x);

// The highest power of 2 less than or equal to n
unsigned int highest_pwrOf2(unsigned int x);

// Performs the regular modulo operation with just a bit operation. y must be a power of 2
unsigned int mod_pwr2(int x, int y);
/************************************************************************/

int query_num, query_node, query_level, query_answer;

struct timespec start_proc, end_proc, start_query, end_query;

#endif



