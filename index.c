#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 

#define WORDS_FILE "words.txt"
#define TEXT_FILE "texto.txt"

//CHAMA SW CORRECTAMENTE
//por agora apenas chama sw
//uso de pipes para passar nome do ficheiro a pesquisar ao filho ??

int main(int argc, char* argv[]){
  
  //pid_t pid;
    
  if(argc != 2){   
    printf("usage : %s <dir>\n\n", argv[0]);    
  }
  
  FILE *in;
  
  char path[20]; //src/words.txt
  sprintf(path, "%s/%s", argv[1], WORDS_FILE);
  
  if((in=fopen(path,"r")) == NULL){
    printf("The words file cannot be open\n");
    exit(1);
  }
  fclose(in);
  
  char cmd[25];  
  sprintf(cmd , "/usr/bin/gcc -Wall -o sw src/sw.c");
  if(!(in = popen(cmd, "r"))){
    exit(1);
  } 
  pclose(in);
  
  sprintf(cmd, "src/sw %s", TEXT_FILE);

  if(!(in = popen(cmd, "r"))){
    exit(1);
  } 
  
  pclose(in);  
  return 0;
}