/* Fill in your Name and GNumber in the following two comment fields
 * Name:Isaac Kim
 * GNumber:01201648
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "r1.h"

/* Create (allocate and initialize) and return a new Node,
 * using the provided initial value.
 */
Node *new_node(int value) {
	Node *node = malloc(sizeof(struct node));
	if (!node) { return NULL; }
	node->value = value;
	node->next = NULL;
	return node;
}

/* Iterate the nodes in the list to find out whether or not
 * the given input value is contained within the list.
 * If found, return the first matching node found.
 * If not, then return NULL.
 */
Node *find_node(List* list, int value) {
	Node *current = list->head;
	while(current != NULL){
		if(current->value == value){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/* Insert a new node at the front of the linked list for integer num
 * Remeber to update the node count
 * The list might have zero node to start with
 */
void insert_at_front(List *list, int value){
	Node *newNode = new_node(value);
	newNode->next = list->head;
	list->head = newNode;
	list->num++;
}

/* =============================================
 * Optional clear_list() function
 *
 * Deallocate the storage pointed by list 
 * =============================================
 */
void clear_list(List *list){
	Node *current = list->head;
	Node *next;

	while(current != NULL){
		next = current->next;
		free(current);
		current = next;
	}
	list->head = NULL;
	free(list);
}


/* Return only the bit at index from integer value
 * The return value should be either 1 or 0
 * Assume the index of LSB (rightmost) is 0
 * Assume the index of MSB (leftmost) is 31
 * Assume the index is always valid (no need to check)
 */
int get_bit(int value, int index) {
	int bit = (value >> index) & 1;

	return bit;
}

/* Toggle the bit at the index of the integer value
 * (if it's 1, make it 0, and if it's 0, make it 1).
 * Return the new value of integer after toggling the bit.
 * Assume the index of LSB (rightmost bit) is 0
 * Assume the index of MSB (leftmost bit) is 31
 * Assume the index is always valid (no need to check)
 */
int flip_bit(int value, int index) {
	int flipBit = (value ^ (1 << (index - 1)));


	return flipBit;
}

/* Helper: print all bits in an integer, using get_bit().
 */
void print_bits(int x) {
  for (int i=31; i>=0; i--){
    //printf("integer: %08X, index:%d, bit:%d\n", x, i, get_bit(x,i));
    printf("%d", get_bit(x,i));
    if (i%4==0) printf(" ");
  }
}

/* =============================================
 * Provided main() function
 * =============================================
 */
int main(int argc, char *argv[]) {

  //initialize an empty list of zero nodes
  List *num_list = malloc(sizeof(List));
  num_list->head = NULL;
  num_list->num = 0;
  
  //build the list
  printf("============Linked list building==========\n"); 
  for (int i=0; i<10; i++){
    int v = i * 5; 
    //feel free to change the numbers used here, 
    //e.g. int n = rand()%200
    insert_at_front(num_list, v);
  }
  printf("Number of elements in list: %d\n", num_list->num);

  //check the list contents
  printf("============Linked list checking==========\n"); 
  for (int i=0; i<10; i++){
    int v = i * 5; 
    Node *check = find_node(num_list, v);
    if (check && check->value == v) {
	    printf("List contains %d\n", v);
    } else {
	    printf("Error: %d missing from list\n", v);
    }
  }

  // optional: deallocate the list
  clear_list(num_list);

  //check get_bit()
  printf("============get_bit() checking==========\n"); 

  int x = 0xABCD1234;
  printf("integer: %08X\n", x);
  print_bits(x); // use the provided helper and get_bit() to print x
  printf("\n");
 
  //check toggle_bit()
  printf("============toggle_bit() checking==========\n"); 

  for (int i=31; i>=0; i-=4){
    printf("integer: %08X (", x);
    print_bits(x);
    printf("), toggle bit index %d\n", i);
    printf("            ------>");
    print_bits(flip_bit(x,i));
    printf("\n");
  }

  return 0;
}

