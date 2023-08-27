/* Do Not Modify this File */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "srtf.h"
#include "clock.h"
#include "structs.h"
#include "constants.h"

/* Local Globals */
typedef struct pid_tracker_struct {
  int pid;
  int exit_code;
  struct pid_tracker_struct *next;
} Pid_Tracker;

static FILE *fp_trace = NULL;
static Pid_Tracker *pid_tracker = NULL;

/* Local Functions */
static void print_queue(process_t *list);
static int parse_kill(header_t *header, char *trace_buffer);
static int parse_reap(header_t *header, char *trace_buffer);
static int valid_header(header_t *header);
static int add(int pid, int code);
static int get_code(int pid);
static process_t *parse_command(char *trace_buffer, header_t *header);
static void dump_header(header_t *header, process_t *p);
static void dump_queue(FILE *fp, char *command, queue_t *list);
static void sys_teardown();
static void free_trackers();
static void load_flags(char *ary, int flags);

/* Initialization Steps for the OS process_t Simulator */
void sys_init(FILE *fp) {
  int ret = atexit(sys_teardown);
  if(ret != 0) {
    printf("Error registering sys_teardown.  Exiting.\n");
    exit(EXIT_FAILURE);
  }
  fp_trace = fp;
}

/* At Exit: Close the Trace File */
static void sys_teardown() {
  if(fp_trace != NULL) {
    fclose(fp_trace);
  }
}

/* Main Run Loop for the OS process_t Simulator
 * Stages of the Simulator:
 *  0) *runs once*
 *  --- Calls header_tr's Init Function to create the header_t struct
 *  1) Unload the Running process_t from CPU
 *  --- Call header_tr's Add Function on process_t
 *  2) Perform 1 action from the trace file.
 *  --- If Create process_t:
 *  ------- A) Call header_tr's Generate Function with process_t Data
 *  ------- B) Call header_tr's Add Function on process_t
 *  --- If kill -STOP: 
 *  ------- Call header_tr's Stop Function on PID
 *  --- If kill -CONT: 
 *  ------- Call header_tr's Continue Function on PID
 *  --- If reap: 
 *  ------- Call header_tr's Reap Function on PID
 *  --- If exit:
 *  ------- Exits after Advance Time stage.
 *  3) Get New process_t to Run
 *  --- Call header_tr's Select function to get process_t
 *  --- Adds process_t to the CPU Running Set (if one selected)
 *  4) Advance Time
 *  --- Advance the Clock by 1
 *  --- Decrement time remaining on Running process_t
 *  --- If time remaining is 0
 *  ------- Add exit code to flags
 *  --- If exit was selected:
 *  ------- Writes header_t to a File
 *  ------- Exits the program
 */
