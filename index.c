#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 

#define WORDS_FILE "words.txt"
#define TEXT_FILE "texto.txt"

//TODO usar funcao realpath

//CHAMA SW CORRECTAMENTE
//por agora apenas chama sw
//uso de pipes para passar nome do ficheiro a pesquisar ao filho ??

int main(int argc, char* argv[]){
  
  //pid_t pid;
  FILE *in;
  char path[20]; //src/words.txt
  
  if(argc != 2){   
    printf("usage : %s <dir>\n\n", argv[0]);    
  }   
  
  sprintf(path, "%s/%s", argv[1], WORDS_FILE);
  
  if((in=fopen(path,"r")) == NULL){
    printf("The words file cannot be open\n");
    exit(1);
  }
  fclose(in);
  
  if(execl("/usr/bin/gcc", "-Wall","-o" , "sw", "./src/sw.c", (char*)NULL) == -1){
    printf("error compiling sw\n");
    exit(1);
  }   
    
  if( execl("src/sw" ,TEXT_FILE , "1",NULL) == -1){
    printf("Erro executing sw\n");
    exit(1);
  }
  
  return 0;
}
