# PARALLEL_STRING_SEARCH
in this folder is a parallel string search that parallelize the Knuth-Morris-Pratt algorithm, in mpi_search.c use a mpi and search.c use a pthread 
both use reader.c for read the file in input.
file reader.c 
compiler:
--gcc -Wall -o reader reader.c

file for pattern match in MPI 
-mpi_search.c 
use:
reader.c  for read the file 
mpi_search.c for search the pattern in the local text
for comile run:
mpicc -g -Wall -o mpi_search mpi_search.c
for run 
mpiexec -n  <number of process> mpi_search <string to search> <path file>

file for pattern match in Pthread 
-pthread_serach.c 
use:
reader.c  for read the file 
for compile run:
-gcc -Wall -o pthread_search pthread_search.c -lpthread

for run:
-./pthread_serach <number of thread> <string to search> <path file>
