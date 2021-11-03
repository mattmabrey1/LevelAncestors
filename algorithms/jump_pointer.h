#ifndef JUMP_POINTER_H
#define JUMP_POINTER_H

#include "../la_common.h"

/************************************************************************/
/* Jump-Pointers Algo Data Structures					*/
/************************************************************************/
#if LA_ALGORITHM == STATIC
    // Static data structures
    unsigned int **jump_table;
    
#else
    // Dynamic data structures
    vec_vec_t jump_table;
#endif
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void jump_pointer_preprocessing();

int jump_pointer_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
    void add_jump_pointers(int node);

    void add_jump_pointer_leaf(int parent, int leaf, bool is_left_child);

    void remove_jump_pointer_leaf(int leaf);
#endif
/************************************************************************/
#endif