void sys_run() {
  header_t *header = NULL;
  process_t *on_cpu = NULL;
  process_t *new_process = NULL;
  /* Manage the Trace file Reading */
  char trace_buffer[512]; 
  int trace_len = 512;
  int ret_code = 0;
  int do_exit = 0; /* Set by Trace function's exit command */

/* Stage 0) Calls header_tr's Init Function to create the Data  */
  header = srtf_init();
  if(!valid_header(header)) {
    printf("Error: Failed to create header properly.\n");
    exit(EXIT_FAILURE);
  }

  /* Initialize the Master OS Clock */
  clock_init(1);

  /* Run the System until Time to End */
  while(!do_exit) {
    printf(".=============\n");
    printf("| Starting Time: %2d\n", clock_get_time());
    printf("+------------\n");
/* Stage 1) Unload the Running process_t from CPU and Return to header_tr */
    if(on_cpu != NULL) {
      /* Call header_tr's Add Function on process_t */
      printf("| Returning Process (PID: %d) to SRTF\n", on_cpu->pid);
      print_queue(on_cpu);
      printf("+------------\n");
      srtf_add(header, on_cpu);
      on_cpu = NULL;
    }
    else {
      printf("| Nothing to Return to Your Queues\n");
      printf("+------------\n");
    }
/* Stage 2) Perform 1 action from the trace file  */
    ret_code = (fgets(trace_buffer, trace_len, fp_trace) != NULL);

    /* If exit (or EOF): Exits after Advance Time stage. */
    if(ret_code <= 0 || strstr(trace_buffer, "exit") != NULL) {
      printf("| Action for Time %2d: Exit\n", clock_get_time());
      do_exit = 1;
      ret_code = 0;
    }
    /* If kill... */
    else if(strstr(trace_buffer, "kill") != NULL) {
      printf("| Action for Time %2d: %s", clock_get_time(), trace_buffer);
      ret_code = parse_kill(header, trace_buffer);
    }
    /* If reap... */
    else if(strstr(trace_buffer, "reap") != NULL) {
      printf("| Action for Time %2d: %s", clock_get_time(), trace_buffer);
      ret_code = parse_reap(header, trace_buffer);
    }
    /* If pass...*/
    else if(strstr(trace_buffer, "pass") != NULL) {
      printf("| Action for Time %2d: Pass (No Action)\n", clock_get_time());
      ret_code = 0;
    }
    /* If Create process_t:
     *   Call header_tr's Generate Function with process_t Data
     *   Call header_tr's Insert Function on process_t
     */
    else {
      printf("| Action for Time %2d: %s", clock_get_time(), trace_buffer);
      new_process = parse_command(trace_buffer, header);

      if(new_process != NULL) {
        printf("| New Process is Starting\n");
        print_queue(new_process);
        ret_code = srtf_add(header, new_process);
      }
    }

    /* An action has been taken. Did it work? */
    if(ret_code != 0) {
      printf("Error: Failed to Parse the next Trace Action\n");
      exit(EXIT_FAILURE);
    }
      
    printf("+------------\n");
    printf("| Ready Queue:   %2d processes\n", srtf_count(header->ready_queue));
    print_queue(header->ready_queue->head);
    printf("| Stopped Queue: %2d processes\n", srtf_count(header->stopped_queue));
    print_queue(header->stopped_queue->head);
    printf("| Defunct Queue: %2d processes\n", srtf_count(header->defunct_queue));
    print_queue(header->defunct_queue->head);
    
/* Step 3) Select the Next process_t to Run */
    on_cpu = srtf_schedule(header);
    if(on_cpu != NULL) {
      on_cpu->next = NULL; /* Clean up the Remove */
    }

    printf("+------------\n");
    printf("| Selecting to Run on the CPU\n");
    print_queue(on_cpu);
    printf("\\=============\n\n");

/* Step 4) Advance Time (if not exiting) */
    if(do_exit == 0) {
      if(on_cpu != NULL) {
        on_cpu->time_last_run = clock_get_time();
        on_cpu->time_remaining--;
        if(on_cpu->time_remaining <= 0) {
          on_cpu->time_remaining = 0;
          on_cpu->flags &= 0x3F; // Zeroize upper bits (exit code)
          on_cpu->flags |= (get_code(on_cpu->pid) << 6);
        }
      }
      clock_advance_time();
    }
  }

  dump_header(header, on_cpu);
  if(on_cpu != NULL) {
    free(on_cpu->command);
    free(on_cpu);
  }
  srtf_free(header);
  free_trackers();

  printf("We're done!\n");
  return;
}

/* Prints a queue_t
 */
static void print_queue(process_t *list) {
  process_t *walker = NULL;
  char flags[9] = {0};
  if(list == NULL) {
    printf("|\tNone\n");
  } else {
    for(walker = list; walker != NULL; walker = walker->next) {
      load_flags(flags, walker->flags);
      printf("|\tPID: %3d Time Remaining: %2d (Last Run: %2d) Flags:%s Command: %s\n", 
             walker->pid, walker->time_remaining, walker->time_last_run,
             flags, walker->command);
    }
  }
  return;
}

