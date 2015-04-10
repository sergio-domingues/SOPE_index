#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <string.h> 

#define MAXLINE 60
#define MAXCMD 25
#define WORDS "src/words.txt"
#define WRITE 1
#define READ 0

/*
 * execl/dup2 tá a merdar
 */

//argv[1] -> text file a percorrer
//argv[2] -> numero do ficheiro a criar ; Pex 1 -> cria ficheiro temp1.txt
int main(int argc, char* argv[]) {
  
  if(argc != 3){    
    printf("usage : %s <txtfile> <temp file number>\n",argv[0]);
    exit(1);
  }
  
  FILE *txt;  
  if((txt = fopen(WORDS, "r")) == NULL){ //palavras a pesquisar
    printf("erro na abertura de words\n");
    exit(1);
  }
  
  FILE *tmp;
  char tp[20];
  sprintf(tp,"src/%s",argv[1]);
   if((tmp = fopen(tp, "r")) == NULL){ //palavras a pesquisar
    printf("erro na abertura de texto\n");
    exit(1);
  }  
  
  //vars
  FILE *temp;  
  char cmd[MAXCMD];
  char word[MAXLINE];
  pid_t pid;
  int fd[2]; //descritores escrita/leitura
  int status;
  //===========
  
  if(pipe(fd) < 0){  //cria pipe
    perror("Pipe error\n");
  } 
  
  sprintf(cmd, "src/temp%s.txt", argv[2]);
  temp = fopen(cmd,"a");       
  
   //dup2(fd[WRITE], STDOUT_FILENO); //redirecciona output
  puts("lol");
  //pesquisa todas as palavras
  while(fgets(word, MAXLINE, txt) != NULL){
    word[strlen(word)-1] = 0;
    
    pid = fork();
    if(pid == -1){
      printf("Fork error\n");
      exit(1);
    }
    
    if(pid > 0){//pai    
      
      close(fd[WRITE]);      
      wait(&status); //espera pelo filho
      
      char c[1];
      int found = 0; 
      int ignore_til_endofline = 0;
      //lê pipe
      while( (read(fd[READ],c, sizeof(c)) != 0) && c[1]!=0){	
	
	if(found == 0){ //escreve palavra pesquisada 1 unica vez
	  found = 1;
	  char* file = strtok(argv[1],"."); //file.txt => file   
	  
	  fprintf(temp,"%s : %s-", word,file);		
	}	
	
	if(c[1] == '\n'){
	  ignore_til_endofline = 0;
	}
	else if(c[1] == ':'){	  
	  ignore_til_endofline = 1;
	}
	else if(ignore_til_endofline != 1)
	  fprintf(temp,"%s",c);	
      }
      if(found == 1)
	fprintf(temp,"\n");	
    }
    
    else {//filho          
      close(fd[READ]);      
      //printf("x");      
      char file[30];
      sprintf( file, "src/%s",argv[1]);      
      //-xn: nao ha falsos positivos/display linha
       execl("/bin/grep","/bin/grep", "-x" ,"-n",word,file,NULL); //grep -xn 1.txt texto.txt (ex)     
      exit(1);
    }
  }
  
  fclose(temp);
  fclose(txt);
  return 0;
}
