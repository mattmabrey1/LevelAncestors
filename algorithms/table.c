#include "../la_algorithm.h"

#if LA_ALGORITHM == TABLE
  #include "table.h"

  #if LA_ALGORITHM == STATIC
    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void table_preprocessing()
    {
      int i, j;

      big_table = alloc(n*sizeof(int*));
      big_table[0] = alloc(sizeof(int));
      big_table[0][0] = 0;
      for (i = 1; i < n; i++)
      {
        big_table[i] = alloc((tree[i].depth+1)*sizeof(int));
        for (j = 0; j < tree[i].depth; j++)
        {
          big_table[i][j] = big_table[i-1][j];
        }
        big_table[i][tree[i].depth] = i;
      }
      if (DEBUG_TABLE)
      {
        for (i = 0; i < n; i++)
        {
          printf("Line %d: ", i);
          for (j = 0; j < tree[i].depth; j++)
          {
      printf("%d, ", big_table[i][j]);
          }
          printf("%d\n", big_table[i][tree[i].depth]);
        }
      }
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int table_query(int q_node, int q_level)
    {
      if (tree[q_node].depth < q_level) 
        return -1;
      return big_table[q_node][q_level];
    }
    /************************************************************************/

  #elif LA_ALGORITHM == DYNAMIC
    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void table_preprocessing()
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int table_query(int q_node, int q_level)
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);

      return 0;
    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_table_leaf(int parent, int leaf, bool is_left_child){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }   

    void remove_table_leaf(int leaf){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/
  #endif
  
#endif