/* If kill:
 *   If kill -STOP: 
 *     Call header_tr's Stop Function on PID
 *   If kill -CONT: 
 *     Call header_tr's Continue Function on PID
 */
static int parse_kill(header_t *header, char *trace_buffer) {
  if(!valid_header(header) || trace_buffer == NULL) {
    return -1;
  }

  int ret_code = 0;
  int pid = 0;
  int do_stop = 0; /* Set by Trace function's kill command */
  int do_cont = 0; /* Set by Trace function's kill command */
  char *p_buf = NULL;

  p_buf = strtok(trace_buffer, " -"); /* Pop the kill - out of there */
  p_buf = strtok(NULL, " "); /* Position p_buf to SIG Code */
  if(strstr(p_buf, "STOP") != NULL) {
    do_stop = 1;
  }
  else if(strstr(p_buf, "CONT") != NULL) {
    do_cont = 1;
  }
  else {
    printf("Error: Cannot parse the Trace File.  kill misformatted\n");
    printf("Exiting.\n");
    exit(EXIT_FAILURE);
  }

  /* Now get the PID */
  p_buf = strtok(NULL, " "); /* Position p_buf to SIG Code */
  ret_code = sscanf(p_buf, "%d", &pid);
  if(ret_code != 1) {
    printf("Error: Cannot parse the Trace File.  kill misformatted\n");
    printf("Exiting.\n");
    exit(EXIT_FAILURE);
  }
  if(do_stop) {
    printf("| Sending STOP to process_t (PID: %2d)\n", pid);
    ret_code = srtf_stop(header, pid);
  }
  else if(do_cont) {
    printf("| Sending CONT to process_t (PID: %2d)\n", pid);
    ret_code = srtf_continue(header, pid);
  }
  else {
    ret_code = -1;
  }
  return ret_code;
}

/* If reap:
 *   Call header_tr's Reap Function on PID
 */
static int parse_reap(header_t *header, char *trace_buffer) {
  if(!valid_header(header) || trace_buffer == NULL) {
    return -1;
  }

  int exit_code = 0;
  int pid = 0;
  int ret = 0;
  char *p_buf = NULL;

  p_buf = strtok(trace_buffer, " "); /* Position p_buf to SIG Code */
  p_buf = strtok(NULL, " "); /* Position p_buf to PID */
  ret = sscanf(p_buf, "%d", &pid);
  if(ret != 1) {
    printf("Error: Cannot parse the Trace File.  kill misformatted\n");
    printf("Exiting.\n");
    exit(EXIT_FAILURE);
  }
  exit_code = srtf_reap(header, pid);
  printf("| Reaping process (PID: %2d). Exited with code: %d\n", pid, exit_code);

  return 0;
}

/* Looking for [sudo] commmand [args] \[pid,runtime,exitcode\]
 * Returns the process created or NULL on error
 */
static process_t *parse_command(char *trace_buffer, header_t *header) {
  int is_sudo = 0;
  int pid = 0;
  int run_time = 0;
  int exit_code = 0;
  int count = 0;
  char *p_buf = NULL;
  char *command = NULL;
  process_t *new_process = NULL;

  if(!valid_header(header) || trace_buffer == NULL) {
    return NULL;
  }

  if(strstr(trace_buffer, "sudo") != NULL) {
    is_sudo = 1;
    p_buf = strtok(trace_buffer, " "); /* Pops the sudo out */
    command = strtok(NULL, "[");
  }
  else {
    command = strtok(trace_buffer, "["); /* Pops out everything ahead of '[' */
  }
  p_buf = strtok(NULL, "]"); /* Pop the vals between [ ] */
  count = sscanf(p_buf, "%d,%d,%d", &pid, &run_time, &exit_code);
  if(count != 3) {
    printf("Error: Could not parse the program to run in the trace file\n");
    exit(EXIT_FAILURE);
  }

  if(add(pid, exit_code) != 0) {
    printf("Error: Internal Tracker Error\n");
    printf("If you get this error, it's Prof. Andrea's bug, not yours.\n");
    printf("Please let him know.\n");
    exit(EXIT_FAILURE);
  }

  new_process = srtf_generate(command, pid, run_time, is_sudo);
  return new_process;
}

