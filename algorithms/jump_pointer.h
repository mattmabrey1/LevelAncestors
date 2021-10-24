#ifndef JUMP_POINTER_H
#define JUMP_POINTER_H

#include "../la_common.h"

/************************************************************************/
/* Jump-Pointers Algo Data Structures					*/
/************************************************************************/
unsigned int **jump_table;
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void jump_pointer_preprocessing();

int jump_pointer_query(int query_node, int query_level);

void add_jump_pointer_leaf(int parent);

void remove_jump_pointer_leaf(int leaf);
/************************************************************************/
#endif