// C Program to design a shell in Linux
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<stdio.h>
#include <errno.h>

#include"common.h"
#include"linkedList.h"
#include"linkedList.c"
#include"parsing.c"

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J") //ctrl L

static node_t * start;

char* buffer[MAXCOM] = {0};

char headerText[]=
" \033[0;32m_____\033[0;36m__     __\033[0;34m_    _     \033[0;31m_     \n"
" \033[0;32m|  __ \\ \033[0;36m\\   / / \033[0;34m|  | |   \033[0;31m| |    \n"
" \033[0;32m| |  | \\ \033[0;36m\\_/ /\033[0;34m| |__| |\033[0;35m___\033[0;31m| |__  \n"
" \033[0;32m| |  | |\033[0;36m\\   / \033[0;34m|  __  \033[0;35m/ __\033[0;31m| '_  \\ \n"
" \033[0;32m| |__| | \033[0;36m| |  \033[0;34m| |  | \033[0;35m\\__ \\ \033[0;31m| | |\n"
" \033[0;32m|_____/  \033[0;36m|_|  \033[0;34m|_|  |_\033[0;35m|___/\033[0;31m_| |_|\n\0"
;

// Greeting shell during startup
void init_shell()
{
	clear();
	printf("\033[0;32m");
	printf("\n\n %s \033[0;32m", headerText);
	printf("\n\n\n\n"
	"******************************************\n"
	"*                                        *\n"
	"*                                        *\n"
	"*         DANIEL YANG HANSEN SHELL       *\n"
	"*           -USAGE NOT ADVISED-          *\n"
	"*                                        *\n"
	"*                                        *\n"
	"******************************************");
	char* username = getenv("USER");
	printf("\n\n\nUSER is: \033[0;33m@%s", username);
	printf("\033[0m\n"); //reset text color
}

void zombie_cleanup(node_t *start) {


	for(node_t* n = start->next; n != NULL; n = n->next){
		int wstatus;
		pid_t deadstatus = waitpid(n->pidData, &wstatus, WNOHANG | WUNTRACED | WCONTINUED);

		if (deadstatus != 0) {
			if (deadstatus > 0) {
				kill(deadstatus, SIGKILL);
			}

			printf("Exit status [%s] = %i\n", n->commandData, WEXITSTATUS(wstatus) );
			n = n->previous;
			deleteNode(n->next);
		}
		
	}

	fflush(stdout);
	
	return;
}

// Function to take input
int takeInput(char* str)
{
	char* buf;

	buf = readline(" ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 1;
	} else {
		return 0;
	}
}

// Function to print Current Directory.
void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\n\033[0;31mDir: \033[0;36m%s :\033[0m", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed, int isBackgroundProcess)
{
	int argLen = getArgLen(parsed);

	char *pathin[MAXCOM] = {0};
	char *pathout[MAXCOM] = {0};
	int inputFlag = 0;
	int outputFlag = 0;

	//printf("\nparsed: %s, %s, %s, %s\n", parsed[0], parsed[1], parsed[2], parsed[3]);
	printf("Size of parsed array: %i", argLen);
	fflush(stdout);

	for (int n = 0; n < argLen; n++) {
		printf("\nArray no: %i: [%s] \n", n, parsed[n]);
	}
	printf("\nLooking for I/O Redirection...\n");

	for (int n = 0; n < argLen; n++) {
		char *p_char_in = strchr(parsed[n], (int) '<');
		char *p_char_out = strchr(parsed[n], (int) '>');

		//NOTE: If there are multiple repeated in/output streams, an option is just to overwrite the source
		//The **DIFFICULT** alternative would be to chain the streams.
		//Output appending (>>) is not supported
		if (p_char_in != NULL && parsed[n+1] != NULL) {

			inputFlag = 1; 
			strcpy(pathin, parsed[n+1]);
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the < symbol entry as well as the entry behind it)
				parsed[m] = parsed[m+2];
			}
			parsed[argLen -1] = NULL;
			parsed[argLen -2] = NULL;
			argLen -= 2;
			n--;

		} else if (p_char_out != NULL  && parsed[n+1] != NULL) {
			outputFlag = 1; 
			strcpy(pathout, parsed[n+1]);
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the > symbol entry as well as the entry behind it)
				parsed[m] = parsed[m+2];
			}
			parsed[argLen -1] = NULL;
			parsed[argLen -2] = NULL;
			argLen -= 2;
			n--;
		}
	}
