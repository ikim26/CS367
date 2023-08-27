/* This is the only file you should update and submit. */

/* Fill in your Name and GNumber in the following two comment fields
 * Name: Isaac Kim
 * GNumber: 01201648
 */

#include <sys/wait.h>
#include "textproc.h"
#include "parse.h"
#include "util.h"

/* Constants */
#define DEBUG 0
/*

static const char *textproc_path[] = { "./", "/usr/bin/", NULL };
static const char *instructions[] = { "quit", "help", "list", "new", "open", "write", "close", "print", "active", "pause", "resume", "cancel", "exec", NULL};
*/

/*
 * Linked List structure to handle adding and removing buffers
 */
struct node{
    int id;
    struct node *next;
    int state;      //0 is ready, 1 is working, 2 is paused
    char* string;
    char* cmd;
    int pid;        //by default is 0 (for no pid)
};

/*global vars*/
struct node *head = NULL;
struct node *current = NULL;
int buffer_id_count = 0;
int activeBuffer = 0;
int numOfBuffers = 0;
int cancelledPid = 0;
int pausedPid = 0;
int resumePid = 0;
int toPause = 0;
int toResume = 0;
int activeFinished = 0;
int INTyet = 0;
int TSTPyet = 0;

/* method declarations*/
void insertFront(int id);
bool isEmpty();
struct node* get(int id);
struct node* getPid(int pid);
struct node* removeNode(int id);
bool hasWorking();
bool hasPaused();

void tpNew(char* str);
void tpList();
void tpActive(int bufferID);
void tpClose(int bufferID);
void tpOpen(char* fd);
void tpWrite(int id, char* fd);
void tpPrint(int id);
void tpExec(char* instruct, int id, char* argv[]);
void tpNewV2(char* instruct, char* argv[]);
void tpCancel(int id);
void tpPause(int id);
void tpResume(int id);

void sig_handler(int sig);

/* 
 * Simple push method for our linked list
 * Allocates memory for new Node
 * Defaults to setting state to ready (0)
 * Inserts at the front of linked list
 */
void insertFront(int id){
    struct node *newNode = (struct node*) malloc(sizeof(struct node));

    newNode->id = id;
    newNode->next = head;
    newNode->string = "";
    newNode->state = 0;
    newNode->cmd = NULL;
    newNode->pid = 0;
    head = newNode;
}

/* 
 * Simple push method for our linked list
 * Allocates memory for new Node
 * Defaults to setting state to ready (0)
 * Sets string attribute to 2nd parameter
 * Inserts at the front of linked list
 */
void insertFrontString(int id, char* str){
    struct node *newNode = (struct node*) malloc(sizeof(struct node));

    newNode->id = id;
    newNode->next = head;
    newNode->state = 0;
    newNode->string = str;
    head = newNode;
}

/* 
 * Method for determining is list is empty or not
 */
bool isEmpty() {
    return head == NULL;
}

/* 
 * Retrieval method for id
 * Returns NULL if not found
 */
struct node* get(int id) {
    struct node* current = head;
    if(isEmpty()) {
       return NULL;
    }

    while(current->id != id) {
        if(current->next == NULL) {
            return NULL;
        } 
        else {
            current = current->next;
        }
    }      
    //if we reach here, current is in the list and the current node we are looking for
   return current;
}

/* 
 * Retrieval method for pid
 * Returns NULL if not found
 */
struct node* getPid(int pid) {
    struct node* current = head;
    if(isEmpty()) {
       return NULL;
    }
    while(current->pid != pid) {
        if(current->next == NULL) {
            return NULL;
        } 
        else {
            current = current->next;
        }
    }      
    //if we reach here, current is in the list and the current node we are looking for
   return current;
}

/* 
 * Remove method for node with specific id
 * Returns NULL if not found
 */
struct node* removeNode(int id) {

   struct node* current = head;
   struct node* previous = NULL;    //tracker for when we remove
    
    if(isEmpty()) {
        return NULL;
    }

   while(current->id != id) {
        if(current->next == NULL) {
            return NULL;
        } 
        else {
         previous = current;
         current = current->next;
        }
    }
    //after we exit while loop, if we havent returned null, we have found the id we want

    //check if its first in list
    if(current == head) {
        head = head->next;
    } 
    else {
        //removal process
        previous->next = current->next;
    }    
    //return current with the id we want
    return current;
}

