//
//  autocomplete.c
//  
//
//  Created by Peishuo Cai on 2021-02-16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

int string_compare(const void *g, const void *l)
{
    return strcmp(((struct term *)g) -> term, ((struct term *)l)->term);
    /* strcmp functions works exactly as expected from
    comparison function */
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    char line[200];

    FILE *fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters
    
    *pnterms = atoi(line); //sets the number of lines

    *terms = (struct term *)malloc(sizeof(struct term)*(*pnterms)); //sets the allocated space
    
    for(int i = 0; i < *pnterms; i++){
        
        fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters
        char * start = strtok(line,"\t");    //get the first token as the number

        double n_start = atof(start);   //set the value to a double to be stored

        (*terms)[i].weight = n_start;   //store it in weight
        
        while (start != NULL){
            start = strtok(NULL, "\t");  //set the city name
            if (start == NULL){break;}
            for (int g = 0; g < strlen(start); g++){
                if (start[g] != '\n'){
                ((*terms)[i].term)[g] = start[g];//copy the city name into the struct
                }
                else{
                    ((*terms)[i].term)[g] = '\0';
                    break;
                }
            }

            }
        }

    qsort(*terms, *pnterms, sizeof(struct term), string_compare); //sort the struct list
}
        
        
void copy(char *dest, char *src, long size){ //This makes so that the compared string is the same length as the substr
    
    for(int j = 0; j < size ; j++){
        
        if (*src == '\0'){
            break;
        }

        *dest = *src;
        src++;
        dest++;
    }

    *dest = '\0';

}

int autocompare(char *fullword, char *substr){  //This compares the strings and returns if they have the same value

    char temp[200];

    copy(temp, fullword, strlen(substr));

    return strcmp(temp, substr);

}



int recurs_lowest(struct term *terms, int nterms, char *substr, int start)
{
//    printf("start is:%d\n", start);
//    printf("nterms is:%d\n", nterms);
//    printf("********");
    if (nterms == 2){
        if (autocompare(terms[start].term, substr) == 0){
            return start;
        }
        else{
            if (autocompare(terms[start+1].term, substr) != 0){
                return -1;
            }
            return start + 1;
        }
    }
    
    if (nterms % 2 == 0)
    {
        if (autocompare(terms[start + nterms/2].term, substr) >= 0)
        {
            return recurs_lowest(terms, nterms/2 + 1, substr, start);  //even case return the left if its bigger or the same, includes the checking term
        }
        else
        {
            return recurs_lowest(terms, nterms/2, substr, start + nterms/2);  //even case return the right if it is smaller, including checking term
        }
    }
    else
    {
        if (autocompare(terms[start + (nterms-1)/2].term, substr) >= 0)
        {
            return recurs_lowest(terms, (nterms-1)/2 + 1, substr, start);  //odd case return the left if its bigger or the same, includes the checking term
        }
        else
        {
            return recurs_lowest(terms, (nterms-1)/2 + 1, substr, start + (nterms-1)/2);  //odd case return the right if it is smaller, includes checking term
        }
    }
}



int lowest_match(struct term *terms, int nterms, char *substr)
{
    int start = 0;
    return recurs_lowest(terms, nterms, substr, start);
}



int recurs_highest(struct term *terms, int nterms, char *substr, int end)
{
//    printf("end is:%d\n", end);
//    printf("nterms is:%d\n", nterms);
//    printf("********");
    if (nterms == 2){
        if (autocompare(terms[end].term, substr) == 0){
            return end;
        }
        else{
            if (autocompare(terms[end-1].term, substr) != 0){
                return -1;
            }
            return end-1;
        }
    }
    
    if (nterms % 2 == 0)
    {
        if (autocompare(terms[end - nterms/2].term, substr) > 0)
        {
            return recurs_highest(terms, nterms/2, substr, end - nterms/2);  //even case return the left if its bigger or the same, includes the checking term
        }
        else
        {
            return recurs_highest(terms, nterms/2 + 1, substr, end);  //even case return the right if it is smaller, including checking term
        }
    }
    else
    {
        if (autocompare(terms[end - (nterms-1)/2].term, substr) > 0)
        {
            return recurs_highest(terms, (nterms-1)/2 + 1, substr, end - (nterms-1)/2);  //odd case return the left if its bigger or the same, includes the checking term
        }
        else
        {
            return recurs_highest(terms, (nterms-1)/2 + 1, substr, end);  //odd case return the right if it is smaller, includes checking term
        }
    }
}


int highest_match(struct term *terms, int nterms, char *substr)
{
    int end = nterms - 1;//This sets it so that it starts from the right side and goes left. Essnetially reverse lowest search
    return recurs_highest(terms, nterms, substr, end);
}

int weight_compare(const void *g, const void *l)
{
    if (((((struct term *)l) -> weight) - (((struct term *)g) -> weight)) >= 0){
        return 1;
    }
    else{
            return -1;
        }
    /* strcmp functions works exactly as expected from
    comparison function */
}




void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    if (highest_match(terms, nterms, substr) == -1){*n_answer = 0;}
    else{*n_answer = highest_match(terms, nterms, substr) - lowest_match(terms, nterms, substr)+1;}
//    printf("%d", *n_answer);
//    printf("?*?*?*?*?*?*?*?*\n\n\n\n\n\n");
//    printf("%s", terms[lowest_match(terms, nterms, substr)].term);
//    printf("%s", terms[highest_match(terms, nterms, substr)].term);
//    printf("?*?*?*?*?*?*?*?*\n\n\n\n\n\n");
    int start = lowest_match(terms, nterms, substr);
    *answer = (struct term *)malloc(sizeof(struct term) * (*n_answer));
    for (int point = 0; point < *n_answer; point++)
    {
        strcpy((*answer)[point].term, terms[point + start].term);
        (*answer)[point].weight = terms[point + start].weight;
    }
    qsort(*answer, *n_answer, sizeof(struct term), weight_compare);
    for (int bum = 0; bum < *n_answer; bum++)
    {
//        printf("%d", bum);
//        printf("%s\n", (*answer)[bum].term);
//        printf("%f\n", (*answer)[bum].weight);
//        printf("-------------\n");
    }
}

