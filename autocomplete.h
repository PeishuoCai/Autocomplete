//
//  autocomplete.h
//  Project_1
//
//  Created by Peishuo Cai on 2021-02-16.
//

#ifndef autocomplete_h
#define autocomplete_h

#include <stdio.h>

struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
};


void read_in_terms(struct term **terms, int *pnterms, char *filename);
int lowest_match(struct term *terms, int nterms, char *substr);
int highest_match(struct term *terms, int nterms, char *substr);
void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr);


#endif /* autocomplete_h */
