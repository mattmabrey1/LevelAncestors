#ifndef MENGHANI_MATANI_H
#define MENGHANI_MATANI_H

#include "../la_common.h"

/************************************************************************/
/* Menghani-Matani Algo Data Structures					*/
/************************************************************************/
/* depth_count holds the current number of nodes at each depth array	    */
/* depth_size holds the max number of nodes at each depth array	    */
/* depth_arr is a 2D array containing the nodes at each depth		    */
/************************************************************************/

// minimum number of nodes to justify Log(Sqrt(N)) binary search. 
#define SQRT_SEARCH_MINIMUM 65536

#if LA_ALGORITHM == STATIC
    // Static data structures

    int* depth_count;           // An array to keep track of how full each subarray is in the 2D depth_arr
    int* depth_meta_count;      // An array to keep track of how full each subarray is in the 2D depth_metaarrays

    int* depth_size;            // An array of the total number of nodes at each given tree depth level
    int* depth_meta_size;       // An array of the total number of nodes for each depth's meta array

    int** depth_arr;            // A 2D array of each tree depth level and all nodes at that depth in ascending label value

    int** depth_metaarray_val;  // A 2D meta array to speed up binary search for very large arrays by tracking every ceil(sqrt(depth_size[d])) node's value
    int** depth_metaarray_pos;  // A 2D meta array to speed up binary search for very large arrays by tracking every ceil(sqrt(depth_size[d])) node's position in depth_arr 
    
#else
    // Dynamic data structures

    vec_int_t depth_count;           // A vector to keep track of how full each subarray is in the 2D depth_arr

    vec_int_t depth_size;            // A vector of the total number of nodes at each given tree depth level
    vec_int_t depth_meta_size;       // A vector of the total number of nodes for each depth's meta array

    vec_vec_t depth_arr;            // A 2D vector of each tree depth level and all nodes at that depth in ascending label value

    vec_vec_t depth_metaarray_val;  // A 2D meta vector to speed up binary search for very large vectors by tracking every ceil(sqrt(depth_size[d])) node's value
    vec_vec_t depth_metaarray_pos;  // A 2D meta vector to speed up binary search for very large vectors by tracking every ceil(sqrt(depth_size[d])) node's position in depth_arr

    vec_int_t new_labels; 
    
    int num_of_unlabeled_nodes;
    int unlabeled_nodes_threshold;
#endif
/************************************************************************/

/************************************************************************/
/* Function Prototypes						*/
/************************************************************************/
void menghani_matani_preprocessing();

void meta_binary_search(int d, int* l, int* r, int goal, int max);

int binary_search(int d, int l, int r, int goal, int max);

int menghani_matani_query(int query_node, int query_level);

#if LA_ALGORITHM == DYNAMIC
    void add_menghani_matani_leaf(int parent, int leaf, bool is_left_child);

    int recompute_labels(int node_idx, int label);

    void reorder_depth_arr(int depth);

    void remove_menghani_matani_leaf(int leaf);
#endif
/************************************************************************/
#endif