/****************************************************************
 
 main.c
 
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

#include <stdio.h>
#include <time.h>
#include "ecosim.h"

int main(int argc, const char * argv[])
{
    e_16bit genetics[ENTITY_GENETICS];
    e_16bit seed[2];
    e_int current_time = time(NULL);
    e_int time = 0;
    
    if (current_time < 0)
    {
        current_time = 0 - current_time;
    }
    
    seed[0] = current_time & 0xffff;
    
    seed[1] = (current_time >> 16) & 0xffff;
    
    ecosim_populate_genetics(seed, genetics);
    
    ecosim_init(genetics);
    
    while (time < 100)
    {
        ecosim_cycle();
        time ++;
    }
    
    return 0;
}
