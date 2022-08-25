#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "search.c"
#include "reader.c"

//var used 
char * file = NULL; //file to read 
int send_count ; 
double start, end , min_start, max_end;
int comm_sz; /*number of processes*/
int my_rank; /*my process rank*/
int local_int [2] = {0,0}; //count the number of the repetition and the last idex of pattern found 
int tot= 0; //tot number of repetiotion
int * table= NULL;//[4] = {0,0,0,0}; // tabel of help search
//int len_pattern ;//= 4;
int local_i = 0;
int len_pattern;
char * pattern =NULL;
int  len_local_test [1];  //var for ricive buffer
int global_start_text;
//var for scatterv 
int * send_counter;

char* local_test;
int *disp;
int * index_p;


void create_tabel(char  * pattern, int my_rank);
void slave(char  * pattern);
void search_text(char local_test[], char  *pattern);





int main(int argc, char * argv[]){
    // printf("\n pattern %s\n", argv[1]);
    //  printf("\n name of file %s\n", argv[2]);
    
    //printf("\nprint start main \n ");
    MPI_Init( NULL,NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);
   // printf("\ninit mpi \n ");
    start = MPI_Wtime(); //start timer
    
    len_pattern = strlen(argv[1]);
    pattern = (char *)malloc(sizeof(char)* len_pattern);
    
    pattern = argv[1];
   // printf("\n len pattern: %d pattern: %s and my rank %d\n", len_pattern, pattern,my_rank );
    //MPI_Bcast(&len_pattern, 1, MPI_INT, 0,MPI_COMM_WORLD);
   // MPI_Bcast(pattern, len_pattern, MPI_CHAR, 0, MPI_COMM_WORLD);

    create_tabel(pattern, my_rank);
   //  printf("\n after create tabel\n ");
 
    
    //char text [12] = "ababcababdig";
    //char greeting[10000];
    if(my_rank == 0){
      
        //this I'm the master 
        file = read_file(argv[2]); // read the file 
         int len_file = strlen(file);
         //make the array for send data 
        printf("\n len_fine %d, file:%s \n", len_file, file);
          send_count = len_file/(comm_sz-1);
         int remaind = len_file%(comm_sz-1);
         //printf("send_count %d, remaind:%d", send_count, remaind);
          send_counter = (int *)malloc(sizeof(int)* comm_sz);
         disp = (int *)malloc(sizeof(int)*comm_sz);
          send_counter[0] = 0;
          disp[0] = 0;

         for(int i = 1 ; i < comm_sz ; i++){
             disp[i] = disp[i-1]+send_counter[i-1];
             if (remaind > 0 && remaind <= i){
                send_counter[i]= send_count+1;

             }
         else{
                   send_counter[i]= send_count;
         }
         }
         printf("\n disp and counter \n");
         for(int i = 0; i < comm_sz ; i++){
            printf("\n send_count di i : %d is %d and disp is %d\n", i, send_counter[i],disp[i]);
         }
         printf("\n \n");
 
    //printf("\n send count is : %d, send_count+pattern is :%d \n", send_count, send_count);
   // MPI_Bcast(tabel, strlen(pattern)+1, MPI_INT, 1, MPI_COMM_WORLD);

   
    MPI_Scatter(send_counter, 1, MPI_INT, len_local_test, 1, MPI_INT,0, MPI_COMM_WORLD);
  //   printf("\n rank 0 len local test is : %d", len_local_test[0]);
     local_test = (char *)malloc(sizeof(char)* len_local_test[0]+1);
      MPI_Scatter(disp, 1, MPI_INT, &global_start_text, 1, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Scatterv(file, send_counter, disp, MPI_CHAR, local_test, len_local_test[0]+1, MPI_CHAR, 0, MPI_COMM_WORLD);
  //  printf("\n rank 0 local test is %s",local_test);

        free(file);
       // free(send_counter);
       // free(disp);
   
    }

    else{
       // printf("\nbefore slave argc %s\n ", pattern);
        MPI_Scatter(send_counter, 1, MPI_INT, len_local_test, 1, MPI_INT,0, MPI_COMM_WORLD);
        MPI_Scatter(disp, 1, MPI_INT, &global_start_text, 1, MPI_INT,0, MPI_COMM_WORLD);
       // printf("\n len local test is : %d \n", len_local_test[0]);
       //worst case 
        int size = len_local_test[0]+1; 
        local_test = (char *)malloc( sizeof(char)*size);
        index_p = (int *)malloc(sizeof(int)* size);
        //printf("\n dopo malloc my rank %d and index_p len is %ld \n", my_rank , (sizeof(index_p)/sizeof(int)));
        MPI_Scatterv(file, send_counter, disp, MPI_CHAR, local_test ,len_local_test[0], MPI_CHAR, 0, MPI_COMM_WORLD);
        local_test[size-1] = '\0';
        //printf("\n my_rank is %d local test is : %s\n ",my_rank, local_test);
        
        
       
    

        search_text(local_test, pattern);

       
        

    }





    MPI_Reduce( &local_int[1], &tot,1,  MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){

        printf("\n numbero of repetition founded %d \n", tot);
    }

    //core dump seach here 
    //int * tot_index = (int *)malloc(sizeof(int)* tot);
    //int *rec_count = (int *)malloc(sizeof(int)* comm_sz);
    //int n = sizeof(index_p)/sizeof(int);

   // printf(" \n dopo realloc my rank %d and index_p len is %ld \n", my_rank , (sizeof(index_p)/sizeof(int)));
   // int * disp_index_p = (int *)malloc(sizeof(int)*my_rank);
    

    //MAKE REC_COUNT gather v is worng
    //MPI_Gather(&n, 1, MPI_INT, rec_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
   // disp_index_p[0]=0;
    //for(int i = 1; i< my_rank; i++){
    //    disp_index_p[i]= disp_index_p[i-1]+ rec_count[i-1];
    //}

   // MPI_Gatherv(index_p, n, MPI_INT, tot_index, rec_count, disp_index_p, MPI_INT,0, MPI_COMM_WORLD);
   
    
    end = MPI_Wtime();
    MPI_Reduce(&start, &min_start,1,  MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD );
    MPI_Reduce(&end, &max_end,1,  MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
  



    
    MPI_Finalize();
    
    
    
    
    return 0;
}




// create tabel of support for search and send it 
void create_tabel(char * pattern, int my_rank){
   // printf("create tabel");
    int len_pattern = strlen(pattern);
    table = (int *)malloc(sizeof(int)* len_pattern);
    int len = 0; 
    int j = 1; 
    while(j < len_pattern){
        if(pattern[j]==pattern[len]){
            len++;
            table[j]=len;
            j++;
        }
        else{
            if(len != 0 ){
                len = *(table + len-1);
            }
            else{
                table[j-1]=0;
                table[j]= 0;
                j++;
            }
        }

    }



     MPI_Bcast(table, len_pattern, MPI_INT, 0, MPI_COMM_WORLD);
    
}




void search_text(char local_test[], char *pattern){
    
    numer_repitition(local_test, pattern, table, local_i,strlen(local_test), local_int, my_rank, index_p,  global_start_text);
    
     printf("\n my_rank %d local i %d\n",my_rank, local_int[0]);
    //printf("print local int %d",local_int[1]);
    if((my_rank < comm_sz-1)){
        MPI_Send(&local_int[0], 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);

    }
    if(my_rank > 1){
        MPI_Recv(&local_i, 1,MPI_INT, my_rank-1, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(local_i > 0){
        numer_repitition(local_test, pattern, table, local_i, strlen(pattern)-1, local_int, my_rank, index_p, global_start_text);
        }
    }
    
    
   

}