/* Local function to add pid:exitcode pairs to a list */
static int add(int pid, int code) {
  Pid_Tracker *entry = malloc(sizeof(Pid_Tracker));
  memset(entry, '\0', sizeof(Pid_Tracker));
  if(entry == NULL) {
    return -1;
  }
  entry->pid = pid;
  entry->exit_code = code;

  if(pid_tracker == NULL) {
    pid_tracker = entry;
  }
  else {
    entry->next = pid_tracker;
    pid_tracker = entry;
  }
  return 0;
}

static void free_trackers() {
  Pid_Tracker *walker = pid_tracker;
  Pid_Tracker *reaper = NULL;
  while(walker) {
    reaper = walker;
    walker = walker->next;
    free(reaper);
  }
}
  
/* Local function to get pid:exitcode pairs from a list */
static int get_code(int pid) {
  Pid_Tracker *head = pid_tracker;
  Pid_Tracker *walker = NULL;
  Pid_Tracker *reaper = NULL;
  int code = -1;
  if(head == NULL) {
    return -1;
  }
  
  if(head->pid == pid) {
    code = head->exit_code;
    reaper = head;
    pid_tracker = head->next;
    reaper->next = NULL;
    free(reaper);
  }
  else {
    for(walker = head; walker->next && walker->next->pid != pid; walker = walker->next)
      ;
    if(walker->next != NULL) {
      code = walker->next->exit_code;
      reaper = walker->next;
      walker->next = reaper->next;
      reaper->next = NULL;
      free(reaper);
    }
  }

  return code;
}

/* Local function to verify that the header is valid */
static int valid_header(header_t *header) {
  if(header == NULL 
      || header->ready_queue == NULL 
      || header->stopped_queue == NULL
      || header->defunct_queue == NULL) {
    return 0;
  }
  return 1;
}

/* Dumps the header_t to File */
static void dump_header(header_t *header, process_t *p) {
  return;
  FILE *fp_output = fopen("out.dat", "w");

  if(!valid_header(header)) {
    fclose(fp_output);
    return;
  }
  
  fprintf(fp_output,   "End of Time %d\n", clock_get_time());
  dump_queue(fp_output, "Ready Queue:   ", header->ready_queue);
  dump_queue(fp_output, "Stopped Queue: ", header->stopped_queue);
  dump_queue(fp_output, "Defunct Queue: ", header->defunct_queue);
  fprintf(fp_output,   "On the CPU:   \n");
  if(p != NULL) {
    fprintf(fp_output, "| %d,0x%x,%d,%d,%s\n", p->pid, p->flags, p->time_remaining, p->time_last_run, p->command);
  }

  fclose(fp_output);
}

/* Dumps a single list to file */
static void dump_queue(FILE *fp, char *command, queue_t *ll) {
  fprintf(fp, "%s\n", command);
  process_t *p = ll->head;
  int i = 0;

  while(p != NULL) {
    for(i = strlen(p->command)-1; isspace(p->command[i]); i--) {
      p->command[i] = '\0';
    }
    fprintf(fp, "| %d,0x%x,%d,%d,%s\n", p->pid, p->flags, p->time_remaining, p->time_last_run, p->command);
    p = p->next;
  }
}

static void load_flags(char *ary, int flags) {
  sprintf(ary, "[%c%c%c%c%c%c]",
      flags & STATE_CREATED    ?'C':' ',
      flags & STATE_READY      ?'R':' ',
      flags & STATE_STOPPED    ?'T':' ',
      flags & STATE_DEFUNCT    ?'Z':' ',
      flags & STATE_TERMINATED ?'X':' ',
      flags & PF_SUPERPRIV     ?'S':' ');
  return;
}
