#include "la_algorithm.h"

void la_initialize(){

    #if LA_ALGORITHM == MENGHANI_MATANI

        depth_count = (int*)calloc(n, sizeof(int));
        depth_meta_count = (int*)calloc(n, sizeof(int));
        depth_size = (int*)calloc(n, sizeof(int));
        depth_meta_size = (int*)calloc(n, sizeof(int));

        depth_size[0] = 1;

    #elif LA_ALGORITHM == HAGERUP

        euler_arr = alloc((2 * n - 2) * sizeof(int));
        rep_arr = alloc(n * sizeof(int));

        rep_arr[0] = 0;
        euler_arr[0] = 0;

        K = 5;
        max_depth = -1;
    #endif
}

// Preprocess that algorithm chosen
void la_preprocessing(){

    #if LA_ALGORITHM == TABLE
        table_preprocessing();
    #elif LA_ALGORITHM == JUMP_POINTER
        jump_pointer_preprocessing();
    #elif LA_ALGORITHM == LADDER
        ladder_preprocessing();
    #elif LA_ALGORITHM == JUMP_LADDER
        jump_ladder_preprocessing();
    #elif LA_ALGORITHM == MACRO_MICRO
        macro_micro_preprocessing();
    #elif LA_ALGORITHM == MENGHANI_MATANI
        menghani_matani_preprocessing();
    #elif LA_ALGORITHM == HAGERUP
        hagerup_preprocessing();
    #endif
}

// Read in and process the number of queries and then all of the queries from standard input
void la_process_queries(){

    int i;
    char c;

    c = getchar();
    scanf("%d\n", &query_num);
    for (i = 0; i < query_num; i++)
    {
        scanf("%d, %d\n", &query_node, &query_level);

        #if LA_ALGORITHM == TABLE
            query_answer = table_query(query_node, query_level);
        #elif LA_ALGORITHM == JUMP_POINTER
            query_answer = jump_pointer_query(query_node, query_level);
        #elif LA_ALGORITHM == LADDER
            query_answer = ladder_query(query_node, query_level);
        #elif LA_ALGORITHM == JUMP_LADDER
            query_answer = jump_ladder_query(query_node, query_level);
        #elif LA_ALGORITHM == MACRO_MICRO
            query_answer = macro_micro_query(query_node, query_level);
        #elif LA_ALGORITHM == MENGHANI_MATANI
            query_answer = menghani_matani_query(query_node, query_level);
        #elif LA_ALGORITHM == HAGERUP
            query_answer = hagerup_query(query_node, query_level);
        #endif

        #if DEBUG_RESULTS
            printf("LA(%d, %d) = %d\n", query_node, query_level, query_answer);

            validate_query_answer(query_node, query_answer);
        #endif
    }
}

int la_query(int query_node, int query_level){

    #if LA_ALGORITHM == TABLE
        return table_query(query_node, query_level);
    #elif LA_ALGORITHM == JUMP_POINTER
        return jump_pointer_query(query_node, query_level);
    #elif LA_ALGORITHM == LADDER
        return ladder_query(query_node, query_level);
    #elif LA_ALGORITHM == JUMP_LADDER
        return jump_ladder_query(query_node, query_level);
    #elif LA_ALGORITHM == MACRO_MICRO
        return macro_micro_query(query_node, query_level);
    #elif LA_ALGORITHM == MENGHANI_MATANI
        return menghani_matani_query(query_node, query_level);
    #elif LA_ALGORITHM == HAGERUP
        return hagerup_query(query_node, query_level);
    #endif
}

void validate_query_answer(int query_node, int query_answer){

    int curr_parent = query_node;

 	while(curr_parent >= 0){

 		if(tree[curr_parent].depth == query_level){
				
 			if(curr_parent != query_answer){
 				printf("ERROR: REAL LA(%d, %d) = %d\n", query_node, query_level, curr_parent);
                exit(-1);
 			}
 			return;
 		}

 		curr_parent = tree[curr_parent].parent;
	}

	if(curr_parent == -1){
        printf("ERROR: CORRECT ANSWER TO LA(%d, %d) NOT FOUND! ALGORITHM RETURNED %d\n", query_node, query_level, query_answer);
        exit(-1);
    }	
}