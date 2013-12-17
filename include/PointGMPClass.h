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

#ifndef POINTGMPCLASS_H
#define POINTGMPCLASS_H
#include <gmpxx.h>
#include "OperatorModel.h"

class PointGMPClass
{
    public:
    //******* Constructor ********//
        PointGMPClass();
        PointGMPClass(const PointGMPClass &_point);
        PointGMPClass(OperatorModel &_op);
        PointGMPClass(mpz_class _x, mpz_class _y, OperatorModel &_op);

    //******* Destructor *********//
        virtual ~PointGMPClass();

    //****** Getters/Setters *****//
        mpz_class Getx() const;
        void Setx(const mpz_class val);
        mpz_class Gety() const;
        void Sety(const mpz_class val);
        OperatorModel& Getop () const;
        OperatorModel* Get_op() const;
        void Setop (OperatorModel &val);
    //******* Functions *********//
        bool operator== (const PointGMPClass &point) const;
     //   PointGMPClass& operator= (const PointGMPClass &point);
        void affiche(std::ostream &flux) const;

        bool isOnCurve();
        std::string toString();

    private:
    //******** Attributes ******//
        mpz_class x;
        mpz_class y;
        OperatorModel *op;
};

std::ostream& operator<< (std::ostream &flux, PointGMPClass const &point);
PointGMPClass& operator+ (const PointGMPClass &p1, const PointGMPClass &p2);
PointGMPClass& operator* (const PointGMPClass &p, const mpz_class k);

#endif // POINTGMPCLASS_H

#ifndef _POINTGMPCLASS_H_INLINES
#define _POINTGMPCLASS_H_INLINES

inline mpz_class PointGMPClass::Getx() const
{
    return x;
}
inline void PointGMPClass::Setx(const mpz_class val)
{
    x = val;
}

inline mpz_class PointGMPClass::Gety() const
{
    return y;
}
inline void PointGMPClass::Sety(const mpz_class val)
{
    y = val;
}

inline OperatorModel& PointGMPClass::Getop() const
{
    return *op;
}
inline void PointGMPClass::Setop(OperatorModel &val)
{
    op = &val;
}
inline OperatorModel* PointGMPClass::Get_op() const
{
    return op;
}
#endif //ndef _POINTGMPCLASS_H_INLINES
