#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <string.h> 

#define TEMP_FILE "temp"
#define WORDS_FILE "words.txt"
#define INDEX_TEMP_FILE "indextemp"
#define INDEX_SORT_TEMP_FILE "indexsorted"

int main(int argc, char* argv[]){
  
  if(argc != 2){   
    printf("usage : %s <dir>\n\n", argv[0]);    
  }   
  
  FILE *in;
  char path[20]; //src/words.txt  
  sprintf(path, "%s/%s", argv[1], WORDS_FILE);
  
  if((in=fopen(path,"r")) == NULL){
    printf("ERROR OPENING WORDS FILE\n\n");
    exit(1);
  }
  
  int status,file_counter;
  
  
  file_counter=1; //contador
  //sw.c
  
  while(1){
    
    sprintf(path,"%s/%d.txt",argv[1],file_counter);
    if((fopen(path,"r")) == NULL){
      break;
    }   
    
    pid_t pid;
    if((pid = fork()) < 0){
      printf("ERROR FORKING IN INDEX.c\n"); 
    }
    
    if(pid > 0){
      wait(&status);
      file_counter++;
    }
    
    if(pid == 0){  //filho executa sw   
      
      char counter[10];
      sprintf(counter,"%d",file_counter);
      
      execlp("./sw","sw",argv[1],counter,(char*)NULL); 
    }    
  }
  
  //csc.c  
  int stat;
  pid_t pid;  
  
  if((pid = fork()) < 0){
    printf("ERROR FORKING 2 IN INDEX.c\n"); 
  }
  
  if(pid > 0){ //pai eleimina ficheiros temporarios criados
    wait(&stat);
    
    sprintf(path,"%s/%s.txt",argv[1],INDEX_TEMP_FILE);
    
    remove(path);
    
    sprintf(path,"%s/%s.txt",argv[1],INDEX_SORT_TEMP_FILE);
    remove(path);
    
    int i = 1;
    for(i = 1; i <= file_counter; i++){ //elimina temps (temp1/temp2..tempn)
      sprintf(path,"%s/%s%d.txt",argv[1],TEMP_FILE,i);
      remove(path);
    }  
  }
  
  if(pid == 0){  //filho executa sw   
    
    execlp("./csc","csc",argv[1], (char*) NULL);
  } 
  
  return 0;
}
