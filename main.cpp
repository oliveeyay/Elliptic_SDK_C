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

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <gmpxx.h>
#include <string.h>
#include "include/PointGMPClass.h"
#include "include/OperatorModel.h"
#include "include/WeierStrassCurve.h"
#include "include/LibTCPWindows.h"
#include "include/random_mpzclass.h"
#include "hashlib2plus/trunk/src/hashlibpp.h"


/*
#define P 8884933102832021670310856601112383279507496491807071433260928721853918699951
#define N 8884933102832021670310856601112383279454437918059397120004264665392731659049
#define A4 2481513316835306518496091950488867366805208929993787063131352719741796616329
#define A6 4387305958586347890529260320831286139799795892409507048422786783411496715073
#define R4 5473953786136330929505372885864126123958065998198197694258492204115618878079
#define R6 5831273952509092555776116225688691072512584265972424782073602066621365105518
#define GX 7638166354848741333090176068286311479365713946232310129943505521094105356372
#define GY 762687367051975977761089912701686274060655281117983501949286086861823169994
#define R 8094458595770206542003150089514239385761983350496862878239630488323200271273
*/

void test (OperatorModel& curve);
void ecdsaLocal (OperatorModel& curve);
void elGamal (OperatorModel& curve);
void elGamalString (OperatorModel& curve);
void addition (OperatorModel& curve);
void multiplication (OperatorModel& curve);
void alice_diffie_hellman(OperatorModel &curve);
void bob_diffie_hellman(OperatorModel &curve);
void alice_diffie_hellman_STS(OperatorModel &curve);
void bob_diffie_hellman_STS(OperatorModel &curve);

using namespace std;

int main ()
{
    char choix;
    bool end_of_program = false;

    OperatorModel *curve;
    mpz_class *P = new mpz_class("8884933102832021670310856601112383279507496491807071433260928721853918699951", 10);
    mpz_class *R = new mpz_class("8094458595770206542003150089514239385761983350496862878239630488323200271273", 10);
    mpz_class *N = new mpz_class("8884933102832021670310856601112383279454437918059397120004264665392731659049", 10);
    mpz_class *A4 = new mpz_class("2481513316835306518496091950488867366805208929993787063131352719741796616329", 10);
    mpz_class *A6 = new mpz_class("4387305958586347890529260320831286139799795892409507048422786783411496715073", 10);
    mpz_class *R4 = new mpz_class("5473953786136330929505372885864126123958065998198197694258492204115618878079", 10);
    mpz_class *R6 = new mpz_class("5831273952509092555776116225688691072512584265972424782073602066621365105518", 10);
    mpz_class *GX = new mpz_class("7638166354848741333090176068286311479365713946232310129943505521094105356372", 10);
    mpz_class *GY = new mpz_class("762687367051975977761089912701686274060655281117983501949286086861823169994", 10);
    curve = new WeierStrassCurve (*P, *R, *N, *A4, *A6,
                                  *R4, *R6, *GX, *GY);

    do
    {
        system("cls");
        cout << "- s : Changer les paramètres de la courbe" << endl;
        cout << "- a : Operation d'addition" << endl;
        cout << "- m : Operation de multiplication" << endl;
        cout << "- d : Diffie Hellman (Alice)" << endl;
        cout << "- h : Diffie Hellman (Bob) [Serveur]" << endl;
        cout << "- l : Diffie Hellman STS(Alice)" << endl;
        cout << "- o : Diffie Hellman STS(Bob) [Serveur]" << endl;
        cout << "- e : ElGamal (Point to point)" << endl;
        cout << "- g : ElGamal (String)" << endl;
        cout << "- i : ECDSA (local)" << endl;
        cout << "- t : TEST (pour le code en test)" << endl;
        cout << "- q : quittez." << endl;

        cin >> choix;
        switch(choix)
        {
            case 's' :
                        test(*curve);
                        break;
            case 'a' :
                        addition(*curve);
                        break;
            case 'm' :
                        multiplication(*curve);
                        break;
            case 'd' :
                        alice_diffie_hellman(*curve);
                        break;
            case 'h' :
                        bob_diffie_hellman(*curve);
                        break;
            case 'l' :
                        alice_diffie_hellman_STS(*curve);
                        break;
            case 'o' :
                        bob_diffie_hellman_STS(*curve);
                        break;
            case 'e' :
                        elGamal(*curve);
                        break;
            case 'g' :
                        elGamalString(*curve);
                        break;
            case 'i' :
                        ecdsaLocal(*curve);
                        break;
            case 't' :
                        test(*curve);
                        break;
            case 'q' :
                        cout << "Fin du programme" << endl;
                        end_of_program = true;
                        break;
            default :
                        break;
        }
    }while(!end_of_program);
    delete curve;
    return EXIT_SUCCESS;
}


