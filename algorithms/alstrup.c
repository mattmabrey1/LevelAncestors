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
            if (calc_subtree(0) != tree.capacity)
            {
                printf("Root subtree size (%d) not equal to n = %d!", calc_subtree(0), tree.capacity);
                exit(-1);
            }

            // CALCULATE RANK FOR EACH NODE
            for (int i = 0; i < tree.length; ++i)
            {
                tree.data[i]->rank = calc_rank(tree.data[i]->depth, tree.data[i]->size);
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
            
            if((1 << N) > bitindex.capacity){
                vec_reserve(&bitindex, (1 << N));
            }
            
            build_bitindex(N);

            vec_init(&macro_tree);
            vec_init(&levelanc);
            vec_init(&jump);

            preprocess_macro_tree();
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
                    if (j >= 0){
                        nMaskTmp = 1 << j;
                    }
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
            int parent = tree.data[v]->parent;
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

                tree.data[v]->micro_tree = node_table.length - 1;

                // anc.data[v] = 1;
                vec_push(&anc, 1);

                int curr_parent = v;

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
                tree.data[v]->micro_tree = parent_tree;

                vec_push(&anc, (1 << (micro_tree->length)) + anc.data[tree.data[v]->parent]);
            }

            vec_push(micro_tree, v);

            if (tree.data[v]->left != -1){
                recurse_micro(tree.data[v]->left);
            }
                
            if (tree.data[v]->right != -1){
                recurse_micro(tree.data[v]->right);
            }
        }

        // Calculate the subtree sizes for all nodes by doing a top-down DFS and returning subtree size at each node
        int calc_subtree(int v)
        {
            int num_children = 0;

            if (tree.data[v]->left != -1){
                num_children = calc_subtree(tree.data[v]->left);
            }

            if (tree.data[v]->right != -1){
                num_children += calc_subtree(tree.data[v]->right);
            }

            return (tree.data[v]->size = ++num_children);
        }

        int calc_macro_subtree(int v)
        {
            int rank = highest_pwrOf2_divides(tree.data[v]->depth);
            int highest_pwr_of_2_for_size = highest_pwrOf2(tree.data[v]->size);

            if(rank > highest_pwr_of_2_for_size || tree.data[v]->depth == 0){
                rank = highest_pwr_of_2_for_size;
            }

            tree.data[v]->rank = log_base2(rank);
        }

        void calculate_jumpM()
        {
            int curr_parent, proper_anc_macro_node;

            // Make sure to reset 'used' since when we recalculate jumpM we will want to restart at index 0
            jumpM.length = 0;

            // Push root's jump pointer
            vec_push(&jumpM, 0);

            for (int v = 1; v < tree.length; v++)
            {
                curr_parent = tree.data[v]->parent;

                // Either take parent's jumpM OR are parent is jumpM
                if (mod_pwr2(tree.data[curr_parent]->depth, M) == 0)
                {
                    vec_push(&jumpM, curr_parent);
                }
                else{
                    vec_push(&jumpM, jumpM.data[curr_parent]);
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

        void add_macro_node(int v){

            int proper_anc_macro_node;

            if(tree.data[jumpM.data[v]]->rank >= r0){
                proper_anc_macro_node = tree.data[jumpM.data[v]]->macro_index;
            }
            else{
                proper_anc_macro_node = tree.data[jumpM.data[jumpM.data[v]]]->macro_index;
            }

            vec_push(&macro_tree, alloc(sizeof(macro_node)));
            
            macro_node* m_node = vec_last(&macro_tree);
            m_node->parent = proper_anc_macro_node;
            m_node->size = 1;
            m_node->depth = macro_tree.data[proper_anc_macro_node]->depth + 1;
            m_node->tree_index = v;
            m_node->children = alloc(sizeof(vec_int_t));
            vec_init(m_node->children);

            tree.data[v]->macro_index = macro_tree.length - 1;

            vec_push(macro_tree.data[m_node->parent]->children, macro_tree.length - 1);

            vec_int_t* levelanc_row = alloc(sizeof(vec_int_t));
            vec_int_t* jump_row = alloc(sizeof(vec_int_t));
            vec_init(levelanc_row);
            vec_init(jump_row);

            vec_push(&levelanc, levelanc_row);
            vec_push(&jump, jump_row);
        }

        void preprocess_macro_tree(){
            
            int i, j, parent, size;

            vec_reserve(&macro_tree, tree.length / M);
            vec_reserve(&levelanc, macro_tree.length);
            vec_reserve(&jump, macro_tree.length);

            vec_push(&macro_tree, alloc(sizeof(macro_node)));
            macro_tree.data[0]->parent = -1;
            macro_tree.data[0]->depth = 0;
            macro_tree.data[0]->tree_index = 0;
            macro_tree.data[0]->children = alloc(sizeof(vec_int_t));
            vec_init(macro_tree.data[0]->children);

            tree.data[0]->macro_index = 0;
            
            // Create macro tree
            for(int v = 1; v < tree.length; v++){

                if(tree.data[v]->rank >= r0){
                    add_macro_node(v);
                }
            }
            
            calc_macro_subtree(0);

            printf("macro preprossing mid\n");

            for(int i = 0; i < macro_tree.length; i++){

                size = 1 << calc_rank(macro_tree.data[i]->depth, macro_tree.data[i]->size);
               
                parent = i;
                printf("macro preprossing inside %d\n", i);

                for(j = 0; j < size; j++){
                    if(parent == -1){
                        break;
                    }

                    vec_push(levelanc.data[i], parent);
                    parent = macro_tree.data[parent]->parent;
                }
                printf("macro preprossing inside %d\n", i);
                size = floor(log_base2(macro_tree.data[i]->depth + 1));
                parent = macro_tree.data[i]->parent;

                for(j = 0; j < size; j++){

                    if(mod_pwr2(macro_tree.data[parent]->depth, (1 << j)) == 0){
                        vec_push(jump.data[i], parent);
                    }
                    else{
                        vec_push(jump.data[i], jump.data[parent]->data[j]);
                    }
                }
            }
            printf("macro preprossing done\n");
        }
        /************************************************************************/

        /************************************************************************/
        /* Querying						*/
        /************************************************************************/
        int LA_macro(int v, int d){

            int i = floor(log_base2(d + 1));
            
            int distance = macro_tree.data[v]->depth - d;
            printf("1 inside LA_ macro  %d   distance: %d  macro_tree_legnth: %d\n", v, distance, macro_tree.length);
            int parent = v;
            while(parent > 0){
                printf("Macro[%d]->parent = %d\n", parent, macro_tree.data[parent]->parent);

                for(int j = 0; j < jump.data[v]->length; j++){
                    printf("Macro[%d]->jump[%d] = %d\n", parent, j, jump.data[parent]->data[j]);
                }

                for(int j = 0; j < levelanc.data[v]->length; j++){
                    printf("Macro[%d]->levelanc[%d] = %d\n", parent, j, levelanc.data[parent]->data[j]);
                }

                printf("\n");
                parent = macro_tree.data[parent]->parent;
            }
            
            // Loops a max of 4 times
            while( (2 * (macro_tree.data[v]->depth - distance)) >= (1 << i)){
                v = jump.data[v]->data[i - 1];
            }

            printf("2 inside LA_ macro   v: %d\n", v);

            

            return levelanc.data[v]->data[macro_tree.data[v]->depth - distance];
        }

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

            // If w is not a macro node
            if (tree.data[w]->macro_index == -1)
            {
                w = jumpM.data[w];
            }

            // Now w is the first macro node on the path from v to root(v)
            if (tree.data[w]->depth >= d)
            {
                printf("LA_ macro(%d, %d)   tree-rank: %d   w-tree:%d   depth of w: %d    d: %d\n", tree.data[w]->macro_index,(int)floor((tree.data[w]->depth - d) / (double)M), tree.data[w]->rank, w, tree.data[w]->depth, d);
                // Now find the macro node of least depth on the path from v to LA(v, d)
                v = LA_macro(tree.data[w]->macro_index, (int)floor((tree.data[w]->depth - d) / (double)M));
                printf("LA_ macro returned %d\n", v);
                v = macro_tree.data[v]->tree_index;
                printf("LA_ macro done\n");
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
            node* parent_node = tree.data[parent];

            new_node->size = 1;
            new_node->rank = 0;

            int curr_parent = parent;

            // CALCULATE NEW RANK AND SIZE FOR EACH ANCESTOR NODE
           /* while (curr_parent >= 0)
            {
                tree.data[curr_parent]->size++;
                tree.data[curr_parent]->rank = calc_rank(tree.data[curr_parent]->depth, tree.data[curr_parent]->size);
                curr_parent = tree.data[curr_parent]->parent;
            }*/

            // M has increased, update jumpM, micro trees, and macro tree
            if (r0 < (int)floor(log_base2(log_base2(tree.length)) - 1))
            {
                r0 = (int)floor(log_base2(log_base2(tree.length)) - 1);

                M = (1 << r0);

                for (int i = 0; i < node_table.length; i++)
                {
                    vec_clear(node_table.data[i]);
                    vec_clear(levelancM.data[i]);
                }

                calculate_jumpM();
                recurse_micro(tree.data[0]->left);
                recurse_micro(tree.data[0]->right);

                // Reset all macro nodes
                for(int i = 0; i < macro_tree.length; i++){
                    tree.data[macro_tree.data[i]->tree_index]->macro_index = -1;
                }

                vec_clear(&macro_tree);
                vec_clear(&levelanc);
                vec_clear(&jump);

                preprocess_macro_tree();
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

                recurse_micro(leaf);
            }

            int floored_log_arr_size = (int)floor(log_base2(tree.length) / 2);

            if (N < floored_log_arr_size)
            {
                N = floored_log_arr_size;

                // Extend bitindex
                build_bitindex(N);
            }

            add_leaf_step(jumpM.data[leaf], jumpM.data[jumpM.data[leaf]]);
        }

        void add_leaf_step(int v, int p){

            if(tree.data[v]->macro_index == -1){
                add_macro_node(v);

                vec_push(levelanc.data[tree.data[v]->macro_index], tree.data[v]->macro_index);
            }

            int parent_macro_index = tree.data[p]->macro_index;
            int macro_index = tree.data[v]->macro_index;
            int w;

            for(int i = 0; i < log_base2(macro_tree.data[macro_index]->depth + 1); i++){

                if(mod_pwr2(macro_tree.data[parent_macro_index]->depth, (1 << i)) == 0){
                    w = p;
                }
                else{
                    w = jump.data[parent_macro_index]->data[i];
                }

                vec_push(jump.data[macro_index], w);
                vec_push(levelanc.data[w], tree.data[vec_last(levelanc.data[w])]->parent); 
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

