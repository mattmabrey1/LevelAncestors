#ifndef LADDER_H
#define LADDER_H

#include "../la_common.h"

/************************************************************************/
/* Ladders Algo Data Structures						*/
/************************************************************************/
struct ladder_index_node
{
  unsigned int no;
  unsigned int pos;
} *ladder_index;

#if LA_ALGORITHM == STATIC
  // Static data structures
  unsigned int **ladder_table;

  unsigned int *ladder_roots;
#else
  // Dynamic data structures

#endif
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void ladder_preprocessing();

int ladder_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
  void add_ladder_leaf(int parent, int leaf, bool is_left_child);

  void remove_ladder_leaf(int leaf);
#endif
/************************************************************************/
#endif