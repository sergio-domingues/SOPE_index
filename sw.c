#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <string.h> 

#define MAXLINE 60
#define MAXCMD 25
#define WORDS "words.txt"
#define WRITE 1
#define READ 0


//TODO usar funcao realpath
//TODO ESTÁ A FUNCIONAR CORRECTAMENTE MAS -> VERIFICAR CENA DOS PATHS / MANDAR MACRO WORDS COM O CARAÇAS

//argv[1] -> path 
//argv[2] -> ficheiro a percorrer / temp a criar
int main(int argc, char* argv[]) {
  
  if(argc != 3){    
    printf("usage : %s <path> <txtfile>\n",argv[0]);
    exit(1);
  }
  
  FILE *txt;  
  char words_path[40];
  sprintf(words_path,"%s/%s",argv[1],WORDS);
  
  if((txt = fopen(words_path, "r")) == NULL){ //palavras a pesquisar
    printf("erro na abertura de words\n");
    exit(1);
  }
  
  FILE *tmp;
  char tp[20];
  sprintf(tp,"%s/%s.txt",argv[1],argv[2]);
  if((tmp = fopen(tp, "r")) == NULL){ //temp a percorrer
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
  sprintf(cmd, "%s/temp%s.txt",argv[1], argv[2]);
  temp = fopen(cmd,"a"); 
  char *pos;
  
  //pesquisa todas as palavras
  while(fgets(word, MAXLINE, txt) != NULL){
    
    if((pos=strchr(word,'\n'))!=NULL){ //remove \n se existir
      *pos='\0';
    }
    
    if(pipe(fd) < 0){  //cria pipe
      perror("Pipe error\n");
    } 
    pid = fork();
    if(pid == -1){
      printf("Fork error\n");
      exit(1);
    }
    
    if(pid > 0){//pai   
      close(fd[WRITE]); 
      wait(&status); //espera pelo filho      
      
      char c[1];/*, line[1];*/
      int line_number = 1; //flag
      char line_index[15]; //nº da linha nao pode ter mais do que 14 digitos
      
      //lê pipe
      while( (read(fd[READ],c, sizeof(c)) != 0) ){	
	
	if(c[0] == ':'){
	  line_index[line_number-1] = 0;
	  line_number = 0;
	}
	
	if(line_number){
	  line_index[line_number-1] = c[0]; 
	  line_number++;
	}
	
	if(c[0] == '\n'){ //escreve palavra pesquisada 1 unica vez
	  //char* file = strtok(argv[1],"."); //file.txt => file
	  fprintf(temp,"%s : %s-%s\n", word,argv[2],line_index);
	  line_number = 1;
	}
      } 
      close(fd[READ]);
    }
    
    else {//filho   
      close(fd[READ]); 
      dup2(fd[WRITE], STDOUT_FILENO); //redirecciona output
      
      //-won: nao ha falsos positivos/display linha
      execl("/bin/grep","/bin/grep", "-w","-o","-n",word,tp,NULL); //grep -won 1.txt texto.txt (ex)           
    }
    
  }
  
  fclose(temp);
  fclose(txt);
  return 0;
}
