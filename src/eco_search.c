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
void search_test(ecosim_search * sim, ecosim_genome * eco, unsigned int species, unsigned int itterations, int show)
{
    unsigned int p, i;
    unsigned int all_surviving = 1;

    i = eco->survival_steps = 0;
    while (i < itterations)
    {
        system_cycle(eco, species, show);
        if (show != 0) show_output_short(sim);
        for (p = 0; p < species; p++) {
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
    int i, j, e, elitism;
    ecosim_search prev_sim;
    int parent[2];

    memcpy((void*)&prev_sim, (void*)sim, sizeof(ecosim_search));

    elitism = SEARCH_SAMPLES/2;

    for (i = 0; i < SEARCH_SAMPLES; i++) {
        parent[0] = rand() % elitism;
        parent[1] = rand() % elitism;
        for (j = 0; j < sim->species; j++) {
            e = 0;
            while (sim->genome[i].eats[j].species_index[e] != -1) {
                /* breed */
                sim->genome[i].eats[j].rate[e] = prev_sim.genome[parent[rand()%2]].eats[j].rate[e];
                /* mutate */
                sim->genome[i].eats[j].rate[e] += ((rand()%20000/10000.0)-1.0)*sim->mutation_rate;
                if (sim->genome[i].eats[j].rate[e] < MIN_EATS) sim->genome[i].eats[j].rate[e] = MIN_EATS;
                if (sim->genome[i].eats[j].rate[e] > MAX_EATS) sim->genome[i].eats[j].rate[e] = MAX_EATS;
                e++;
            }
            /* breed */
            sim->genome[i].growth[j] = prev_sim.genome[parent[rand()%2]].growth[j];
            /* mutate */
            sim->genome[i].growth[j] += ((rand()%20000/10000.0)-1.0)*sim->mutation_rate;
            if (sim->genome[i].growth[j] < MIN_GROWTH) sim->genome[i].growth[j] = MIN_GROWTH;
            if (sim->genome[i].growth[j] > MAX_GROWTH) sim->genome[i].growth[j] = MAX_GROWTH;
        }
    }
}

void search_cycle(ecosim_search * sim)
{
    int i;

#pragma omp parallel for
    for (i = 0; i < SEARCH_SAMPLES; i++) {
        search_test(sim, &sim->genome[i], sim->species, sim->itterations, 0);
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

void set_species_name(ecosim_search * sim, unsigned int index, char * name)
{
    strcpy((char*)&sim->species_name[index], name);
}

void set_species_name_short(ecosim_search * sim, unsigned int index, char * short_name)
{
    strcpy((char*)&sim->species_name_short[index], short_name);
}
