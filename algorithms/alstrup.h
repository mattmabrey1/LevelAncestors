#ifndef ALSTRUP_H
#define ALSTRUP_H

#include "../la_common.h"

/************************************************************************/
/* Alstrup-Holm Algo Data Structures					*/
/************************************************************************/

#if LA_ALGORITHM == DYNAMIC
  // Dynamic data structures

  int r0 = 0, M = 0, N = 0, max_depth = 0;

  node_vec_t tree;

  vec_int_t jumpM;
  vec_vec_t levelancM;

  vec_int_t anc;
  vec_vec_t node_table;

  vec_vec_t bitindex;

  vec_int_t leaves;
#endif
/************************************************************************/


/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void alstrup_preprocessing();

void build_bitindex(int N);

void recurse_micro(int v);

int calc_subtree(int v);

void calc_rank(int v);

void calculate_jumpM();

void init_micro();

int LA_micro(int v, int d);

int alstrup_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
  void add_alstrup_leaf(int parent, int leaf, bool is_left_child);

  void remove_alstrup_leaf(int leaf);
#endif
/************************************************************************/
#endif