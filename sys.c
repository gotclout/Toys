#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/resource.h>

#define MAX_LEN 256

#ifndef bool
  typedef char bool;
  #define true 1
  #define false 0;
#endif

#include "sys.h"
#include <stdlib.h>

/**
 *
 */
void CmdData_SetOutF(CmdData** pCmd, char* of)
{
  CmdData* cmd = *pCmd;
  if(cmd->outf) free(cmd->outf);
  cmd->outf = (char*)malloc(strlen(of) + 1);
  memset(cmd->outf, 0, strlen(of) + 1);
  strncpy(cmd->outf, of, strlen(of));
}

/**
 *
 */
void CmdData_SetInF(CmdData** pCmd, char* ifi)
{
  CmdData* cmd = *pCmd;
  if(cmd->inf) free(cmd->inf);
  cmd->inf = (char*)malloc(strlen(ifi) + 1);
  memset(cmd->inf, 0, strlen(ifi) + 1);
  strncpy(cmd->inf, ifi, strlen(ifi));
}

/**
 *
 */
char* CmdData_at(CmdData* cmd, int i)
{
  return *cmd->args && i < cmd->argc ? cmd->args[i] : 0;
}

/**
 *
 */
char CmdData_argat(CmdData* cmd, int i, int j)
{
  char* arg = CmdData_at(cmd, i);
  return arg ? *(arg + j) : '\0';
}

/**
 *
 */
void CmdData_Free(CmdData** pCmd)
{
  CmdData* cmd = *pCmd;
  int i        = 0;

  if(cmd->pth) { free(cmd->pth); cmd->pth = 0; }
  for( ; i < cmd->argc; ++i) if(CmdData_at(cmd, i)) free(CmdData_at(cmd, i));
}

/**
 *
 */
void CmdData_Init(CmdData** pCmd)
{
  CmdData* cmd = *pCmd;

  if(cmd->pth) { CmdData_Free(&cmd); }
  memset((cmd->pth = (char*)malloc(cmd->plen)), 0, cmd->plen);
  memset(cmd->args, 0, sizeof(char*)*MAX_LEN);
  cmd->inf  = cmd->outf = 0;
  cmd->amp  = false;
  cmd->argc = 0;
}

/**
 *
 */
CmdData* CmdData_Create()
{
  CmdData* cmd = (CmdData*) malloc(sizeof(CmdData));
  cmd->plen = 2*(MAX_LEN)*sizeof(char);
  cmd->pth  = 0;
  CmdData_Init(&cmd);

  return cmd;
};

/**
 * Execute forks a process to execute the specified command
 *
 * @param char** args, the command and arguments
 * @param char* path, the full path to the executable
 * @param char** inn, is the redirection input param
 * @param char** outt, is the redirection output param
 * @param bool amp, indicates whether or not the main proc should wait
 *  for the child proc or return immediately
 */
bool execute(CmdData* cmd)
{
  int ifid, ofid;
  pid_t pid = fork(); //create child
  if(pid < 0) { fprintf(stderr, "ERROR: could not fork subprocess\n"); return false; }
  else if(pid == 0)             //child process
  {
    if(cmd->inf && !close(0))
      ifid = open((const char*)cmd->inf,  O_RDONLY|O_NONBLOCK,       00222);
    if(cmd->outf && !close(1))
      ofid = open((const char*)cmd->outf, O_WRONLY|O_RDONLY|O_CREAT, 00666);
    execv(cmd->pth, cmd->args);   //execute command
    if(cmd->inf) close(0);     //end redirection
    if(cmd->outf) close(1);
    perror(*cmd->args);
    exit(0);
  }
  else if(!cmd->amp) waitpid(pid, 0, 0);
  else usleep(50000);
  return true;
}

/**
 * canexec determines whether or not the specified file is executable
 *
 * @param const char* pPath is the file to be executed
 * @param bool errWarn indicates whether or not errors should be issued
 * @return int8_t 1 if ok, 0 if not executable -1 if DNE
 */
int8_t canexec(const char* pPath, char err[])
{
  int8_t rv = -1;
  if(!access(pPath, F_OK) && !++rv)         //exists
  {
    if(!access(pPath, X_OK)) rv++; //executable
    else strcat(strncpy(err, pPath, strlen(pPath)), " is not executable\n");
  }
  else strcat(strcpy(err, 1 + strrchr(pPath, '/')), " does not exist\n");
  return rv;
}

/**
 * Locates the specified command and verifies that it is executable
 *
 * @param char** args is the string array of command arguments and the
 *  first entry is the command to be executed
 * @return char* is a path to the command to be executed (may be null)
 */
