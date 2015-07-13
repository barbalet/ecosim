/****************************************************************
 
 ecosim.c
 
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

static ecosim_events * event_subscription = 0L;

static e_real population[ENTITY_SIZE];
static e_real total_energy[ENTITY_SIZE - ENTITY_FISH];

static e_real dead_animal_energy;

const ECOSIM_SIZE entity_to_size[ENTITY_SIZE] =
{
    SIZE_20_GRAM,        /* ENTITY_GRASS */
    SIZE_160_KILOGRAMS,  /* ENTITY_BUSH */
    SIZE_3200_KILOGRAMS, /* ENTITY_TREE */
    
    SIZE_400_GRAM, /* ENTITY_FISH */
    
    SIZE_400_GRAM, /* ENTITY_SEED_EATING_BIRD */
    SIZE_400_GRAM, /* ENTITY_INSECT_EATING_BIRD */
    SIZE_8_KILOGRAMS, /* ENTITY_SEA_BIRD */
    SIZE_8_KILOGRAMS, /* ENTITY_BIRD_OF_PREY */
    
    SIZE_1_GRAM, /* ENTITY_INSECT */
    SIZE_20_GRAM, /* ENTITY_MOUSE */
    SIZE_20_GRAM, /* ENTITY_FROG */
    
    SIZE_8_KILOGRAMS, /* ENTITY_LIZARD */
    SIZE_8_KILOGRAMS, /* ENTITY_CAT */
    SIZE_160_KILOGRAMS, /* ENTITY_APE */
};

const ECOSIM_CONSUMPTION_EFFICIENCY entity_to_efficiency[ENTITY_SIZE - ENTITY_FISH] =
{
    EFFICIENCY_MODERATE, /* ENTITY_SEED_EATING_BIRD */
    EFFICIENCY_VERY_GOOD, /* ENTITY_INSECT_EATING_BIRD */
    EFFICIENCY_VERY_GOOD, /* ENTITY_SEA_BIRD */
    EFFICIENCY_VERY_GOOD, /* ENTITY_BIRD_OF_PREY */
    
    EFFICIENCY_EXCELLENT, /* ENTITY_INSECT */
    EFFICIENCY_MODERATE, /* ENTITY_MOUSE */
    EFFICIENCY_VERY_GOOD, /* ENTITY_FROG */
    
    EFFICIENCY_VERY_GOOD, /* ENTITY_LIZARD */
    EFFICIENCY_MODERATE, /* ENTITY_CAT */
    EFFICIENCY_MODERATE, /* ENTITY_APE */
};

#define MAX_EATS (5)

const ECOSIM_ENTITY entity_eats[ENTITY_SIZE - ENTITY_FISH][MAX_EATS] =
{
/* ENTITY_SEED_EATING_BIRD */  { ENTITY_BUSH },
/* ENTITY_INSECT_EATING_BIRD */{ ENTITY_INSECT },
/* ENTITY_SEA_BIRD */          { ENTITY_FISH },
/* ENTITY_BIRD_OF_PREY */      { ENTITY_FISH,               ENTITY_DEAD_ANIMAL,     ENTITY_MOUSE, },

/* ENTITY_INSECT */            { ENTITY_BUSH,               ENTITY_DEAD_ANIMAL,     ENTITY_GRASS,  ENTITY_TREE, },
/* ENTITY_MOUSE */             { ENTITY_GRASS },
/* ENTITY_FROG */              { ENTITY_INSECT },

/* ENTITY_LIZARD */            { ENTITY_INSECT_EATING_BIRD, ENTITY_SEED_EATING_BIRD, ENTITY_INSECT },
/* ENTITY_CAT */               { ENTITY_INSECT_EATING_BIRD, ENTITY_BIRD_OF_PREY,     ENTITY_APE,    ENTITY_SEA_BIRD, ENTITY_LIZARD },
/* ENTITY_APE */               { ENTITY_BUSH,               ENTITY_FISH}
};

e_real consumption_rate[ENTITY_CONSUMPTION];
e_real growth_rate[ENTITY_SIZE];
e_real death_rate[ENTITY_SIZE - ENTITY_FISH];

static e_int ecosim_contributes_to_dead_animal_energy(ECOSIM_ENTITY entity)
{
    return (entity > ENTITY_FISH);
}

static e_int ecosim_is_plant(ECOSIM_ENTITY entity)
{
    return (entity < ENTITY_FISH);
}

void ecosim_subscribe(ecosim_events * event_subscriber)
{
    event_subscription = event_subscriber;
}

e_16bit math_random(e_16bit * local)
{
    e_16bit tmp0;
    e_16bit tmp1;
    
    if (local == 0L) return 0;
    
    tmp0 = local[0];
    tmp1 = local[1];
    
    local[0] = tmp1;
    switch (tmp0 & 7)
    {
        case 0:
            local[1] = (e_16bit)(tmp1 ^ (tmp0 >> 1) ^ 0xd028);
            break;
        case 3:
            local[1] = (e_16bit)(tmp1 ^ (tmp0 >> 2) ^ 0xae08);
            break;
        case 7:
            local[1] = (e_16bit)(tmp1 ^ (tmp0 >> 3) ^ 0x6320);
            break;
        default:
            local[1] = (e_16bit)(tmp1 ^ (tmp0 >> 1));
            break;
    }
    return tmp1;
}

