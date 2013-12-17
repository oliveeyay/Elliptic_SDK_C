/*
This file is part of the project Elliptic_SDK_C, which is an elliptical cryptographic 
library under GPL license v3.
Copyright (C) 2013  Olivier Goutay

Elliptic_SDK_C is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Elliptic_SDK_C is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Elliptic_SDK_C.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/random_mpzclass.h"
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <gmpxx.h>
#include <time.h>

//#define DEBUG
using namespace std;

//Methode de seed aléatoire dans la même seconde via PIDs
#ifdef WIN32
int get_pid()
{
    return GetCurrentProcessId();
}
#else //linux
int get_pid()
{
    return getpid();
}
#endif

//Methode de seed aléatoire dans la même seconde reposant sur le principe d'un environement multithread
//Plus long mais plus aléatoire ??
int get_seed()
{
    time_t seed;
    size_t count;

    seed = time(NULL);
    while(seed == time(NULL))
    {
       ++count;
    }

    return seed + count;
}

mpz_class random_prime (unsigned int bits)
{
    gmp_randclass generator(gmp_randinit_default);
    srand(time(NULL)+get_pid()); //srand(get_seed());
    unsigned long int seed = rand();

#ifdef DEBUG
    cout << "seed :" << seed << endl;
#endif

    generator.seed(seed);

        // Generate random integer with specified bits
        mpz_class rand = generator.get_z_bits(bits);
        mpz_nextprime(rand.get_mpz_t(), rand.get_mpz_t());

#ifdef DEBUG
        cout << "Generated prime:" <<  rand.get_str() << endl;
#endif

        return rand;
}


mpz_class random_number (unsigned int bits)
{
    gmp_randclass generator(gmp_randinit_default);
    srand(time(NULL)+get_pid()); //srand(get_seed());
    unsigned long int seed = rand();

#ifdef DEBUG
    cout << "seed :" << seed << endl;
#endif
    generator.seed(seed);

        // Generate random integer with specified bits
        mpz_class rand = generator.get_z_bits(bits);

#ifdef DEBUG
        cout << "Generated number:" <<  rand.get_str() << endl;
#endif

        return rand;
}


mpz_class random_number (mpz_class range)
{
    gmp_randclass generator(gmp_randinit_default);
    srand(time(NULL)+get_pid()); //srand(get_seed());
    unsigned long int seed = rand();

#ifdef DEBUG
    cout << "seed :" << seed << endl;
#endif

    generator.seed(seed);

        // Generate random integer with specified bits
        mpz_class rand = generator.get_z_range(range);

#ifdef DEBUG
        cout << "Generated number:" <<  rand.get_str() << endl;
#endif

        return rand;
}
