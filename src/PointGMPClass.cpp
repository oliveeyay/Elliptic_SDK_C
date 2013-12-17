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

#include "../include/PointGMPClass.h"
#include "../include/WeierStrassCurve.h"
#include <iostream>
#include <gmpxx.h>


using namespace std;


PointGMPClass::PointGMPClass() : x(0), y(1), op(NULL)
{
}

PointGMPClass::PointGMPClass(const PointGMPClass &_point)
{
    this->x = _point.x;
    this->y = _point.y;
    if(_point.op)
        this->op = _point.op->clone();
    else
        this->op = NULL;
}

PointGMPClass::PointGMPClass(OperatorModel& _op)
: x(0), y(1)
{
    if(&_op)
        this->op = _op.clone();
    else
        this->op = NULL;
}

PointGMPClass::PointGMPClass(mpz_class _x, mpz_class _y, OperatorModel& _op)
{
    this->x = _x;
    this->y = _y;
    if(&_op)
        this->op = _op.clone();
    else
        this->op = NULL;
}

bool PointGMPClass::operator== (PointGMPClass const &point) const
{
    if( (this->x == point.x) && (this->y == point.y) )
        return true;
    else
        return false;
}
/*
PointGMPClass& PointGMPClass::operator= (PointGMPClass const &point)
{
    PointGMPClass temp(point);
    swap(temp.x, this->x);
    swap(temp.y, this->y);
    swap(temp.op, this->op);
    delete &temp;
    return *this;
}*/

PointGMPClass& operator+ (const PointGMPClass &p1, const PointGMPClass &p2)
{
    if( p1.Getop() == p2.Getop() )
        return p1.Getop().add(p1, p2);
    else
        return *( new PointGMPClass(p1) );
}

PointGMPClass& operator* (const PointGMPClass &p, const mpz_class k)
{
    return p.Getop().mult(p, k);
}

void PointGMPClass::affiche (ostream &flux) const
{
    flux << "x->" << x << "\ty->" << y << endl;
}

bool PointGMPClass::isOnCurve()
{
    //vérifier que le point appartient à la courbe ==> y^2 = x^3 + a4 * x + a6
    mpz_class l = y * y;
    mpz_class r = x*x*x;

    WeierStrassCurve *temp = (WeierStrassCurve *) op;
    r = r + (temp->Geta4() * x) + temp->Geta6();

    l = l % op->Getp();
    r = r % op->Getp();

    if (l == r) return true;
    else return false;
}

string PointGMPClass::toString()
{
    string resultat;

    resultat += "x=" ;
    resultat += x.get_str() ;
    resultat += '\n';
    resultat += "y=" ;
    resultat += y.get_str();
    resultat += '\0';

    return resultat;
}

PointGMPClass::~PointGMPClass()
{
    delete op;
}

ostream& operator<< (ostream &flux, PointGMPClass const &point)
{
    point.affiche(flux);
    return flux;
}
