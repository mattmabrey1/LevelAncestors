#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 0

int n, left, right;
long seed;
char *tree;

void fill_tree(int start, int nodes)
{
  int n, left, right;

  if (DEBUG) printf("Start = %d, nodes = %d\n", start, nodes);
  n = nodes-1;
  right = (int)fmod((double)lrand48(), (double)(n));
  if (DEBUG) printf("Right = %d, ", right);
  left = n - right;
  if (DEBUG) printf("Left = %d\n", left);
  tree[start] = '1';
  tree[start + 2*left - 1] = '0';
  if (right > 0)
  {
    tree[start + 2*left] = '1';
    tree[start + 2*n - 1] = '0';
  }
  if (left > 1) fill_tree(start + 1, left);
  if (right > 1) fill_tree(start + 2*left + 1, right);
}

int main(int argc, char *argv[])
{
  int i;

  if (argc != 3)
  {
    printf("\nUsage: tree_generator <number of nodes> <randomization seed>\n\n");
    printf("This program generates a dfs traversal of a random tree\n");
    printf("with the argument-specified number of nodes.\n");
    printf("Each downwards traversal of an edge is represented by '1'\n");
    printf("where each upwards traversal is represented by a '0'.\n");
    printf("The output of the program is produced on the standard output\n");
    printf("in ascii format, which was favored to the binary (and 8-fold\n");
    printf("space saving) binary format due to easier debugging considerations.\n");
    printf("Although the tree is generated randomly, balanced trees\n");
    printf("are favored probabilistically.\n\n");
    exit(-1);
  }
  n = atoi(argv[1]);
  seed = (long) atoi(argv[2]);
  if (DEBUG) printf("Number of nodes = %d\n", n);

  srand48(seed);
  tree = malloc(2*n*(sizeof(char)));
  fill_tree(0, n);
  printf("%d\n", n);
  for (i = 0; i < 2*n - 2; i++)
    putchar(tree[i]);
  putchar('\n');
}
