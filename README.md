# LevelAncestors
Research done at The College of New Jersey for studying the practical implementations of proposed Level Ancestor algorithms and the performance of preprocessing and query time for varying tree and query sizes.

## Branch: **main**
This branch contains the static versions of the level ancestor algorithms.

## Wiki
Read more detailed information and guides on the wiki.
https://github.com/mattmabrey1/LevelAncestors/wiki

## Issues
Read about issues, bugs, or any remaining work on the issues page.
https://github.com/mattmabrey1/LevelAncestors/issues

--- 
# Quickstart Guide

## Compiling the algorithms

- Begin by cloning the repository to your local machine or virtual machine using "git clone"

- To compile the code to run the algorithms you must run the "make" command while inside the LevelAncestors/ directory
    - The Makefile has the different commands possible for compiling the code
        - make : compiles the static algorithms normally with no output besides the performance times
        - make debug : compiles the static algorithms with result debugging and verification of query results
        - make clean : cleans up all algorithm executables from the directory

--- 
## Generating a tree and queries

To generate a tree for the program use the tree_generator executable as such:
> ./tree_generator <size_of_tree> <randomization_seed>

To generate queries for the program use the query_generator executable as such:
> ./tree_queries <number_of_queries> <randomization_seed>

The output of the tree generation scripts can be piped into an algorithm as input or stored in a file.
It's preferrable to store different size steps of trees in a folder so that tree generation isn't needed each time you run a level ancestor algorithm.
This can be done with:
> ./tree_generator <size_of_tree> <seed> | ./tree_queries <number_of_queries> <randomization_seed> > level_ancestor_input
  
A new file will be created called **level_ancestor_input**. You should give it a more descriptive name that includes the size of the tree and possibly seed used to generate it.

The alternative method of piping input directly into the algorithm would be called as such:
> ./tree_generator <size_of_tree> <seed> | ./tree_queries <number_of_queries> <randomization_seed> | ./algorithm_executable
 
--- 
## Executing an algorithm
To execute an algorithm you will need the name of the algorithm you want to run as each algorithm compiles into a seperate executable. Using the Jump-pointer algorithm as an example, you would call it as such:
  > ./jump_pointer < level_ancestor_input
