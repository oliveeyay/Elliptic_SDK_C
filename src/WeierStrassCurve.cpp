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

#include "../include/WeierStrassCurve.h"
#include "../include/PointGMPClass.h"
#include "../include/OperatorModel.h"
#include "../include/TabMpz.h"
#include <gmpxx.h>
#include<iostream>

using namespace std;


//////////////////////////////////////////////////////////
//                  Constructors                        //
/////////////////////////////////////////////////////////

WeierStrassCurve::WeierStrassCurve()
: a1(0), a2(0), a3(0), a4(0), a6(0), r4(0), r6(0)
{
    p = 0;
    r = 0;
    n = 0;
    id = 1612671;
}

WeierStrassCurve::WeierStrassCurve(const WeierStrassCurve &_op)
{
    p = _op.p;
    r = _op.r;
    n = _op.n;
    a1 = _op.a1;
    a2 = _op.a2;
    a3 = _op.a3;
    a4 = _op.a4;
    a6 = _op.a6;
    r4 = _op.r4;
    r6 = _op.r6;
    gx = _op.gx;
    gy = _op.gy;
    id = _op.id;
}

WeierStrassCurve::WeierStrassCurve(mpz_class _a4, mpz_class _a6,
                                    mpz_class _r4, mpz_class _r6,
                                     mpz_class _gx, mpz_class _gy)
: a1(0), a2(0), a3(0)
{
    p = 0;
    r = 0;
    n = 0;
    a4 = _a4;
    a6 = _a6;
    r4 = _r4;
    r6 = _r6;
    gx = _gx;
    gy = _gy;
    id = 1612671;
}


WeierStrassCurve::WeierStrassCurve(mpz_class _p, mpz_class _r, mpz_class _n,
                                    mpz_class _a4, mpz_class _a6,
                                     mpz_class _r4, mpz_class _r6,
                                      mpz_class _gx, mpz_class _gy)
: a1(0), a2(0), a3(0)
{
    p = _p;
    r = _r;
    n = _n;
    a4 = _a4;
    a6 = _a6;
    r4 = _r4;
    r6 = _r6;
    gx = _gx;
    gy = _gy;
    id = 1612671;
}

WeierStrassCurve* WeierStrassCurve::clone () const
{
    return new WeierStrassCurve(*this);
}

WeierStrassCurve* WeierStrassCurve::create() const
{
    return new WeierStrassCurve();
}


//////////////////////////////////////////////////////////
//                  Functions                           //
//////////////////////////////////////////////////////////

/**Fonction qui permet d'additionner 2 points qui sont différents l'un de l'autre
Prend en paramètre les 2 points en question que l'on veut additionner.
Retourne le résultat de l'addition (calcul sur une courbe elliptique de type weirstrass)
**/
PointGMPClass& WeierStrassCurve::addDistinct (PointGMPClass const &m_P, PointGMPClass const &m_Q) const
{
    mpz_class lambda, xR, yR;

    mpz_class denom(m_P.Getx() - m_Q.Getx());
    mpz_invert(denom.get_mpz_t(), denom.get_mpz_t(), p.get_mpz_t());
    //denom = denom % p;
    mpz_mod(denom.get_mpz_t(), denom.get_mpz_t(), p.get_mpz_t());
    lambda = (m_P.Gety() - m_Q.Gety())*denom;

    xR = lambda*lambda - m_P.Getx() - m_Q.Getx();
    yR = m_P.Gety() + lambda * (xR - m_P.Getx());
    yR = -yR;

    //xR = xR % p;
    //yR = yR % p;
    mpz_mod(xR.get_mpz_t(), xR.get_mpz_t(), p.get_mpz_t());
    mpz_mod(yR.get_mpz_t(), yR.get_mpz_t(), p.get_mpz_t());
    return *( new PointGMPClass( xR, yR, m_P.Getop() ) );
}

/**Fonction qui permet de doubler un point
Prend en paramètre le point en question que l'on veut doubler.
Retourne le résultat de l'opération (calcul sur une courbe elliptique de type weirstrass)
**/
PointGMPClass& WeierStrassCurve::doublePoint (PointGMPClass const &m_P) const
{
    mpz_class lambda, xR, yR;

    mpz_class denom(2*m_P.Gety());
    mpz_invert(denom.get_mpz_t(), denom.get_mpz_t(), p.get_mpz_t());
    //denom = denom % p;
    mpz_mod(denom.get_mpz_t(), denom.get_mpz_t(), p.get_mpz_t());
    lambda = (3 *m_P.Getx() *m_P.Getx() + a4)*denom;
    xR = lambda*lambda - 2 * m_P.Getx();
    yR = m_P.Gety() + lambda * (xR - m_P.Getx());
    yR = -yR;

    //xR = xR % p;
    //yR = yR % p;
    mpz_mod(xR.get_mpz_t(), xR.get_mpz_t(), p.get_mpz_t());
    mpz_mod(yR.get_mpz_t(), yR.get_mpz_t(), p.get_mpz_t());
    return *( new PointGMPClass( xR, yR, m_P.Getop() ) );
}

/**Fonction qui permet de calculer l'inverse d'un point
Prend en paramètre le point en question que l'on veut inverser.
Retourne le résultat de l'opération (calcul sur une courbe elliptique de type weirstrass)
**/
PointGMPClass& WeierStrassCurve::oppositePoint (PointGMPClass const &m_P) const
{
    mpz_class yR;
    yR = -m_P.Gety();

    //yR = yR % p;
    mpz_mod(yR.get_mpz_t(), yR.get_mpz_t(), p.get_mpz_t());

    return *( new PointGMPClass( m_P.Getx(), yR, m_P.Getop() ) );
}


