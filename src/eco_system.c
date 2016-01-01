/**************************************************************** eco_system.c ============================================================= Copyright 1996-2016 Tom Barbalet. All rights reserved. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. This software and Noble Ape are a continuing work of Tom Barbalet, begun on 13 June 1996. No apes or cats were harmed in the writing of this software. ****************************************************************/#include "ecosim.h"#include <string.h>void system_clear_eats(ecosim_genome * eco, unsigned int species){    unsigned int i;    for (i = 0; i < species; i++) {        eco->eats[i].species_index[0] = -1;    }}void system_init(ecosim_genome * eco, ecosim_genome * template){    memcpy((void*)eco,(void*)template,sizeof(ecosim_genome));}void system_cycle(ecosim_genome * eco, unsigned int species, int range_check){    int i, index, idx;    double consume;    for (i = 0; i < species; i++) {        /* grow */        eco->pop[i] += (( 1 + eco->growth[i]) * eco->pop[i]);        /* eat things */        index  = 0;        while (eco->eats[i].species_index[index] > -1) {            idx = eco->eats[i].species_index[index];            consume = eco->pop[idx] * eco->eats[i].rate[index];            eco->pop[i] += consume;            eco->pop[idx] -= consume;            index++;        }        /* limits to growth */        if (range_check != 0) {            if (eco->pop[i] < 0)                eco->pop[i] = 0;        }        if (eco->pop[i] > eco->maxpop[i])            eco->pop[i] = eco->maxpop[i];    }}void SetEats(ecosim_genome * eco, int index, int eats_index, double rate){    int i;    i=0;    while ((eco->eats[index].species_index[i] > -1) &&           (i < ECOSYSTEM_MAX_SPECIES-1)) {        i++;    }    if (i >= ECOSYSTEM_MAX_SPECIES-1) {        return;    }    eco->eats[index].species_index[i] = eats_index;    eco->eats[index].rate[i] = rate;    eco->eats[index].species_index[i+1] = -1;}void SetPop(ecosim_genome * eco, unsigned int species_index, unsigned int size){    eco->pop[species_index]=(double)size;}int GetPop(ecosim_genome * eco, unsigned int species_index){    return (int)eco->pop[species_index];}double GetGrow(ecosim_genome * eco, unsigned int species_index){    return eco->growth[species_index];}void SetGrow(ecosim_genome * eco, unsigned int species_index, double rate){    eco->growth[species_index]=rate;}