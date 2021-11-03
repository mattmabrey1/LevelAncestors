#include "../la_algorithm.h"

#if LA_ALGORITHM == 1
  #include "table.h"

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
#endif