/****************************************************************

 eco_system.c

 =============================================================

 Copyright 1996-2015 Tom Barbalet. All rights reserved.

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 This software and Noble Ape are a continuing work of Tom Barbalet,
 begun on 13 June 1996. No apes or cats were harmed in the writing
 of this software.

 ****************************************************************/

#include "ecosim.h"
#include <stdio.h>
#include <string.h>

/* test a given ecosystem */
void search_test(ecosim_genome * eco, unsigned int itterations, int show)
{
    unsigned int p, i;
    unsigned int all_surviving = 1;

    system_init(eco);
    i = eco->survival_steps = 0;
    while (i < itterations)
    {
        system_cycle(eco, show);
        if (show != 0) show_output_short(eco);
        for (p = 0; p < cSize; p++) {
            if (all_surviving == 1) {
                if ((eco->pop[p] <= 0) ||
                    (eco->pop[p] >= 200)) {
                    all_surviving = 0;
                    if (show == 0) {
                        break;
                        i = itterations;
                    }
                }
            }
        }
        if (all_surviving == 1) {
            eco->survival_steps++;
        }
        i++;
    }

    /* include the duration of survival in the score */
    eco->score = eco->survival_steps;
}

/* sort by score */
void search_sort(ecosim_search * sim)
{
    unsigned int i, j, winner;
    ecosim_genome swap;
    double max;

    for (i = 0; i < SEARCH_SAMPLES-1; i++) {
        max = sim->genome[i].score;
        winner = i;
        for (j = i+1; j < SEARCH_SAMPLES; j++) {
            if (sim->genome[j].score > max) {
                max = sim->genome[j].score;
                winner = j;
            }
        }
        if (winner != i) {
            memcpy((void*)&swap, (void*)&sim->genome[i], sizeof(ecosim_genome));
            memcpy((void*)&sim->genome[i], (void*)&sim->genome[winner], sizeof(ecosim_genome));
            memcpy((void*)&sim->genome[winner], (void*)&swap, sizeof(ecosim_genome));
        }
    }
}

/* produce the next set of samples */
void samples_renew(ecosim_search * sim)
{
    int i, j, e, elitism, parent1, parent2;
    ecosim_search prev_sim;

    memcpy((void*)&prev_sim, (void*)sim, sizeof(ecosim_search));

    elitism = SEARCH_SAMPLES/2;

    for (i = 0; i < SEARCH_SAMPLES; i++) {
        parent1 = rand() % elitism;
        parent2 = rand() % elitism;
        for (j = 0; j < cSize; j++) {
            for (e = 0; e < 6; e++) {
                /* breed */
                sim->genome[i].eats[j][e] = prev_sim.genome[rand()%2].eats[j][e];
                /* mutate */
                sim->genome[i].eats[j][e] += ((rand()%20000/10000.0)-1.0)*sim->mutation_rate;
                if (sim->genome[i].eats[j][e] < MIN_EATS) sim->genome[i].eats[j][e] = MIN_EATS;
                if (sim->genome[i].eats[j][e] > MAX_EATS) sim->genome[i].eats[j][e] = MAX_EATS;
            }
            /* breed */
            sim->genome[i].growth[j] = prev_sim.genome[rand()%2].growth[j];
            /* mutate */
            sim->genome[i].growth[j] += ((rand()%20000/10000.0)-1.0)*sim->mutation_rate;
            if (sim->genome[i].growth[j] < MIN_GROWTH) sim->genome[i].growth[j] = MIN_GROWTH;
            if (sim->genome[i].growth[j] > MAX_GROWTH) sim->genome[i].growth[j] = MAX_GROWTH;
        }
    }
}

/* initialise with random values */
void search_init(ecosim_search * sim)
{
    int i, j, e;

    for (i = 0; i < SEARCH_SAMPLES; i++) {
        for (j = 0; j < cSize; j++) {
            for (e = 0; e < 6; e++) {
                sim->genome[i].eats[j][e] = MIN_EATS + ((rand()%20000/20000.0)*(MAX_EATS-MIN_EATS));
            }
            sim->genome[i].growth[j] = MIN_GROWTH + ((rand()%20000/20000.0)*(MAX_GROWTH-MIN_GROWTH));
        }
    }
}

void search_cycle(ecosim_search * sim)
{
    int i;

#pragma omp parallel for
    for (i = 0; i < SEARCH_SAMPLES; i++) {
        search_test(&sim->genome[i], sim->itterations, 0);
    }

    sim->collapses=0;
    for (i = 0; i < SEARCH_SAMPLES; i++) {
        if (sim->genome[i].survival_steps < sim->itterations) {
            sim->collapses++;
        }
    }
    printf("score %.2f collapse %.2f\n",
           sim->genome[0].score,
           sim->collapses*100.0/SEARCH_SAMPLES);

    search_sort(sim);
}
