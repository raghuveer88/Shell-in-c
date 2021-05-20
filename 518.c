#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/wait.h>
#include<limits.h>

int pids[100],count3=0;
char *cmds[100];

void sintHandler(int sig) {                           
    
    signal(SIGINT, sintHandler); 

    printf("STOP QUITS SHELL\n");         // deacticate ctrl c
    fflush(stdout); 
} 

void sighandler(int sig){               //for segchild 
    pid_t pid;
    int st;

    
    while((pid = waitpid(-1, &st, WNOHANG)) > 0){            //prints msg when child process exits
        printf("A background process has exited\n"); 
       
    }
  
  }      






void cd_display(char* dir_names){   // this function is for geting the path and printing it
   char hostname[HOST_NAME_MAX];                 
   char username[LOGIN_NAME_MAX];
   gethostname(hostname, HOST_NAME_MAX);
   
   getlogin_r(username, LOGIN_NAME_MAX);
   printf("<%s@%s:~",username,hostname);  
   if(strcmp(dir_names,"")==0){ 
    printf(">");
   }
   else{
   printf("%s",dir_names);
   printf(">");
   }

}

char* str(char line[], int number){ // when hist is enter this funtion will give the required outputs
    char* tok;
    int num_of_words = 0;
    char* args[11];
    tok = strtok (line,"t");
        while (tok != NULL)
        {
            args[num_of_words] = tok;
            tok = strtok (NULL, "t");
            num_of_words = num_of_words + 1;
        }
    return args[number];
}


int execute(char* line){  //this function is for exectuting the commands
	
	int count=0;
	int length;
	char* myargs[1000];
	bool background=false;
	char* tok = strtok(line," ");   //dividing the command based on tokenizer method
         while (tok != NULL) {
		myargs[count]=tok;
		tok = strtok(NULL," ");
		length = strlen(myargs[count]);
               if(myargs[count][length-1] == '\n')    // to remove \n charactor at the end of the string
               myargs[count][length-1] = 0;
               printf("%s\n",myargs[count]);
		count++;
	}
	myargs[count] = NULL;
	if(strcmp(myargs[count-1],"&")==0){        // this is to check the command if its a backround command
		myargs[count-1]=NULL;
		background=true;
		count--;
	}else{
		myargs[count]=NULL;
	}
	int pid;                    // is the command is for background process or forground process the corresponding action will happen 
	if ((pid = fork()) < 0) {
		  printf("ERROR: Child process wasn't made! \n");
		  exit(1);
	     }
	     else if (pid == 0) {
		  if (execvp(myargs[0],myargs) < 0) {
		       printf(" ERROR: the command didn't work \n");
		     exit(1);
		  }
		  return 0;
	}else if(!background){/*if it is background process it wont wait*/
		wait(NULL);
	}
	return pid;
}





int main(){

   struct sigaction sa;              //for handling sigchld , when child process exits
  sigemptyset(&sa.sa_mask);

  sa.sa_flags = 0;
  sa.sa_handler = sighandler;
  sigaction(SIGCHLD, &sa, NULL);
  signal(SIGINT,sintHandler);


   char hostname[HOST_NAME_MAX];            // this is to get the username and product name
   char username[LOGIN_NAME_MAX];
   gethostname(hostname, HOST_NAME_MAX);
   getlogin_r(username, LOGIN_NAME_MAX);
   printf("<%s@%s:~>",username,hostname);
   char *command = (char*)malloc(300*sizeof(char));   // created three duplicates for stroing the commands
    char *cy_command = (char*)malloc(300*sizeof(char));
     char *dy_command = (char*)malloc(300*sizeof(char));
   char* base_dir_names = (char*)malloc(300*sizeof(char));
   char* dir_names = calloc(300,sizeof(char));
   int i;
   getcwd(base_dir_names,100);
   i = strlen(base_dir_names);
   
   while(1){
        scanf("%[^\n]%*c",command);        // taking the command from the user
        strcpy(cy_command,command);
        strcpy(dy_command,command);
       //  printf("%s\n",cy_command);
        char* arg[1000]; 
       
        
        int length;              
        int j = 0;
   
       char *token = strtok(command," ");  // breaking the string when ever space is encontered and storing it in arg
       
       while(token != NULL){     //seperating all the strings in a command based on spaces to use in execvp
        arg[j] = token;
        token = strtok(NULL," ");  
        length = strlen(arg[j]);
        if(arg[j][length-1] == '\n')    // to remove \n charactor at the end of the string
        arg[j][length-1] = 0;
        j++;
       } 
       arg[j] = NULL; 
       
        if(strcmp(arg[0],"cd") == 0){   // this is specificly for cd command
         if(strcmp(arg[1],"..") != 0){
            if(chdir(arg[1]) == 0){
                 
          	getcwd(dir_names,100);
          	 
          	dir_names = dir_names + i;
       	}
   	       else{
   	   	  printf("directory doesn't exist\n");
   	   	  
       	}
               }
             	else if(strcmp(arg[1],"..") == 0){   // this is for if the user wants to go back a step from the path
    		chdir(arg[1]);
    		getcwd(dir_names,100); 
    		dir_names = dir_names + i;   
      	    }
      	    cmds[count3]=(char*)malloc(512*sizeof(char));
      	    cmds[count3] = "cd";
      	    count3 = count3 +1;
          }
          
        else if(strcmp(command,"STOP")==0){   // to stop the shell
	  printf("shell closing\n");
	  exit(1);   
	  }
	
	 else if(strcmp(arg[0],"pid")==0 && strcmp(arg[1],"all")==0){ // to get the pids of all the commands executed till now
	  for(int i=0;i<count3;i++){
	     printf("command name: %s process id: %d\n",cmds[i],pids[i]);
            }
         }     
	  
	  
	else if(strcmp(command,"pid")==0){    // to get the process id of the running program
      	  printf("command name: ./a.out process id: %d\n",getpid());
         }  
         
        else if(strcmp(command,"hist")==0){   // all the commands exectued till now history
	   for(int i=0;i<count3;i++){
	     printf("%d. %s\n",i+1,cmds[i]);
	    }
         }
        
        
        else if(strcmp(str(cy_command,0),"his")==0){ // to print the last n commands
	char *num=str(cy_command,1);
	int hist_num=atoi(num);
	if(hist_num > count3){
	  hist_num = count3;
	}
	int i=0;
	while(i<hist_num){
             printf("%d. %s\n",i+1,cmds[count3-(hist_num-i)]);
	     i++;
	  }
	}
	
	else if(strcmp(str(cy_command,0),"!his")==0){//to print nth command we executed
	   char *num=str(cy_command,1);
           int hist_num=atoi(num);
           if(hist_num > count3){
	      printf("number is too high\n");
	   }
	   else{
	   printf("%s\n",cmds[hist_num-1]);
	   //execute(cmds[hist_num-1]);
	   }
	}
	 
   	
   	else{                           // to execute orginal commands
	  cmds[count3]=(char*)malloc(512*sizeof(char));
	  strcpy(cmds[count3],command);
	  // printf("%s\n",dy_command);
	  pids[count3]=execute(dy_command);
	  count3++;
	}
           cd_display(dir_names);     // call the function to print the path
   
   
   }
   


return 0;
}
