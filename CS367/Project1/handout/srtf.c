/* Fill in your Name and GNumber in the following two comment fields
 * Name: Isaac Kim
 * GNumber: 01201648
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"
#include "srtf.h"

/* Feel free to create any helper functions you like! */
int isStarving(process_t *proc);

/* Initialize the header_t Struct
 * Follow the specification for this function.
 * Returns a pointer to the new header_t or NULL on any error.
 */
header_t *srtf_init() {
	header_t *newHeaderT = malloc(sizeof(header_t));

	//check for errors
	if(newHeaderT == NULL){
		return NULL;
	}

	//allocate ready queue, header, count, and pointer for ready q
	queue_t *newReadyQueue = malloc(sizeof(queue_t));
	newReadyQueue->head = NULL;
	newReadyQueue->count = 0;
	newHeaderT->ready_queue = newReadyQueue;

	//allocate stopped queue, ""
	queue_t *newStoppedQueue = malloc(sizeof(queue_t));
	newStoppedQueue->head = NULL;
	newStoppedQueue->count = 0;
	newHeaderT->stopped_queue = newStoppedQueue;

	//allocate defunct queue,""
	queue_t *newDefunctQueue = malloc(sizeof(queue_t));
	newDefunctQueue->head = NULL;
	newDefunctQueue->count = 0;
	newHeaderT->defunct_queue = newDefunctQueue;

	//check for errors
	if(newHeaderT == NULL){
		return NULL;
	}

  return newHeaderT; // Replace Me with Your Code!
}