void test(OperatorModel& curve)
{
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);//le point générateur qui servira à calculer notre clef secrète

    //calcul chez Alice
    mpz_class clefSecreteAlice = random_prime(225);//la clef secrète n'appartient qu'à Alice
    PointGMPClass pA = generateur * clefSecreteAlice;//pA est la clef publique que l'on va partager à Bob

    //calcul chez Bob
    mpz_class clefSecreteBob = random_prime(225);//la clef secrète n'appartient qu'à Bob
    PointGMPClass pB = generateur * clefSecreteBob;//pB est la clef publique que l'on va partager à alice

    //on partage donc en clair pB et pA (pas sécurisé mais pas de de serveur dans ce cas là)

    //calcul chez Alice
    mpz_class kA = random_prime(225);//un chiffre random pour Alice
    PointGMPClass middleA = generateur * kA;//middleA est la clef intermédiaire d'Alice

    //Calcul chez Bob
    mpz_class kB = random_prime(225);//un chiffre random pour Bob
    PointGMPClass middleB = generateur * kB;//middleB est la clef intermédiaire de Bob

    hashwrapper *myWrapper = new sha256wrapper(); // sha 256
    //calcul chez Alice
    std::string hashA = myWrapper->getHashFromString(middleA.toString()); // sha 256
    mpz_class zA(hashA, 62);// le haché de Alice

    //calcul chez Bob
    std::string hashB = myWrapper->getHashFromString(middleB.toString()); // sha 256
    mpz_class zB(hashB, 62);// le haché de Alice

    WeierStrassCurve c = (WeierStrassCurve&)curve;
    //calcul chez Alice
    PointGMPClass signatureA = c.sign_ECDSA(zA, clefSecreteAlice);
    //on envoie donc middleA et signature A à Bob

    //calcul chez bob ==> on vérifie la signature avant de continuer
    bool resA = c.verif_ECDSA(signatureA.Getx(), signatureA.Gety(), pA, zA);
    if (resA) cout << "Signature ok d'Alice" << endl;
    else cout << "Probleme de signature d'Alice" << endl;

    //si tout est ok, on continue et on calcule chez Bob
    PointGMPClass signatureB = c.sign_ECDSA(zB, clefSecreteBob);
    //on envoie donc middleB et la signature de B à Alice

    //on vérifie la signature de Bob chez Alice
    bool resB = c.verif_ECDSA(signatureB.Getx(), signatureB.Gety(), pB, zB);
    if (resB) cout << "Signature ok de Bob" << endl;
    else cout << "Probleme de signature de Bob" << endl;


    //tout est vérifié chacun calcule leur clef partagée finale
    PointGMPClass finBob = middleA * kB;
    PointGMPClass finAlice = middleB * kA;

    if(finBob==finAlice) cout << "Diffie Hellman STS complet" << endl;
    else cout << "Probleme Diffie Hellman STS" << endl;

    cout << "fin Bob " << finBob << endl;
    cout << "fin Alice " << finAlice << endl;

    system("pause");
}


void ecdsaLocal(OperatorModel& curve)
{
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);//le point générateur qui servira à calculer notre clef secrète

    hashwrapper *myWrapper = new sha256wrapper(); // sha 256
    std::string hash1 = myWrapper->getHashFromString("bonjour l'esil"); // sha 256
    mpz_class z(hash1, 62);// le haché

    mpz_class clefSecrete = random_prime(225);//la clef secrète n'appartient qu'à Alice
    PointGMPClass qA = generateur * clefSecrete;//qA est la clef partagé entre bob et alice


    WeierStrassCurve c = (WeierStrassCurve&)curve;
    PointGMPClass signature = c.sign_ECDSA(z, clefSecrete);

    //partie verification
    bool res = c.verif_ECDSA(signature.Getx(), signature.Gety(), qA, z);
    if (res) cout << "Signature ok" << endl;
    else cout << "Probleme de signature" << endl;

    system("pause");
}


