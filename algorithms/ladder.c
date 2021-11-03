#include "../la_algorithm.h"

#if LA_ALGORITHM == LADDER || LA_ALGORITHM == JUMP_LADDER || LA_ALGORITHM == MACRO_MICRO
  #include "ladder.h"

  #if LA_ALGORITHM == STATIC
    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void ladder_preprocessing()
    {
      int i, j;
      int ladders_num;
      int current_ladder_root, new_ladder_root, current_ladder, upwards_extension;
      int ladder_pos, ladder_size, current_node;
      int *height;

      height = alloc(n*sizeof(int));
      ladders_num = 0;

      for (i = n-1; i >= 0; i--)
      {
        if (tree[i].right == -1)
        {
          if (tree[i].left == -1)
          {
            ladders_num++;
            height[i] = 1;
          }
          else
          {
            height[i] = height[tree[i].left] + 1;
          }
        }
        else
        {
          if (height[tree[i].left] > height[tree[i].right])
          {
            height[i] = height[tree[i].left] + 1;
          }
          else
          {
            height[i] = height[tree[i].right] + 1;
          }
        }
      }
      
      if (DEBUG_LADDER)
      {
        printf("Height: ");
        for (i = 0; i < n; i++)
          printf("%d ", height[i]);
        printf("\n");
      }
      
      ladder_table = alloc(ladders_num*sizeof(int*));
      ladder_roots = alloc(ladders_num*sizeof(int*));
      ladder_index = alloc(n*sizeof(struct ladder_index_node));
      ladder_roots[0] = 0;
      new_ladder_root = 1;
      current_ladder = 0;

      while(current_ladder < new_ladder_root)
      { 
        current_ladder_root = ladder_roots[current_ladder];

        if (height[current_ladder_root] < tree[current_ladder_root].depth)
        {
          ladder_pos = height[current_ladder_root];
          ladder_size = 2*height[current_ladder_root];
          upwards_extension = height[current_ladder_root]-1;
        }
        else
        {
          ladder_pos = tree[current_ladder_root].depth;
          ladder_size = height[current_ladder_root] + tree[current_ladder_root].depth;
          upwards_extension = tree[current_ladder_root].depth-1;
        }

        ladder_table[current_ladder] = alloc(ladder_size*sizeof(int));
        current_node = current_ladder_root;

        for(i = 0; i < height[current_ladder_root]; i++)
        {
          ladder_table[current_ladder][ladder_pos+i] = current_node;
          ladder_index[current_node].no = current_ladder;
          ladder_index[current_node].pos = ladder_pos+i;

          if (tree[current_node].right != -1)
          {
            if (height[tree[current_node].left] >= height[tree[current_node].right])
            {
              ladder_roots[new_ladder_root++] = tree[current_node].right;
              current_node = tree[current_node].left;
            }
            else
            {
              ladder_roots[new_ladder_root++] = tree[current_node].left;
              current_node = tree[current_node].right;
            }
          }
          else
          {
            current_node = tree[current_node].left;
          }
        } 

        current_node = current_ladder_root;

        for(i = upwards_extension; i >= 0; i--)
        {
          ladder_table[current_ladder][i] = tree[current_node].parent;
          current_node = tree[current_node].parent;
        }

        if (DEBUG_LADDER)
        {
          printf("Ladder %d: ", current_ladder);
          for (i = 0; i < ladder_size-1; i++)
          {
            printf("%d, ", ladder_table[current_ladder][i]);
          }
          printf("%d\n", ladder_table[current_ladder][ladder_size - 1]);
        }

        current_ladder++;
      }

      free(height);
      free(ladder_roots);
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int ladder_query(int query_node, int query_level)
    {
      int i, j;
      int mydiff, myladder, mypos;

      mydiff = tree[query_node].depth - query_level;
      if (mydiff < 0) return -1;
      if (mydiff == 0) return query_node;
      myladder = ladder_index[query_node].no;
      mypos = ladder_index[query_node].pos;
      if (tree[ladder_table[myladder][0]].depth <= query_level)
      {
        return ladder_table[myladder][mypos - mydiff];
      }
      else
      {
        return ladder_query(ladder_table[myladder][0], query_level);
      }
    }
    /************************************************************************/
  #elif LA_ALGORITHM == DYNAMIC

    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void ladder_preprocessing()
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int ladder_query(int query_node, int query_level)
    {
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);

      return 0;
    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_ladder_leaf(int parent, int leaf, bool is_left_child){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }   

    void remove_ladder_leaf(int leaf){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/
  #endif
#endif