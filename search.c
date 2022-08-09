
#include <string.h>
#include <stdio.h>



void numer_repitition(char text[], char pattern[], int  tabel[], int i, int len_text, int result []){
    
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
         if(i == len_pattern){
             //printf("\n substring found at index %d  \n", j-i);
             count_pattern++;
             i = tabel[i-1]; 
             //continue;
             //break;
         }
        
         else if(j < len_text && pattern[i] != text[j]){
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
    //printf("\ni:%d, count: %d \n", result[0], result[1]);
    
   
}

