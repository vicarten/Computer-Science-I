#include <stdio.h>
#include <stdlib.h>
#define INVALID -1
long long int L_people = 0, R_people = 0, finish_L = 0, finish_R = 0, sum_time = 0;

//struct for data related to the group
struct Group {
    long long int arrive, size, process, finish, side;
};
// Queue implemented using a circular linked list
struct Queue {
    struct Node * tail;
};
// Node of a linked list
struct Node {
    struct Node * next;
    struct Group data;
};

//Swapping the groups' values to swap the order
void swap(long long int *x, long long int *y)
{
    long long int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

// Left Queue Prototypes
int front_L(struct Queue * l);
void dequeue_L(struct Queue * l);
void enqueue_L(struct Queue * l, struct Group * data);
struct Queue * createQueue_L();
void cleanQueue_L(struct Queue * l);
// Right Queue Prototypes
int front_R(struct Queue * r);
void dequeue_R(struct Queue * r);
void enqueue_R(struct Queue * r, struct Group * data);
struct Queue * createQueue_R();
void cleanQueue_R(struct Queue * r);

// Circular Linked List prototypes
struct Node * removeHead(struct Node * tail);
struct Node * insertTail(struct Node * tail, struct Group * data);
struct Node * createNode(struct Group * data);

//check front left queue
int front_L(struct Queue * l) {
    if (l == NULL)
        return INVALID;
    if (l->tail == NULL)
        return INVALID;
    if (l->tail->next == NULL){
        return INVALID;
    }
    return 0;
}
//check front right queue
int front_R(struct Queue * r) {
    if (r == NULL)
        return INVALID;
    if (r->tail == NULL)
        return INVALID;
    if (r->tail->next == NULL){
        return INVALID;
    }
    return 0;
}
//create left queue
struct Queue * createQueue_L() {
    struct Queue * result = (struct Queue *) malloc(sizeof(struct Queue));
    result->tail = NULL;
    return result;
}
//create right queue
struct Queue * createQueue_R() {
    struct Queue *result = (struct Queue *) malloc(sizeof(struct Queue));
    result->tail = NULL;
    return result;
}

//Add group to the end of the queue
//Left
void enqueue_L(struct Queue * l, struct Group * data) {
    l->tail = insertTail(l->tail, data);
    //compute the time when group will exit the line
    if (finish_L < data->arrive) {
        data->finish = data->arrive + data->process;
    } else {
        data->finish = finish_L + data->process;
    }
    finish_L = data->finish;
    //add people to the people counter in the left queue
    L_people += data->size;
    //add waiting time of each person together
    sum_time += (data->finish - data->arrive) * data->size;
}
//Right
void enqueue_R(struct Queue * r, struct Group * data) {
    r->tail = insertTail(r->tail, data);
    //compute the time when group will exit the line
    if (finish_R < data->arrive) {
        data->finish = data->arrive + data->process;
    } else {
        data->finish = finish_R + data->process;
    }
    finish_R = data->finish;
    //add people to the people counter in the right queue
    R_people += data->size;
    //add waiting time of each person together
    sum_time += (data->finish - data->arrive) * data->size;
}
struct Node * insertTail(struct Node * tail,struct Group * data) {
    if (tail == NULL) {
        return createNode(data);
    }
    struct Node * oldTail = tail;
    struct Node * oldHead = oldTail->next;
    struct Node * newTail = createNode(data);
    // Adjust pointers
    oldTail->next = newTail;
    newTail->next = oldHead;
    // Return the resulting tail
    return newTail;
}
struct Node * createNode(struct Group * data) {
    struct Node * result = (struct Node *) malloc(sizeof(struct Node));
    result->data = *data;
    result->next = result;
    return result;
}
//Remove group from the group from the front of the queue
//Left
void dequeue_L(struct Queue * l) {
    l->tail = removeHead(l->tail);
}
//Right
void dequeue_R(struct Queue * r) {
    r->tail = removeHead(r->tail);
}
struct Node * removeHead(struct Node * tail) {
    // Empty list case
    if (tail == NULL) {
        return NULL;
    }
    // 1 node list case
    if (tail == tail->next) {
        free(tail);
        // the resulting is empty
        return NULL;
    }
    // More than 1 node
    struct Node * oldTail = tail;
    struct Node * oldHead = tail->next;
    struct Node * newHead = oldHead->next;
    // Free the memory to remove
    free(oldHead);

    // Adjust pointers
    oldTail->next = newHead;
    // Return the resulting tail
    return oldTail;
}
//Empty left queue
void cleanQueue_L(struct Queue * l) {
    while (front_L(l) != INVALID){
        dequeue_L(l);
    }
}
//Empty right queue
void cleanQueue_R(struct Queue * r) {
    while (front_R(r) != INVALID){
        dequeue_R(r);
    }
}

int main() {
    //create two queues
    struct Queue * l = createQueue_L();
    struct Queue * r = createQueue_R();

    long long int numGroups;

    scanf("%lld", &numGroups);

    struct Group arr[numGroups];
    //read the value into struct
    for (int i=0; i<numGroups; i++){
        scanf("%lld", &arr[i].size);
        scanf("%lld", &arr[i].arrive);
        scanf("%lld", &arr[i].process);
        arr[i].finish = 0;
    }

    //sorting in ascending order
    for (int i = 0; i < numGroups; i++) {
        for (int j = i+1; j < numGroups; j++) {
            // check if j arrived earlier than i
            if (arr[j].arrive < arr[i].arrive){
                //swap the struct values if condition is true
                swap(&arr[i].size, &arr[j].size);
                swap(&arr[i].arrive, &arr[j].arrive);
                swap(&arr[i].process, &arr[j].process);
            }
        }
    }

    //timer to count the time and check when group entered and exited the lines
    long long int count_time = 0, c=0;
    while ((arr[numGroups-1].finish == 0) || (count_time < finish_L) || (count_time < finish_R)) {
        //the group entered
        if (arr[c].arrive == count_time) {
            //determine the queue with fewer people
            if (L_people<=R_people){
                arr[c].side = 1; // 1 - left queue
                enqueue_L(l, &arr[c]);
            } else {
                arr[c].side = 2; // 2 - right queue
                enqueue_R(r, &arr[c]);
            }
            c++;
        }
        //check if the group is done processing and should exit the line
        for (int j = 0; j < c-1; j++) {
            //which group
            if (arr[j].finish == count_time) {
                //from which line
                if (arr[j].side == 1) {
                    dequeue_L(l);
                    //remove people from people counter in left queue
                    L_people -= arr[j].size;
                }
                if (arr[j].side == 2) {
                    dequeue_R(r);
                    //remove people from people counter in right queue
                    R_people -= arr[j].size;
                }
            }
        }
        count_time++;
    }
    //Finished processing all groups. Check and empty the queue
    cleanQueue_L(l);
    cleanQueue_R(r);
    //Get the total time all people waited
    printf("%lld", sum_time);
    return 0;
}
