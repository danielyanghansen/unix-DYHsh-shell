// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J") //ctrl L

char headerText[]=
" _______     ___    _     _     \n"
" |  __ \\ \\   / / |  | |   | |    \n"
" | |  | \\ \\_/ /| |__| |___| |__  \n"
" | |  | |\\   / |  __  / __| '_ \\ \n"
" | |__| | | |  | |  | \\__ \\ | | |\n"
" |_____/  |_|  |_|  |_|___/_| |_|\n\0"
;

// Greeting shell during startup
void init_shell()
{
	clear();
	printf("\033[0;32m");
	printf("\n\n %s", headerText);
	printf("\n\n\n\n******************"
		"************************");
	printf("\n\n\n\tDANIEL YANG HANSEN SHELL");
	printf("\n\n\t-USAGE NOT ADVISED-");
	printf("\n\n\n\n*******************"
		"***********************");
	char* username = getenv("USER");
	printf("\n\n\nUSER is: \033[0;33m@%s", username);
	printf("\033[0m\n"); //reset text color
}

// Function to take input
int takeInput(char* str)
{
	char* buf;

	buf = readline("\n>>> ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}

// Function to print Current Directory.
void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\n\033[0;31mDir: \033[0;36m%s\033[0m", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
	// For("Parent process id: %i\n", getpid()); a child
	printf("Parent process id: %i\n", getpid());
	fflush(stdout);
	
	pid_t pid = fork();
	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command..");
			fflush(stdout);
		}
		exit(0);
	} else {
		// waiting for child to terminate
		waitpid(pid, NULL, 0);
		return;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}

	if (p1 == 0) {
		// Child 1 executing..Type Something 
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command 1..");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0) {
				printf("\nCould not execute command 2..");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}

// Help command builtin
void openHelp()
{
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nCopyright @ Suprotik Dey"
		"\n-Use the shell at your own risk..."
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>exit"
		"\n>jobs"
		"\n>all other general commands available in UNIX shell"
		"\n>pipe handling"
		"\n>improper space handling");

	return;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
	int NoOfOwnCmds = 6, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "cd";
	ListOfOwnCmds[2] = "help";
	ListOfOwnCmds[3] = "hello";
	ListOfOwnCmds[4] = "jobs";
	ListOfOwnCmds[5] = "children";

	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}

	switch (switchOwnArg) {
	case 1:
		printf("\nGoodbye\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		openHelp();
		return 1;
	case 4:
		username = getenv("USER");
		printf("\nHello %s.\nMind that this is "
			"not a place to play around."
			"\nUse help to know more..\n",
			username);
		return 1;
	case 5:
		printf("Something about background processes");
		pid_t parentPid = getpid();

		return 1;
	case 6:
		{
			int pid = getpid();
			char * mypid = malloc(sizeof(pid) +1);   // ex. 34567
			sprintf(mypid, "%d", pid);

			char* fullpath[1000] = {'\0'};
			char* absolutepath[1000];
			strcat(fullpath, "/proc/");
			strcat(fullpath, "1");
			strcat(fullpath, "/task/");
			strcat(fullpath, "1");
			strcat(fullpath, "/children");
			printf("Printing full path : %s\n",fullpath);

			//check valid path (if the path length is short enough)
			if (realpath(fullpath, absolutepath) == NULL) {    
				printf("Badpath: %s", absolutepath);
				return 1;;
			}
			printf("Absolutepath: %s\n", absolutepath);

			FILE *fp = fopen(absolutepath, "r");
			size_t bytes_read;
			int bufsiz = 4096;
			char buff[bufsiz];
			char bufferOut[bufsiz];
			fscanf(fp, "%4096c", buff);
   			printf("%s", buff );


			fclose(fp);

			return 1;
		}
	default:
		break;
	}

	return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
	int i;
	for (i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}

	if (strpiped[1] == NULL)
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed, char** parsedpipe)
{

	char* strpiped[2];
	int piped = 0;

	piped = parsePipe(str, strpiped);

	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);

	} else {

		parseSpace(str, parsed);
	}

	if (ownCmdHandler(parsed)) {
		return 0;
	}
	else{
		return 1 + piped;
	}
}

int main()
{
	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	char* parsedArgsPiped[MAXLIST];
	int execFlag = 0;
	init_shell();

	for(;;){
		// print shell line
		printDir();
		// take input
		if (takeInput(inputString))
			continue;
		// process
		execFlag = processString(inputString,
		parsedArgs, parsedArgsPiped);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute
		if (execFlag == 1)
			execArgs(parsedArgs);

		if (execFlag == 2)
			execArgsPiped(parsedArgs, parsedArgsPiped);
	}
	return 0;
}
