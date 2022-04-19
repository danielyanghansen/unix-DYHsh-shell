// C Program to design a shell in Linux
#include"dyhsh.h"

#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<errno.h>
#include<pwd.h>

#include"processLinkedList.c"
#include"parsing.c"

struct passwd *pw;

char *homedir;

static node_t * start;

static char CMD_BUFFER[MAXCOM] = {0};

// Greeting shell during startup
void init_shell() {
	clear();
	printf("\033[0;32m");
	printf("\n\n %s \033[0;32m", headerText);
	printf("%s\n", headerCard);
	printf("\n\n\nUSER is: \033[0;33m@%s", getUsername());
	printf("\nHome dir is %s", homedir);
	printf("\033[0m\n"); //reset text color
	return;
}

// Function to take input
int takeInput(char* str) {
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
void printDir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	if (strcmp(cwd, homedir) == 0) {
		strcpy(cwd, "~");
	}
	printf("\n\033[0;31mDir: \033[0;36m%s :\033[0m", cwd);
	return;
}

void redirectInput(char* filepath){
  FILE *fd;
  fd = fopen(filepath, "r");
  if (fd == NULL) {
    perror("Error: Couldn't redirect input stream");
    return;
  }
  dup2(fileno(fd), STDIN_FILENO);
  fclose(fd);
}

void redirectOutput(char* filepath){
  FILE *fd;
  fd = fopen(filepath, "w");
  if (fd == NULL) {
    perror("Error: Couldn't redirect output stream");
    return;
  }
  dup2(fileno(fd), STDOUT_FILENO);
  fclose(fd);
}
void flagIORedirect(int *inputFlag, int *outputFlag, char *pathin, char *pathout, char **parsed) {
	int argLen = getArgLen(parsed);
	
	for (int n = 0; n < argLen; n++) {
		char *p_char_in = strchr(parsed[n], (int) '<');
		char *p_char_out = strchr(parsed[n], (int) '>');

		//NOTE: If there are multiple repeated in/output streams, an option is just to overwrite the source
		//The **DIFFICULT** alternative would be to chain the streams.
		//Output appending (>>) is not supported
		if (p_char_in != NULL && parsed[n+1][0] != '\0') {

			*inputFlag = 1; 
			strcpy(pathin, parsed[n+1]);
			parsed[n][0] = '\0';
			parsed[n+1][0] = '\0';
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the < symbol entry as well as the entry behind it)
				memcpy(parsed[m], parsed[m+2], sizeof(char *));
			}
			strcpy(parsed[argLen -1], "\0");
			strcpy(parsed[argLen -2],"\0");
			argLen -= 2;
			n--;

		} else if (p_char_out != NULL  && parsed[n+1][0] != '\0') {
			*outputFlag = 1; 
			strcpy(pathout, parsed[n+1]);
			parsed[n][0] = '\0';
			parsed[n+1][0] = '\0';
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the > symbol entry as well as the entry behind it)
				parsed[m] = parsed[m+2];
			}
			parsed[argLen -1][0] = '\0';
			parsed[argLen -2][0] = '\0';
			argLen -= 2;
			n--;
		}
	}
}

// Function where the system command is executed
void execArgs(char** parsed, int isBackgroundProcess) {

	char pathin[MAXCOM] = {0};
	char pathout[MAXCOM] = {0};
	int inputFlag = 0;
	int outputFlag = 0;

	flagIORedirect(&inputFlag, &outputFlag, pathin, pathout, parsed);

	/*
	int argLen = getArgLen(parsed);
	for (int n = 0; n < argLen; n++) {
		char *p_char_in = strchr(parsed[n], (int) '<');
		char *p_char_out = strchr(parsed[n], (int) '>');

		//NOTE: If there are multiple repeated in/output streams, an option is just to overwrite the source
		//The **DIFFICULT** alternative would be to chain the streams.
		//Output appending (>>) is not supported
		if (p_char_in != NULL && parsed[n+1][0] != '\0') {

			inputFlag = 1; 
			strcpy(pathin, parsed[n+1]);
			parsed[n][0] = '\0';
			parsed[n+1][0] = '\0';
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the < symbol entry as well as the entry behind it)
				memcpy(parsed[m], parsed[m+2], sizeof(char *));
			}
			strcpy(parsed[argLen -1], "\0");
			strcpy(parsed[argLen -2],"\0");
			argLen -= 2;
			n--;

		} else if (p_char_out != NULL  && parsed[n+1][0] != '\0') {
			outputFlag = 1; 
			strcpy(pathout, parsed[n+1]);
			parsed[n][0] = '\0';
			parsed[n+1][0] = '\0';
			for (int m = n; m < argLen -2; m++) { //Truncate 'parsed' by 2. (Removing the > symbol entry as well as the entry behind it)
				parsed[m] = parsed[m+2];
			}
			parsed[argLen -1][0] = '\0';
			parsed[argLen -2][0] = '\0';
			argLen -= 2;
			n--;
		}
	}
	*/
	int argLen = getArgLen(parsed); 

	char* executable [MAXLIST] = {'\0'};
	for (int i = 0; i < argLen; i++) {
		executable[i] = parsed[i];
	}

	pid_t pid = fork();
	if (pid == -1) {
		printf("\nFailed forking child...\n");
		return;
	} else if (pid == 0) {
		if (inputFlag == 1) { //Return -1 if there's an error with the input stream
			redirectInput(pathin);
		}
	
		if (outputFlag == 1) { //Return -1 if there's an error with the output stream
			redirectOutput(pathout);
		}

		if (execvp(executable[0], executable) < 0) {
			printf("\nCould not execute command [%s]...\n\n", parsed[0]);
			fflush(stdout);
		}

		freopen("dev/stdin", "r", stdin);
		freopen("dev/stdout", "w", stdout);

		return exit(0);
	}

	if (pid != 0){
		node_t * node = createNode(pid, CMD_BUFFER);
		addNode(node);
		memset(CMD_BUFFER, 0, sizeof(CMD_BUFFER)); //Reset CMD_BUFFER to zero

		// waiting for child to terminate
		if (!isBackgroundProcess) {
			int status;
			waitpid(pid, &status, 0);
		}
		return;
	}
	return;
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	int argLen1 = getArgLen(parsed);
	int argLen2 = getArgLen(parsedpipe);

	char* executable1 [MAXLIST] = {'\0'};
	char* executable2 [MAXLIST] = {'\0'};
	
	for (int i = 0; i < argLen1; i++) {
		executable1[i] = parsed[i];
	}
	for (int i = 0; i < argLen2; i++) {
		executable2[i] = parsedpipe[i];
	}

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
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(executable1[0], executable1) < 0) {
			printf("\nCould not execute command 1 [%s]...\n", executable1[0]);
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
			if (execvp(executable2[0], executable2) < 0) {
				printf("\nCould not execute command 2 [%s]...\n", executable2[0]);
				checkArgsList(executable1);
				checkArgsList(executable2);
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
void openHelp() {
	printf("%s\n", helpText);
	return;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed) {
	int NoOfOwnCmds = 6, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];

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
			if (parsed[1][0] == '\0') {
				chdir(homedir);
				return 1;
			}
			chdir(parsed[1]);
			return 1;
		case 3:
			openHelp();
			return 1;
		case 4:
			printf(helloText,
				getUsername());
			return 1;
		case 5:
			jobs(start);
			return 1;
		default:
			break;
		}

	return 0;
}

