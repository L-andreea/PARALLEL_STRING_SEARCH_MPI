#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "search.c"
#include "reader.c"






int main(void){
    double start, end ;
    int comm_sz; /*number of processes*/
    int my_rank; /*my process rank*/
    
    MPI_Init( NULL , NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);
    start = MPI_Wtime(); //start timer
    char * file = NULL;
    //char text [12] = "ababcababdig";
    int  local_int [2] = {0,0}; //count the number of the repetition and the last idex of pattern found 
    int tot= 0; //tot number of repetiotion
    char pattern [5] = "abab"; // pattern to search 
    int tabel [4] = {0,0,0,0}; // tabel of help search
    int len_pattern = strlen(pattern);
    int send_count = 0;
    char greeting[10000];
    
    
    //creating tabel of helping with repetition 
    int len = 0; 
    int j = 1; 
    while(j < len_pattern){
        if(pattern[j]==pattern[len]){
            len++;
            tabel[j]=len;
            j++;
        }
        else{
            if(len != 0 ){
                len = tabel[len-1];
            }
            else{
                tabel[j]= 0;
                j++;
            }
        }
    }
    //send_count = strlen(text) / comm_sz; //this need to be outside (if not work )
    
    if(my_rank == 0){
        //this I'm the master 
        
        
        
        
    }
    file = read_file();
    
    send_count = strlen(file)/comm_sz;
    
    char local_test [send_count+1];
    printf("send count : %d", send_count);
    MPI_Scatter(file, send_count, MPI_CHAR, local_test, send_count, MPI_CHAR, 0, MPI_COMM_WORLD);
    local_test[send_count]= '\0';
    


    //MPI_Scatterv(text,send_count, send_count, MPI_CHAR, local_test, send_count, MPI_CHAR, 0, MPI_COMM_WORLD);
    //printf("my rank is: %d- and local-text :%s \n", my_rank, loacl_test);
    if(my_rank != 0){
            sprintf(greeting, "my_rank: %d and local test : %s" , my_rank, local_test);
            printf("\n loacl test: %s my rank %d \n" , local_test, my_rank);
            MPI_Send( greeting , strlen(greeting)+1 , MPI_CHAR , 0 , 0 , MPI_COMM_WORLD);

    }

    else{
        printf("\n my loacl test is : %s \n", local_test);
        printf(" Greeting from process %d of %d \n", my_rank, comm_sz);
        for(int q = 1 ; q < comm_sz ; q++){
            MPI_Recv(greeting, 10000, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                     printf(" \n %s\n", greeting);
        }
    }

    

    numer_repitition(local_test, pattern, tabel, 0,strlen(local_test), local_int);
   
    int local_i;
    //printf("print local int %d",local_int[1]);
    if((my_rank< comm_sz-1) && (local_int[0]>0) ){
        MPI_Send(&local_int[0], 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);

    }
    if((my_rank > 0)&&(my_rank< comm_sz)){
        MPI_Recv(&local_i, 1,MPI_INT, my_rank-1, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        numer_repitition(local_test, pattern, tabel, local_i, strlen(pattern), local_int);
    }
    MPI_Reduce( &local_int[1], &tot,1,  MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
        printf("\n numbero of repetition founded %d \n", tot);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
   
    MPI_Finalize();
    printf("\n tempo impegato: %f \n", end-start);
    return 0;
}