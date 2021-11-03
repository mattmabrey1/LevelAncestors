#ifndef HAGERUP_H
#define HAGERUP_H

#include "../la_common.h"

/************************************************************************/
/* Hagerup Algo Data Structures					*/
/************************************************************************/
struct valley_struct
{
  int x;
  int low;
  int high;
};
typedef struct valley_struct valley_tuple;

int K, K_prime, K_max;
int max_depth, y_min, y_max;

#if LA_ALGORITHM == STATIC

  // Static data structures
  int* height_arr;    // One-difference array that holds the y value of the current place in the Euler tour representation
  int* euler_arr;     // ith vertex in the Euler tour 
  int* rep_arr;     // rep_arr(v) returns the index i of the last occurence of v in the height_arr

  int* valley;    // valley[x] is the valley for point x where a valley is the deepest right most point reachable without going above y of x
  int** ladder;
  int* jump;
#else
  // Dynamic data structures

#endif
/************************************************************************/


/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void hagerup_preprocessing();

int hagerup_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
  void add_hagerup_leaf(int parent, int leaf, bool is_left_child);

  void remove_hagerup_leaf(int leaf);
#endif
/************************************************************************/
#endif