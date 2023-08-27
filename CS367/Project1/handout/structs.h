/* Do Not Modify this File */

/* Definitions for Structs for this Project */
#ifndef STRUCTS_H
#define STRUCTS_H


/* Process Struct Definition */
typedef struct process_struct {
  char *command;      /* Process Command */
  int pid;            /* Process ID (unique) */
  int flags;          /* Process Flags */
  int time_remaining; /* Time Units Left to Execute */
  int time_last_run;  /* Last Time Executed (or time created) */
  struct process_struct *next; 
} process_t;

/* Queue Struct Definition */
typedef struct list_struct {
  process_t *head; /* Singly Linked Queue */
  int count;     /* Number of items in list */
} queue_t;

/* Header Struct Definition */
typedef struct header_struct {
  queue_t *ready_queue;   /* Ready Processes */
  queue_t *stopped_queue; /* Stopped Processes */
  queue_t *defunct_queue; /* Defunct Processes */
} header_t;

#endif /* STRUCTS_H */
