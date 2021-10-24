#ifndef MACRO_MICRO_H
#define MACRO_MICRO_H

#include "../la_common.h"

/************************************************************************/
/* Macro-Micro Algo Data Structures					*/
/************************************************************************/
/* The auxiliary variable in the mm_table structure holds different	*/
/* values for different kinds of nodes.					*/
/* For MACRO nodes: It holds a pointer to a JUMP ancestor node		*/
/* For MICRO nodes: It holds a pointer to the MICROROOT node		*/
/* For MICROROOT nodes: It holds the signature of the tree		*/
/* For JUMP nodes: index in the table holding the pointers		*/
/************************************************************************/
typedef enum {NOTHING, MACRO, MICRO, MICROROOT, JUMP} node_type;

struct mm_struct
{
  node_type type;
  unsigned int aux;
} *mm_table;

unsigned int ***micro_tables;	/* Table to keep the table for the micro trees	*/
unsigned int **mm_jump;		/* Table to keep the jump pointers		*/
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void create_micro_table(unsigned int signature, int size);

void macro_micro_preprocessing();

int macro_micro_query(int query_node, int query_level);

void add_macro_micro_leaf(int parent);

void remove_macro_micro_leaf(int leaf);
/************************************************************************/
#endif