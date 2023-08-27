/* Definitions for Structs for this Recitation */
#ifndef R1_H
#define R1_H


/* Node Struct Definition */
typedef struct node {
  int value;          /* Value stored in node */
  struct node *next;  /* Pointer connecting to the next node in the list */
} Node;

/* List Struct Definition */
typedef struct list_struct {
  Node *head;    /* First node in signly-linked list */
  int num;       /* Number of nodes in list */
} List;

#endif /* R1_H */