void addition (OperatorModel& curve)
{
    string pX, pY, qX, qY;

    cout << "Entrez le point P :" << endl << "x->";
    cin >> pX;
    cout << "y->";
    cin >> pY;
    cout << "Entrez le point Q :" << endl << "x->";
    cin >> qX;
    cout << "y->";
    cin >> qY;
    cout << endl << "R = P + Q" << endl;

    PointGMPClass p(mpz_class(pX,10), mpz_class(pY, 10), curve);
    PointGMPClass q(mpz_class(qX,10), mpz_class(qY, 10), curve);
    PointGMPClass r;

    r = p + q;
    cout << "R : " << r << endl;
    cin.ignore();
    getchar();
}

void elGamal(OperatorModel& curve)
{
    //Déroulement de El Gamal
    PointGMPClass generateur(curve.Getgx(),curve.Getgy(), curve);

    mpz_class clefSecrete = random_prime(225);//on alloue une clef secrete aleatoire
    PointGMPClass q = generateur * clefSecrete;//on calcule notre clef publique a partir du generateur

    mpz_class test("10", 10);
    PointGMPClass m = generateur * test;//le point a retrouver

    mpz_class k = random_prime(225);//on prend un k random

    PointGMPClass m1 = generateur * k;//on calcule m1
    PointGMPClass m2 = q * k;//on calcule m2
    m2 = m + m2;//on finit de calculer m2

    //ici c'est la partie décryptage
    PointGMPClass temp = m1 * clefSecrete;

    temp = temp.Getop().oppositePoint(temp);
    PointGMPClass messageTraduit = temp + m2;

    if(m==messageTraduit) cout << "El Gamal fonctionnel" << endl;
    else cout << "El Gamal non fonctionnel" << endl;

    cout << "Point a retrouver " << m << "\n";
    cout << "Resultat El gamal " << messageTraduit << "\n";


    if(m.isOnCurve()) cout << "m sur la courbe" << endl;
    else cout << "m pas sur la courbe" << endl;
    if(messageTraduit.isOnCurve()) cout << "messageTraduit sur la courbe" << endl;
    else cout << "messageTraduit pas sur la courbe" << endl;

    system("pause");
}


void elGamalString(OperatorModel& curve)
{
    //Déroulement de El Gamal
    //Cryptage
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);//le point générateur qui servira à calculer notre clef secrète


    mpz_class clefSecrete = random_prime(225);
    PointGMPClass h = generateur * clefSecrete;//on calcule notre clef secrète, qui nous servira à crypter les messages (h)

    mpz_class m("bonjour lesil", 62);

    mpz_class k = random_prime(225);//on choisit un k random

    PointGMPClass C1 = generateur * k;//on multiplie le générateur par k
    PointGMPClass tmp = h * k;
    mpz_class C2 = m + tmp.Getx();//on multiplie notre clef secrète par k

    //Décryptage
    PointGMPClass temp = C1 * clefSecrete;//on multiplie C1 par notre clef secrète
    mpz_class messageTraduit = C2 - temp.Getx();//et on obtient le message en ajoutant m2 avec l'opposé de temp
    messageTraduit = messageTraduit%curve.Getp();

    if(m==messageTraduit) cout << "El Gamal fonctionnel" << endl;
    else cout << "El Gamal non fonctionnel" << endl;

    cout << "Message a retrouver " << m  << " " << mpz_get_str(NULL, 62, m.get_mpz_t()) << "\n";
    cout << "Resultat El gamal " << messageTraduit << " " << mpz_get_str(NULL, 62, messageTraduit.get_mpz_t()) << "\n";

    system("pause");
}

void multiplication (OperatorModel& curve)
{

}

