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

#include <gmpxx.h>
#include <iostream>
#include "../include/OperatorModel.h"
#include "../include/PointGMPClass.h"

using namespace std;


OperatorModel::~OperatorModel()
{
}


TabMpz* OperatorModel::decompositionPremier()
{
    TabMpz *tab = new TabMpz();

    if(checkPrime())
    {
        tab->add(n);
        return tab;
    }

    mpz_class max = sqrt(n);

    for(mpz_class i=2; i<max;)
    {
        if(n%i == 0)
        {
            tab->add(i);
            n = n/i;
            max = n;
        }

        else mpz_nextprime(i.get_mpz_t(),i.get_mpz_t());
    }

    return tab;
}

mpz_class OperatorModel::ordre(PointGMPClass &p1)
{
    TabMpz *tab = decompositionPremier();
    if(tab->nb == 1) return tab->tab[0];

    mpz_class m = n;
    PointGMPClass Q;

    for(int i=0; i<tab->nb; i++)
    {
        m = m/tab->tab[i];
        Q = mult(p1, m);

        if(Q.Getx()==0 && Q.Gety()==1)
        {
            Q = mult(Q, tab->tab[i]);
            m = m * tab->tab[i];
        }
    }

    return m;
}

bool OperatorModel::checkPrime()
{
    int prob = mpz_probab_prime_p(n.get_mpz_t(), 50);
    if( prob == 1 || prob == 2 ) return true;
    else return false;
}
