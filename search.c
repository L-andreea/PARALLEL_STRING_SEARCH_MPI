
#include <string.h>
#include <stdio.h>
#include <unistd.h>



void numer_repitition(char* text, char * pattern, int * tabel, int i, int len_text, int result[], int my_r, int * index, int start_text){
  
    
    int j=0;
    int len_pattern = strlen(pattern);
    int count = 0; //var for how loop need 
    int count_pattern = 0;// var for how 
    while((j < len_text  ) ){  //enter in loop 
      
        count ++;
        
        if(pattern[i]== text[j]){
          
             j++;
             i++;


         }
        
         if(i == len_pattern ){
        
            index[result[1]] = start_text + (j-len_pattern);
            result[1]=result[1]+1;
             i = tabel[i-1]; 
            
         }
         else if((j < len_text) && (pattern[i] != text[j])){
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
  
   
}

