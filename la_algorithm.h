#define TABLE 1
#define JUMP_POINTER 2
#define LADDER 3
#define JUMP_LADDER 4
#define MACRO_MICRO 5
#define MENGHANI_MATANI 6
#define HAGERUP 7

// In case compilation doesn't define preprocessing directive LA_ALGORITHM to determine the algorithm
#ifndef LA_ALGORITHM
    #define LA_ALGORITHM TABLE
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
    #endif

    /************************************************************************/
    /* Function Prototypes						*/
    /************************************************************************/

    void la_preprocessing();

    void la_process_queries();

    void la_initialize();
    
    int la_query(int query_node, int query_level);

    void validate_query_answer(int query_node, int query_answer);
    /************************************************************************/

#endif
