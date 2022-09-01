
#include <string.h>
#include <stdio.h>
#include <unistd.h>



void numer_repitition(char* text, char * pattern, int * tabel, int i, int len_text, int result[], int my_r, int * index, int start_text){
    //printf("\n pattern is %s and text is %s and my_rank is %d\n",pattern, text, my_r);
    
    int j=0;
    int len_pattern = strlen(pattern);
    int count = 0; //var for how loop need 
    int count_pattern = 0;// var for how 
  // printf("\nbefore while  len text %d\n , \n text: %s\n, pattern:%s\n len_text:%d and len_pattern:%d\n rank in function:%d", len_text, text, pattern, len_text, len_pattern, my_r);
    while((j < len_text  ) ){  //enter in loop 
      
        count ++;
        
        if(pattern[i]== text[j]){
           // printf("\n j:%d, \n first if %c, == %c \n",j, pattern[i],  text[j]);
             j++;
             i++;


         }
        
         if(i == len_pattern ){
           //  printf("secondo if i:%d , len_pattern: %d", i, len_pattern);
             //printf("\n substring found at index %d  \n", j-i);
            index[result[1]] = start_text + (j-len_pattern);
            result[1]=result[1]+1;
             i = tabel[i-1]; 
            
         }
         else if((j < len_text) && (pattern[i] != text[j])){
          //  printf("else if  pattern+i:%d , text+j: %d", pattern[i], text[j]);
                if(i != 0){
                 i = tabel[i-1];
             }
                else{
                 j = j+1;
             }

         }
    }

    result[0] = i; 
    result[1] = result[1]+count_pattern;
    //index  = (int *)realloc(index, sizeof(int)*(count_pattern));
    //printf("\ni:%d, count: %d my_rank %d \n", i, result[1], my_r);
    
   
}