/*
*/

	pid_t pid = fork();
	if (pid == -1) {
		printf("\nFailed forking child...\n");
		return;
	} else if (pid == 0) {
		
		if (inputFlag == 1) { //Return -1 if there's an error with the input stream
			if (freopen(pathin, "r", stdin) == NULL) perror("Error: Couldn't redirect input stream");
		}
	
		if (outputFlag == 1) { //Return -1 if there's an error with the output stream
			printf("Now we are trying to set output stream to %s \n", pathout);
			if (freopen(pathout, "w", stdout) == NULL) perror("Error: Couldn't redirect output stream");
		}

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command [%s]...\n\n", parsed[0]);
			fflush(stdout);
		}

		freopen("dev/stdin", "r", stdin);
		freopen("dev/stdout", "w", stdout);

		exit(0);
	}

	if (pid != 0){

		for(;;) {/*Things, see below*/ break;}
		node_t * node = createNode(pid, buffer);
		addNode(node);
		memset(buffer, 0, sizeof(buffer));


		for(;;) {/*Things, see below*/ break;}
		// waiting for child to terminate
		if (!isBackgroundProcess) {
			int status;
			waitpid(pid, &status, 0);
		}
		return;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe, int isBackgroundProcess)
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
	puts("\n***WELCOME TO DYHSH HELP***"
		"\n-Use the shell at your own risk..."
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>exit"
		"\n>jobs"
		"\n>hello"
		"\n>all other general commands available in UNIX shell"
		"\n>I/O Redirection"
		"\n>Improper pipe handling"
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
			printf("=============Processes: =============\n\n");

			node_t *n;
			n = start;

			printf("Parent PID: %i\n", n->pidData);

			while (n->next != NULL) { 
				n = n->next;
				printf("\tProcess PID: %i, Command: %s\n", n->pidData, n->commandData);
			}

			printf("=====================================\n");
			fflush(stdout);

			return 1;
		default:
			break;
		}

	return 0;
}

void parseIO(char **parsed) {
	parseCharToArgs(parsed, "<");
	parseCharToArgs(parsed, ">");


}

int processString(char* str, char** parsed, char** parsedpipe, int* isBackgroundTask)
{

	char* strpiped[2];
	int piped = 0;

	int daemon = 0;
/*
*/
	daemon = parseDaemon(str);
	*isBackgroundTask = daemon;

	

	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);

	} else {
		parseSpace(str, parsed);
		if (parsed[0] == NULL) return 0;
		parseIO(parsed);
	}
	printf("\nstr: %s, strpiped: %s, parsed0: %s\n", str, *strpiped, parsed[0]);

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
	start = createNode(getpid(), "Parent\0");
	setLastNode(start);


	for(;;){

		zombie_cleanup(start);

		printDir();
		if (!takeInput(inputString))
			continue;
		strcpy(buffer, inputString);
		zombie_cleanup(start);

		int isBackgroundProcess = 0;
		// process
		execFlag = processString(inputString,
		parsedArgs, parsedArgsPiped, &isBackgroundProcess);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute

		if (execFlag == 1) {
			printf("%s", *parsedArgs);
			execArgs(parsedArgs, isBackgroundProcess);

		}
		else if (execFlag == 2) {
			execArgsPiped(parsedArgs, parsedArgsPiped, isBackgroundProcess);
		}
	}
	return 0;
}