/* 
 * Method to see if there are any nodes that are in the "working" state
 * Returns false if not found
 */
bool hasWorking(){
    struct node* current = head;
    if(isEmpty()) {
       return false;
    }
    if(current->state == 1){    //return true if at least one we find is "working"
        return true;
    }

   while(current->state != 1) {
        if(current->next == NULL) {
            return NULL;
        } 
        else {
            current = current->next;
        }
    }      
    //if we reach here, current is in the list and the current node we are looking for
   return current->state == 1;
}

/* 
 * Method to see if there are any nodes that are in the "paused" state
 * Returns false if not found
 */
bool hasPaused(){
    struct node* current = head;
    if(isEmpty()) {
       return false;
    }
    if(current->state == 2){    //return true if at least one we find is "paused"
        return true;
    }

   while(current->state != 2) {
        if(current->next == NULL) {
            return NULL;
        } 
        else {
            current = current->next;
        }
    }      
    //if we reach here, current is in the list and the current node we are looking for
   return current->state == 2;
}

/* 
 * Method to handle "new" command
 * Parameter char = NULL if there is no text to fill our buffer with
 * Creates a new buffer and adds it to list structure
 * Handles buffer id requirements
 */
void tpNew(char* str){
    //increment global buffer id to keep track of max id
    buffer_id_count++;
    numOfBuffers++;
    //check if any buffers exist
    if(isEmpty()){
        buffer_id_count = 1;
    }
    //check if we are filling buffer with no text
    if(str == NULL){
        insertFront(buffer_id_count);   //insert to our linked list structure
    }
    else{
        insertFrontString(buffer_id_count, str);    //insert to our linked list structure using string method
    }
    log_open(buffer_id_count);
    log_activate(buffer_id_count);  //activate call
    activeBuffer = buffer_id_count; //set current active buffer to newly created buffer
}

/* 
 * Method to handle "list" command
 */
void tpList(){
    log_buf_count(numOfBuffers);
    int count = 0;
    //check if no buffers exist
    if(buffer_id_count == 0){
        log_show_active(0);
    }
    while(count < buffer_id_count){
        count++;
        //search for that id in our list
        if(get(count) != NULL){
            //placeholders---------------------------------------------------
            log_buf_details(count, get(count)->state , get(count)->pid, get(count)->cmd);
        }
    }
    log_show_active(activeBuffer);  //show the active buffer
}

/* 
 * Method to handle "active" command
 * Has one parameter for which buffer to activate given buffer id
 * Parameter defaults to current active buffer if no arg is given
 */
void tpActive(int bufferID){
    if(bufferID == 0){
        bufferID = activeBuffer;
    }
    //check if our list has any buffers in the first place
    if(isEmpty()){
        log_buf_id_error(bufferID);
    }
    //check if the buffer id is in linked list of buffer ids
    else if(get(bufferID) != NULL){
        log_activate(bufferID);
        activeBuffer = bufferID;
        int status = 0;
        if(get(bufferID)->state == 1){   //1 is working state
            pid_t returnPid = waitpid(get(bufferID)->pid, &status, 0);
            if(WIFEXITED(status)){      //if command is successful
                log_cmd_state(returnPid, LOG_ACTIVE, get(bufferID)->cmd, LOG_CANCEL);
                //get(id)->state = LOG_STATE_PAUSED;
            } 
        }
    }
    else{
        log_buf_id_error(bufferID);
    }
}

/* 
 * Method to handle "close" command
 * Has one parameter for which buffer to close given buffer id
 * Parameter defaults to current active buffer if no arg is given
 */
