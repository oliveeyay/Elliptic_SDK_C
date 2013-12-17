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

#ifndef OPERATORMODEL_H
#define OPERATORMODEL_H
#include<gmpxx.h>
#include "TabMpz.h"

class PointGMPClass;

class OperatorModel
{
    public:
        //*********** Constructor ***************//
        virtual OperatorModel* clone() const = 0;
        virtual OperatorModel* create() const = 0;

        //******* Pure virtual Functions ********//
        virtual PointGMPClass& oppositePoint (const PointGMPClass &m_P) const = 0;
        virtual PointGMPClass& doublePoint (const PointGMPClass &m_P) const = 0;
        virtual PointGMPClass& addDistinct (const PointGMPClass &m_P, const PointGMPClass &m_Q) const = 0;
        virtual PointGMPClass& add (const PointGMPClass &m_P, const PointGMPClass &m_Q) const = 0;
        virtual PointGMPClass& mult (const PointGMPClass &m_P, const mpz_class k) const = 0;

        virtual bool operator== (const OperatorModel &_op) const = 0;

        //*********** Functions ****************//

        bool checkPrime();
        mpz_class ordre(PointGMPClass &p1);
        TabMpz* decompositionPremier();

        //****** Getters & Setters *************//
        void Setp (const mpz_class _p);
        void Setr (const mpz_class _r);
        void Setn (const mpz_class _n);
        void Setgx (const mpz_class _gx);
        void Setgy (const mpz_class _gy);
        mpz_class Getp () const;
        mpz_class Getr () const;
        mpz_class Getn () const;
        mpz_class Getgx () const;
        mpz_class Getgy () const;
        int Getid () const;

        //************* Destructor *************//
        virtual ~OperatorModel();

    protected:
        //********** Attributes ***************//
        mpz_class p;
        mpz_class r;
        mpz_class n;
        mpz_class gx;
        mpz_class gy;
        int id;
};

#endif // OPERATORMODEL_H

#ifndef _OPERATORMODEL_H_INLINES
#define _OPERATORMODEL_H_INLINES

inline void OperatorModel::Setp (const mpz_class _p)
{
    p = _p;
}

inline void OperatorModel::Setr (const mpz_class _r)
{
    r = _r;
}

inline void OperatorModel::Setn (const mpz_class _n)
{
    n = _n;
}

inline mpz_class OperatorModel::Getp () const
{
    return p;
}

inline mpz_class OperatorModel::Getn () const
{
    return n;
}

inline mpz_class OperatorModel::Getr () const
{
    return r;
}

inline int OperatorModel::Getid () const
{
    return id;
}

inline void OperatorModel::Setgx (const mpz_class _gx)
{
    gx = _gx;
}
inline mpz_class OperatorModel::Getgx () const
{
    return gx;
}

inline void OperatorModel::Setgy (const mpz_class _gy)
{
    gy = _gy;
}
inline mpz_class OperatorModel::Getgy () const
{
    return gy;
}
#endif //ndef _OPERATORMODEL_H_INLINES