void alice_diffie_hellman(OperatorModel& curve)
{
    int socketCree,
        portDest = 1234;
    char dest[15] = "127.0.0.1";
    char donneesRecues[1000];
    string message;

    system("cls");

    if(InitialiseWindowsSocket() == -1)
    {
        cout << "Echec initialisation des sockets windows" << endl;
        system("pause");
        return;
    }

    /************Création socket********************/
    socketCree = CreerSocketTCP();
    if(socketCree == -1)
    {
        cout << "Echec creation d'un socket" << endl;
        system("pause");
        return ;
    }
    cout << "Socket Alice cree" << socketCree << endl;

/* //Décommenter pour test non local
    cout << "Entrez l'adresse du Bob distant : ";
    cin.ignore();
    cin >> dest;
    */
  /*********************Connexion au serveur*************************/
    if(ConnexionServeur(socketCree, dest, portDest) == -1)
    {
      cout << "Echec connexion au serveur : " << dest << endl;
      system("pause");
      return ;
    }
    cout << "Connexion a Bob distant reussie sur : " << dest <<endl;

    /**************************************************************/
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);

    mpz_class alice = random_prime(50);

    PointGMPClass pA, pK;

    pA = generateur * alice;

    /********* Envoi de pB ****************/
    if(EnvoyerDonnees(socketCree, pA.toString().c_str() ) == -1 )
    {
        cout << "Echec envoi" << endl;
        system("pause");
        return ;
    }

    /*********** Reception de pB ***************/
    int car_rec = RecevoirDonnees(socketCree, donneesRecues, sizeof(donneesRecues));
    if(car_rec == -1)
    {
        cout << "echec reception" << endl;
        system("pause");
        return ;
    }
    donneesRecues[car_rec] = '\0';
    //Récupération du point à partir des donnees reçues
    string data(donneesRecues);
    string x,y;
    int posX, posY, posEndl, posEnds;
    posX = data.find("x=")+2;
    posY = data.find("y=")+2;
    posEndl = data.find('\n');
    posEnds = data.find('\0');

    x = data.substr(posX, posEndl - posX );
    y = data.substr(posY, posEnds - posY );

    PointGMPClass pB(mpz_class(x.c_str(), 10), mpz_class(y.c_str(), 10), curve);

    pK = pB * alice;

    cout << pK << endl;

    if(pK.isOnCurve()) cout << "on curve" << endl;
    else cout << "problem : not on curve" << endl;
    PointGMPClass t = pK * curve.Getn();
    cout << t << endl;

    cout << "Message envoye" << endl;
    FermerSocket(socketCree);
    cout << "Deconnexion" << endl;

    system("pause");
}

void bob_diffie_hellman(OperatorModel& curve)
{
    int socketCree,
        socketCommunic,
        Port = 1234;
    char donneesRecues[1000];

    system("cls");

    if(InitialiseWindowsSocket() == -1)
    {
        cout << "Echec initialisation des sockets windows" << endl;
        system("pause");
        return ;
    }

    /************* Création socket **********************/
    socketCree = CreerSocketTCP();
    if(socketCree == -1)
    {
        cout << "Echec creation d'un socket" << endl;
        system("pause");
        return ;
    }
    cout << "Le socket bob (serveur) cree est :" << socketCree << endl;

    /************** Création serveur *******************/
    if(CreerServeur(socketCree,Port) == -1 )
    {
        cout << "Echec creation serveur" << endl;
        system("pause");
        return ;
    }
    cout << "En attente d'une connexion ..." << endl;

    /********** Mise en écoute du serveur *************/
    socketCommunic = AttenteConnexionClient(socketCree);
    if(socketCommunic == -1 )
    {
        cout << "Erreur d'attente" << endl;
        system("pause");
        return ;
    }
    cout << "Connexion entrante : Le socket utilise pour cette session est : " << socketCommunic << endl;

    /*********** Reception de pA ***************/
    int car_rec = RecevoirDonnees(socketCommunic, donneesRecues, sizeof(donneesRecues));
    if(car_rec == -1)
    {
        cout << "echec reception" << endl;
        system("pause");
        return ;
    }
    donneesRecues[car_rec] = '\0';
    //Récupération du point à partir des donnees reçues
    string data(donneesRecues);
    string x,y;
    int posX, posY, posEndl, posEnds;
    posX = data.find("x=")+2;
    posY = data.find("y=")+2;
    posEndl = data.find('\n');
    posEnds = data.find('\0');

    x = data.substr(posX, posEndl - posX );
    y = data.substr(posY, posEnds - posY );

    mpz_class bob = random_prime(50);
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);
    PointGMPClass pA( mpz_class(x.c_str(), 10), mpz_class(y.c_str(), 10), curve), pB, pK;

    pB = generateur * bob;

    /************* Envoi de pB ***************/
    if(EnvoyerDonnees(socketCommunic, pB.toString().c_str() ) == -1)
    {
        cout << "Echec envoi" << endl;
        system("pause");
        return ;
    }

    pK = pA * bob;

    cout << pK << endl;

    if(pK.isOnCurve()) cout << "on curve" << endl;
    else cout << "problem : not on curve" << endl;
    PointGMPClass t = pK * curve.Getn();
    cout << t << endl;

    cin.ignore();
    FermerSocket(socketCommunic);
    cout << "Deconnexion" << endl;
    FermerSocket(socketCree);
    cout << "Fin serveur" << endl;
    system("pause");
}