void tpClose(int bufferID){
    struct node* removedNode = get(bufferID);
    //if there are no active buffers, throw error
    if(numOfBuffers == 0){
        log_buf_id_error(bufferID);
        return;
    }
    //no argument case
    if(bufferID == 0){
        numOfBuffers--;
        if(numOfBuffers == 0){
            removeNode(bufferID);
            log_close(activeBuffer);
            head = NULL;
            return;
        }
        removeNode(activeBuffer);
        //assign the active buffer to the highest buffer id
        log_close(activeBuffer);
        log_activate(head->id);
        activeBuffer = head->id;
        buffer_id_count = head->id;
    }
    //if invalid buffer arg
    else if(removedNode == NULL){
        log_buf_id_error(bufferID);
    }
    //if buffer is not ready
    else if(removedNode->state != 0){
        log_close_error(bufferID);
    }
    //if we successfully removed node
    else if(removedNode != NULL){
        numOfBuffers--;
        //if the buffer we remove is the current active buffer
        if(activeBuffer == bufferID && numOfBuffers != 0){
            free(removeNode(bufferID));
            //assign the active buffer to the highest buffer id
            log_close(bufferID);
            log_activate(head->id);
            activeBuffer = head->id;
            buffer_id_count = head->id;
        }
        else{
            //simply remove the buffer since it is not the active one
            removeNode(bufferID);
            log_close(bufferID);
            if(numOfBuffers == 0){
                head = NULL;
            }
        }
    }
}

/* 
 * Method to handle "open" command
 * Has one parameter for name of file
 * Calls tpNew() with string of text that was in file param
 */
void tpOpen(char* fd){
    //make file descriptor
    int fileDes = open(fd, O_RDONLY | O_CREAT);
    //check if error during opening file
    if(fileDes == -1){
        log_file_error(LOG_FILE_OPEN_READ, fd);
        return;
    }
    char* newString = fd_to_text(fileDes);  //text of file
    tpNew(newString);
    log_read(buffer_id_count, fd);

    int closeInt = close(fileDes);
    if(closeInt < 0){
        exit(1);    //if closeInt == -1, there was an error
    }
}

/* 
 * Method to handle "write" command
 * Has 2 parameters, buffer id and file name
 * ID is 0 if no id is specified -> use activeBuffer
 */
void tpWrite(int id, char* fd){
    struct node* removedNode = get(id);
    //check for default buffer
    if(id == 0){
        id = activeBuffer;
        removedNode = get(id);
    }
    else{
        //check if id is valid
        if(removedNode == NULL){
            log_buf_id_error(id);
            return;
        }
    }
    //make file descriptor
    int fileDes = open(fd, O_RDWR | O_CREAT | O_TRUNC, 0600);
    //check if error during opening file
    if(fileDes == -1){
        log_file_error(LOG_FILE_OPEN_READ, fd);
        return;
    }
    text_to_fd(removedNode->string, fileDes);
    log_write(id, fd);
}

/* 
 * Method to handle "print" command
 * Has 1 parameters which is buffer id
 * ID is 0 if no id is specified -> use activeBuffer
 */
void tpPrint(int id){
    struct node* removedNode = get(id);
    //check for default buffer
    if(id == 0){
        id = activeBuffer;
        removedNode = get(id);
    }
    else{
        //check if id is valid
        if(removedNode == NULL){
            log_buf_id_error(id);
            return;
        }
    }
    log_print(id, removedNode->string);
}

/* 
 * Method to handle "exec" command
 * Has 2 parameters: buffer id and list of cmdline args
 * ID is 0 if no id is specified -> use activeBuffer
 */
