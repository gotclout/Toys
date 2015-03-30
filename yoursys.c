/* HMWK2.c

Name: Randeep Sidhu
Date: 3/26/2015
Class: ISA563

*/

#include <unistd.h>
//#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/param.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef system
#undef system
#endif

int tokenize(const char *comm, char* args[], char* &outfile, char* &infile)  //used to get all
{
	
	int argc=1;
	
	char* token;
	//get initial token; while there is a token stll left; get the next token
	for(token = strtok((char*)comm," ");token !=0;token=strtok(0," ") )
	{
		//copy a token into the first available argument
		args[argc]=(char*)malloc(strlen(token) + 1);  //allocating memory for the token
		memset(args[argc], 0, strlen(token) + 1);
		strcpy(args[argc],token);
		
		if(strcmp(args[i],">")==0)
		{
			//redirect output = true
			token=strtok(0," ");
			outfile=(char*)malloc(strlen(token));
			memset(args[outfile], 0, strlen(token) + 1);
			strcpy(args[outfile],token);
		}
		else if(strcmp(args[i],"<")==0)
		{
			// redirect inpuut = true
			token=strtok(0," ");
			infile=(char*)malloc(strlen(token));
			memset(args[infile], 0, strlen(token) + 1);
			strcpy(args[infile],token);
		}
		argc++;
	}
	
	return(argc);
	
	
}



int system(const char *comm)
{
#ifndef ARG_MAX //max length of arguements
	size_t _ARG_MAX = sysconf(_SC_ARG_MAX);  
	#define ARG_MAX _ARG_MAX
#endif
	
	printf("Randys System\n");
	char* args[ARG_MAX];
	memset(args, 0, sizeof(char*) * ARG_MAX);	
	
	int rval=-1;
	
	//Step 1: Parse commandline
	const char* arg0="-c";

	args[0]=(char*)malloc(strlen(arg0));
	strcpy(args[0], arg0);
	char* infile=0,*outfile=0;
	int argc=tokenize(comm,args,outfile,infile); //catching the return value of tokenize function
	
	bool waitfor=1; //waiting for a child process or not
	bool redirectin = 0;
	bool redirectout = 0;
	int i;
	
	int flag=argc;
	//iterating over args from 0 to argc
	for(i=0;i<argc;i++)
		{
			//check for operators in the arguments list(<,>,&)
			if(strcmp(args[i],">")==0)
			{
				//redirect output = true
				redirectout = 1;
			}
			else if(strcmp(args[i],"<")==0)
			{
				// redirect inpuut = true
				redirectin = true;
			}
			else if(strcmp(args[i],"&")==0)
			{
				waitfor=0;
			}
			if(flag==argc && redirectout || redirectin || !waitfor)
				flag=i;
		} 
	//Step 2: Execute the parsed command (Retrieve the result)
		
		pid_t child;
		int cstatus; //exit status of child
		pid_t c; //pid of child to be returned by the wait
		
		int j = 0; 
		for(; j < argc; ++j)
			printf("Arg %d %s\n", j, args[j]);
		args[flag]=0;
		if((child= fork())==0)
		{
			if(redirectout==1)
			{
				close(1);
				open(outfile, O_RDONLY|O_WRONLY|O_CREAT, 00666);
				
			}
			if(redirectin==1)
			{
				close(0);
				open(infile, O_RDONLY|O_NONBLOCK|, 00222);
			}
			execv("/bin/sh",args);
			if(redirectin)
				close (0);
			if(redirectout)
				close (1);
			//check for errors
		}
		else
		{
			if(child ==(pid_t)(-1))
			{
				
			}
		else if (waitfor==1) 
			{
				c=wait(&cstatus); //waiting for child to complete
			}
						
		}
	//Step 3: Check for errors
	//Step 4: Return our value
	
	
	return(rval);
	

	
}

int main(int argc, char *argv[])
{
	
	const char* comm = "ls";
	
	system(comm);
	
	return (0);
	
}
