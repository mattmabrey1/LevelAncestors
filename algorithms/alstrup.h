#ifndef ALSTRUP_H
#define ALSTRUP_H

#include "../la_common.h"

/************************************************************************/
/* Alstrup-Holm Algo Data Structures					*/
/************************************************************************/


#if LA_ALGORITHM == DYNAMIC
  // Dynamic data structures
  /* Creates the type vec_vec_t for storing vectors */
  vec_int_t jumpM;  // contains the first proper anestor of v whose depth is divisible by M.
  vec_vec_t levelancM;  // for each micro tree, levelancM[u][], store the first u ancestors to micro_root(u)

  vec_int_t anc;
  vec_vec_t node_table; 

  vec_vec_t bitindex;

  // Data structures for tree induced from taking only the macro nodes
  vec_vec_t jump;
  vec_vec_t levelanc;

  struct macro_struct
  {
      int parent;
      vec_int_t* children;
      int depth;
      int size;
      int tree_index;
  };
  typedef struct macro_struct macro_node;

  typedef vec_t(macro_node*) macro_vec_t;

  macro_vec_t macro_tree; // tree induced from taking only the macro nodes
#endif

int r0, M, N;

/************************************************************************/


/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void alstrup_preprocessing();

void build_bitindex(int N);

void recurse_micro(int v);

int calc_subtree(int v);

int calc_macro_subtree(int v);

int calc_rank(int depth, int subtree_size);

void calculate_jumpM();

void init_micro();

void add_macro_node(int v);

void preprocess_macro_tree();

int LA_macro_tree(int v, int d);

int LA_micro(int v, int d);

int alstrup_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
  void add_alstrup_leaf(int parent, int leaf, bool is_left_child);

  void add_leaf_step(int v, int p);

  void remove_alstrup_leaf(int leaf);
#endif
/************************************************************************/
#endif