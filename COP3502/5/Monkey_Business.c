#include <stdio.h>
#include <stdlib.h>
typedef struct Node Node;

struct Node{
    struct Node * left, * right, * parent;
    int value, id, answer;
};

Node * createNode (int id, int value);
Node * insert(Node * root, int id, int value);
Node * goBack(Node * root);
int sum(Node * root, int value);
void cleanTree(Node * root);
void answerSearch(Node * root, int destination);

//Function to create a new node
Node * createNode (int id, int value){
    Node * ret = (Node *) malloc(sizeof(Node));
    ret->id = id;
    ret->value = value;
    ret->answer = value;
    ret->right = ret->left = ret->parent = NULL;
    return ret;
}

//Function to add the new node to the tree
Node * insert(Node * root, int id, int value) {
    // Case with empty tree
    if (root == NULL) {
        return createNode(id, value);
    }
    // Check which side the value belongs on
    //the value will go to the left if there is no child on the left
    if (root->left == NULL) {
        root->left = insert(root->left, id, value);
        root->left->parent = root;
        root->left->answer = sum(root->left, value);
        return root->left;
    } else { //if there is already the child on the left, the value will go to the right
        root->right = insert(root->right, id, value);
        root->right->parent = root;
        root->right->answer = sum(root->right, value);
        return root->right;
    }
}

//function that go back to parent when line contain -1
Node * goBack(Node * root) {
    root = root->parent;
    return root;
}

//function to calculate the total number of monkeys on the way to the particular exhibit
 int sum(Node * root, int value){
    int answer = root->parent->answer + value;
    return answer;
}

//Function to clean the contents of the tree
void cleanTree(Node * root) {
    // Empty case
    if (root == NULL) return;
    // Clean children before self
    cleanTree(root->right);
    cleanTree(root->left);
    // Clean the node
    free(root);
}

//Function to find the information about the number of monkeys that visitors will see (answer)
void answerSearch(Node * root, int destination) {
    // Empty tree case
    if (root == NULL) return;
    // Process
    if  (root->id == destination) {
        printf("%d\n", root->answer);
    }
    //go to children
    answerSearch(root->left, destination);
    answerSearch(root->right, destination);
}

int main() {
    int num_exhibits, exhibit_id, monkeys, path = 0, count=0, visitors, destination;
    Node * root = NULL;
    scanf("%d", &num_exhibits);

    //loop to make the path map
    while((count != num_exhibits) || (path-1 != 0)){
        scanf("%d", &exhibit_id);
        //go back
        if (exhibit_id == -1){
           root = goBack(root);
           path -=1;
        } else {//go to the exhibit that did not visit yet
            scanf("%d", &monkeys);
            root = insert(root, exhibit_id, monkeys);
            path +=1;
            count +=1;
        }
    }
    //number of visitors and where they want to go
    scanf("%d", &visitors);
    for (int i = 0; i < visitors; i++) {
        scanf("%d", &destination);
        answerSearch(root, destination);
    }

    cleanTree(root);
    return 0;
}
