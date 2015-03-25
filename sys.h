#ifndef __sys__
#define __sys__

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

/**
 * Contains data associated with a command to be executed
 */
typedef struct _CmdData
{
  char* args[MAX_LEN], *pth, *inf, *outf; //args, path to exec io redirect
  bool amp; //execute no wait
  int argc , plen; //num args
  char* strcmd;

} CmdData;

int system(const char* command);

#endif
