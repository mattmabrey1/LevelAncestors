#ifndef JUMP_LADDER_H
#define JUMP_LADDER_H

#include "../la_common.h"

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void jump_ladder_preprocessing();

int jump_ladder_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
    void add_jump_ladder_leaf(int parent, int leaf, bool is_left_child);

    void remove_jump_ladder_leaf(int leaf);
#endif
/************************************************************************/
#endif