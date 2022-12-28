# PARALLEL_STRING_SEARCH
in this folder is a parallel string search that parallelize the Knuth-Morris-Pratt algorithm with mpi.


- reader.c  read the path of the file in input 
- mpi_search.c is the file with the directive mpi 
- search.c  search the string in the chunk

compiler:

--gcc -Wall -o reader reader.c

file for pattern match in MPI 

-mpi_search.c 
use:

reader.c  for read the file 
search.c for search the pattern in the local text

for compile:

mpicc -g -Wall -o mpi_search mpi_search.c

for run:

-mpiexec -n  <number of process> mpi_search <string to search> <path file>

