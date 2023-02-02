#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 20+1

// Declare global variable to track number of names
int count_names = 0;
int num_names=0;

typedef struct TrieNode TrieNode;

struct TrieNode {
    TrieNode * children[26];
    int subtreeSum;
    int nodeTotal;
};
// Declare functions
TrieNode *createNode();
TrieNode * insert(TrieNode * root, char * name, int mango);
TrieNode * change(TrieNode * root, char * old_name);
TrieNode * search(TrieNode * root, char * sequence);
void cleanTrie(TrieNode *root);

//Function to create a new node
TrieNode *createNode() {
    TrieNode *result = (TrieNode *)malloc(sizeof(TrieNode));
    for (int i = 0; i < 26; i++) {
        result->children[i] = NULL;
    }
    result->nodeTotal = 0;
    result->subtreeSum = 0;

    return result;
}

// Function to add new node
TrieNode * insert(TrieNode * root, char * name, int mango) {
    TrieNode * cur = root;
    //Walking through the nodes to see if the node exist, or we need to add a new one
    for (int i=0; name[i] !='\0'; i++) {
        assert(name[i] <= 'Z' && name[i] >= 'A');   //check if uppercase letter
        //when node does not exist
        if (cur->children[name[i] - 'A'] == NULL) {
            // create a root
            cur->children[name[i] - 'A'] = createNode();
            cur->children[name[i] - 'A']->subtreeSum += mango;
        } else {
            //node exist, store mango value for the letter, not name
            cur->children[name[i] - 'A']->subtreeSum += mango;
        }
        cur = cur->children[name[i] - 'A'];
    }
    //Check if the name already exist or not
    if (cur->nodeTotal == 0){
        count_names+=1; //if it is a new name
        // if more names than mentioned are used, free memory and terminate
        if (count_names>num_names){
            cleanTrie(root);
            exit(0);
        }
    }
    //Store how many mangoes did the orangutans (name) ate
    cur->nodeTotal = mango;
    return root;
}

// Function to replace the name with a new name
TrieNode * change(TrieNode * root, char * old_name){
    TrieNode * cur = root;
    TrieNode * r_cur = root;

    //Determine how many mangoes did the orangutan ate
    for (int i = 0; old_name[i] != '\0'; i++) {
        if (cur->children[old_name[i] - 'A'] == NULL) {
            return 0;
        }
        cur = cur->children[old_name[i] - 'A'];
    }
    int mango = cur->nodeTotal;

    //Subtract mango from the name letter
    for (int i = 0; old_name[i] != '\0'; i++) {
        if (r_cur->children[old_name[i] - 'A'] == NULL) {
            return 0;
        }
        r_cur->children[old_name[i] - 'A']->subtreeSum -= mango;
        r_cur = r_cur->children[old_name[i] - 'A'];
    }
    r_cur->nodeTotal = 0;

    //Transfer mango to another name
    char new_name[SIZE];
    scanf("%s", new_name);
    root = insert(root, new_name, mango);
    return root;
}

//Function to find the total number of mango that was eaten by orangutans with specific letter sequence in name
TrieNode * search(TrieNode * root, char * sequence){
    int answer;
    TrieNode * cur = root;
    //Search through the letter sequences
    //if sequence is not found
    for (int i = 0; sequence[i] != '\0'; i++) {
        if (cur->children[sequence[i] - 'A'] == NULL) {
            printf("0\n");
            return root;
        } else {    //sequence can be found
            answer = cur->children[sequence[i] - 'A']->subtreeSum;
        }
        cur = cur->children[sequence[i] - 'A'];
    }
    printf("%d\n", answer);
    return root;
}

//Free dynamic memory
void cleanTrie(TrieNode *root) {
    if (root == NULL)
        return;
    for (int i = 0; i < 26; i++)
        cleanTrie(root->children[i]);
    free(root);
}


int main() {
    TrieNode * root = createNode();
    int num_events, event, mango;
    char name[SIZE], old_name[SIZE], sequence[SIZE];
    scanf("%d", &num_names);
    scanf("%d", &num_events);

    //Events:
    for (int i=0; i<num_events; i++){
        scanf("%d", &event);
        //Feeding
        if(event==1){
                scanf("%s", name);
                scanf("%d", &mango);

                root = insert(root, name, mango);
        }
        //Name change
        if(event==2){
            count_names-=1;
            scanf("%s", old_name);
            root = change(root, old_name);
        }
        //Inquiry
        if(event==3){
            scanf("%s", sequence);
            search(root, sequence);

        }
    }
    cleanTrie(root);
    return 0;
}
