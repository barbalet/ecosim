/****************************************************************
 
 ecosim.h
 
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


#ifndef __ECOSIM_H__
#define __ECOSIM_H__

typedef double          e_real;
typedef long            e_int;
typedef unsigned short  e_16bit;
typedef char *          e_string;

typedef enum
{
    ENTITY_NONE = -1,
    ENTITY_GRASS = 0,
    ENTITY_BUSH,
    ENTITY_TREE,
    
    ENTITY_FISH,
    
    ENTITY_SEED_EATING_BIRD,
    ENTITY_INSECT_EATING_BIRD,
    ENTITY_SEA_BIRD,
    ENTITY_BIRD_OF_PREY,
    
    ENTITY_INSECT,
    ENTITY_MOUSE,
    ENTITY_FROG,
    
    ENTITY_LIZARD,
    ENTITY_CAT,
    ENTITY_APE,
    ENTITY_SIZE,
    
    ENTITY_DEAD_ANIMAL,
} ECOSIM_ENTITY;

#define ENTITY_CONSUMPTION (22)

#define ENTITY_GENETICS (ENTITY_SIZE + ENTITY_CONSUMPTION)

typedef enum
{
    SIZE_1_GRAM = 1,
    SIZE_20_GRAM = 20,
    SIZE_400_GRAM = 400,
    SIZE_8_KILOGRAMS = 8000,
    SIZE_160_KILOGRAMS = 160000,
    SIZE_3200_KILOGRAMS = 3200000,
    SIZE_BOUNDARY = 64000000,
}ECOSIM_SIZE;

typedef enum
{
    EFFICIENCY_NOT_APPLICABLE = 0,
    EFFICIENCY_VERY_POOR      = (2 - 1),
    EFFICIENCY_POOR           = (4 - 1),
    EFFICIENCY_MODERATE       = (8 - 1),
    EFFICIENCY_GOOD           = (16 - 1),
    EFFICIENCY_VERY_GOOD      = (32 - 1),
    EFFICIENCY_EXCELLENT      = (64 - 1),
}ECOSIM_CONSUMPTION_EFFICIENCY;

#define ECOSIM_CONSUMPTION_EFFICIENCY_DIVISOR (6.4E+01)

typedef enum
{
    EVENT_EXTINCTION = 0,
    EVENT_SIZE
}ECOSIM_EVENT;

typedef void (ecosim_events)(ECOSIM_EVENT event, e_string information);

void ecosim_init(e_16bit * genetics);

void ecosim_cycle(void);

void ecosim_populate_genetics(e_16bit * seed, e_16bit * genetics);

e_real ecosim_population(ECOSIM_ENTITY entity);

e_16bit ecosim_random(e_16bit * local); /* taken from Noble Ape */

void ecosim_subscribe(ecosim_events * event_subscriber);

#endif /* __ECOSIM_H__ */