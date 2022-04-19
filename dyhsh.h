#ifndef __dyhsh_h__
#define __dyhsh_h__

// C Program to design a shell in Linux

#include"common.h"

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J") //ctrl L
#define getUsername() getenv("USER")

static const char headerText[]=
" \033[0;32m_____\033[0;36m__     __\033[0;34m_    _     \033[0;31m_     \n"
" \033[0;32m|  __ \\ \033[0;36m\\   / / \033[0;34m|  | |   \033[0;31m| |    \n"
" \033[0;32m| |  | \\ \033[0;36m\\_/ /\033[0;34m| |__| |\033[0;35m___\033[0;31m| |__  \n"
" \033[0;32m| |  | |\033[0;36m\\   / \033[0;34m|  __  \033[0;35m/ __\033[0;31m| '_  \\ \n"
" \033[0;32m| |__| | \033[0;36m| |  \033[0;34m| |  | \033[0;35m\\__ \\ \033[0;31m| | |\n"
" \033[0;32m|_____/  \033[0;36m|_|  \033[0;34m|_|  |_\033[0;35m|___/\033[0;31m_| |_|\n\0"
;

static const char helpText[]=
"\n********************WELCOME TO \033[0;32mD\033[0;36mY\033[0;34mH\033[0;35mS\033[0;31mH\033[0m HELP********************"
"\n*             Use the shell at your own risk...             *"
"\n*                                                           *"
"\n* List of Commands supported:                               *"
"\n*   >cd                                                     *"
"\n*   >exit                                                   *"
"\n*   >jobs                                                   *"
"\n*   >hello                                                  *"
"\n*   >all other general commands available in UNIX shell     *"
"\n*                                                           *"
"\n* Other features:                                           *"
"\n*   >I/O Redirection                                        *"
"\n*   >Improper pipe handling                                 *"
"\n*   >improper space handling                                *"
"\n*************************************************************"
"\n"
;

static const char headerCard[]=
"\n\n\n\n"
"******************************************\n"
"*                                        *\n"
"*                                        *\n"
"*         DANIEL YANG HANSEN SHELL       *\n"
"*           -USAGE NOT ADVISED-          *\n"
"*                                        *\n"
"*                                        *\n"
"******************************************"
;

static const char helloText[]=
"\nHello \033[0;33m%s\033[0m."
"\nMind that this is not a place to play around."
"\nUse help to know more...\n"
;

// Greeting shell during startup
void init_shell();

// Function to take input
int takeInput(char* str);

// Function to print Current Directory.
void printDir();

// Function where the system command is executed
void execArgs(char** parsed, int isBackgroundProcess);

// Function where the piped system commands is executed
void execArgsPiped(char** unparsedPipe, int isBackgroundProcess);

// Help command builtin
void openHelp();

// Function to execute builtin commands
int ownCmdHandler(char** parsed);

int processString(char* str, char** parsed, char** unparsedPiped, int* isBackgroundTask);

#endif