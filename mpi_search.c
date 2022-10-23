#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "search.c"
#include "reader.c"

//var used 
char * file = NULL; //file to read 
double start, end , min_start, max_end;
int comm_sz; /*number of processes*/
int my_rank; /*my process rank*/
int local_int [2] = {0,0}; //count the number of the repetition and the last idex of pattern found 
int tot= 0; //tot number of repetiotion
int * table= NULL;//pointer of table of support 
int local_i = 0;
int len_pattern;//lenght  of the pattern 
char * pattern =NULL; //pointer of the pattern 
int  len_local_test [1];  //var for ricive buffer
int global_start_text;
//var for scatterv 
int * send_counter;


char* local_test;
int *disp;
int * index_p;
int ping_master;
int ping_resp;


void create_tabel(char  * pattern, int my_rank);
void slave(char  * pattern);
void search_text(char local_test[], char  *pattern);
void check_worker();





int main(int argc, char * argv[]){


    //chake the input 

     if(argc <= 2){
        
              printf("\n number of arg need to be 2 <string to search> <path of file>\n");
              return 0;
        }
    MPI_Init( NULL,NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);


    if(comm_sz < 2){
        printf("need at last 2 process");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    
    start = MPI_Wtime(); //start timer
    
    len_pattern = strlen(argv[1]);
    pattern = (char *)malloc(sizeof(char)* len_pattern);
    
    pattern = argv[1];
   
   
    if(my_rank == 0){
      
        //this I'm the master 
        file = read_file(argv[2]); // read the file 
        if(file == NULL){
            printf("file not extis");
            MPI_Abort(MPI_COMM_WORLD,1);
            
        }
         int len_file = strlen(file);
         //make the array for send data 
       
         int  send_count = len_file/(comm_sz-1);
         int remaind = len_file%(comm_sz-1);
         
        send_counter = (int *)malloc(sizeof(int)* comm_sz);
        disp = (int *)malloc(sizeof(int)*comm_sz);
        send_counter[0] = 0;
        disp[0] = 0;
        int p = len_pattern-1;
         for(int i = 1 ; i < comm_sz ; i++){
             disp[i] = disp[i-1]+send_counter[i-1]-p;
             if(i == 1){
                disp[i]= 0;
             }
            if(i == comm_sz-1){
                p = 0;
            }
             
            if (remaind > 0 && remaind <= i){
                send_counter[i]= send_count+1+p;
                remaind--;

             }
            else{
                   send_counter[i]= send_count+p;
         }
         }
        

   
    MPI_Scatter(send_counter, 1, MPI_INT, len_local_test, 1, MPI_INT,0, MPI_COMM_WORLD);

        
     local_test = (char *)malloc(sizeof(char)* len_local_test[0]+1);
      MPI_Scatter(disp, 1, MPI_INT, &global_start_text, 1, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Scatterv(file, send_counter, disp, MPI_CHAR, local_test, len_local_test[0]+1, MPI_CHAR, 0, MPI_COMM_WORLD);


    
    
    int state[comm_sz];
    state[0]=0;
    for(int i = 1; i < comm_sz ; i++){
            state[i]=1;

        }
    int complet = 0;
    MPI_Status status;
    MPI_Request request;
   
    int rec;
    while(complet < comm_sz){
  
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &ping_master, &status);
     
        if(ping_master==1){

            
            MPI_Recv(&rec , 1, MPI_INT, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(rec == 1){
            state[status.MPI_SOURCE]=0;
            }
        }
   
        int count;
       
        for(int i = 0; i < comm_sz-1 ; i++){
         
            if(state[i]!=0){
                state[i]++;
                if(state[i]>999999){
              
                for(int j = 1; j < comm_sz ; j++){
                    char * send = malloc(sizeof(char)* send_counter[i]);
                    send = file + disp[i];
                    if(state[j]==0){
                        MPI_Isend(send, send_counter[i], MPI_CHAR, j ,0, MPI_COMM_WORLD, &request);
                        state[j]=1;
                        break;
                    }
                    
                }
            
            }
            }
            
            else{
                count++;
            }
            
        }
        if(complet<count){
            complet = count;
        }

    
    }
    
    }

    else{
         create_tabel(pattern, my_rank);
 
        MPI_Scatter(send_counter, 1, MPI_INT, len_local_test, 1, MPI_INT,0, MPI_COMM_WORLD);
        MPI_Scatter(disp, 1, MPI_INT, &global_start_text, 1, MPI_INT,0, MPI_COMM_WORLD);

        int size = len_local_test[0]+1; 
        local_test = (char *)malloc( sizeof(char)*size);
        index_p = (int *)malloc(sizeof(int)* size);
       
        MPI_Scatterv(file, send_counter, disp, MPI_CHAR, local_test ,len_local_test[0], MPI_CHAR, 0, MPI_COMM_WORLD);
        local_test[size-1] = '\0';
        ping_resp = 1;
        MPI_Request  request;
        MPI_Status status;
        if(MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &ping_resp, &status)){
            int len;
            MPI_Get_count(&status, MPI_CHAR, &len );
            char * local_test = malloc(sizeof(char)* (len+1));
            MPI_Recv(local_test, len, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("%s \n\n",local_test);
            search_text(local_test, pattern);
        }
         if(my_rank == 2){
            sleep(2);
            sleep(2);
        }
    
        search_text(local_test, pattern);
        
      
        
       
        MPI_Isend(&ping_resp, 1, MPI_INT, 0, 0,MPI_COMM_WORLD, &request);
        

       
        

    }
    MPI_Reduce( &local_int[1], &tot,1,  MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){

        printf("\n numbero of repetition founded %d \n", tot);
    }

    end = MPI_Wtime();
    MPI_Reduce(&start, &min_start,1,  MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD );
    MPI_Reduce(&end, &max_end,1,  MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

    if(my_rank == 0){
       
       printf("\n timing %f:\n", end-start);
        printf("\n len text: %ld\n", strlen(file));
        
    }
  



    
    MPI_Finalize();
    
    
    
    
    return 0;
}




void create_tabel(char * pattern, int my_rank){
 
    int len_pattern = strlen(pattern);
    table = (int *)malloc(sizeof(int)* len_pattern);
    int len = 0; 
    int j = 1; 
    table[0]=0;
    while(j < len_pattern){
        if(pattern[j]==pattern[len]){
            len++;
            table[j]=len;
            j++;
        }
        else{
            if(len != 0 ){
                len = table[len]-1;
            }
            else{
                table[j-1]=0;
                table[j]= 0;
                j++;
            }
        }

    }



    
}





void search_text(char local_test[], char *pattern){
    
    numer_repitition(local_test, pattern, table, local_i,strlen(local_test), local_int, my_rank, index_p,  global_start_text);

}

