#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


//built in setenv
void cUnsetenv(char** line){
    if(line[1] == NULL){
       printf("Environment variable not provied\n"); 
       return;
    }
    if(line[2] != NULL){
       printf("Too many arguments provided\n"); 
       return;
    }

    int code = unsetenv(line[1]);
    if(code == 0){
    }else{
        printf("Failed to unset Environment variable\n");
    }
}

//built in setenv
void cSetenv(char** line){
    if(line[1] == NULL){
       printf("Environment variable not provied2\n"); 
       return;
    }
    if(line[2] == NULL){
       printf("Environment variable value not provied\n"); 
       return;
    }
     if(line[3] != NULL){
       printf("Too many arguments provided\n"); 
       return;
    }

    int code = setenv(line[1],line[2],1);
    if(code == 0){
    }else{
        printf("Failed to set Environment variable\n");
    }
}

//built in cd
void cdir(char** line){
    char* path;
    if(line[1] == NULL || (strcmp(line[0], "~") ) ==0){
        path = getenv("HOME");
    }else{
        path = line[1];
    }

     if(line[2] != NULL){
       printf("Too many arguments provided\n"); 
       return;
    }

    int code = chdir(path);
    if(code == 1){
        printf("Can not find directory\n");
    }
}

//built in cd
void cFork(char** line){
 pid_t id= fork();
   if(id==0) {//child
        execvp(line[0], line);
        printf("Error starting child prosses\n");
    }else{
        waitpid(id, NULL, 0);
    }
}


int parse(char* line){
    char **tokens = malloc(520 * sizeof(char*));
    char *token;
    token = strtok(line, " \n");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " \n");
    }
    
    tokens[i] = NULL;
    
    if(tokens[0]==NULL){//empty
        return 1;
    }else if(strcmp(tokens[0], "exit")==0){
        return 0;
    }else if(strcmp(tokens[0], "setenv")==0){
        cSetenv(tokens);
    }else if(strcmp(tokens[0], "unsetenv")==0){
        cUnsetenv(tokens);
    }else if(strcmp(tokens[0], "cd")==0){
        cdir(tokens);
    }else{
        cFork(tokens);//fork all others
    }
    free(tokens);
    return 1;
}

void stop(){
return;
}

//handle file
void readfile(){
    char name[500];
    char* path = getenv("HOME");
    strcpy(name, path);
   strcat(name, "/.kapishrc");
   printf("name : %s",name);
    
    FILE* file = fopen(name, "r"); 
    char line[520];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
        if(parse(line)==0){
            exit(0);
        }
    }
    fclose(file); 
}

void loop(){
    while(1){
        printf("? ");
        char line[520];
        fgets(line,520,stdin);
        if(parse(line)==0){
            break;
        }
    }
}

int main(){
 signal(SIGINT, stop);  
 readfile(); 
 loop();
 return 0;
}
