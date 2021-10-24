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
    tree = alloc(n*(sizeof(node)));

    for (i = 0; i < n; i++)
        tree[i].parent = tree[i].left = tree[i].right = -1;

    current_pos = 0;
    parent_pos = 0;
    current_depth = 0;
    tree[0].depth = current_depth;

    la_initialize();

    for (i = 0; i < 2*n-2; i++)
    {
        c = getchar();
        if (c == '1')
        {
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

            parent_pos = current_pos;
        
        }
        else if (c == '0')
        {
            parent_pos = tree[parent_pos].parent;
            current_depth--;
        }
        else
        {
            printf("Illegal input: Expecting '0' or '1'\n");
            exit(-1);
        }

        #if LA_ALGORITHM == HAGERUP

            if(current_depth > max_depth){  max_depth = current_depth;  }

            euler_arr[i + 1] = parent_pos;
            rep_arr[parent_pos] = i + 1;
            
        #endif
        
    }

    if (DEBUG_INPUT)
    {
        printf("Input tree:\n");
        for (i = 0; i < n; i++)
        {
        printf("Node %d\tParent: %d\t Left: %d\tRight: %d\tDepth: %d\n", i, \
            tree[i].parent, tree[i].left, tree[i].right, tree[i].depth);
        }
    }

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