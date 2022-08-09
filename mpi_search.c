#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include "search.c"





int main(void){
    int comm_sz; /*number of processes*/
    int my_rank; /*my process rank*/
    
    MPI_Init( NULL , NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);
    //char local_test [12]  ;
    char text [12] = "ababcababdig";
    int local_int= 0; //conut the namber of reptition 
    int tot= 0; //tot number of repetiotion
    char pattern [5] = "abab"; // pattern to search 
    int tabel [4] = {0,0,0,0}; // tabel of help search
    int len_pattern = strlen(pattern);
    int send_count = 0;
    char greeting[100];
    
    
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
    send_count = strlen(text) / comm_sz; //this need to be outside (if not work )
    char local_test [send_count+1];
    if(my_rank == 0){
        //this I'm the master 
        
        
        
    }
    
    MPI_Scatter(text, send_count, MPI_CHAR, local_test, send_count, MPI_CHAR, 0, MPI_COMM_WORLD);
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
            MPI_Recv(greeting, 100, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                     printf(" \n %s\n", greeting);
        }
    }

    

    local_int = numer_repitition(local_test, pattern, tabel);
    MPI_Reduce(&local_int, &tot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
        printf("\n numbero of repetition founded %d \n", tot);
    }
   
    MPI_Finalize();
    return 0;
}