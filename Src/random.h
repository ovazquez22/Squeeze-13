/* -*- c -*- */
/*
 *  File        : random.h
 *
 *  Description : C interface of random number generator.
 *                Translated from Goldberg's SGA code in Pascal.
 * 
 *  Author      : Fernando Lobo, 1996
 */

#ifndef _random_h
#define _random_h

void   randomize( double seed ); /* initializes the random number generator */
int    flip(double probability); /* flip a biased coin */
double random01();               /* returns a random number between 0 and 1 */
int    rnd( int low, int high ); /* returns a random integer in [low..high] */

#endif