/**Fonction qui permet d'ennvoyer un PointGMPCLass, nécessite qu'un programme soit en attente d'abord
Prend en paramètre le string du point (point.toString() tout simplement)
**/
void envoyerPointGMPSocket(string str)
{
    int socketCree,
        portDest = 1234;
    char dest[15] = "127.0.0.1";
    string message;

    if(InitialiseWindowsSocket() == -1)
    {
        cout << "Echec initialisation des sockets windows" << endl;
        system("pause");
        return;
    }

    /************Création socket********************/
    socketCree = CreerSocketTCP();
    if(socketCree == -1)
    {
        cout << "Echec creation d'un socket" << endl;
        system("pause");
        return ;
    }
    cout << "Socket Alice cree" << socketCree << endl;

  /*********************Connexion au serveur*************************/
    if(ConnexionServeur(socketCree, dest, portDest) == -1)
    {
      cout << "Echec connexion au serveur : " << dest << endl;
      system("pause");
      return ;
    }
    cout << "Connexion a Bob distant reussie sur : " << dest <<endl;


    /********* Envoi de point ****************/
    if(EnvoyerDonnees(socketCree, str.c_str() ) == -1 )
    {
        cout << "Echec envoi" << endl;
        system("pause");
        return ;
    }
    cout << "Donnee envoyee " << endl;
    FermerSocket(socketCree);
}


/**Fonction qui permet de mettre en attente le programme afin de recevoir un PointGMPClass et le reconstituer
Prend en paramètre la courbe sur laquelle est définie le point
Retourne le point reçu
**/
PointGMPClass recevoirPointGMPSocket(OperatorModel& curve)
{
    int socketCree,
        socketCommunic,
        Port = 1234;
    char donneesRecues[1500];
    PointGMPClass p;

    if(InitialiseWindowsSocket() == -1)
    {
        cout << "Echec initialisation des sockets windows" << endl;
        system("pause");
        return p;
    }

    /************* Création socket **********************/
    socketCree = CreerSocketTCP();
    if(socketCree == -1)
    {
        cout << "Echec creation d'un socket" << endl;
        system("pause");
        return p;
    }
    cout << "Le socket bob (serveur) cree est :" << socketCree << endl;

    /************** Création serveur *******************/
    if(CreerServeur(socketCree,Port) == -1 )
    {
        cout << "Echec creation serveur" << endl;
        system("pause");
        return p;
    }
    cout << "En attente d'une connexion ..." << endl;

    /********** Mise en écoute du serveur *************/
    socketCommunic = AttenteConnexionClient(socketCree);
    if(socketCommunic == -1 )
    {
        cout << "Erreur d'attente" << endl;
        system("pause");
        return p;
    }
    cout << "Connexion entrante : Le socket utilise pour cette session est : " << socketCommunic << endl;

    /*********** Reception de pA ***************/
    int car_rec = RecevoirDonnees(socketCommunic, donneesRecues, sizeof(donneesRecues));
    if(car_rec == -1)
    {
        cout << "echec reception" << endl;
        system("pause");
        return p;
    }
    donneesRecues[car_rec] = '\0';
    //Récupération du point à partir des donnees reçues
    string data(donneesRecues);
    string x,y;
    int posX, posY, posEndl, posEnds;
    posX = data.find("x=")+2;
    posY = data.find("y=")+2;
    posEndl = data.find('\n');
    posEnds = data.find('\0');
    x = data.substr(posX, posEndl - posX );
    y = data.substr(posY, posEnds - posY );
    PointGMPClass res( mpz_class(x.c_str(), 10), mpz_class(y.c_str(), 10), curve);

    FermerSocket(socketCree);
    cout << "Donnee recue " << res << endl;

    return res;
}