int processString(char* str, char** parsed, char** parsedpipe, int* isBackgroundTask)
{
	int daemon = 0;
	daemon = parseDaemon(str);
	*isBackgroundTask = daemon;

	char* strpiped[2];
	char before[MAXCOM];
	char after[MAXCOM];
	memset(before, 0, sizeof(before));
	memset(after, 0, sizeof(after));
	strpiped[0] = before;
	strpiped[1] = after;
	int isPiped = 0;

	//int isPiped = parseChar(str, parsedpipe, "|", MAXPIPE);
	isPiped = parsePipe(str, strpiped); //strip string into strpiped[0] on "|"

	if(isPiped) {
		parseChar(strpiped[0], parsed, " ", MAXLIST);
		parseChar(strpiped[1], parsedpipe, " ", MAXLIST);
		strcat(strpiped[0], "\0");
		strcat(strpiped[1], "\0");
		parseIO(parsed);
		parseIO(parsedpipe);
		checkArgsList(parsed);
		checkArgsList(parsedpipe);
	}
	else {
		parseChar(str, parsed, " ", MAXLIST);
		if (parsed[0][0] == '\0') return 0;
		parseIO(parsed);
	}

	if (ownCmdHandler(parsed)) {
		return 0;
	}
	else{
		return 1 + isPiped;
	}
}

int main() {
	pw = getpwuid(getuid());
	homedir = pw->pw_dir;

	char *inputString = malloc(sizeof(char) * MAXCOM);
	char **parsedArgs = malloc(sizeof(char*) * (MAXLIST + 1));
	char **parsedArgsPipe = malloc(sizeof(char*) * (MAXPIPE + 1)); //Need an extra spot for judging end state

	
	for(int i = 0; i < MAXLIST + 1; i++) {
		parsedArgs[i] = (char*) malloc(sizeof(char) * MAXCOM);
	} 
	for(int i = 0; i < MAXPIPE + 1; i++) {
		parsedArgsPipe[i] = (char*) malloc(sizeof(char) * MAXCOM);
	} 
	
	
	
	//char* parsedArgsPiped[MAXLIST];
	int execFlag = 0;
	init_shell();
	start = createNode(getpid(), "Parent\0");
	setLastNode(start);

	for(;;){

		memset(inputString, 0, sizeof(char) * MAXCOM);
		zombie_cleanup(start);

		for(int i =0; i < MAXLIST + 1; i ++) {
			parsedArgs[i][0] = '\0';
		}
		for(int i =0; i < MAXPIPE + 1; i ++) {
			parsedArgsPipe[i][0] = '\0';
		}
		
		printDir();
		if (!takeInput(inputString))
			continue;
		
		
		strcpy(CMD_BUFFER, inputString);
		zombie_cleanup(start);

		int isBackgroundProcess = 0;
		// process
		execFlag = processString(inputString, parsedArgs, parsedArgsPipe, &isBackgroundProcess);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute
		if (execFlag == 1) {
			execArgs(parsedArgs, isBackgroundProcess);
		}
		if (execFlag == 2) {
			execArgsPiped(parsedArgs, parsedArgsPipe);
		}
	}
	freeArgs(parsedArgs);
	return 0;
}