bool get_cmd_pth(CmdData* cmd)
{
  char err[MAX_LEN];
  int errno     = -1, i = 1;
  char* pwdenv  = getenv("PWD"), *pthenv = 0;
  size_t pwdln  = strlen(pwdenv), pthenvln, cpyln;
  char argat = CmdData_argat(cmd, 0, 0);
  if(argat != '.' && argat != '/' && (pthenv = getenv("PATH"))) //cmd
  {
    char pth[(pthenvln = strlen(pthenv))+1];
    strncpy((char*)memset(pth, 0, pthenvln + 1), pthenv, pthenvln);
    char* p = strtok(pth, ":");
    for( ; p && errno == -1; p = strtok(0, ":"))
    {
      memset(cmd->pth, 0, cmd->plen);
      if(*p == '.')strcat(strcat(strncpy(cmd->pth, pwdenv, pwdln), "/"), CmdData_at(cmd, 0));
      else strcat(strcat(strncpy(cmd->pth, p, strlen(p)), "/"), CmdData_at(cmd, 0));
      errno = canexec(cmd->pth, (char*)memset(err, 0, MAX_LEN));
    }
  }
  else // ./cmd else ../ else err
  {
    if(CmdData_argat(cmd, 0, 0) == '/'){ strcat(cmd->pth, CmdData_at(cmd, 0)); i = 0;}
    else if(CmdData_argat(cmd, 0, 0) == '.' && CmdData_argat(cmd, 0, i) == '/') cpyln = pwdln;
    else if(CmdData_argat(cmd, 0,i++) == '.')cpyln = pwdln-strlen(strrchr(pwdenv, '/'));
    else {fprintf(stderr, "ERROR: Invalid command input\n"); return 0;}
    if(i)strcat(strncpy(cmd->pth, pwdenv, cpyln), CmdData_at(cmd, 0)+i);
    errno = canexec(cmd->pth, (char*)memset(err, 0, MAX_LEN));
  }
  return errno == 1 || !(fprintf(stderr, "ERROR: %s",  err));
}

/**
 * Counts and parses the command line into args
 *
 * @param char** args, the string array of parsed arguments
 * @param int num_args, the num arguments in args
 * @return bool true if success, false otherwise
 */
bool get_args(CmdData* d)
{
#ifndef ARG_MAX //maximum length of arguments for a new process
  size_t _ARG_MAX = sysconf(_SC_ARG_MAX);
  #define ARG_MAX _ARG_MAX
#endif

  printf("\n>> "); //prompt
  char* buf = 0;
  size_t ap, len,  pos = getline(&buf, &ARG_MAX, stdin), cpos = pos;//read

  d->strcmd = (char*) malloc(pos);
  memset(d->strcmd, 0, pos);
  memcpy(d->strcmd, buf, pos);
  if(!(d->argc = 0) && pos > 0)
  {
    while(pos-- && !(ap = 0))
    {
      while(buf && !isspace(*buf++)){ --pos; ++ap; }
      d->args[d->argc] = (char*)malloc((len = 1 + sizeof(char) * ap++));
      strncpy((char*)memset(d->args[d->argc++], 0, len), buf - ap, ap - 1);
    }
    free((buf - cpos));
  }
  return d->argc > 0;
}

#ifdef system
  #undef system
#endif

// sh -c "cat br.txt"
// system() executes a command specified in command by calling
// /bin/sh -c command,
int system(const char* command)
{
  CmdData* cmd = CmdData_Create();
  get_args(cmd);
  int ifid, ofid;
  pid_t pid = fork(); //create child
  if(pid < 0) { fprintf(stderr, "ERROR: could not fork subprocess\n"); return false; }
  else if(pid == 0)             //child process
  {
    if(cmd->inf && !close(0))
      ifid = open((const char*)cmd->inf,  O_RDONLY|O_NONBLOCK,       00222);
    if(cmd->outf && !close(1))
      ofid = open((const char*)cmd->outf, O_WRONLY|O_RDONLY|O_CREAT, 00666);
    execlp("/bin/sh", "-c", cmd->strcmd);//cmd->args);   //execute command
    if(cmd->inf) close(0);     //end redirection
    if(cmd->outf) close(1);
    perror(*cmd->args);
    exit(0);
  }
  else if(!cmd->amp) waitpid(pid, 0, 0);
  else usleep(50000);
  return true;
}

/**
 * Read a list of commands and arguments from the command line
 * stores them and executes as a generic shell using child procs
 * via fork execv and waitpid
 */
int main(int argc, char** argv)
{
  CmdData* cmd = CmdData_Create();

  for( ; get_args(cmd) && strcmp(CmdData_at(cmd, 0),"exit"); CmdData_Init(&cmd))
  {
    int i = --argc;
    int fflag = -1;
    for( ; i < cmd->argc; ++i) //check for & < > flags
    {
      if(CmdData_argat(cmd, i, 0) == '<') CmdData_SetInF(&cmd, CmdData_at(cmd, ++i));
      else if(CmdData_argat(cmd, i, 0) == '>') CmdData_SetOutF(&cmd, CmdData_at(cmd, ++i));
      else if(CmdData_argat(cmd, i, 0) == '&') cmd->amp = true;
      else if(!cmd->inf && !cmd->outf && !cmd->amp) ++argc;

      if(cmd->inf || cmd->outf && fflag == -1) fflag = i - 1;
      else if( cmd->amp && fflag == -1) fflag = i;
    }
    if(fflag > 0)cmd->args[fflag] = 0;
    if(get_cmd_pth(cmd)) execute(cmd);
  }

  if(cmd) free(cmd);

  return 0;
}
