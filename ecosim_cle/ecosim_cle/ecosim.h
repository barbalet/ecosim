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

typedef double e_real;
typedef long   e_int;

typedef enum
{
    ENTITY_NONE = -1,
    ENTITY_GRASS = 0,
    ENTITY_BUSH,
    ENTITY_TREE,
    
    ENTITY_SEED_EATING_BIRD,
    ENTITY_INSECT_EATING_BIRD,
    ENTITY_SEA_BIRD,
    ENTITY_BIRD_OF_PREY,
    
    ENTITY_FISH,
    
    ENTITY_INSECT,
    ENTITY_MOUSE,
    ENTITY_FROG,
    
    ENTITY_LIZARD,
    ENTITY_CAT,
    ENTITY_APE,
    ENTITY_SIZE,
    
    ENTITY_SUN
} ECOSIM_ENTITY;

/*
 
 Apes eat Bushes and Fish
 
 Birds of Prey eats Fish, Frogs, Mice and the Dead Animals
 
 Cats eat Apes, Birds of Prey, Insect Eating Birds. Sea Birds and Lizards
 
 Frogs eat Insects
 
 Insect Eating Birds eat Insects
 
 Insects eat Bushes, Grasses, Trees and the Dead Animals
 
 Lizards eat Insect Eating Birds, Insects, Insect Eating Birds (eggs)
 
 Mice at Grass
 
 Sea Birds eat Fish
 
 Seed Eating Birds eat Bushes
 
 */

void ecosim_init(void);

void ecosim_cycle(void);

e_int ecosim_population(ECOSIM_ENTITY entity);

#endif /* __ECOSIM_H__ */