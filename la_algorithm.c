#include "la_algorithm.h"

void init_node(node* node){

    node->parent = node->left = node->right = node->depth = -1;

    #if LA_ALGORITHM == DYNAMIC
        node->leaf_pos = -1;
    #endif

    #if LA_ALGORITHM == MENGHANI_MATANI
        node->label = -1;
    #elif LA_ALGORITHM == ALSTRUP
        node->size = node->rank = node->micro_tree = -1;
    #endif
}

void la_initialize(){

    int i;

    #if LA_ALGORITHM == STATIC

        tree = alloc(n*(sizeof(node)));

        for (i = 0; i < n; i++){
            init_node(&tree[i]);
        }

        tree[0].depth = 0;

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

    #elif LA_ALGORITHM == DYNAMIC

        num_of_queries = num_of_leaf_additions = 0;

        vec_init(&tree);
        vec_init(&leaves);

        vec_reserve(&tree, n);
	    vec_reserve(&leaves, (1 << log_base2(n)));
        
        vec_push(&leaves, 0);

        node* new_node;

        for(i = 0; i < tree.capacity; i++)
        {
            new_node = alloc(sizeof(node));
            vec_push(&tree, new_node);
            init_node(new_node);
        }
        
        tree.data[0]->depth = tree.data[0]->leaf_pos = 0;

        #if LA_ALGORITHM == MENGHANI_MATANI

            vec_init(&depth_size); 

            vec_init(&depth_meta_size); 

            vec_init(&new_labels);

            vec_push(&depth_size, 1);

            num_of_unlabeled_nodes = 0;

            unlabeled_nodes_threshold = (int)floor(tree.length * 0.5); 

        #elif LA_ALGORITHM == ALSTRUP

            r0 = (int)floor(log_base2(log_base2(n)) - 1);
            M = (1 << r0);
            N = 0;

            tree.data[0]->micro_tree = 0;
        #endif 

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
    #elif LA_ALGORITHM == ALSTRUP
        alstrup_preprocessing();
    #endif
}

void la_execute(){

    int query_node, query_level, query_answer;

    #if LA_ALGORITHM == STATIC

        while(la_operations > 0){

            la_operations--;

            query_node = rand() % n;
            query_level = rand() % (tree[query_node].depth + 1);

            query_answer = la_query(query_node, query_level);

            #if DEBUG_RESULTS
                printf("LA(%d, %d) = %d\n", query_node, query_level, query_answer);

                validate_query_answer(query_node, query_answer);
            #endif
        }

    #elif LA_ALGORITHM == DYNAMIC

        while(la_operations > 0){

            la_operations--;

            if((rand() & 1) == 0){

                query_node = rand() % tree.length;
                query_level = rand() % (tree.data[query_node]->depth + 1);

                query_answer = la_query(query_node, query_level);

                #if DEBUG_RESULTS
                    printf("LA(%d, %d) = %d\n", query_node, query_level, query_answer);

                    validate_query_answer(query_node, query_answer);
                #endif

                num_of_queries++;
            }
            else{
                // Add leaf
                add_leaf(leaves.data[rand() % leaves.length]);

                num_of_leaf_additions++;
            }
        }
        
    #endif 
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
    #elif LA_ALGORITHM == ALSTRUP
        return alstrup_query(query_node, query_level);
     #endif
}

void validate_query_answer(int query_node, int query_answer){

    int curr_parent = query_node;
    int node_depth, query_level;

 	while(curr_parent >= 0){

        #if LA_ALGORITHM == STATIC
            node_depth = tree[curr_parent].depth;
        #elif LA_ALGORITHM == DYNAMIC
            node_depth = tree.data[curr_parent]->depth;
        #endif
        
 		if(node_depth == query_level){
				
 			if(curr_parent != query_answer){
 				printf("ERROR: REAL LA(%d, %d) = %d\n", query_node, query_level, curr_parent);
                exit(-1);
 			}
 			return;
 		}

        #if LA_ALGORITHM == STATIC
            curr_parent = tree[curr_parent].parent;
        #elif LA_ALGORITHM == DYNAMIC
            curr_parent = tree.data[curr_parent]->parent;
        #endif
 		
	}

	if(curr_parent == -1){
        printf("ERROR: CORRECT ANSWER TO LA(%d, %d) NOT FOUND! ALGORITHM RETURNED %d\n", query_node, query_level, query_answer);
        exit(-1);
    }	
}

#if LA_ALGORITHM == DYNAMIC

    void add_leaf(int parent_idx){

        bool is_left_child = false;

        node* parent_node = tree.data[parent_idx];
        node* leaf_node = alloc(sizeof(node));

        int leaf_idx = tree.length;

        init_node(leaf_node);
        vec_push(&tree, leaf_node);

        leaf_node->parent = parent_idx;
        leaf_node->depth = parent_node->depth + 1;

        if (parent_node->left == -1) 
        {
            parent_node->left = leaf_idx;

            is_left_child = true;

            vec_push(&leaves, leaf_idx);
            leaf_node->leaf_pos = leaves.length - 1;
        }
        else if(parent_node->right == -1)
        {
            parent_node->right = leaf_idx;
            
            leaves.data[parent_node->leaf_pos] = leaf_idx;
            leaf_node->leaf_pos = parent_node->leaf_pos;
            parent_node->leaf_pos = -1;
        }
        else
        {
            printf("Illegal input: Input tree is not binary\n");
            exit(-1);
        }

        #if LA_ALGORITHM == TABLE
            add_table_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == JUMP_POINTER
            add_jump_pointer_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == LADDER
            add_ladder_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == JUMP_LADDER
            add_jump_ladder_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == MACRO_MICRO
            add_macro_micro_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == MENGHANI_MATANI
            add_menghani_matani_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == HAGERUP
            add_hagerup_leaf(parent_idx, leaf_idx, is_left_child);
        #elif LA_ALGORITHM == ALSTRUP
            add_alstrup_leaf(parent_idx, leaf_idx, is_left_child);
        #endif
    }   
#endif