/**
 * @author Ryan Scherbarth
 * cs241
 * 3/30/23
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{ // represents one node in a Linked List

  void *data;        // pointer to data associated with node
  struct node *next; // pointer to next node in List
  struct node *prev; // pointer to previous node in List

} Node;

typedef struct
{ // represents a Linked List

  Node *header; // pointer to the "dummy header node" of
                // the Linked List
  int size;     // number of nodes in the Linked List

} LinkedList;

// function proto-types
void createList(LinkedList *someList);
void addEnd(LinkedList *someList, void *newElement);
void *delete(LinkedList *someList, int position);
void outputList(LinkedList *someList);
void addFirst(LinkedList *someList, void *newElement);
void *removeLast(LinkedList *someList);
void clear(LinkedList *someList);
void *set(LinkedList *someList, int position, void *newElement);
Node* findLast(LinkedList *someList);  // Helper proto-type

#define MAX_NAME_LENGTH 20

int main()
{

  LinkedList myList;

  LinkedList *roster = &myList;

  createList(roster); // initialize the fields of the list

	/*
 *	redirect through stdin to take input and output
 *	./a.out < input > output1
  */

	char line[MAX_NAME_LENGTH];
	char operation;
	char *input;
  int num;


  while(fgets(line, MAX_NAME_LENGTH, stdin) != NULL){
/*
 * stores the variables we will use for the doing things
 * Operation takes the first char of the line
 * input will store the string that follows the line
 *
 * -- If the operation needs an int as an input, it will
 *  be cast to an int in the swtich statement
 */
		char *input = malloc(MAX_NAME_LENGTH);

    sscanf(line, "%c", &operation);   // Store variables

    switch(operation){
      case 'a':
        sscanf(line, "%c %s", &operation, input);
        addEnd(roster, input);  // Append
        break;
      case 'd':
        sscanf(line, "%c %s", &operation, input);
        delete(roster, atoi(input));   // Delete at index input
        break;
      case 'o':
        outputList(roster);   // Output the list
        break;
      case 'f':
        sscanf(line, "%c %s", &operation, input);
        addFirst(roster, input);  // Add to beginning of list
        break;
      case 'r':
        removeLast(roster);   // Remove last item in list
        break;
      case 'c':
        clear(roster);  // Clear the list
        break;
      case 's':
        sscanf(line, "%c %d %s", &operation, &num, input);
        set(roster, num, input);
        break;
      default:
        exit(2);  // If the first char is invalid, exit
    }
  }
  return 0;
}

// Initially the List is empty
// We must create memory for the "dummy header node"
void createList(LinkedList *someList)
{
  someList->size = 0;

  someList->header = malloc(sizeof(Node));

  someList->header->data = NULL;
  someList->header->next = someList->header;
  someList->header->prev = someList->header;
}

// add the new data element to the end of the List
void addEnd(LinkedList *someList, void *newData)
{
  Node *newNode = malloc(sizeof(Node));

  newNode->data = newData; // set the fields of the new Node
  newNode->next = someList->header;
  newNode->prev = someList->header->prev;

  someList->header->prev->next = newNode; // splice-in the newNode
  someList->header->prev = newNode;       // into the List

  someList->size++;
}

// remove the item in the given positionn in the List, and
// return that data
void *delete(LinkedList *someList, int position)
{
  if (position < 0 || position >= someList->size)
    exit(2);

  // walk down the list until we reach the node to be removed
  Node *temp = someList->header;
  int i;
	for (i = 0; i <= position; i++)
    temp = temp->next;

  void *removedData = temp->data;

  temp->prev->next = temp->next; // splice-out the Node
  temp->next->prev = temp->prev;

  someList->size--;

  free(temp); // free-up the memory of the deleted Node

  return (removedData);
}

// output the contents of the List
// it is assumed that the elements in the list are strings
void outputList(LinkedList *someList)
{
  if (someList == NULL)
    return;

	if(someList->size == 0)
  {
    printf("[]\n");
		return;
	}

  printf("[");

  Node *temp = someList->header->next;

	int num;
  for (num = 0; num < someList->size; num++)
  {
    printf("%s%s", (char *)temp->data,
									 (num < someList->size - 1) ? " " : "");
    temp = temp->next;
  }
  printf("]\n");
}

// add the newElement to position 0 in the List
// i.e., the newElement precedes all other elements in the list
void addFirst(LinkedList *someList, void *newElement)
{
  Node *newNode = (Node*)malloc(sizeof(Node));

  newNode->data = newElement;   // Data
  newNode->next = someList->header->next;   // Next
  newNode->prev = someList->header;   // Previous
	someList->header->next->prev = newNode;
	someList->header->next = newNode;
	someList->size ++;
}

// remove the last element in the list, and return a pointer
// to that data item.
// if the list is empty, exit the program with status 2
void *removeLast(LinkedList *someList)
{
  Node *current = someList->header->prev;
  current->prev->next = someList->header;   // Final item should point to head
  current->prev = NULL;   // reset
  current->next = NULL;   // reset
  free(current->data);  // free (dynamically allocated)
  someList->size --;
  return 0;
}

// this function effectively empties the list
void clear(LinkedList *someList)
{
	// already empty
	if(someList == NULL){
		return;
	}

  Node *current = someList->header;
  while(current != NULL){
		Node *next = current->next; 	// create next
		// free(current->data); 	//
		// free(current); 	// free current

    current->prev = NULL;
    free(current->data);
    current->next = NULL;

		current = next; 	// move to next
	}
  someList->header->next = someList->header;
	someList->header->prev = someList->header;
	someList->size = 0;
}

// alter the data in the given position in the List to
// be the newElement.  Return a pointer to the data that
// was over-written and deleted from the list
// if the position is illegal t
// then exit the program with status 2
void *set(LinkedList *someList, int position, void *newElement)
{

  Node *current = someList->header->next;
  int i;
  for(i=0; i<position; i++){
    current = current->next;
  }
  // we now have the correct current

  // now just change the data
  current->data = newElement;
  return 0;
}

// Helper method, loops through and returns the Node which is in the last position
Node* findLast(LinkedList *someList){
  Node *iter = someList->header;
  while(iter != NULL && iter->next != NULL){
    iter = iter->next;
  }
  return iter;
}
