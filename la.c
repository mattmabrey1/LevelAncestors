#include "la_algorithm.h"

int main(int argc, char *argv[])
{
    int i;
    int current_pos, parent_pos, current_depth;
    char c;

    clock_gettime(CLOCK_REALTIME, &start_proc);
    
    /************************************************************************/
    /* Process input and create tree					*/
    /************************************************************************/

    scanf("%d\n", &n);

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
                else
                {
                    if (tree[parent_pos].right == -1)
                    {
                        tree[parent_pos].right = current_pos;
                        if (DEBUG_INPUT) printf("tree[%d].right = %d\n", parent_pos, current_pos);
                    }
                    else
                    {
                        printf("Illegal input: Input tree is not binary\n");
                        exit(-1);
                    }
                }

            #elif LA_ALGORITHM == DYNAMIC

                tree.data[++current_pos]->parent = parent_pos;
                tree.data[current_pos]->depth = ++current_depth;

                if (tree.data[parent_pos]->left == -1)
                {
                    tree.data[parent_pos]->left = current_pos;
                    vec_push(&leaves, current_pos);
                    tree.data[current_pos]->leaf_pos = leaves.length - 1;

                    if (DEBUG_INPUT){
                        printf("tree.data[%d]->left = %d\n", parent_pos, current_pos);
                    }
                }
                else
                {
                    if (tree.data[parent_pos]->right == -1)
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

    clock_gettime(CLOCK_REALTIME, &end_proc);
    clock_gettime(CLOCK_REALTIME, &start_query);

    la_process_queries();

    clock_gettime(CLOCK_REALTIME, &end_query);
    
    long proc_seconds = end_proc.tv_sec - start_proc.tv_sec;
    long proc_nanoseconds = end_proc.tv_nsec - start_proc.tv_nsec;
    double preprocessing_time = proc_seconds + (proc_nanoseconds * 0.000000001);

    long query_seconds = end_query.tv_sec - start_query.tv_sec;
    long query_nanoseconds = end_query.tv_nsec - start_query.tv_nsec;
    double query_time = query_seconds + (query_nanoseconds * 0.000000001);

    #if DEBUG_RESULTS
        printf("\n\t[DEBUG RESULTS MODE ENABLED]");
    #endif

    printf("\n\t---Algorithm %d---\n", LA_ALGORITHM);
    printf("\tPreprocessing Time: %f seconds\n", preprocessing_time);
    printf("\tQuery Time: %f seconds\n\n", query_time);
}