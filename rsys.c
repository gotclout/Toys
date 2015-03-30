/* HMWK2.c

Name: Randeep Sidhu
Date: 3/26/2015
Class: ISA563

*/

#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/param.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef system
#undef system
#endif

#define MAX_LEN 256

//used to get all

/**
 * Tokenizes the command to be executed by the system function
 */
int tokenize(const char *comm, char** args, char** outfile, char** infile)
{

  int argc=2;
  int p = 0;
  size_t len = strlen(comm);
  for( ; p < len && comm[p] != ' '; ++p){;}

  if(p == len)
  {
    args[argc]=(char*)malloc(strlen(comm) + 1);  //allocating memory for the token
    memset(args[argc], 0, strlen(comm) + 1);
    strcpy(args[argc], comm);
    argc++;
  }
  else
  {
  char* token;
  //get initial token; while there is a token stll left; get the next token
  for(token = strtok((char*)comm," ");token !=0 && argc < MAX_LEN; token=strtok(0," ") )
  {
    //copy a token into the first available argument
    args[argc]=(char*)malloc(strlen(token) + 1);  //allocating memory for the token
    memset(args[argc], 0, strlen(token) + 1);
    strcpy(args[argc],token);

    if(strcmp(args[argc],">")==0)
    {
      //redirect output = true
      token=strtok(0," ");
      *outfile=(char*)malloc(strlen(token));
      memset(*outfile, 0, strlen(token) + 1);
      strcpy(*outfile, token);
    }
    else if(strcmp(args[argc],"<")==0)
    {
      // redirect inpuut = true
      token=strtok(0," ");
      *infile=(char*)malloc(strlen(token));
      memset(*infile, 0, strlen(token) + 1);
      strcpy(*infile, token);
    }
    argc++;
  }
  }
  return(argc);


}

/**
 * Implements the system function using fork and exec
 *
 * @param const char* comm, is the command line string to be executed
 * @return int ???? what values and why?
 */
int system(const char *comm)
{

  printf("Randys System\n");

  //the max num args we define earlier
  //this is different from ARG_MAX which is no longer present because
  //we assume the test code will not overflow your allocated array but
  //we will check in tokenize just to be sure
  char* args[MAX_LEN];
  memset(args, 0, sizeof(char*) * MAX_LEN);

  int rval=-1;

  //Step 1: Parse commandline
  const char* cmd = "sh";
  args[0]=(char*)malloc(strlen(cmd));
  strcpy(args[0], cmd);

  const char* arg0="-c";

  args[1]=(char*)malloc(strlen(arg0));
  strcpy(args[1], arg0);
  char* infile=0,*outfile=0;
  int p = 0;
  size_t len = strlen(comm);
  for( ; p < len && comm[p] != ' '; ++p){;}

  //catching the return value of tokenize function
  int argc=tokenize(comm,args,&outfile,&infile);

  bool waitfor=1; //waiting for a child process or not
  bool redirectin = 0;
  bool redirectout = 0;
  int i;
  int flag = argc +1;

  //iterating over args from 0 to argc
  for(i=0;i<argc;i++)
  {
    //check for operators in the arguments list(<,>,&)
    if(strcmp(args[i],">")==0)
    {
      redirectout = 1;
    }
    else if(strcmp(args[i],"<")==0)
    {
      redirectin = true;
    }
    else if(strcmp(args[i],"&")==0)
    {
      waitfor=0;
    }
    if(flag==argc && redirectout || redirectin || !waitfor) flag=i;
  }

  //Step 2: Execute the parsed command (Retrieve the result)
  pid_t child;
  int cstatus; //exit status of child
  pid_t c; //pid of child to be returned by the wait

  int j = 0;
  args[flag]=0;

  if((child= fork())==0)
  {
    if(redirectout==1)
    {
      close(1);
      int ofid = open(outfile, O_RDONLY|O_WRONLY|O_CREAT, 00666);

    }
    if(redirectin==1)
    {
      close(0);
      int ifid = open(infile, O_RDONLY|O_NONBLOCK, 00222);
    }
    execv("/bin/sh", args);
    if(redirectin)
      close (0);
    if(redirectout)
      close (1);
    //check for errors
    perror(*args);
    //exit child
    exit(0);
  }
  else
  {
    if(child ==(pid_t)(-1))
    {
      fprintf(stderr, "ERROR: could not fork subprocess\n");
    }
    else if (waitfor==1)
    {
      //is this the wait operation your really want to call?

      c = wait(&cstatus); //waiting for child to complete

      //maybe you should check the man pages to make sure
      //perhaps this would be better?
      //waitpid(child, 0, 0);
      //man wait?
      //man waitpid

    }
  }

  //Step 3: Check for errors
  //Step 4: Return our value


  //I don't see rval being modified within this function
  //where do you think that might need to occur
  return(rval);



}

/**
 * Main entry point into the system.c
 * Used exclusively for testing
 * TODO: Comment out main before submission
 */
int main(int argc, char *argv[])
{

  const char* ls       = "ls",
             *lsl      = "ls -la",
             *lsln     = "ls -la &",
             *lslnro   = "ls -la > ro.out",
             *lslnron  = "ls -la > ro.out &",
             *infloop  = "loop",
             *aifloop  = "loop &",
             *rifloop  = "loop > loop.out";

  //char *Env_argv[] = { (char *)"/bin/ls", (char *)"-l", (char *)"-a", (char *) 0 };

  system(ls);
  system(lsl);
  system(lsln);
  system(lslnro);
  system(lslnron);

  int c = 0;

  char cmd[64];
  memset(cmd, 0, 64);

  while(1)
  {
    memset(cmd, 0, 64);
    sprintf(cmd, "echo c: %i\n", c);
    system(cmd);
    c++;
  }

  return (0);

}

/**
 *
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <string.h>
 *
 * int main(int argc, char* argv)
 * {
 *   int c = 0;
 *
 *     while(1)printf("c: %i\n", c);
 *
 *       return 0;
 *       }
 *       */
