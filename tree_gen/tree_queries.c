#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 0

int n, left, right, queries;
long seed;
int *tree;

int main(int argc, char *argv[])
{
  int i;
  int cur_pos, cur_height;
  int query_pos, query_height;
  char c;

  if (argc != 3)
  {
    printf("\nUsage: tree_queries <number of queries> <randomization seed>\n\n");
    printf("This program generates specified random level ancestor queries\n");
    printf("for the tree on its standard input, as produced by the\n");
    printf("tree_generator program. It outputs the tree and the queries\n");
    printf("on the standard output, ready as input for the la program.\n\n");
    exit(-1);
  }
  queries = atoi(argv[1]);
  seed = (long) atoi(argv[2]);

  srand48(seed);
  scanf("%d\n", &n);
  printf("%d\n", n);
  tree = malloc(n*(sizeof(int)));
  tree[0] = 0;
  cur_pos = 1;
  cur_height = 0;
  for (i = 0; i < 2*n-2; i++)
  {
    c = getchar();
    putchar(c);
    if (c == '1')
    {
      tree[cur_pos++] = ++cur_height;
    }
    else if (c == '0')
    {
      --cur_height;
    }
    else
    {
      printf("Illegal input: Expecting '0' or '1'\n");
      exit(-1);
    }
  }
  printf("\n");
  if (DEBUG)
  {
    for (i = 0; i < n-1; i++)
      printf("%d, ", tree[i]);
    
    printf("%d\n", tree[n-1]);
  }
  printf("%d\n", queries);
  for (i = 0; i < queries; i++)
  {
    query_pos = (int) fmod((double)lrand48(), (double)(n));
    query_height = (int) fmod((double)lrand48(), (double)(tree[query_pos]+1));
    printf("%d, %d\n", query_pos, query_height);
  }
}
