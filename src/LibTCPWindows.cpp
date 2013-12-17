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

#include <winsock2.h>
#include <iostream>
#include <cstdlib>
#include "../include/LibTCPWindows.h"


/****************************************************************************
 Rôle: Permet l'utilisation de la DLL qui gère les socket sous Windows
Valeur retournée: -1 si erreur, 0 sinon
****************************************************************************/
int InitialiseWindowsSocket(void)
{
    WORD WVersionRequested;
    WSADATA WsaData;
    WVersionRequested = MAKEWORD(2,0);
    return WSAStartup(WVersionRequested,&WsaData);
 }

/****************************************************************************
 Rôle: Crée un socket pour TCP
Valeur retournée: -1 si erreur, le numéro du socket sinon
****************************************************************************/
int CreerSocketTCP(void)
{
    int Sock;
    Sock=socket(AF_INET,SOCK_STREAM,0);
    return Sock;
}

/****************************************************************************
Rôle:Connecte un client (machine sur laquelle tourne le programme) avec un
serveur repéré par son adresse et son numéro de port.
Arguments:
-	Sock: descripteur de la socket créée (valeur retournée par la fonction CreerSocket)
-	AdresseServeur: adresse du serveur en notation décimale pointée
-	Port: port destinataire
Valeur retournée: -1 si erreur, 0 sinon

****************************************************************************/
int ConnexionServeur(int Socket, char *AdresseServeur, int Port)
{
    struct sockaddr_in AddrServ;
    AddrServ.sin_family = AF_INET;
    AddrServ.sin_addr.s_addr = inet_addr(AdresseServeur);
    AddrServ.sin_port = htons(Port);
    return connect(Socket,(struct sockaddr*)&AddrServ,sizeof(struct sockaddr_in));
}

/****************************************************************************
Rôle:Envoie les données passées en argument au serveur avec qui l'on est connecté
Arguments:
-	Sock: descripteur de la socket
-	Donnees : Tableau qui contient la chaîne de caractères à envoyer.
Valeur retournée: -1 si erreur, le nombre de caractères envoyés sinon
****************************************************************************/
int EnvoyerDonnees(int Socket, const char* Donnees)
{
    unsigned int NbChar;
    NbChar=send(Socket, Donnees, strlen(Donnees), 0);
    return NbChar;
}

/****************************************************************************
Rôle:Reçoit des données du serveur avec qui l'on est connecté
Arguments:
-	Sock: descripteur de la socket
-	Donnees : Tableau qui contient la chaîne de caractère des données reçues.
-   Size: Taille du tableau Donnees
Valeur retournée: -1 si erreur, le nombre de caractères reçus sinon.
****************************************************************************/
int RecevoirDonnees(int Socket, char *Donnees, int Size)
{
    int NbChar;
    NbChar=recv(Socket, Donnees, Size, 0);
    return NbChar;
}

/****************************************************************************
Rôle:Crée un serveur sur cette machine
Arguments:
-	Sock: descripteur de la socket créée (valeur retournée par la fonction
          CreerSocket)
-	Port : Numéro de port du service créé.
Valeur retournée: -1 si erreur, 0 sinon.
****************************************************************************/
int CreerServeur(int Socket, int Port)
{
    struct sockaddr_in AddrServ;
    AddrServ.sin_family = AF_INET;
    AddrServ.sin_port = htons(Port);
    AddrServ.sin_addr.s_addr = INADDR_ANY;

    if(bind(Socket,(struct sockaddr *)&AddrServ,sizeof(struct sockaddr_in))!=0)
    {return -1;}

    if (listen(Socket,10) == -1)
    {return -1;}

    return 0;
}

/****************************************************************************
Rôle:Une fois créé, met le serveur en écoute (attente d'une connexion client)
     cette fonction est bloquante.
Arguments:
-	Sock: descripteur de la socket créée (valeur retournée par la fonction
          CreerSocket)
Valeur retournée: -1 si erreur, le numéro de la socket sur laquelle il faut
                  communiquer sinon.
****************************************************************************/
int AttenteConnexionClient(int Socket)
{
    struct sockaddr_in AddrClient;
    int SockSlave, TailleAdresse;
    TailleAdresse = sizeof(struct sockaddr);
    SockSlave = accept(Socket,(struct sockaddr*)&AddrClient,&TailleAdresse);
    return SockSlave;
}

/****************************************************************************
Rôle: Ferme la connexion
Arguments:
-	Sock: descripteur de la socket
 Valeur retournée: -1 si erreur, 0 sinon
****************************************************************************/
void FermerSocket(int Socket)
{
    shutdown(Socket,2);
    closesocket(Socket);
    WSACleanup();
}