void ecosim_populate_genetics(e_16bit * seed, e_16bit * genetics)
{
    e_int loop = 0;
    while (loop < ENTITY_GENETICS)
    {
        genetics[loop++] = math_random(seed);
    }
}


static e_real ecosim_initial_population(ECOSIM_ENTITY entity)
{
    return (((e_real)SIZE_BOUNDARY)) / ((e_real)entity_to_size[entity]);
}

e_real ecosim_genetics_to_real(e_16bit value)
{
    e_int int_value = ((e_int)value) + 1;
    
    return ((e_real)int_value / 1E+6);
}

void ecosim_init(e_16bit * genetics)
{
    e_int  loop = 0;
    ECOSIM_ENTITY entity = ENTITY_GRASS;
    dead_animal_energy = (e_real)(SIZE_BOUNDARY)/2E+00;
    while (entity < ENTITY_SIZE)
    {
        population[entity] = ecosim_initial_population(entity);
        entity++;
    }
    entity = ENTITY_GRASS;
    while (entity < ENTITY_SIZE)
    {
        total_energy[entity - ENTITY_FISH] = (e_real)(SIZE_BOUNDARY)/2E+00;
        entity++;
    }
    
    while (loop < ENTITY_SIZE)
    {
        growth_rate[loop] = ecosim_genetics_to_real(genetics[loop]);
        loop++;
    }
    loop = 0;
    while (loop < ENTITY_CONSUMPTION)
    {
        consumption_rate[loop] = ecosim_genetics_to_real(genetics[loop + ENTITY_SIZE]);
        loop++;
    }
}

void ecosim_cycle(void)
{
    ECOSIM_ENTITY entity = ENTITY_GRASS;
    e_real new_population[ENTITY_SIZE];
    e_real new_dead_animal_energy = dead_animal_energy;
    e_int  track_consumption = 0;
    
    /* Handle Growth Rate Calculations */
    
    while (entity < ENTITY_SIZE)
    {
        new_population[entity] = population[entity];
        
        if (ecosim_is_plant(entity))
        {
            new_population[entity] += growth_rate[entity] * new_population[entity];
            
        } else if (ecosim_contributes_to_dead_animal_energy(entity))
        {
            e_real  actual_death_rate = death_rate[entity] * new_population[entity];
            new_population[entity] -= actual_death_rate;
            dead_animal_energy += (actual_death_rate * (e_real)(entity_to_size[entity]));
            
        }
        entity++;
    }
    
    entity = ENTITY_GRASS;
    
    while (entity < ENTITY_SIZE)
    {
        if (ecosim_contributes_to_dead_animal_energy(entity))
        {
            e_int  loop = 0;
            e_real efficiency = (((e_real)entity_to_efficiency[entity - ENTITY_FISH]) / ECOSIM_CONSUMPTION_EFFICIENCY_DIVISOR);
            e_real efficiency_wasted = (((e_real)(ECOSIM_CONSUMPTION_EFFICIENCY_DIVISOR - entity_to_efficiency[entity - ENTITY_FISH])) / ECOSIM_CONSUMPTION_EFFICIENCY_DIVISOR);
            e_real delta_energy = 0;
            while (loop < MAX_EATS)
            {
                ECOSIM_ENTITY eaten_entity = entity_eats[entity][loop];
                if (eaten_entity != ENTITY_NONE)
                {
                    e_real consumption = consumption_rate[track_consumption];
                    
                    if (eaten_entity == ENTITY_DEAD_ANIMAL)
                    {
                        e_real entity_energy = efficiency * consumption * dead_animal_energy;
                        if (entity_energy > 0)
                        {
                            new_dead_animal_energy = new_dead_animal_energy - entity_energy;
                            delta_energy = delta_energy + entity_energy;
                        }
                    }
                    else
                    {
                        e_real entity_consumed = consumption * population[eaten_entity];
                        e_real entity_consumed_energy = entity_consumed * (e_real)entity_to_size[eaten_entity];
                        
                        delta_energy = delta_energy + (entity_consumed_energy * efficiency);
                        if (ecosim_contributes_to_dead_animal_energy(eaten_entity))
                        {
                            new_dead_animal_energy = new_dead_animal_energy + (entity_consumed_energy * efficiency_wasted);
                        }
                    }
                    
                    track_consumption++;
                }
                loop++;
            }
            {
                /*
                e_real delta_growth = delta_energy / (e_real)(entity_to_size[entity]);
                e_real projected_growth = population[entity] * growth_rate[entity];
                
                 three cases -
                    there is energy to save, 
                    there is enough energy to make th expected growth rate, OR,
                    the energy available will impact the growth rate */
                
                
 /*               if (delta_growth > projected_growth)
                {
                    e_real projected_growth_energy = projected_growth * (e_real)(entity_to_size[entity]);
                    
                    new_population[entity] =  new_population[entity] + projected_growth;
                    
                    
                    energy[entity] = energy[entity] + delta_energy - projected_growth_energy;
                }
                else
                {
                    if (energy[entity] > delta_energy)
                    
                    energy[entity] = energy[entity] - delta_energy;
                    new_population[entity] =  new_population[entity] + delta_growth;
                }*/
            }
        }
        entity++;
    }
    
    entity = ENTITY_GRASS;

    while (entity < ENTITY_SIZE)
    {
        new_population[entity] = population[entity];
        entity++;
    }
}

e_real ecosim_population(ECOSIM_ENTITY entity)
{
    return population[entity];
}