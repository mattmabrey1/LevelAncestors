#include "../la_algorithm.h"

#if LA_ALGORITHM == JUMP_POINTER || LA_ALGORITHM == JUMP_LADDER

  #include "jump_pointer.h"

  #if LA_ALGORITHM == STATIC
    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void jump_pointer_preprocessing()
    {
      int i, j;
      int next;
      int mylog;

      jump_table = alloc(n * sizeof(int*));

      for (i = 1; i < n; i++)
      {
        frexp((double)tree[i].depth, &mylog);
        
        mylog--;

        jump_table[i] = alloc((mylog+1)*sizeof(int));
        jump_table[i][0] = tree[i].parent;
        next = tree[i].parent;

        for (j = 1; j <= mylog; j++)
        {
          #if DEBUG_JUMP == 1
            printf("jump_table[%d][%d] = jump_table[%d][%d] = %d\n", i, j, next, j-1, jump_table[next][j-1]);
          #endif

          jump_table[i][j] = jump_table[next][j-1];
          next = jump_table[next][j-1];
        }
      }

      #if DEBUG_JUMP == 1
        for (i = 1; i < n; i++)
        {
          printf("Node %d: ", i);
          for (j = 0; j < log_base2(tree[i].depth); j++)
          {
            printf("%d, ", jump_table[i][j]);
          }
          printf("%d\n", jump_table[i][log_base2(tree[i].depth)]);
        }
      #endif
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int jump_pointer_query(int query_node, int query_level)
    {
      int i, j;
      int mydiff, mypow, mylog;

      if (query_level > tree[query_node].depth) return -1;
      if (query_level == tree[query_node].depth) return query_node;
      do
      {
        mydiff = tree[query_node].depth - query_level;
    /*
        mylog = log_base2( mydiff);
    */
        frexp((double)mydiff, &mylog);
        mylog--;
    /*
        mypow = (int) pow((double)2, (double)mylog);
    */
        mypow = 1 << mylog;
        if (mydiff == mypow)
        {
          return jump_table[query_node][mylog];
        }
        else
        {
          query_node = jump_table[query_node][mylog];
        }
      } while (mydiff > mypow);
    }
    /************************************************************************/
  #elif LA_ALGORITHM == DYNAMIC
    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void jump_pointer_preprocessing()
    {
      int i, j;

      vec_init(&jump_table);
      vec_reserve(&jump_table, n);

      jump_table.length++;

      for (i = 1; i < n; i++)
      {
        add_jump_pointers(i);
      }

      #if DEBUG_JUMP == 1
        for (i = 1; i < n; i++)
        {
          printf("Node %d: ", i);
          for (j = 0; j < log_base2(tree.data[i]->depth); j++)
          {
            printf("%d, ", jump_table.data[i]->data[j]);
          }
          printf("%d\n", jump_table.data[i]->data[log_base2(tree.data[i]->depth)]);
        }
      #endif
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int jump_pointer_query(int query_node, int query_level)
    {
      int i, j;
      int mydiff, mypow, mylog;

      if (query_level > tree.data[query_node]->depth) return -1;
      if (query_level == tree.data[query_node]->depth) return query_node;
      do
      {
        mydiff = tree.data[query_node]->depth - query_level;
        
        frexp((double)mydiff, &mylog);
        mylog--;

        mypow = 1 << mylog;
        if (mydiff == mypow)
        {
          return jump_table.data[query_node]->data[mylog];
        }
        else
        {
          query_node = jump_table.data[query_node]->data[mylog];
        }
      } while (mydiff > mypow);
    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_jump_pointers(int node){

      int next, mylog;
      vec_int_t* jump_arr = alloc(sizeof(vec_int_t));

      frexp((double)tree.data[node]->depth, &mylog);

      mylog--;

      vec_init(jump_arr);
      vec_reserve(jump_arr, mylog + 1);

      vec_push(&jump_table, jump_arr);

      vec_push(jump_arr, tree.data[node]->parent);

      next = tree.data[node]->parent;

      for (int i = 1; i <= mylog; i++)
      {
        vec_push(jump_arr, jump_table.data[next]->data[i-1]);
          
        next = jump_table.data[next]->data[i - 1];
      }
    }

    void add_jump_pointer_leaf(int parent, int leaf, bool is_left_child){

      if(is_left_child){
        add_jump_pointers(leaf);
      }
      else{
        
        int left_sibling = tree.data[parent]->left;

        vec_push(&jump_table, jump_table.data[left_sibling]);
      }
      
    }   

    void remove_jump_pointer_leaf(int leaf){
        printf("Error: Function Not Yet Implemented!\n");
        exit(-1);
    }
    /************************************************************************/
  #endif
#endif