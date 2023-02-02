#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cage{
    char animal[1000+1];
};

struct Section {
    struct Cage * cages;
    int numCages;
};


int main() {
    struct Section * allSections = NULL;
    struct Cage *cages = NULL;

    int numSections; //representing the number of park sections
    scanf("%d", &numSections);

    int S, C;

    //Allocating Sections
    allSections = (struct Section *) calloc(numSections, sizeof(struct Section));

    //Allocating 0 Cages to each Section
    for (int all=0; all<numSections; all++) {
        allSections[all].cages = (struct Cage *) calloc(0, sizeof(struct Cage));
    }

    int command=0;

    while (command != 4) //4
    {
        scanf("%d", &command);

        // 1 S C
        if (command == 1) {
            scanf("%d", &S);
            scanf("%d", &allSections[S].numCages);
            allSections[S].cages = (struct Cage *) realloc(cages, allSections[S].numCages * sizeof(struct Cage));
        }

        // 2 S C A
        if (command == 2) {
            scanf("%d", &S);
            scanf("%d", &C);
            if (strlen(allSections[S].cages[C].animal) == 0) {
                scanf("%s\n", allSections[S].cages[C].animal);
            }
        }

        // 3 S C A
        if (command == 3) {
            scanf("%d", &S);
            scanf("%d", &C);
            if (strlen(allSections[S].cages[C].animal) !=0){
                printf("%s\n", allSections[S].cages[C].animal);
            } else {
                printf("No animal found.\n");
            }
        }
    }

    //free

    for (int f=0; f<numSections; f++){
        free(allSections[f].cages);
        allSections[f].cages = NULL;
    }
    free(allSections);
    allSections = NULL;

    return 0;
}