/**Fonction qui permet d'additionner deux points (c'est celle là qu'on doit appeler coté utilisateur)
Prend en paramètre les points en question que l'on veut additionner.
Retourne le résultat de l'opération (calcul sur une courbe elliptique de type weirstrass)
**/
PointGMPClass& WeierStrassCurve::add (PointGMPClass const &m_P, PointGMPClass const &m_Q) const
{
    if(m_P.Getx() == 0 && m_P.Gety() == 1)
        return *( new PointGMPClass(m_Q) );
    if(m_Q.Getx() == 0 && m_Q.Gety() == 1)
        return *( new PointGMPClass(m_P) );

    if(m_P == oppositePoint(m_Q) )
        return *( new PointGMPClass(m_P.Getop()) );

    if(m_P == m_Q)
        return doublePoint(m_P);
    else
        return addDistinct(m_P, m_Q);
}

/**Fonction qui permet de multiplier un point par un entier (de type mpz_class)
Prend en paramètre le point en question et l'entier.
Retourne le résultat de l'opération (calcul sur une courbe elliptique de type weirstrass)
L'algorithme utilisé est "square and multiply"
**/
PointGMPClass& WeierStrassCurve::mult(PointGMPClass const &m_P, mpz_class const k) const
{
    if(k == 0)//si k=0 on retourne le point neutre
    {
        return *( new PointGMPClass(0, 1, m_P.Getop() ) );
    }

    else if(k==1)//si k=1 on retourne le point lui même
    {
        return *( new PointGMPClass(m_P) );
    }

    PointGMPClass res = *( new PointGMPClass(m_P) );
    mpz_class i(k);

    while(i>1)
    {
        res = res + res;
        if(i%2 == 0)
            return res*(i/2);
        else
            return res*((i-1)/2) + m_P;
    }

    return res;
}

/**Fonction qui vérifie que les points sont sur la même courbe (surcharge)
Prend en paramètre le point en question que l'on veut tester.
Retourne le booléen qui indique si oui ou non les points sont égaux
**/
bool WeierStrassCurve::operator== (OperatorModel const &_op) const
{
    if( (id == _op.Getid()) && (p == _op.Getp()) && (r == _op.Getr()) && (n == _op.Getn()) )
        return ( *(this) == (const WeierStrassCurve&)_op);
    else
        return false;
}

/**Fonction qui vérifie que les points sont égaux en testant toutes leurs coordonnées (surcharge)
Prend en paramètre le point en question que l'on veut tester.
Retourne le booléen qui indique si oui ou non les points sont égaux
**/
bool WeierStrassCurve::operator== (const WeierStrassCurve &_op) const
{
    if((a1 == _op.a1) && (a2 == _op.a2) && (a3 == _op.a3)
       && (a4 == _op.a4) && (a6 == _op.a6)
       && (r4 == _op.r4) && (r6 == _op.r6)
       && (gx == _op.gx) && (gy == _op.gy) )
       return true;
    else
        return false;
}

/**Fonction qui permet de décomposer le nombre n en entiers premiers
Retourne un tableau contenant le nombre d'entiers premiers trouvés ainsi que ces mêmes entiers
**/
TabMpz* WeierStrassCurve::decompositionPremier()
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

/**Fonction qui permet de calculer l'ordre d'un point
Prend en paramètre le point en question que l'on veut tester.
Retourne la valeur de l'ordre
**/
mpz_class WeierStrassCurve::ordre(PointGMPClass &p1)
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

/**Fonction qui vérifie si le nombre n de la courbe est premier ou non
Retourne le booléen qui indique si oui ou non le nombre n est premier
**/
bool WeierStrassCurve::checkPrime()
{
    int prob = mpz_probab_prime_p(n.get_mpz_t(), 50);
    if( prob == 1 || prob == 2 ) return true;
    else return false;
}

/**Fonction qui permet de calculer une signature
Prend en paramètre le hash que l'on veut signer ainsi que la clef secrète de l'utilisateur
Retourne un point qui aura comme coordonée x le r, et comme coordonée y le s (les deux chiffres de la signature)
**/
PointGMPClass& WeierStrassCurve::sign_ECDSA(mpz_class hash, mpz_class clefSecrete)
{
    //partie signature
    PointGMPClass generateur(gx, gy, *this);//le point générateur qui servira à calculer notre clef secrète

    mpz_class k = random_prime(225);//on choisit un k random

    PointGMPClass rPoint = generateur * k;//on multiplie le générateur par k
    mpz_class r = rPoint.Getx();

    mpz_invert(k.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());
    mpz_class parenthese = (hash + (r*clefSecrete)) % n;
    mpz_class s = (k * parenthese) % n;

    return *( new PointGMPClass(r, s, *this ) );
}

/**Fonction qui permet de vérifier une signature
Prend en paramètre le r, le s (les deux chiffres de la signature), puis la clef publique de celui qui a signé, et pour finir
le hash de la chaîne de caractère qui a été signée
Retourne un booléen qui indique si oui ou non la signature est vérifiée
**/
bool WeierStrassCurve::verif_ECDSA(mpz_class r, mpz_class s, PointGMPClass qA, mpz_class hash)
{
    PointGMPClass generateur(gx, gy, *this);//le point générateur qui servira à calculer notre clef secrète

    mpz_class w;
    mpz_invert(w.get_mpz_t(), s.get_mpz_t(), n.get_mpz_t());

    mpz_class u1 = (hash*w) % n;
    mpz_class u2 = (r*w) % n;

    PointGMPClass res = generateur*u1 +  qA*u2;

    if(r == res.Getx()) return true;
    else return false;
}



//////////////////////////////////////////////////////////
//                  Destructor                          //
//////////////////////////////////////////////////////////
WeierStrassCurve::~WeierStrassCurve()
{
}
