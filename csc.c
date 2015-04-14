#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <string.h> 

#define TEMP_FILE "temp"
#define INDEX_TEMP_FILE "indextemp"
#define INDEX_SORT_TEMP_FILE "indexsorted"
#define INDEX_FINAL_FILE "index"
#define MAXLINE 60

int main(int argc, char*argv[]){
  
  if(argc != 2){  
    printf("usage: argv[0] <index path> \n\n");
    exit(1);    
  }
  
  char index_temp_path[50];
  char temp_path[50];
  
  FILE* index;  
  sprintf(index_temp_path,"%s/%s.txt",argv[1],INDEX_TEMP_FILE);  
  index = fopen(index_temp_path, "a");  
  
  FILE* temp; 
  pid_t pid;
  int temp_file_counter = 1;
  int stat;
  
  //==============================================
  //ciclo de concatenacao de temporarios
  while(1){
    //constroi path para o temp file  //directorio/
    sprintf(temp_path, "%s/%s%d.txt",argv[1],TEMP_FILE,temp_file_counter);
    
    if((temp = fopen(temp_path, "r")) == NULL) //faz cat dos temps até nao existirem
      break; 
    else {
      fclose(temp);    
      temp_file_counter++;
    }
    
    if((pid = fork()) == -1){
      exit(2); //erro no fork
    }
    
    if(pid > 0){ //espera que cats terminem
      wait(&stat);
    }
    
    if(pid == 0){ //filho executa cat	
      dup2(fileno(index), STDOUT_FILENO);
      execl("/bin/cat","/bin/cat",temp_path ,NULL);
      printf("erro a executar cat\n\n");
    }      
  }  
  
  //==============================================
  //sort do index_temp
  
  FILE* index_sort;
  char index_tempx[50];  
  sprintf(index_tempx,"%s/%s.txt",argv[1],INDEX_SORT_TEMP_FILE);  
  
  int status;
    
  if((pid = fork()) == -1){
    exit(2); //erro no fork
  }
  
  if(pid > 0 ){    
    wait(&status);
    //formatacao do ficheiro index
    FILE* index_final;
    //char temp_path[50]//se der coco e por causa disto
    sprintf(temp_path,"%s/%s.txt",argv[1],INDEX_FINAL_FILE);
    index_sort = fopen(index_tempx,"r");
    index_final = fopen(temp_path, "w");  
    
    char line[MAXLINE];
    char word[15]="";
    char aux_word[15] = "";
    char file[20] = "";
    int ret,print_word; 
    print_word = 0;
    
    fputs("INDEX\n",index_final);
    
    while(fgets(line, MAXLINE, index_sort) != NULL){
      
      sscanf(line,"%s : %s\n",word,file);
     // printf("%d",line_num);
      
      if(word[0]=='0'){ //primeira palavra   
	strcpy(aux_word,word);
	print_word = 1;
      }
      
      if((ret = strcmp(aux_word,word)) == 0){ //mesma palavra      
	print_word = 0;
      }
      else print_word = 1;
      
      if(print_word){      
	print_word = 0;
	strcpy(aux_word,word);
	fprintf(index_final,"\n%s: %s",word,file);       
      }
      else {
	fprintf(index_final,", %s",file);
      }      
    }  
    
    fclose(index_final);
    fclose(index_sort);
  }  
  
  else { //filho executa sort	
    index_sort = fopen(index_tempx, "a");    
    dup2(fileno(index_sort), STDOUT_FILENO);
    //-v com numeros no meio do texto
    execl("/usr/bin/sort","/usr/bin/sort","-d","-V",index_temp_path ,NULL);
    printf("erro a executar sort\n\n");
  }
  
  return 0;
}