void tpExec(char* instruct, int id, char* argv[]){
    //formatting exec arguments
    char ex1[99] = "./";
    char ex2[99] = "/usr/bin/";
    char idStr[2];
    //formatting cmdline for log calls
    char* cmd = malloc(sizeof(char*));
    strcat(cmd, instruct);
    if(id != 0){
        strcat(cmd, " ");
        sprintf(idStr, "%d", id);
        strcat(cmd, idStr);
    }
    strcat(ex1, argv[0]);
    strcat(ex2, argv[0]);
    for(int i = 0; argv[i]; i++){
        strcat(cmd, " ");
    	strcat(cmd, argv[i]);
    }

    int pfd[2];     //creating pipe
    int pfd1[2];    //creating 2nd pipe
    pipe(pfd);
    pipe(pfd1);

    //checking for errors or state conflicts
    if(id == 0){
        id = activeBuffer;
    }
    if(get(id) == NULL){
        log_buf_id_error(id);
        return;
    }
    if(get(id)->state != 0){	//0 is ready state int
        log_cmd_state_conflict(id, 1);
        return;
    }

    int attempt = 0;
    //blocking signals
    sigset_t mask, prev;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &prev);

    pid_t parentId = fork();
    get(id)->state = LOG_STATE_WORKING; //if we reach here, then we are ok to work on this buffer
    //unblocking signals
    sigprocmask(SIG_SETMASK, &prev, NULL);

    if(!parentId){    //child
        setpgid(0,0);

        close(pfd[1]);
        close(pfd1[0]);
        setpgid(0, getpid());
        /* We want the read end to become our standard input: */
        dup2(pfd[0],  STDIN_FILENO);              // pread
        dup2(pfd1[1], STDOUT_FILENO);             //change stdout to our pipe[1]
        
        /* Run cmd*/
        attempt = execv(ex1, argv); 
        if(attempt == -1){
            attempt = execv(ex2, argv);
        }
        if(attempt == -1){
            log_command_error(argv[0]);
            exit(1);
        }
        exit(0);
    }

    else{    //parent
    	close(pfd[0]);
        close(pfd1[1]);
        setpgrp();
        /* Turn the write end into a FILE* so that we can use fprintf. */
        FILE *fp = fdopen(pfd[1], "w"); // pwrite

        //write to buffer string
        fprintf(fp, get(id)->string);

        fclose(fp);     //close file descriptor
        int status;

        if(id == activeBuffer){
            //update buffer contents
            get(id)->cmd = cmd;
            get(id)->pid = parentId;

            log_start(id, parentId, LOG_ACTIVE, cmd);
            activeFinished = 1;
            //wait
        	pid_t returnPid = waitpid(parentId, &status, 0);
            if(WIFEXITED(status) && returnPid > 0){      //if command is successful
        		get(id)->string = fd_to_text(pfd1[0]);
                log_cmd_state(returnPid, LOG_ACTIVE, cmd, LOG_CANCEL);
                //update buffer contents
                get(id)->state = 0;
                activeFinished = 0;
                get(id)->pid = 0;
                get(id)->cmd = NULL;
        	} 
            
        }
        else{
            //don't wait
            log_start(id, parentId, LOG_BACKGROUND, cmd);
            get(id)->state = 1;
            get(id)->cmd = cmd;
            get(id)->pid = parentId;        //store parentId for when we want to get it later
        }
    }
    return;
}

/* 
 * Method to handle upgraded "new" command
 * Has 1 parameters: cmdline args
 * calls tpNew() and tpExec()
 */
void tpNewV2(char* instruct, char* argv[]){
    tpNew(NULL);
    tpExec(instruct, 0, argv);
}

/* 
 * Method to handle "cancel" command
 * Has 1 parameter: buffer id; defaults to active when 0
 */
void tpCancel(int id){
    if(id == 0){            //check for default buffer
        id = activeBuffer;
    }
    if(get(id) == NULL){    //check for invalid buffer
        log_buf_id_error(id);
        return;
    }
    if(!hasWorking()){      //check if there are no running processes
        log_cmd_state_conflict(id, get(id)->state);
        return;
    }
    if(get(id)->state == LOG_STATE_READY){  //check if buffer is in ready state
        log_buf_id_error(id);
        return;
    }
    log_cmd_signal(LOG_CMD_CANCEL, id);
    cancelledPid = get(id)->pid;
    kill(get(id)->pid, SIGINT);
}

/* 
 * Method to handle "pause" command
 * Has 1 parameter: buffer id; defaults to active when 0
 */
void tpPause(int id){
    if(id == 0){            //check for default buffer
        id = activeBuffer;
    }
    if(get(id) == NULL){    //check for invalid buffer
        log_buf_id_error(id);
        return;
    }
    if(!hasWorking()){      //check if there are no running processes
        log_cmd_state_conflict(id, get(id)->state);
        return;
    }
    if(get(id)->state == LOG_STATE_READY){  //check if buffer is in ready state
        log_buf_id_error(id);
        return;
    }
    log_cmd_signal(LOG_CMD_PAUSE, id);
    pausedPid = get(id)->pid;
    toResume++;
    toPause++;
    kill(get(id)->pid, SIGTSTP);
}

/* 
 * Method to handle "resume" command
 * Has 1 parameter: buffer id; defaults to active when 0
 */
