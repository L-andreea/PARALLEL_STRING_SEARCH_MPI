
#include <string.h>
#include <stdio.h>



int numer_repitition(char text[], char pattern[], int  tabel[]){
    printf(" \nin number of repetiontion text: %s \n", text);
    int i = 0; 
    int j=0;
    int len_text = strlen(text);
    int len_pattern = strlen(pattern);
    int count = 0; //var for how loop need 
    int count_pattern = 0;// var for how 
    while((j < len_text  ) && (len_text - j) >= (len_pattern-i) ){  //enter in loop 
        count ++;
        if(pattern[i]== text[j]){
             j++;
             i++;
         }
         if(i == len_pattern){
             printf("\n substring found at index %d  \n", j-i);
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

    return count_pattern;
   
}

