#ifndef TABLE_H
#define TABLE_H

#include "../la_common.h"

/************************************************************************/
/* Table Algorithm Data structures					*/
/************************************************************************/
unsigned int **big_table;
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void table_preprocessing();

int table_query(int query_node, int query_level);

void add_table_leaf(int parent);

void remove_table_leaf(int leaf);
/************************************************************************/
#endif