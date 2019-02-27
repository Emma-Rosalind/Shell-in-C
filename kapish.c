#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//handle file
void readfile(){
    char name[256]; 
    printf("HOME : %s\n", getenv("HOME"));
    strcat(strcpy(name, getenv("HOME")), ".kapishrc");
    printf("a");
    printf("%s",name);
    FILE* file = fopen(name, "r"); 
    char line[520];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); 
    }
    fclose(file);   
}
//built in setenv
void cUnsetenv(char** line){
    if(line[1] == NULL){
       printf("Environment variable not provied"); 
       return;
    }
    if(line[2] != NULL){
       printf("Too many arguments provided"); 
       return;
    }

    int code = unsetenv(line[1]);
    if(code == 0){
        printf("success");
    }else{
        printf("Failed to unset Environment variable");
    }
}

//built in setenv
void cSetenv(char** line){
    if(line[1] == NULL){
       printf("Environment variable not provied"); 
       return;
    }
    if(line[2] == NULL){
       printf("Environment variable value not provied"); 
       return;
    }
     if(line[3] != NULL){
       printf("Too many arguments provided"); 
       return;
    }

    int code = setenv(line[1],line[2],1);
    if(code == 0){
        printf("success");
    }else{
        printf("Failed to set Environment variable");
    }
}

//built in cd
void cdir(char** line){
    char* path;
    if(line[1] == NULL || strcmp(line[0], "~")){
        printf("move to home directory");
        path = getenv("HOME");
    }else{
        path = line[1];
    }

     if(line[2] != NULL){
       printf("Too many arguments provided"); 
       return;
    }

    int code = chdir(path);
    if(code == 1){
        printf("Can not find directory");
    }
}

//built in cd
void cFork(char** line){
   pid_t id= fork();
   if(id==0) {//child
        execvp(line[0], line);
        printf("Error starting child prosses\n");
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
    }else if(strcmp(tokens[0], "exit")){
        return 0;
    }else if(strcmp(tokens[0], "setenv")){
        cSetenv(tokens);
    }else if(strcmp(tokens[0], "unsetenv")){
        cUnsetenv(tokens);
    }else if(strcmp(tokens[0], "cd")){
        cdir(tokens);
    }else{
        cFork(tokens);//fork prosses
    }
    free(tokens);
    return 1;
}

void loop(){
    while(1){
        printf("? ");
        char line[520];
        scanf( "%s" , line);
        if(parse(line)==0){
            break;
        }
    }
}

int main(){
 loop();
 return 0;
}