/* Add the process into the appropriate linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_add(header_t *header, process_t *process) {
	//if header is null, return error
	if(header == NULL){
		return -1;
	}
	//if process is NULL, return error
	if(process == NULL){
		return -1;
	}
	//------------------------------------------------------------------------------------------//
	//for state created, move to ready state
	//setting proper flags for sudo and non sudo                  CREATED -> READY QUEUE
	if(process->flags == STATE_CREATED || process->flags == (STATE_CREATED | PF_SUPERPRIV)){
		if(process->flags == STATE_CREATED){
			process->flags = STATE_READY;
		}
		//for superpriv flag
		else if(process->flags == (STATE_CREATED | PF_SUPERPRIV)){
			process->flags = (STATE_READY | PF_SUPERPRIV);
		}
		//adding to ready queue
		//if nothing is in ready queue
		if(header->ready_queue->head == NULL){
			header->ready_queue->head = process;
			header->ready_queue->count++;
		}
		//if pid of process is smaller, make it the head
		else if(process->pid < header->ready_queue->head->pid){
			process->next = header->ready_queue->head;
			header->ready_queue->head = process;
			header->ready_queue->count++;
		}
		//else, loop through queue to insert in ascending order
		else{
			process_t *current = header->ready_queue->head;
			process_t *currentPrev = NULL;

			while(current->next != NULL && current->next->pid < process->pid){
				currentPrev = current;
				current = current->next;
			}
			//when we reach here, the next pid is greater, so insert here

			//if currentprev is null, then there is only one in the queue 
			//(we have to put it after the head)
			if(currentPrev == NULL){
				current->next = process;
				process->next = NULL;
			}
			else{
				process->next = current;
				currentPrev->next = process;
			}
			//update count
			header->ready_queue->count++;
		}
		return 0;
	}
	//--------------------------------------------------------------------------------//

	//for state ready
	//seeing proper flags                      READY -> READY QUEUE
	else if(process->flags == STATE_READY || process->flags == (STATE_READY | PF_SUPERPRIV)){
		//if remaining time is not 0
		if(process->time_remaining > 0){
			//if nothing is in ready queue
			if(header->ready_queue->head == NULL){
				header->ready_queue->head = process;
				//update count
				header->ready_queue->count++;
			}
			//if pid of process is smaller, make it the head
			else if(process->pid < header->ready_queue->head->pid){
				process->next = header->ready_queue->head;
				header->ready_queue->head = process;
				//update count
				header->ready_queue->count++;
			}
			//else, loop through queue to insert in ascending order
			else{
				process_t *current = header->ready_queue->head;
				process_t *currentPrev = NULL;

				while(current->next != NULL && current->next->pid < process->pid){
					currentPrev = current;
					current = current->next;
				}
				//when we reach here, the next pid is greater, so insert here
				//if currentprev is null, then there is only one in the queue 
				//(we have to put it after the head)
				if(currentPrev == NULL){
					current->next = process;
					process->next = NULL;
				}
				else{
					process->next = current;
					currentPrev->next = process;
				}
				//update count
				header->ready_queue->count++;
			}
			return 0;
		}

		//if the remaining time is 0
		else if(process->time_remaining == 0){
			//setting proper flags for sudo and non sudo          READY    -> DEFUNCT QUEUE
			if(process->flags == STATE_READY){
				process->flags = STATE_DEFUNCT;
			}
			else if(process->flags == (STATE_READY | PF_SUPERPRIV)){
				process->flags = (STATE_DEFUNCT | PF_SUPERPRIV);
			}

			//adding to defunct queue
			//if nothing is in ready queue
			if(header->defunct_queue->head == NULL){
				header->defunct_queue->head = process;
				//update count
				header->defunct_queue->count++;
			}
			//if pid of process is smaller, make it the head
			else if(process->pid < header->defunct_queue->head->pid){
				process->next = header->defunct_queue->head;
				header->defunct_queue->head = process;
				//update count
				header->defunct_queue->count++;
			}
			//else, loop through queue to insert in ascending order
			else{
				process_t *current = header->defunct_queue->head;
				process_t *currentPrev = NULL;

				while(current->next != NULL && current->next->pid < process->pid){
					currentPrev = current;
					current = current->next;
				}
				//when we reach here, the next pid is greater, so insert here
				//if currentprev is null, then there is only one in the queue 
				//(we have to put it after the head)
				if(currentPrev == NULL){
					current->next = process;
					process->next = NULL;
				}
				else{
					process->next = current;
					currentPrev->next = process;
				}
				//update count
				header->ready_queue->count++;
			}
			return 0;
		}
	}
	//setting proper flags for sudo and non sudo                  Defunct -> Defunct QUEUE
	else{
		//adding to defunct queue
		//if nothing is in defunct queue
		process->flags = STATE_DEFUNCT;

		if(header->defunct_queue->head == NULL){
			header->defunct_queue->head = process;
			header->defunct_queue->count++;
		}
		//if pid of process is smaller, make it the head
		else if(process->pid < header->defunct_queue->head->pid){
			process->next = header->defunct_queue->head;
			header->defunct_queue->head = process;
			header->defunct_queue->count++;
		}
		//else, loop through queue to insert in ascending order
		else{
			process_t *current = header->defunct_queue->head;
			process_t *currentPrev = NULL;

			while(current->next != NULL && current->next->pid < process->pid){
				currentPrev = current;
				current = current->next;
			}
			//when we reach here, the next pid is greater, so insert here
			//if currentprev is null, then there is only one in the queue 
			//(we have to put it after the head)
			if(currentPrev == NULL){
				current->next = process;
				process->next = NULL;
			}
			else{
				process->next = current;
				currentPrev->next = process;
			}
			//update count
			header->ready_queue->count++;
		}
		return 0;
	}
	//if we somehow reach here without entering any of the conditionals, we have an error
  	return -1; 
}

/* Move the process with matching pid from Ready to Stopped.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_stop(header_t *header, int pid) {
	//if header is null, return error
	if(header == NULL){
		return -1;
	}
	//assign current
	process_t *current = header->ready_queue->head;
	process_t *currentPrev = NULL;

	int found = 0;
	int first = 0;

	//loop to search for right pid
	while(current != NULL){
		first++;
		//traverse until we find the right process with pid
		if(current->pid == pid){
			found++;
			break;
		}
		currentPrev = current;
		current = current->next;
	}
	//check if we found it in the ready queue or not
	if(found == 0){
		return -1;
	}
	//removal
	if(currentPrev == NULL){
		header->ready_queue->head = NULL;
	}
	else{
		currentPrev->next = current->next;
	}
	current->next = NULL;
	//update count
	header->ready_queue->count--;
	//set flag for current to stopped
	if(current->flags == (STATE_READY | PF_SUPERPRIV)){
		current->flags = (STATE_STOPPED | PF_SUPERPRIV);
	}
	else{
		current->flags = STATE_STOPPED;
	}

	//tracker for stopped queue
	process_t *tracker = header->stopped_queue->head;
	//inserting process:------------------------------------------------------//
	//if stopped queue is empty
	if(tracker == NULL){
		header->stopped_queue->head = current;
	}
	//if current is the least already and there are elements in the stopped queue
	else if(current->pid < tracker->pid){
		current->next = tracker;
		header->stopped_queue->head = current;
	}
	else{
		//traverse stopped queue, checking pids
		while(tracker->next != NULL && tracker->next->pid < current->pid){
			tracker = tracker->next;
		}
		//once we exit loop, we can insert
		current->next = tracker->next;
		tracker->next = current;
	}
	header->stopped_queue->count++;
  return 0; // Replace Me with Your Code!
}

/* Move the process with matching pid from Stopped to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_continue(header_t *header, int pid) {
	//if header is null, return error
	if(header == NULL){
		return -1;
	}
	//assign current
	process_t *current = header->stopped_queue->head;
	process_t *currentPrev = NULL;

	int first = 0;
	int found = 0;

	//loop to search for right pid
	while(current != NULL){
		first++;
		//traverse until we find the right process with pid
		if(current->pid == pid){
			found++;
			break;
		}
		currentPrev = current;
		current = current->next;
	}
	//if current is not in the stopped queue
	if(found == 0){
		return -1;
	}
	//removal
	if(currentPrev == NULL){
		header->stopped_queue->head = NULL;
	}
	else{
		currentPrev->next = current->next;
	}
	current->next = NULL;
	//update count
	header->stopped_queue->count--;
	//set flag for current to ready
	if(current->flags == (STATE_STOPPED | PF_SUPERPRIV)){
		current->flags = (STATE_READY | PF_SUPERPRIV);
	}
	else{
		current->flags = STATE_READY;
	}

	//tracker for stopped queue
	process_t *tracker = header->ready_queue->head;
	//inserting process:------------------------------------------------------//
	//if stopped queue is empty
	if(tracker == NULL){
		header->ready_queue->head = current;
	}
	//if current is the least already and there are elements in the stopped queue
	else if(current->pid < tracker->pid){
		current->next = tracker;
		header->ready_queue->head = current;
	}
	else{
		//traverse stopped queue, checking pids
		while(tracker->next != NULL && tracker->next->pid < current->pid){
			tracker = tracker->next;
		}
		//once we exit loop, we can insert
		current->next = tracker->next;
		tracker->next = current;
	}
	header->ready_queue->count++;
  return 0; // Replace Me with Your Code!
}

/* Remove the process with matching pid from Defunct.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int srtf_reap(header_t *header, int pid) {
	//if header is null, return error
	if(header == NULL){
		return -1;
	}
	//variable for current in traversing queue
	process_t *current = header->defunct_queue->head;
	process_t *currentPrev = current;

	int first = 0;
	int exitCode;
	int found = 0;

	while(current != NULL){
		first++;
		//traverse until we find the right process with pid
		if(current->pid == pid){
			found++;
			break;
		}
		currentPrev = current;
		current = current->next;
	}
	//if we get here and we haven't found it in the defunct queue, then we have an error
	if(found == 0){
		return -1;
	}
	//to see if match is head
	if(first == 1){
		header->defunct_queue->head = current->next;
		current->next = NULL;
	}
	else{
		//remove current from queue
		currentPrev->next = current->next;
		current->next = NULL;
	}
	if(current == NULL){
		return -1;
	}
	//update count
	header->defunct_queue->count--;
	//extract exitCode from process
	exitCode = current->flags;
	exitCode = exitCode >> 3;
	

	//free the memory
	free(current);

	return exitCode;
}

/* Create a new process_t with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the process_t on success or a NULL on any error.
 */