void tpResume(int id){
    if(id == 0){            //check for default buffer
        id = activeBuffer;
    }
    if(get(id) == NULL){    //check for invalid buffer
        log_buf_id_error(id);
        return;
    }
    if(!hasWorking() && !hasPaused()){  //check if there are no running or paused processes
        log_cmd_state_conflict(id, get(id)->state);
        return;
    }
    if(get(id)->state == LOG_STATE_READY){  //check if buffer is in ready state
        log_buf_id_error(id);
        return;
    }
    if(get(id)->state == LOG_STATE_WORKING){//check if buffer is in working state
        return;
    }
    log_cmd_signal(LOG_CMD_RESUME, id);
    resumePid = get(id)->pid;
    kill(get(id)->pid, SIGCONT);
}

/* 
 * Handler for signals
 * Has 1 parameter: signal
 */
void sig_handler(int sig){
    int status = 0;
    pid_t pid = 0;
    struct node* pidNode;
    
    if(sig == SIGINT && INTyet == 0){       //SIGINT case
        log_ctrl_c();
        kill(get(activeBuffer)->pid, SIGINT);
        INTyet++;
    }
    
    else if(sig == SIGTSTP && TSTPyet == 0){    //SIGTSTP case
        log_ctrl_z();
        kill(get(activeBuffer)->pid, SIGTSTP);
        TSTPyet++;
        toPause++;
        toResume++;
    }
    /*
    else if(sig == SIGCONT){
        pidNode = getPid(resumePid);
        pidNode->state = 1;     //1 is working state
        log_cmd_state(resumePid, LOG_BACKGROUND, pidNode->cmd, LOG_RESUME);
        if(pidNode->id == activeBuffer){
            pid_t returnPid = waitpid(pidNode->pid, &status, 0);
        }
    }*/
    else if(sig == 17){      //SIGCHLD case
        do {
            /* If we use waitpid() with the -1 option, we wait for any child
             * that is done - similar to wait().  If we use the WNOHANG option,
             * it will exit immediately if there are *no* other children ready
             * to be reaped.  This guarantees that we will catch all of the
             * children which are done, but not get stuck waiting for any
             * others. */
            pid = waitpid(-1, &status, WNOHANG);
            //printf("%d", pid);
            //printf("\n");

            //SIGINT case (cancel)
            if(status == SIGINT){
                if(INTyet > 0){
                    log_cmd_state(get(activeBuffer)->pid, LOG_ACTIVE, get(activeBuffer)->cmd, LOG_CANCEL_SIG);
                    get(activeBuffer)->state = 0;
                    activeFinished = 0;
                    get(activeBuffer)->pid = 0;
                    get(activeBuffer)->cmd = NULL;
                    INTyet--;
                    return;
                }
                pidNode = getPid(cancelledPid);
                log_cmd_state(cancelledPid, LOG_BACKGROUND, pidNode->cmd, LOG_CANCEL_SIG);
                pidNode->state = 0;
                pidNode->pid = 0;
                pidNode->cmd = NULL;
                return;
            }
            //SIGTSTP case (pause)
            else if(toPause > 0){
                if(TSTPyet > 0){
                    log_cmd_state(get(activeBuffer)->pid, LOG_ACTIVE, get(activeBuffer)->cmd, LOG_PAUSE); 
                    get(activeBuffer)->state = 2;
                    toPause--;
                    TSTPyet--;
                    return;
                }
                pidNode = getPid(pausedPid);
                pidNode->state = 2;     //2 is paused state
                log_cmd_state(pausedPid, LOG_BACKGROUND, pidNode->cmd, LOG_PAUSE); 
                toPause--;
                return;
            }
            //SIGCONT case (resume)
            else if(toResume > 0){
                pidNode = getPid(resumePid);
                pidNode->state = 1;     //1 is working state
                log_cmd_state(resumePid, LOG_BACKGROUND, pidNode->cmd, LOG_RESUME);
                if(pidNode->id == activeBuffer){
                    pid_t returnPid = waitpid(pidNode->pid, &status, 0);
                    if(WIFEXITED(status)){      //if command is successful
                        log_cmd_state(returnPid, LOG_ACTIVE, pidNode->cmd, LOG_CANCEL);
                        pidNode->state = 0;
                        pidNode->pid = 0;
                        pidNode->cmd = NULL;
                    }
                }
                toResume--;
                return;
            }
            else if(status == 0 && activeFinished != 1 && toPause == 0 && toResume == 0) {
                pidNode = getPid(pid);
                if(pidNode == NULL){
                    return;
                }
                log_cmd_state(pid, LOG_BACKGROUND, pidNode->cmd, LOG_CANCEL);
                pidNode->state = 0;
                pidNode->pid = 0;
                pidNode->cmd = NULL;
            }
        } while (pid > 0);
    }
}

