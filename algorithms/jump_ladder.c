#include "../la_algorithm.h"

#if LA_ALGORITHM == JUMP_LADDER

  #include "jump_ladder.h"
  #include "jump_pointer.h"
  #include "ladder.h"

  /************************************************************************/
  /* Preprocessing						*/
  /************************************************************************/
  void jump_ladder_preprocessing()
  {
    jump_pointer_preprocessing();
    ladder_preprocessing();
  }
  /************************************************************************/

  /************************************************************************/
  /* Querying						*/
  /************************************************************************/
  int jump_ladder_query(int query_node, int query_level)
  {
    int i, j;
    int mydiff, mypow, mylog;

    if (query_level > tree[query_node].depth) return -1;
    if (query_level == tree[query_node].depth) return query_node;
    mydiff = tree[query_node].depth - query_level;
  /*
    mylog = log_base2((double) mydiff);
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
      return ladder_query(jump_table[query_node][mylog], query_level);
    }
  }
  /************************************************************************/
#endif