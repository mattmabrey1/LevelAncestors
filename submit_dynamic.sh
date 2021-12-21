#!/usr/bin/bash

#SBATCH --chdir=./
#SBATCH --mail-user=*REPLACE_WITH_YOUR_TCNJ_EMAIL*
#SBATCH --mail-type=ALL
#SBATCH --job-name=Level_Ancestor_Dyn   # How the job will appear in Active Jobs list
#SBATCH --output=lvl_anc_dyn.%j.out     # Name of the output file created for the job
#SBATCH --nodes=1                       # Set to the # of servers you need
#SBATCH --ntasks=1                      # Set to the # of CPU cores you want to use
#SBATCH --partition=amd                 # Set to the partition/queue to use
#SBATCH --exclusive                     # Reserve the entire machine for my job
#SBATCH --time=2-00:00:00               # Format is days-hh:mm:ss

# Program Variables
tree_size=100000    # the number of nodes in the generated binary tree
num_of_operations=100000000   # the number of LA queries and leaf additions

year=$(date "+%y")
seed=$SLURM_JOB_ID$year

RunExperiment () {

    for k in {1..5}
    do
        echo -e "\n\n--------------------------------------------------------------------------\n"
        echo -e ">>>>>>>>>>>>>>>>>>>>  Run $k : Operations $num_of_operations Size $tree_size  Ratio $1 <<<<<<<<<<<<<<<<<<<<\n"
        echo -e "--------------------------------------------------------------------------\n"

        ./jump_pointer $num_of_operations $1 $2 < la_input

        ./menghani_matani $num_of_operations $1 $2 < la_input

        ./alstrup $num_of_operations $1 $2 < la_input
        
    done
}

# Get current directory
export PROJECTDIR=$(pwd)

# Make a "scratch" directory on local disk
export SCRATCHDIR=/local/scratch/$USER/$SLURM_JOB_ID
mkdir -p $SCRATCHDIR

# Copy your input files/programs/etc to the SCRATCHDIR. The "." at the end says to use the same name as the source filename.
echo "Copying inputs to scratch ..."
cp tree_generator $SCRATCHDIR/.
cp alstrup $SCRATCHDIR/.
cp jump_pointer $SCRATCHDIR/.
cp menghani_matani $SCRATCHDIR/.
echo "Done."

# Change into the "scratch" directory
cd $SCRATCHDIR

for i in {1..5}
do
    # Run your program
    echo -e "\n///////----- SEED($seed) -----////////"

    for j in {1..4}
    do 
        
        echo "Creating tree with size: $tree_size..."

        ./tree_generator $tree_size $seed > la_input

        echo "Done."

        RunExperiment 0.05 $seed
        RunExperiment 0.10 $seed
        RunExperiment 0.25 $seed
        RunExperiment 0.50 $seed

        let tree_size=tree_size*10
    done

    let seed=seed+1
    let tree_size=100000
done



# Copy output file(s) back to local directory (the "." represents the current working directory)
#echo "Copying outputs from scratch ..."
#cp myoutputfile $PROJECTDIR/.
echo "Done."

# Clean-up SCRATCHDIR
/bin/rm -fr $SCRATCHDIR
