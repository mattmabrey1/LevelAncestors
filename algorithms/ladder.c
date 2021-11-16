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
      ladder_index = alloc(n * sizeof(struct ladder_index_node));
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
      int i, j;
      int ladders_num;
      int current_ladder_root, new_ladder_root, current_ladder, upwards_extension;
      int ladder_pos, ladder_size, current_node;
      int *height;

      height = alloc(n*sizeof(int));
      ladders_num = 0;

      for (i = n-1; i >= 0; i--)
      {
        if (tree.data[i]->right == -1)
        {
          if (tree.data[i]->left == -1)
          {
            ladders_num++;
            height[i] = 1;
          }
          else
          {
            height[i] = height[tree.data[i]->left] + 1;
          }
        }
        else
        {
          if (height[tree.data[i]->left] > height[tree.data[i]->right])
          {
            height[i] = height[tree.data[i]->left] + 1;
          }
          else
          {
            height[i] = height[tree.data[i]->right] + 1;
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
      
      vec_init(&ladder_table);
      vec_reserve(&ladder_table, ladders_num);

      ladder_roots = alloc(ladders_num*sizeof(int*));

      vec_init(&ladder_index_no);
      vec_reserve(&ladder_index_no, n);
      vec_init(&ladder_index_pos);
      vec_reserve(&ladder_index_pos, n);

      ladder_roots[0] = 0;
      new_ladder_root = 1;
      current_ladder = 0;

      while(current_ladder < new_ladder_root)
      { 
        current_ladder_root = ladder_roots[current_ladder];

        if (height[current_ladder_root] < tree.data[current_ladder_root]->depth)
        {
          ladder_pos = height[current_ladder_root];
          ladder_size = 2*height[current_ladder_root];
          upwards_extension = height[current_ladder_root]-1;
        }
        else
        {
          ladder_pos = tree.data[current_ladder_root]->depth;
          ladder_size = height[current_ladder_root] + tree.data[current_ladder_root]->depth;
          upwards_extension = tree.data[current_ladder_root]->depth-1;
        }
        
        vec_int_t* ladder = alloc(sizeof(vec_int_t));
        vec_init(ladder);
        vec_reserve(ladder, ladder_size);

        vec_push(&ladder_table, ladder);

        current_node = current_ladder_root;

        for(i = 0; i < height[current_ladder_root]; i++)
        {
          vec_insert(ladder, ladder_pos + i, current_node);

          vec_insert(&ladder_index_no, current_node, current_ladder);
          vec_insert(&ladder_index_pos, current_node, ladder_pos + i);

          if (tree.data[current_node]->right != -1)
          {
            if (height[tree.data[current_node]->left] >= height[tree.data[current_node]->right])
            {
              ladder_roots[new_ladder_root++] = tree.data[current_node]->right;
              current_node = tree.data[current_node]->left;
            }
            else
            {
              ladder_roots[new_ladder_root++] = tree.data[current_node]->left;
              current_node = tree.data[current_node]->right;
            }
          }
          else
          {
            current_node = tree.data[current_node]->left;
          }
        } 
        
        current_node = current_ladder_root;

        for(i = upwards_extension; i >= 0; i--)
        {
          vec_insert(ladder, i, tree.data[current_node]->parent);

          current_node = tree.data[current_node]->parent;
        }

        if (DEBUG_LADDER)
        {
          printf("Ladder %d: ", current_ladder);
          for (i = 0; i < ladder_size-1; i++)
          {
            printf("%d, ", ladder_table.data[current_ladder]->data[i]);
          }
          printf("%d\n", ladder_table.data[current_ladder]->data[ladder_size - 1]);
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

      mydiff = tree.data[query_node]->depth - query_level;

      if (mydiff < 0) return -1;
      if (mydiff == 0) return query_node;

      myladder = ladder_index_no.data[query_node];
      mypos = ladder_index_pos.data[query_node];

      if (tree.data[ladder_table.data[myladder]->data[0]]->depth <= query_level)
      {
          return ladder_table.data[myladder]->data[mypos - mydiff];
      }
      else
      {
          return ladder_query(ladder_table.data[myladder]->data[0], query_level);
      }
    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_ladder_leaf(int parent, int leaf, bool is_left_child){
      
      int parent_ladder = ladder_index_no.data[parent];

      if (is_left_child) 
      {
          vec_insert(&ladder_index_no, leaf, parent_ladder);
          vec_insert(&ladder_index_pos, leaf, ladder_table.data[parent_ladder]->length);

          vec_push(ladder_table.data[parent_ladder], leaf);
      }
      else
      {
          vec_int_t* new_ladder = alloc(sizeof(vec_int_t));
          vec_push(new_ladder, parent);
          vec_push(new_ladder, leaf);
          
          vec_insert(&ladder_index_no, leaf, ladder_table.length);
          vec_insert(&ladder_index_pos, leaf, 1);

          vec_push(&ladder_table, new_ladder);
      }
    }   

    void remove_ladder_leaf(int leaf){
      printf("Error: Function Not Yet Implemented!\n");
      exit(-1);
    }
    /************************************************************************/
  #endif
#endif