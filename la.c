#include "la_algorithm.h"

int main(int argc, char *argv[])
{
    int i;
    int current_pos, parent_pos, current_depth;
    char c;

    clock_gettime(CLOCK_REALTIME, &start_time);
    
    /************************************************************************/
    /* Process input and create tree					*/
    /************************************************************************/

    scanf("%d\n", &n);

    if (argc < 2)
    {
        printf("\nUsage: ./<algorithm> <number_of_la_operations> <operation_ratio (default 0.5)> <seed (default = 1)> < <tree>\n\n");
        printf("The tree is read from the standard input.\n\n");
        exit(0);
    }

    la_operations = atoi(argv[1]);

    // Set leaf addition 
    if (argc >= 3){
        ratio = atof(argv[2]);
    }
    else{
        ratio = 0.5;
    }

    // Set random number generator seed
    if (argc >= 4){
        srand(atoi(argv[3]));
    }
    else{
        srand(1);
    }

    current_pos = 0;
    parent_pos = 0;
    current_depth = 0;

    la_initialize();

    for (i = 0; i < 2*n-2; i++)
    {
        c = getchar();
        if (c == '1')
        {
            #if LA_ALGORITHM == STATIC

                tree[++current_pos].parent = parent_pos;
                tree[current_pos].depth = ++current_depth;
                
                #if LA_ALGORITHM == MENGHANI_MATANI
                    depth_size[current_depth]++;
                #endif

                if (tree[parent_pos].left == -1) 
                {
                    tree[parent_pos].left = current_pos;
                    if (DEBUG_INPUT) printf("tree[%d].left = %d\n", parent_pos, current_pos);
                }
                else if (tree[parent_pos].right == -1)
                {
                    tree[parent_pos].right = current_pos;
                    if (DEBUG_INPUT) printf("tree[%d].right = %d\n", parent_pos, current_pos);
                }
                else
                {
                    printf("Illegal input: Input tree is not binary\n");
                    exit(-1);
                }

            #elif LA_ALGORITHM == DYNAMIC

                tree.data[++current_pos]->parent = parent_pos;
                tree.data[current_pos]->depth = ++current_depth;

                #if LA_ALGORITHM == MENGHANI_MATANI
                
                    if(current_depth >= depth_size.length){
                        vec_push(&depth_size, 1);
                    }
                    else{
                        depth_size.data[current_depth]++;
                    }

                    tree.data[current_pos]->label = current_pos;
                #endif

                if (tree.data[parent_pos]->left == -1)
                {
                    tree.data[parent_pos]->left = current_pos;
                    vec_push(&leaves, current_pos);
                    tree.data[current_pos]->leaf_pos = leaves.length - 1;

                    if (DEBUG_INPUT){
                        printf("tree.data[%d]->left = %d\n", parent_pos, current_pos);
                    }
                }
                else if (tree.data[parent_pos]->right == -1)
                {
                    tree.data[parent_pos]->right = current_pos;

                    leaves.data[tree.data[parent_pos]->leaf_pos] = current_pos;
                    tree.data[current_pos]->leaf_pos = tree.data[parent_pos]->leaf_pos;
                    tree.data[parent_pos]->leaf_pos = -1;

                    if (DEBUG_INPUT)
                    {
                        printf("tree.data[%d]->right = %d\n", parent_pos, current_pos);
                    }
                }
                else
                {
                    printf("Illegal input: Input tree is not binary\n");
                    exit(-1);
                }

            
            #endif
            
            parent_pos = current_pos;
        }
        else if (c == '0')
        {
            #if LA_ALGORITHM == STATIC
                parent_pos = tree[parent_pos].parent;
            #elif LA_ALGORITHM == DYNAMIC
                parent_pos = tree.data[parent_pos]->parent;
            #endif
            
            current_depth--;
        }
        else
        {
            printf("Illegal input: Expecting '0' or '1'\n");
            exit(-1);
        }

        #if LA_ALGORITHM == HAGERUP

            if(current_depth > max_depth){  max_depth = current_depth;  }

            #if LA_ALGORITHM == STATIC
                euler_arr[i + 1] = parent_pos;
                rep_arr[parent_pos] = i + 1;
            #endif
        #endif
        
    }

    #if DEBUG_INPUT
    
        printf("Input tree:\n");
        for (i = 0; i < n; i++)
        {
            #if LA_ALGORITHM == STATIC
                printf("Node %d\tParent: %d\t Left: %d\tRight: %d\tDepth: %d\n", i, tree[i].parent, tree[i].left, tree[i].right, tree[i].depth);
            #elif LA_ALGORITHM == DYNAMIC
                printf("Node %d\tParent: %d\t Left: %d\tRight: %d\tDepth: %d\n", i, tree.data[i]->parent, tree.data[i]->left, tree.data[i]->right, tree.data[i]->depth);
            #endif
        }
    #endif

    /************************************************************************/
    /* Call the appropriate preprocessing routine/algorithm			*/
    /************************************************************************/
    la_preprocessing();

    clock_gettime(CLOCK_REALTIME, &end_time);

    #if DEBUG_RESULTS
        printf("\n\t[DEBUG RESULTS MODE ENABLED]");
    #endif

    printf("\n\t---Algorithm %s---\n", LA_ALGORITHM_STRING);
    printf("\tPreprocessing Time: %f seconds\n", get_elapsed_time(start_time, end_time));

    clock_gettime(CLOCK_REALTIME, &start_time);
    la_execute();
    clock_gettime(CLOCK_REALTIME, &end_time);
    
    printf("\tExecution Time: %f seconds\n", get_elapsed_time(start_time, end_time));

    #if LA_ALGORITHM == DYNAMIC && DEBUG_RESULTS
        la_operations = num_of_queries + num_of_leaf_additions;
        printf("\tQueries: %d  %f%%\n", num_of_queries, ((float)num_of_queries / la_operations));
        printf("\tLeaf Additions: %d  %f%%\n", num_of_leaf_additions, ((float)num_of_leaf_additions / la_operations));
    #endif

    printf("\n");
}