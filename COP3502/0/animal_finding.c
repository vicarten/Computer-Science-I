#include <stdio.h>
#include <string.h>

/* Victoria Ten
 * COP3502C-22Fall 0004
 * Assignment 0: Animal Finding
 * 08/26/2022
 */

//search the pattern and count repeats
int occurrence_of_pattern(char *animal, char *park) {
    int count=0;
    int length = strlen(animal);    //size of the searched pattern

    for (int j = 0; park[j] != '\0'; j++) {
        int i=0;    //position in animal
        int m=j;    //position in park
        while (i<length && animal[i]==park[m]){
            i++;
            m++;
        }
        if (i==length){
            count++;
        }
    }
    return count;
}
int main() {
    char S[20+1];
    int N=0;

    scanf("%s", S);
    scanf("%d\n", &N);

    //run the sections/lines
    for (int i=0; i<N; i++) {
        char park_pattern[100000 + 1];
        fgets(park_pattern,sizeof(park_pattern), stdin);
        printf("%d\n", occurrence_of_pattern(S, park_pattern));
    }
    return 0;
}