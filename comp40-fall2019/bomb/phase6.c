#include <stdlib.h>
#include <stdio.h>


/* 
 * Node struct for our Linked List
 * Contains a long and a pointer to the next Node
 */
typedef struct Node {
    long data;
    struct Node * next;
} Node;


void phase_6(char *);
void explode_bomb();
Node * fun6(Node *);

/* A global pointer to begining of our LL */
Node * FRONT;

int main()
{
    /* Making of LL, not actually in main but somewhere in initialize bomb */
    Node *node0 = malloc(sizeof(struct Node));
    
    node0->data = 76;
    Node * node1 = malloc(sizeof(struct Node));
    node0->next = node1;

    /* Set Global Pointer */
    FRONT = node0;
    
    node1->data = 770;
    Node * node2 = malloc(sizeof(struct Node));
    node1->next = node2;
    
    node2->data = 187;
    Node * node3 = malloc(sizeof(struct Node));
    node2->next = node3;
    
    node3->data = 628;
    Node * node4 = malloc(sizeof(struct Node));
    node3->next = node4;
    
    node4->data = 180;
    Node * node5 = malloc(sizeof(struct Node));
    node4->next = node5;

    node5->data = 157;
    Node * node6 = malloc(sizeof(struct Node));
    node5->next = node6;

    node6->data = 961;
    Node * node7 = malloc(sizeof(struct Node));
    node6->next = node7;

    node7->data = 435;
    Node * node8 = malloc(sizeof(struct Node));
    node7->next = node8;

    node8->data = 340;
    Node * node9 = malloc(sizeof(struct Node));
    node8->next = node9;

    node9->next = NULL;

    /* Call phase_6 with our correct input */
    char* input = "435";
    phase_6(input);

    
    return(0);
}


/*
 * phase_6
 * Parameters: char* or string for our input
 * Returns: Nothing
 * Does: Finds the fourth largest element in the Linked List and
 *       compares it to 435. Explodes bomb if it is not.
 */
void phase_6(char *input)
{
    /* Convert input to a long */
    long inputNum = strtol(input, 0, 10);
    
    Node *curr = NULL;
    
    /* Calls fun6 if a linked list exists */
    if (FRONT != NULL) {
        curr = fun6(FRONT);
    }
    
    /* sets current node to 4th node */
    for (int i = 0; i < 3; i++)
    {
        if (curr != NULL) {
            curr = curr->next;
        }
    }

    /* Explodes if input is incorrect */
    if ((curr == NULL) || (curr->data != 435)) {
        explode_bomb();
    }
}

/*
 * fun6
 * Parameters: one Node* (first node of the Linked List)
 * Returns: Node pointer (Beginning of Linked List)
 * Does: Orders the Linked Lists from greatest to least
 */
Node * fun6(Node *front)
{
    /* Edge case: List is empty or has one node */
    if ((front == NULL) || (front->next == NULL)) {
        return front;
    }
    Node *prev = front;
    Node *curr = front->next;

    while(curr != NULL) {
        /* Advance through sorted elements */
        if (prev->data >= curr->data) {
            curr = curr->next;
            prev = prev->next;
        
        /* Swaps non sorted elements */
        } else {
            /* Edge case: curr is larger than the first element of the list */
            if (front->data < curr->data) {
                prev->next = curr->next;
                curr->next = front;
                front = curr;
            } else {
                /* Start at beginning of list and find place to swap */
                Node *temp = front;
                
                /* Finds correct values to swap */
                while ((temp->next != NULL) && (curr->data < temp->next->data)){
                    temp = temp->next;
                }
                /* Swap values */
                prev->next = curr->next;
                curr->next = temp->next;
                temp->next = curr;
            }
        }
        /* Reset curr to correct value */
        curr = prev->next;
    }
    return front;
}

void explode_bomb()
{
    fprintf(stderr, "rip, your bomb exploded\n");
}