process_t *srtf_generate(char *command, int pid, int time_remaining, int is_sudo) {
	//allocate mem for new process
  	process_t *newProcessT = malloc(sizeof(process_t));

  	//check for errors 
  	if(newProcessT == NULL){
  		return NULL;
  	}

  	//assign command, pid and time remaining
  	newProcessT->command = malloc(sizeof(char));
  	strcpy(newProcessT->command, command);

  	newProcessT->pid = pid;
  	newProcessT->time_remaining = time_remaining;

  	//error checks
  	if(newProcessT->command == NULL){
  		return NULL;
  	}

  	//current time = last time ran
  	newProcessT->time_last_run = clock_get_time();
  	
  	//state is set to created
  	newProcessT->flags = STATE_CREATED;

  	//check for sudo 
  	if(is_sudo != 0){
  		newProcessT->flags = (STATE_CREATED | PF_SUPERPRIV);
  	}
  	
  	//check for errors 
  	if(newProcessT == NULL){
  		return NULL;
  	}

  	return newProcessT;
}
 
/* Schedule the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
process_t *srtf_schedule(header_t *header) {
	//if header is null, return error
	if(header == NULL){
		return NULL;
	}

	//create placeholders when we traverse queue
	process_t *current = header->ready_queue->head;
	process_t *currentPrev = current;
	process_t *lowestPrev = NULL;

	//variable for process with lowest time remaining
	process_t *lowest = NULL;

	//set variable for if there is a starving process
	int numOfStarving = 0;

	//traverse through ready queue to check for starving
	while(current != NULL){
		//check for starving
		if(isStarving(current) == 1){
			numOfStarving++;
		}
		current = current->next;
	}
	//------------------------------------------------------------------------------//
	//if we have 0 starving
	if(numOfStarving == 0){

		//reset current
		current = header->ready_queue->head;

		if(current == NULL){
			return NULL;
		}
		//traverse through ready queue to check for lowest time remaining
		//NOTE: queue must have 2 or more processes in it to run the loop
		while(current != NULL){
			//base case
			if(lowest == NULL){
				lowest = current;
				lowestPrev = NULL;
			}
			//if lowest is greater than current, then it is not the lowest
			else if(lowest->time_remaining > current->time_remaining){
				//change lowest to current
				lowest = current;
				lowestPrev = currentPrev;
			}
			//if they are equal to each other
			else if(lowest->time_remaining == current->time_remaining){
				//check pid's, pick lower one
				if(lowest->pid > current->pid){
					lowest = current;
					lowestPrev = currentPrev;
				}
			}
			//updating current vars
			currentPrev = current;
			current = current->next;
		}
		//if lowest is the only one OR the first one in the queue
		if(lowestPrev == NULL){
			header->ready_queue->head = lowest->next;
			lowest->next = NULL;
		}
		else{
			//remove from ready queue normally
			lowestPrev->next = lowest->next;
			lowest->next = NULL;
		}
		
		//update count
		header->ready_queue->count--;
	}
	//---------------------------------------------------------------------------------//
	//if we have ONE starving process
	else if(numOfStarving == 1){

		//reset current
		current = header->ready_queue->head;
		//traverse through ready queue to check for lowest time remaining
		//NOTE: queue must have 2 or more processes in it to run the loop
		while(current != NULL){
			//current is starving, assign that to lowest for removal since there is only ONE
			if(isStarving(current) == 1){
				lowest = current;
				lowestPrev = currentPrev;
				break;
			}
			//updating current vars
			currentPrev = current;
			current = current->next;
		}

		//remove from ready queue
		lowestPrev->next = lowest->next;
		lowest->next = NULL;
		//update count
		header->ready_queue->count--;
	}
	//--------------------------------------------------------------------------------//
	//if we have MORE than ONE starving processes
	else if(numOfStarving > 1){

		//reset current
		current = header->ready_queue->head;
		//traverse through ready queue to check for lowest time remaining
		//NOTE: queue must have 2 or more processes in it to run the loop
		while(current != NULL){
			//if current is starving
			if(isStarving(current) == 1){
				//if lowest is NULL (base case)
				if(lowest == NULL){
					//change lowest to current
					lowest = current;
					lowestPrev = currentPrev;
				}
				//else if lowest is starving, check pid's
				else if(lowest->pid > current->pid){
					lowest = current;
					lowestPrev = currentPrev;
				}	
			}
			//updating current vars
			currentPrev = current;
			current = current->next;
		}	

		//if lowest is the only one in the queue
		if(lowestPrev == NULL){
			header->ready_queue->head = lowest->next;
			lowest->next = NULL;
		}
		else{
			//remove from ready queue normally
			lowestPrev->next = lowest->next;
			lowest->next = NULL;
		}
		//update count
		header->ready_queue->count--;
	}
//----------------------------------------------------------------------------------------//
//lowest will be NULL if the code didnt work properly
  return lowest; 
}

/*helper method to check if a process is starving
 *returns true if it is starving, and false otherwise
 */
