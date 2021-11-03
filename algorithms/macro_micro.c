#include "../la_algorithm.h"

#if LA_ALGORITHM == MACRO_MICRO

  #include "macro_micro.h"
  #include "ladder.h"
  
  #if LA_ALGORITHM == STATIC

    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void create_micro_table(unsigned int signature, int size)
    {
      int i, j;
      int pos, curr_num;

    if (DEBUG_MMTABLE) printf("Request for creation of table with signature %d, size = %d\n", signature, size);
      if (micro_tables[signature] == NULL)
      {
        micro_tables[signature] = alloc(size*sizeof(int *));
        micro_tables[signature][0] = alloc(sizeof(int));
        micro_tables[signature][0][0] = 0;
        curr_num = 0;
        pos = 1;
        for (i = 1; i < 2*size - 2; i++)
        {
          unsigned int current_bit_pos;
          unsigned int bit_value;

          current_bit_pos = 2*size - 2 - i;
          bit_value = (signature & (1 << current_bit_pos)) >> current_bit_pos;
    if (DEBUG_MMTABLE) printf("i = %d, current_bit_pos = %d, bit_value = %d\n", i, current_bit_pos, bit_value);
          if (bit_value == 0)
          {
      pos--;
          }
          else
          {
      micro_tables[signature][++curr_num] = alloc((pos+1)*sizeof(int));
      for (j = 0; j < pos; j++)
      {
        micro_tables[signature][curr_num][j] = micro_tables[signature][curr_num-1][j];
    if (DEBUG_MMTABLE) printf("micro_tables[%d][%d][%d] = %d\n", signature, curr_num, j, micro_tables[signature][curr_num][j]);
      }
      micro_tables[signature][curr_num][pos++] = curr_num;
    if (DEBUG_MMTABLE) printf("micro_tables[%d][%d][%d] = %d\n", signature, curr_num, pos-1, micro_tables[signature][curr_num][pos-1]);
          }
        }
      }
    }

    void macro_micro_preprocessing()
    {
      int i, j, logn4;
      int current_node, current_micro_root, micro_root_weight;
      unsigned int signature;
      int jump_nodes_num, last_jump, current_jump_index;
      int *weight, my_weight;
      int wanted_index;

      ladder_preprocessing();
      weight = alloc(n*sizeof(int));
      mm_table = alloc(n*sizeof(struct mm_struct));
      logn4 = (int) (((float) log_base2(n))/4.0);
      jump_nodes_num = 0;
      for (i = n-1; i >= 0; i--)
      {
        if (tree[i].right == -1)
        {
          if (tree[i].left == -1)
          {
      weight[i] = 1;
          }
          else
          {
      weight[i] = weight[tree[i].left]+1;
          }
        }
        else
        {
          weight[i] = weight[tree[i].left] + weight[tree[i].right] + 1;
        }
        if (weight[i] <= logn4) 	/* The node is MICRO or MICROROOT 	*/
        {
          mm_table[i].type = MICRO;
        }
        else			/* The node is MACRO or JUMP 		*/
        {
          mm_table[i].type = NOTHING;
          if ((tree[i].right != -1) && (weight[tree[i].right] <= logn4))
          {
      mm_table[i].type = JUMP;
      last_jump = i;
      jump_nodes_num++;
      mm_table[tree[i].right].type = MICROROOT;
          }
          if ((tree[i].left != -1) && (weight[tree[i].left] <= logn4))
          {
      if (mm_table[i].type == NOTHING)
      {
        mm_table[i].type = JUMP;
        last_jump = i;
        jump_nodes_num++;
      }
      mm_table[tree[i].left].type = MICROROOT;
          }
          if (mm_table[i].type == NOTHING) 
          {
      mm_table[i].type = MACRO;
      mm_table[i].aux = last_jump;
          }
        }
      }
      if (DEBUG_MM)
      {
        for (i = 0; i < n; i++)
          printf("Node %d: Weight %d, type %d, aux = %d\n", i, \
      weight[i], mm_table[i].type, mm_table[i].aux);
      }
      mm_jump = alloc(jump_nodes_num*sizeof(int *));
      micro_tables = alloc(((int)pow((double)2,(double)(2*logn4-2)))*sizeof(int **));
      current_jump_index = 0;
      current_node = 0;
      while (current_node < n)
      {
        if (mm_table[current_node].type == JUMP)
        {
          int mylog, next;

    /*
          frexp((double)tree[current_node].depth, &mylog);
          mylog--;
    */
          mylog = log_base2(tree[current_node].depth);
          mm_jump[current_jump_index] = alloc((mylog+1)*sizeof(int));
          mm_table[current_node].aux = current_jump_index;
          if (current_node != 0) 
          {
    if (DEBUG_MM2) printf("Examining jump node %d\n", current_node);
      mm_jump[current_jump_index][0] = tree[current_node].parent;
      next = tree[current_node].parent;
    if (DEBUG_MM2) printf("  mm_jump[%d][0] = %d\n", current_jump_index, mm_jump[current_jump_index][0]);
      wanted_index = 1;
      for (j = 1; j <= mylog; j++)
      {
        mm_jump[current_jump_index][j] = ladder_query(next, tree[next].depth - wanted_index);
        next = mm_jump[current_jump_index][j];
    if (DEBUG_MM2) printf("  mm_jump[%d][%d] = %d\n", current_jump_index, j, mm_jump[current_jump_index][j]);
        wanted_index *= 2;
      }
          }
          current_jump_index++;
        }
        else if (mm_table[current_node].type == MICROROOT)
        {
          current_micro_root = current_node;
          micro_root_weight = weight[current_node];
          signature = 0;
          for (i = 1; i < micro_root_weight; i++)
          {
      int curr_depth, prev_depth;
      mm_table[++current_node].aux = current_micro_root;
      curr_depth = tree[current_node].depth;
      prev_depth = tree[current_node-1].depth;
      if (curr_depth <= prev_depth) signature <<= prev_depth - curr_depth + 1;
      signature <<= 1; 
      signature++;
          }
          signature <<= tree[current_node].depth - tree[current_micro_root].depth;
          mm_table[current_micro_root].aux = signature;
    if (DEBUG_MM2) printf("Microroot %d signature: %d\n", current_micro_root, signature);
          create_micro_table(signature, weight[current_micro_root]);
        }
        current_node++;
      }
      if (DEBUG_MM2)
      {
        for (i = 0; i < n; i++)
        {
          printf("Node %d\ttype: %d\taux: %d\n", i, mm_table[i].type, mm_table[i].aux);
        }
        printf("\n");
        for (i = 0; i < jump_nodes_num; i++)
        {
        }
      }
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int macro_micro_query(int query_node, int query_level)
    {
      int i, j;
      int mydiff, mypow, mylog, root;

      if (query_level > tree[query_node].depth) return -1;
      if (query_level == tree[query_node].depth) return query_node;
      switch (mm_table[query_node].type)
      {
        case MACRO: 
      return macro_micro_query(mm_table[query_node].aux, query_level);
      break;
        case MICRO: 
      root = mm_table[query_node].aux;
      if (query_level < tree[root].depth)
      {
        return macro_micro_query(tree[root].parent, query_level);
      }
      else
      {
        unsigned int my_signature; 
        int root_depth;

        my_signature = mm_table[root].aux;
        root_depth = tree[root].depth;
    if (DEBUG_MMQUERY) printf("My signature = %d, ", my_signature);
    if (DEBUG_MMQUERY) printf("root = %d, ", root);
    if (DEBUG_MMQUERY) printf("query_node = %d, ", query_node);
    if (DEBUG_MMQUERY) printf("query_level = %d, ", query_level);
    if (DEBUG_MMQUERY) printf("root_depth = %d\n", root_depth);

    if (DEBUG_MMQUERY) printf("MICRO query returns root (%d) + micro_tables[%d][%d][%d] (%d) = %d\n",
      root, my_signature, query_node - root, query_level - root_depth, micro_tables[my_signature][query_node - root][query_level - root_depth], \
      root + micro_tables[my_signature][query_node - root][query_level - root_depth]);
        return (root + micro_tables[my_signature][query_node - root][query_level - root_depth]);
      }
      break;
        case MICROROOT:	 
      return macro_micro_query(tree[query_node].parent, query_level);
      break;
        case JUMP: 
      mydiff = tree[query_node].depth - query_level;
    /*
          frexp((double)mydiff, &mylog);
          mylog--;
    */
      mylog = log_base2(mydiff);
    /*
      mypow = (int) pow((double)2, (double)mylog);
    */
            mypow = 1 << mylog;
      if (mydiff == mypow)
      {
        return mm_jump[mm_table[query_node].aux][mylog];
      }
      else
      {
    if (DEBUG_MMQUERY) printf("We are ladder querying node jump_table[%d][%d] at level %d\n",\
      mm_table[query_node].aux, mylog, query_level);
        return ladder_query(mm_jump[mm_table[query_node].aux][mylog], query_level);
      }
      break;
      }
    }
    /************************************************************************/

  #elif LA_ALGORITHM == DYNAMIC

    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void create_micro_table(unsigned int signature, int size)
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }

    void macro_micro_preprocessing()
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int macro_micro_query(int query_node, int query_level)
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_macro_micro_leaf(int parent, int leaf, bool is_left_child){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }   

    void remove_macro_micro_leaf(int leaf){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/
  #endif
#endif