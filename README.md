# LevelAncestors
Research done at The College of New Jersey for studying the practical implementations of proposed Level Ancestor algorithms and the performance of preprocessing and query time for varying tree and query sizes.

--- 
## Compiling the algorithms

- Begin by cloning the repository to your local machine or virtual machine using "git clone"

- To compile the code to run the algorithms you must run the "make" command while inside the LevelAncestors/ directory
    - The Makefile has the different commands possible for compiling the code
        - make : compiles the static algorithms normally with no output besides the performance times
        - make debug : compiles the static algorithms with result debugging and verification of query results
        - make dynamic : compiles the dynamic algorithms normally with no output besides the performance times
        - make dynamic-debug : compiles the dynamic algorithms with result debugging and verification of query results
        - make clean : cleans up all algorithm executables from the directory

--- 
## Creating a tree

To create a tree for the program use the tree_generator executable compiled with every make command as such:
> ./tree_generator <size_of_tree> <randomization_seed>

The output of the tree generation scripts can be piped into an algorithm as input or stored in a file.
I prefer to store different size steps of tree in a folder so that tree generator isn't needed each time I need to run an algorithm.
This can be done with:
> ./tree_generator <size_of_tree> <seed> > my_generated_tree
  
A new file will be created called **my_generated_tree**, but you should give it a more descriptive name that includes the size of the tree and possibly seed used to generate it.
  
--- 
## Executing an algorithm
To execute an algorithm you will need the name of the algorithm you want to run as each algorithm compiles into a seperate executable. Using the Jump-pointer algorithm as an example you would call it as such:
  > ./jump_pointer <number_of_operations> <leaf_insertion_ratio> <randomization_seed> < my_generated_trees