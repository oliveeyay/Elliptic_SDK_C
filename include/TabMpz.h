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

#ifndef TABMPZ_H
#define TABMPZ_H
#include <gmpxx.h> //On inclut la bibliothèque gmp


class TabMpz
{
    public:
        TabMpz();
        virtual ~TabMpz();

        mpz_class *tab;
        int nb;
        bool add(mpz_class n);

    protected:
    private:
};

#endif // TABMPZ_H
