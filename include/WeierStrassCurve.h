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

#ifndef WEIERSTRASSCURVE_H
#define WEIERSTRASSCURVE_H

#include "OperatorModel.h"
#include "PointGMPClass.h"
#include "../hashlib2plus/trunk/src/hashlibpp.h"
#include "random_mpzclass.h"



class WeierStrassCurve : public OperatorModel
{
    public:
    //************ Constructors *************//
        WeierStrassCurve();
        WeierStrassCurve(const WeierStrassCurve &_op);
        WeierStrassCurve(mpz_class _a4, mpz_class _a6,
                          mpz_class _r4, mpz_class _r6,
                           mpz_class _gx, mpz_class _gy);
        WeierStrassCurve(mpz_class _p, mpz_class _r, mpz_class _n,
                          mpz_class _a4, mpz_class _a6,
                           mpz_class _r4, mpz_class _r6,
                            mpz_class _gx, mpz_class _gy);
        virtual WeierStrassCurve* clone() const;
        virtual WeierStrassCurve* create() const;

    //******** Getters & Setters ************//
        void Seta1 (const mpz_class _a1);
        void Seta2 (const mpz_class _a2);
        void Seta3 (const mpz_class _a3);
        void Seta4 (const mpz_class _a4);
        void Seta6 (const mpz_class _a6);
        void Setr4 (const mpz_class _r4);
        void Setr6 (const mpz_class _r6);
        mpz_class Geta1 () const;
        mpz_class Geta2 () const;
        mpz_class Geta3 () const;
        mpz_class Geta4 () const;
        mpz_class Geta6 () const;
        mpz_class Getr4 () const;
        mpz_class Getr6 () const;

    //************ Functions ***************/
        virtual PointGMPClass& oppositePoint (PointGMPClass const &m_P) const ;
        virtual PointGMPClass& doublePoint (PointGMPClass const &m_P) const ;
        virtual PointGMPClass& addDistinct (PointGMPClass const &m_P, PointGMPClass const &m_Q) const ;
        virtual PointGMPClass& add (PointGMPClass const &m_P, PointGMPClass const &m_Q) const ;
        virtual PointGMPClass& mult (PointGMPClass const &m_P, mpz_class const k) const ;
        TabMpz* decompositionPremier();
        mpz_class ordre(PointGMPClass &p1);
        bool checkPrime();
        PointGMPClass& sign_ECDSA(mpz_class hash, mpz_class clefSecrete);
        bool verif_ECDSA(mpz_class r, mpz_class s, PointGMPClass qA, mpz_class hash);


        virtual bool operator== (OperatorModel const &_op) const ;
        virtual bool operator== (WeierStrassCurve const &_op) const ;

    //************ Destructors *************//
        virtual ~WeierStrassCurve();

    private:
    //********** Attributes ***************//
        mpz_class a1;
        mpz_class a2;
        mpz_class a3;
        mpz_class a4;
        mpz_class a6;
        mpz_class r4;
        mpz_class r6;
};

#endif // WEIERSTRASSCURVE_H

#ifndef _WEIERSTRASSCURVE_H_INLINES
#define _WEIERSTRASSCURVE_H_INLINES

inline void WeierStrassCurve::Seta1 (const mpz_class _a1)
{
    a1 = _a1;
}
inline mpz_class WeierStrassCurve::Geta1 () const
{
    return a1;
}

inline void WeierStrassCurve::Seta2 (const mpz_class _a2)
{
    a2 = _a2;
}
inline mpz_class WeierStrassCurve::Geta2 () const
{
    return a2;
}

inline void WeierStrassCurve::Seta3 (const mpz_class _a3)
{
    a3 = _a3;
}
inline mpz_class WeierStrassCurve::Geta3 () const
{
    return a3;
}

inline void WeierStrassCurve::Seta4 (const mpz_class _a4)
{
    a4 = _a4;
}
inline mpz_class WeierStrassCurve::Geta4 () const
{
    return a4;
}

inline void WeierStrassCurve::Seta6 (const mpz_class _a6)
{
    a6 = _a6;
}
inline mpz_class WeierStrassCurve::Geta6 () const
{
    return a6;
}

inline void WeierStrassCurve::Setr4 (const mpz_class _r4)
{
    r4 = _r4;
}
inline mpz_class WeierStrassCurve::Getr4 () const
{
    return r4;
}

inline void WeierStrassCurve::Setr6 (const mpz_class _r6)
{
    r6 = _r6;
}
inline mpz_class WeierStrassCurve::Getr6 () const
{
    return r6;
}

#endif // ndef _WEIERSTRASSCURVE_H_INLINES
