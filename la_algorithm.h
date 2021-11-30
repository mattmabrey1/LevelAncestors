#define TABLE 1
#define JUMP_POINTER 2
#define LADDER 3
#define JUMP_LADDER 4
#define MACRO_MICRO 5
#define MENGHANI_MATANI 6
#define HAGERUP 7
#define BENAMRAM 8
#define ALSTRUP 9

// In case compilation doesn't define preprocessing directive LA_ALGORITHM to determine the algorithm
#ifndef LA_ALGORITHM
    #define LA_ALGORITHM TABLE
#endif

// String for the name of the algorithm
#if LA_ALGORITHM == TABLE 
    #define LA_ALGORITHM_STRING "Table"
#elif LA_ALGORITHM == JUMP_POINTER 
    #define LA_ALGORITHM_STRING "Jump Pointer"
#elif LA_ALGORITHM == LADDER 
    #define LA_ALGORITHM_STRING "Ladder"
#elif LA_ALGORITHM == JUMP_LADDER 
    #define LA_ALGORITHM_STRING "Jump Ladder"
#elif LA_ALGORITHM == MACRO_MICRO 
    #define LA_ALGORITHM_STRING "Macro Micro"
#elif LA_ALGORITHM == MENGHANI_MATANI 
    #define LA_ALGORITHM_STRING "Menghani & Matani"
#elif LA_ALGORITHM == HAGERUP 
    #define LA_ALGORITHM_STRING "Hagerup"
#elif LA_ALGORITHM == BENAMRAM 
    #define LA_ALGORITHM_STRING "Ben-Amram"
#elif LA_ALGORITHM == ALSTRUP 
    #define LA_ALGORITHM_STRING "Alstrup"
#endif

#ifndef LA_DYNAMIC
    #define STATIC LA_ALGORITHM
    #define DYNAMIC -1
#else
    #define STATIC -1
    #define DYNAMIC LA_ALGORITHM
#endif

struct node_struct
{
    int parent;
    int left, right;	/* Children pointers	*/
    int depth;

#if LA_ALGORITHM == DYNAMIC
    int leaf_pos;
#endif

#if LA_ALGORITHM == MENGHANI_MATANI
    int label;
#elif LA_ALGORITHM == ALSTRUP
    int size;
    int rank;
    int micro_tree;
    int macro_index;
#endif
};
typedef struct node_struct node;

#if LA_ALGORITHM == DYNAMIC
    #include "vec.h"

    /* Creates the type node_vec_t for storing Nodes */
    typedef vec_t(node*) node_vec_t;

    /* Creates the type vec_vec_t for storing vectors */
    typedef vec_t(vec_int_t*) vec_vec_t;
#endif

#ifndef LA_ALGORITHM_H

    #define LA_ALGORITHM_H

    #include "la_common.h"

    #if LA_ALGORITHM == TABLE
        #include "algorithms/table.h"
    #elif LA_ALGORITHM == JUMP_POINTER
        #include "algorithms/jump_pointer.h"
    #elif LA_ALGORITHM == LADDER
        #include "algorithms/ladder.h"
    #elif LA_ALGORITHM == JUMP_LADDER
        #include "algorithms/jump_ladder.h"
    #elif LA_ALGORITHM == MACRO_MICRO
        #include "algorithms/macro_micro.h"
    #elif LA_ALGORITHM == MENGHANI_MATANI
        #include "algorithms/menghani_matani.h"
    #elif LA_ALGORITHM == HAGERUP
        #include "algorithms/hagerup.h"
    #elif LA_ALGORITHM == ALSTRUP
        #include "algorithms/alstrup.h"
    #endif

    /************************************************************************/
    /* Function Prototypes						*/
    /************************************************************************/
    void la_preprocessing();

    void la_initialize();

    void la_execute();
    
    int la_query(int query_node, int query_level);

    void validate_query_answer(int query_node, int query_answer);

    void add_leaf(int parent);
    /************************************************************************/

    /************************************************************************/
    /* Tree Data Structures							*/
    /************************************************************************/

    unsigned int n;		                        /* The number of nodes in the tree			                                */

    #if LA_ALGORITHM == STATIC
        node *tree;	                            /* Our tree, which will be also an array of nodes	*/
    #else 
        node_vec_t tree;
        vec_int_t leaves;

        int num_of_queries, num_of_leaf_additions;  /* The number of queries and leaf additions exectued over the program		*/
        float ratio;
    #endif
#endif
