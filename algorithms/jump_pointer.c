#include "../la_algorithm.h"

#if LA_ALGORITHM == JUMP_POINTER || LA_ALGORITHM == JUMP_LADDER

  #include "jump_pointer.h"

  /************************************************************************/
  /* Preprocessing						*/
  /************************************************************************/
  void jump_pointer_preprocessing()
  {
    int i, j;
    int next;
    int mylog;

    jump_table = alloc(n*sizeof(int*));
    for (i = 1; i < n; i++)
    {
  /*
      mylog = log_base2((double) tree[i].depth);
  */
      frexp((double)tree[i].depth, &mylog);
      mylog--;
      jump_table[i] = alloc((mylog+1)*sizeof(int));
      jump_table[i][0] = tree[i].parent;
      next = tree[i].parent;
      for (j = 1; j <= mylog; j++)
      {
        if (DEBUG_JUMP) printf("jump_table[%d][%d] = jump_table[%d][%d] = %d\n", \
    i, j, next, j-1, jump_table[next][j-1]);
        jump_table[i][j] = jump_table[next][j-1];
        next = jump_table[next][j-1];
      }
    }
    if (DEBUG_JUMP)
    {
      for (i = 1; i < n; i++)
      {
        printf("Node %d: ", i);
        for (j = 0; j < log_base2(tree[i].depth); j++)
        {
          printf("%d, ", jump_table[i][j]);
        }
        printf("%d\n", jump_table[i][log_base2(tree[i].depth)]);
      }
    }
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
#endif