void alice_diffie_hellman_STS(OperatorModel& curve)
{
    hashwrapper *myWrapper = new sha256wrapper(); // sha 256
    WeierStrassCurve c = (WeierStrassCurve&)curve;
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);//le point générateur qui servira à calculer notre clef secrète
    mpz_class clefSecreteAlice = random_prime(50);//la clef secrète n'appartient qu'à Alice
    PointGMPClass pA = generateur * clefSecreteAlice;//pA est la clef publique que l'on va partager à Bob

    cout << "Clef publique a envoyer " << pA << endl;

    envoyerPointGMPSocket(pA.toString());
    PointGMPClass pB = recevoirPointGMPSocket(curve);

    mpz_class kA = random_prime(50);//un chiffre random pour Alice
    PointGMPClass middleA = generateur * kA;//middleA est la clef intermédiaire d'Alice

    std::string hashA = myWrapper->getHashFromString(middleA.toString()); // sha 256
    mpz_class zA(hashA, 62);// le haché de Alice

    PointGMPClass signatureA = c.sign_ECDSA(zA, clefSecreteAlice);

    //envoi de middleA et signatureA
    envoyerPointGMPSocket(middleA.toString());
    int i = 0;
    while(i<10000000)
    {
        i++;
    }
    envoyerPointGMPSocket(signatureA.toString());

    PointGMPClass middleB = recevoirPointGMPSocket(curve);
    PointGMPClass signatureB = recevoirPointGMPSocket(curve);

    if(middleB.isOnCurve()) cout << "on curve" << endl;
    else cout << "problem" << endl;
    PointGMPClass t = middleB * curve.Getn();
    cout << t << endl;

    std::string hashB = myWrapper->getHashFromString(middleB.toString()); // sha 256
    mpz_class zB(hashB, 62);// le haché de Alice
    bool resB = c.verif_ECDSA(signatureB.Getx(), signatureB.Gety(), pB, zB);
    if (resB) cout << "Signature ok de Bob" << endl;
    else cout << "Probleme de signature de Bob" << endl;

    if(resB)//si la signature est ok
    {
        PointGMPClass finAlice = middleB * kA;
        cout << "fin Alice " << finAlice << endl;

        if(finAlice.isOnCurve()) cout << "on curve" << endl;
        else cout << "problem : not on curve" << endl;
        t = finAlice * curve.Getn();
        cout << t << endl;
    }

    system("pause");
}

void bob_diffie_hellman_STS(OperatorModel& curve)
{
    WeierStrassCurve c = (WeierStrassCurve&)curve;
    hashwrapper *myWrapper = new sha256wrapper(); // sha 256
    PointGMPClass generateur(curve.Getgx(), curve.Getgy(), curve);//le point générateur qui servira à calculer notre clef secrète
    mpz_class clefSecreteBob = random_prime(50);//la clef secrète n'appartient qu'à Bob
    PointGMPClass pB = generateur * clefSecreteBob;//pB est la clef publique que l'on va partager à alice

    cout << "Clef publique a envoyer " << pB << endl;

    PointGMPClass pA = recevoirPointGMPSocket(curve);
    envoyerPointGMPSocket(pB.toString());

    mpz_class kB = random_prime(50);//un chiffre random pour Bob
    PointGMPClass middleB = generateur * kB;//middleB est la clef intermédiaire de Bob

    std::string hashB = myWrapper->getHashFromString(middleB.toString()); // sha 256
    mpz_class zB(hashB, 62);// le haché de Bob

    //on recoit middleA et signatureA
    PointGMPClass middleA = recevoirPointGMPSocket(curve);
    PointGMPClass signatureA = recevoirPointGMPSocket(curve);

    if(middleA.isOnCurve()) cout << "on curve" << endl;
    else cout << "problem" << endl;
    PointGMPClass t = middleA * curve.Getn();
    cout << t << endl;

    std::string hashA = myWrapper->getHashFromString(middleA.toString()); // sha 256
    mpz_class zA(hashA, 62);// le haché de Bob
    bool resA = c.verif_ECDSA(signatureA.Getx(), signatureA.Gety(), pA, zA);
    if (resA) cout << "Signature ok d'Alice" << endl;
    else cout << "Probleme de signature d'Alice" << endl;

    if(resA && middleA.isOnCurve())//si la signature ok et le point est sur la courbe on continue
    {
        PointGMPClass signatureB = c.sign_ECDSA(zB, clefSecreteBob);
        envoyerPointGMPSocket(middleB.toString());
        int i = 0;
        while(i<10000000)
        {
            i++;
        }
        envoyerPointGMPSocket(signatureB.toString());

        PointGMPClass finBob = middleA * kB;

        cout << "fin Bob " << finBob << endl;

        if(finBob.isOnCurve()) cout << "on curve" << endl;
        else cout << "problem : not on curve" << endl;
        t = finBob * curve.Getn();
        cout << t << endl;
    }



    system("pause");
}
