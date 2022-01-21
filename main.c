//
//  main.c
//  Project_1
//
//  Created by Peishuo Cai on 2021-02-16.
//

#include <stdio.h>
#include <stdlib.h>
#include "autocomplete.h"

int main(void)
{
    
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
//    printf("%d", lowest_match(terms, nterms, "Tor"));
//    printf("%s", terms[lowest_match(terms, nterms, "Tor")].term);
    highest_match(terms, nterms, "Tor");
    printf("%s", terms[highest_match(terms, nterms, "Tor")].term);
    
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Tor");
    //free allocated blocks here -- not required for the project, but good practice
    return 0;
}
