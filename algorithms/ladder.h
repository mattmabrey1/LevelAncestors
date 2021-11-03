#ifndef LADDER_H
#define LADDER_H

#include "../la_common.h"

/************************************************************************/
/* Ladders Algo Data Structures						*/
/************************************************************************/
unsigned int **ladder_table;
struct ladder_index_node
{
  unsigned int no;
  unsigned int pos;
} *ladder_index;
unsigned int *ladder_roots;
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void ladder_preprocessing();

int ladder_query(int query_node, int query_level);
/************************************************************************/
#endif