int isStarving(process_t *proc){
	if(clock_get_time() - proc->time_last_run >= TIME_STARVATION){
		return 1;
	}
	return 0;
}

/* Returns the number of items in a given queue_t
 * Follow the specification for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int srtf_count(queue_t *ll) {
	if(ll != NULL){
		return ll->count;
	}
  return -1; // Replace Me with Your Code!
}

/* Frees all allocated memory in the header_tr */
void srtf_free(header_t *header) {
	//current for loops
	process_t *currentReady = header->ready_queue->head;
	process_t *currentStopped = header->stopped_queue->head;
	process_t *currentDefunct = header->defunct_queue->head;

	process_t *temp = NULL;

	//freeing ready queue
	while(currentReady != NULL){
		temp = currentReady;
		currentReady = currentReady->next;
		free(temp->command);
		free(temp);
	}
	free(currentReady);
	free(header->ready_queue);
	//freeing stopped queue
	while(currentStopped != NULL){
		temp = currentStopped;
		currentStopped = currentStopped->next;
		free(temp->command);
		free(temp);
	}
	free(currentStopped);
	free(header->stopped_queue);
	//freeing defunct queue
	while(currentDefunct != NULL){
		temp = currentDefunct;
		currentDefunct = currentDefunct->next;
		free(temp->command);
		free(temp);
	}
	free(currentDefunct);
	free(header->defunct_queue);

	//free placeholders and the schedule itself
	free(header);
}
