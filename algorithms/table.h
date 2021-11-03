#ifndef TABLE_H
#define TABLE_H

#include "../la_common.h"

/************************************************************************/
/* Table Algorithm Data structures					*/
/************************************************************************/
#if LA_ALGORITHM == STATIC
    // Static data structures
    unsigned int **big_table;

#else
    // Dynamic data structures

#endif
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void table_preprocessing();

int table_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
    void add_table_leaf(int parent, int leaf, bool is_left_child);

    void remove_table_leaf(int leaf);
#endif
/************************************************************************/
#endif