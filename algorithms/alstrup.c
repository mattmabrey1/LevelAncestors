#include "../la_algorithm.h"

#if LA_ALGORITHM == ALSTRUP

    #include "alstrup.h"

    #if LA_ALGORITHM == STATIC
        void alstrup_preprocessing(){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        void build_bitindex(int N){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        void recurse_micro(int v){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        int calc_subtree(int v){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);

            return -1;
        }

        void calc_rank(int v){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        void calculate_jumpM(){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        void init_micro(){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }

        int LA_micro(int v, int d){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);

            return -1;
        }

        int alstrup_query(int query_node, int query_level){
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);

            return -1;
        }

    #elif LA_ALGORITHM == DYNAMIC
        /************************************************************************/
        /* Preprocessing						*/
        /************************************************************************/
        void alstrup_preprocessing()
        {
            int i, j;

            if (calc_subtree(0) != tree.capacity)
            {
                printf("Root subtree size (%d) not equal to n = %d!", calc_subtree(0), tree.capacity);
                exit(-1);
            }

            // CALCULATE RANK FOR EACH NODE
            for (i = 0; i < tree.length; ++i)
            {
                calc_rank(i);
            }

            // CALCULATE MACRO[] FOR EACH NODE
            vec_init(&jumpM);
            vec_reserve(&jumpM, tree.length);

            calculate_jumpM();

            vec_init(&anc);
            vec_reserve(&anc, tree.length);

            // CALCULATE Micro nodetable
            int maxNumOfMacroNodes = (tree.length / M);

            vec_init(&levelancM);
            vec_reserve(&levelancM, maxNumOfMacroNodes);

            vec_init(&node_table);
            vec_reserve(&node_table, maxNumOfMacroNodes);

            vec_init(&anc);
            vec_reserve(&anc, tree.length);

            init_micro();

            // CALCULATE Bitindex
            N = (int)floor((double)log_base2(tree.length) / 2);

            vec_init(&bitindex);
            
            i = (1 << N);
            
            printf("tree.len: %d N: %d  M: %d  i: %d\n", tree.length, N, M,  i);
            
            if(i > bitindex.capacity){
                vec_reserve(&bitindex, i);
            }
            
            build_bitindex(N);
        }

        void build_bitindex(int N)
        {
            int i, j = 0;

            int n2ExpB = 1 << N;
            int nMaskTmp;

            vec_int_t* bitindex_row;

            for (i = 0; i < n2ExpB; i++)
            {
                // If this row of the bitindex is not definied, add a new row
                if (i >= bitindex.length)
                {
                    bitindex_row = alloc(sizeof(vec_int_t));
                    vec_init(bitindex_row);
                    vec_reserve(bitindex_row, N);

                    vec_push(&bitindex, bitindex_row);

                    j = N - 1;
                    nMaskTmp = n2ExpB / 2;
                }
                // Else, we're recalculating bitindex so get existing row
                else
                {
                    bitindex_row = bitindex.data[i];

                    // get the next bit that should be checked
                    j = vec_last(bitindex_row) - 1;

                    // convert it to a power of 2 for mask
                    if (j >= 0)
                        nMaskTmp = 1 << j;
                }

                // Start at the MSB that has not been checked yet and loop until 0
                while (j >= 0)
                {

                    // Only get the N most-significant bits
                    if (bitindex_row->length >= N)
                        break;

                    // if there is 1 at the j's place, keep his place in the table.
                    if ((i & nMaskTmp) == nMaskTmp)
                    {
                        vec_push(bitindex_row, j);
                    }

                    // Make mask next power of 2
                    nMaskTmp /= 2;

                    j--;
                }
            }
        }

        // Do a top-down DFS of the tree T to create micro trees. Try to add node V to it's parent's micro tree, if it is full make a new micro tree
        void recurse_micro(int v)
        {
            int curr_parent;
            int parent_tree = tree.data[tree.data[v]->parent]->micro_tree;

            vec_int_t* micro_tree;

            if (node_table.data[parent_tree]->length >= M)
            {
                vec_int_t* lvlanc;

                micro_tree = alloc(sizeof(vec_int_t));
                lvlanc = alloc(sizeof(vec_int_t));

                vec_init(micro_tree);
                vec_reserve(micro_tree, M);

                vec_init(lvlanc);
                vec_reserve(lvlanc, M);

                vec_push(&node_table, micro_tree);

                vec_push(&levelancM, lvlanc);

                parent_tree = node_table.length - 1;

                anc.data[v] = 1;

                curr_parent = v;

                // Fill each micro tree root's levelancM array with M of it's ancestors
                for (int i = 0; i < M; i++)
                {
                    vec_push(lvlanc, curr_parent);

                    // Once curr_parent is -1 don't try to get next anymore but fill in array
                    if (curr_parent >= 0)
                    {
                        curr_parent = tree.data[curr_parent]->parent;
                    }
                }
            }
            else
            {
                micro_tree = node_table.data[parent_tree];
                anc.data[v] = (1 << (micro_tree->length)) + anc.data[tree.data[v]->parent];
            }

            anc.length++;	// possibly just change to anc.push(value) so we dont need to increment length

            tree.data[v]->micro_tree = parent_tree;

            vec_push(micro_tree, v);

            if (tree.data[v]->left != -1)
                recurse_micro(tree.data[v]->left);

            if (tree.data[v]->right != -1)
                recurse_micro(tree.data[v]->right);
        }

        // Calculate the subtree sizes for all nodes by doing a top-down DFS and returning subtree size at each node
        int calc_subtree(int v)
        {
            int num_children = 0;

            if (tree.data[v]->left != -1)
                num_children = calc_subtree(tree.data[v]->left);

            if (tree.data[v]->right != -1)
                num_children += calc_subtree(tree.data[v]->right);

            return (tree.data[v]->size = ++num_children);
        }

        void calc_rank(int v)
        {
            int rank = tree.data[v]->rank;
            int pwr_of_2 = 1 << (rank + 1);

            // Continually check next power of 2 higher
            while (mod_pwr2(tree.data[v]->depth, pwr_of_2) == 0 && tree.data[v]->size >= pwr_of_2)
            {

                pwr_of_2 <<= 1;
                rank++;
            }

            tree.data[v]->rank = rank;
        }

        void calculate_jumpM()
        {
            int curr_parent;

            // Make sure to reset 'used' since when we recalculate jumpM we will want to restart at index 0
            jumpM.length = 0;

            // Push root's jump pointer
            vec_push(&jumpM, 0);

            for (int v = 1; v < tree.length; v++)
            {

                curr_parent = tree.data[v]->parent;

                while (curr_parent >= 0)
                {

                    if (mod_pwr2(tree.data[curr_parent]->depth, M) == 0)
                    {

                        vec_push(&jumpM, curr_parent);
                        break;
                    }

                    curr_parent = tree.data[curr_parent]->parent;
                }
            }
        }

        void init_micro()
        {
            vec_int_t* node_table_row;
            vec_int_t* level_ancM_row;

            node_table_row = alloc(sizeof(vec_int_t));
            level_ancM_row = alloc(sizeof(vec_int_t));

            vec_init(node_table_row);
            vec_reserve(node_table_row, M);

            vec_push(node_table_row, 0);
            vec_push(&node_table, node_table_row);

            vec_push(&anc, 1);

            // initialize levelancM for root which only has itself as an ancestor
            vec_init(level_ancM_row);
            vec_push(level_ancM_row, 0);
            vec_push(&levelancM, level_ancM_row);

            recurse_micro(tree.data[0]->left);
            recurse_micro(tree.data[0]->right);
        }
        /************************************************************************/

        /************************************************************************/
        /* Querying						*/
        /************************************************************************/
        int LA_micro(int v, int d)
        {
            int i = bitindex.data[anc.data[v]]->data[d];

            if (i > -1)
                return node_table.data[tree.data[v]->micro_tree]->data[i];
            else
            {
                printf("LA_micro generated i=-1 EXIT!\n");
                exit(-1);
                return -1;
            }
        }

        int alstrup_query(int v, int d)
        {
            int w = jumpM.data[v];

            int curr_jump, viable_macro, micro_root;

            if (tree.data[w]->rank < r0)
            {
                w = jumpM.data[w];
            }

            // Now w is the first macro node on the path from v to root
            if (tree.data[w]->depth >= d)
            {

                v = viable_macro = w;

                curr_jump = jumpM.data[v];

                while (tree.data[curr_jump]->depth >= d)
                {

                    if (tree.data[curr_jump]->rank >= r0)
                        viable_macro = curr_jump;

                    if (curr_jump == 0)
                        break;

                    curr_jump = jumpM.data[curr_jump];
                }

                v = viable_macro;
            }

            micro_root = node_table.data[tree.data[v]->micro_tree]->data[0];

            // Check first micro tree for answer
            if (tree.data[micro_root]->depth <= d)
            {
                return LA_micro(v, tree.data[v]->depth - d);
            }

            v = tree.data[micro_root]->parent;
            micro_root = node_table.data[tree.data[v]->micro_tree]->data[0];

            // Check second micro tree for answer
            if (tree.data[micro_root]->depth <= d)
            {
                return LA_micro(v, tree.data[v]->depth - d);
            }

            // Check second micro tree's first M ancestors in levelancM
            return levelancM.data[tree.data[v]->micro_tree]->data[tree.data[micro_root]->depth - d];
        }
        /************************************************************************/

        /************************************************************************/
        /* Tree Manipulation						*/
        /************************************************************************/
        void add_alstrup_leaf(int parent, int leaf, bool is_left_child)
        {
            node* new_node = tree.data[leaf];
            noe* parent_node = tree.data[parent];

            new_node->size = 1;
            new_node->rank = 0;

            int curr_parent = parent;

            // CALCULATE NEW RANK AND SIZE FOR EACH ANCESTOR NODE
            while (curr_parent >= 0)
            {
                tree.data[curr_parent]->size++;
                calc_rank(curr_parent);
                curr_parent = tree.data[curr_parent]->parent;
            }

            // M has increased, change jumpM and micro trees
            if (r0 < (int)floor(log_base2(log_base2(tree.length)) - 1))
            {

                r0 = (int)floor(log_base2(log_base2(tree.length)) - 1);

                M = (1 << r0);

                printf("New M = %d   r0 = %d\n", M, r0);

                // Destroying old micro structures and re-initalizing since reallocating everything might take more time
                // when we don't care about the old data inside
                for (int i = 0; i < node_table.length; i++)
                {
                    vec_deinit(node_table.data[i]);
                    vec_deinit(levelancM.data[i]);
                }

                calculate_jumpM();
                init_micro();
            }
            else
            {

                if (mod_pwr2(parent_node->depth, M) == 0)
                {
                    vec_push(&jumpM, parent);
                }
                else
                {
                    vec_push(&jumpM, jumpM.data[parent]);
                }

                recurse_micro(v);
            }

            int floored_log_arr_size = (int)floor(log_base2(tree.length) / 2);

            if (N < floored_log_arr_size)
            {

                N = floored_log_arr_size;
                printf("New N = %d\n", N);

                // Extend bitindex
                build_bitindex(N);
            }
        }

        void remove_alstrup_leaf(int leaf)
        {
            printf("Error: Function Not Yet Implemented!\n");
            exit(-1);
        }
        /************************************************************************/
    #endif
#endif