/* The entry of your text processor program */
int main() {
    
    // Initialize a sigaction struct
    struct sigaction sact;
    memset(&sact, 0, sizeof(sact));

    // Select the signal handler to use
    sact.sa_handler = sig_handler;

    // Install the sact signal action to handle various signals
    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGCHLD, &sact, NULL);
    sigaction(SIGTSTP, &sact, NULL);
    
    //----------------------------CODE GIVEN---------------------------------------------------------
    char cmdline[MAXLINE];        /* Command line */
    char *cmd = NULL;

    /* Intial Prompt and Welcome */
    log_help();

    /* Shell looping here to accept user command and execute */
    while (1) {
        char *argv[MAXARGS];        /* Argument list */
        Instruction inst;           /* Instruction structure: check parse.h */

        /* Print prompt */
        log_prompt();

        /* Read a line */
        // note: fgets will keep the ending '\n'
        if (fgets(cmdline, MAXLINE, stdin) == NULL) {
            if (errno == EINTR) {
                continue;
            }
            exit(-1);
        }

        if (feof(stdin)) {  /* ctrl-d will exit text processor */
          exit(0);
        }

        /* Parse command line */
        if (strlen(cmdline)==1)   /* empty cmd line will be ignored */
          continue;     

        cmdline[strlen(cmdline) - 1] = '\0';        /* remove trailing '\n' */

        cmd = malloc(strlen(cmdline) + 1);
        snprintf(cmd, strlen(cmdline) + 1, "%s", cmdline);

        /* Bail if command is only whitespace */
        if(!is_whitespace(cmd)) {
            initialize_command(&inst, argv);    /* initialize arg lists and instruction */
            parse(cmd, &inst, argv);            /* call provided parse() */

            if (DEBUG) {  /* display parse result, redefine DEBUG to turn it off */
                debug_print_parse(cmd, &inst, argv, "main (after parse)");
	    }

            /* After parsing: your code to continue from here */
            /*================================================*/

            //help call - 0 args
            if(strcmp(inst.instruct, "help") == 0){
                log_help();
            }
            //quit call - 0 args
            else if(strcmp(inst.instruct, "quit") == 0){
                log_quit();
                exit(0);
            }
            //new call - 0 args or 1 args (for upgraded new)
            else if(strcmp(inst.instruct, "new") == 0){
                if(argv[0] != NULL){
                    tpNewV2(inst.instruct, argv);
                }
                else{
                    tpNew(NULL);
                }
            }
            //list call - 0 args
	        else if(strcmp(inst.instruct, "list") == 0){
                tpList();
            }
            //active call - 1 args
            else if(strcmp(inst.instruct, "active") == 0){
                tpActive(inst.id);  //inst.id = 0 if no arg is given
            }
            //close call - 1 args
            else if(strcmp(inst.instruct, "close") == 0){
                tpClose(inst.id);   //inst.id = 0 if no arg is given
            }
            //open command - 1 arg
            else if(strcmp(inst.instruct, "open") == 0){
                tpOpen(inst.file);
            }
            //write command - 2 args (inst.id == 0 for 1 args)
            else if(strcmp(inst.instruct, "write") == 0){
                tpWrite(inst.id, inst.file);
            }
            //print command - 1 arg
            else if(strcmp(inst.instruct, "print") == 0){
                tpPrint(inst.id);
            }
            //exec command - 3 args
            else if(strcmp(inst.instruct, "exec") == 0){
                tpExec(inst.instruct, inst.id, argv);
            }
            //cancel command - 1 arg
            else if(strcmp(inst.instruct, "cancel") == 0){
                tpCancel(inst.id);
            }
            //pause command - 1 arg
            else if(strcmp(inst.instruct, "pause") == 0){
                tpPause(inst.id);
            }
            //resume command - 1 arg
            else if(strcmp(inst.instruct, "resume") == 0){
                tpResume(inst.id);
            }
        }

        free_command(&inst, argv);
    }
    return 0;
}

