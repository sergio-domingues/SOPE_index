#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <string.h> 

#define MAXLINE 60
#define MAXCMD 25
#define WORDS "src/words.txt"

//argv[1] -> text file a percorrer
int main(int argc, char* argv[]) {
  
  if(argc != 2){    
    printf("usage : %s <txtfile> \n",argv[0]);
    exit(1);
  }
    
  FILE *in, *txt;
  char cmd[MAXCMD];
  char word[MAXLINE];
  char grep_line[MAXLINE];
  char aux[MAXLINE];
  
  if((txt = fopen(WORDS, "r")) == NULL){ //palavras a pesquisar
    printf("erro na abertura de words\n");
    exit(1);
  }
  
#define num_file 1 //a receber através do pipe
  
  sprintf(cmd, "src/temp%d.txt", num_file);
  int fd = fileno(fopen(cmd,"a"));       
  dup2(fd, STDOUT_FILENO);
  
  while(fgets(word, MAXLINE, txt) != NULL){
    
    word[strlen(word)-1] = 0; //subtitui /n por 0, por causa do grep
    
    //-xn: nao ha falsos positivos/display linha
    sprintf(cmd, "grep -xn %s src/%s",word, argv[1]);
    
    if(!(in = popen(cmd, "r"))){
      printf("erro no popen\n");
      exit(1);
    } 
    
    while(fgets(grep_line, MAXLINE, in) != NULL){ 
      
      char* n_line = strtok(grep_line, ":"); //1º token=num linha
      
      char x[] = WORDS;
      char* file = strtok(x,".");     
         
      //formato especifcado: palavra : ficheiro-linha
      sprintf(aux, "%s : %s-%s", word, file, n_line);
      printf("%s\n", aux);
    }
    
    pclose(in);
  }
  
  fclose(txt);
  return 0;
}
