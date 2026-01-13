#ifndef CAPTEUR_H
#define CAPTEUR_H

#include "structures.h"

// PROTOTYPES - SANS ERREURS
Capteur* initialiser_capteur(float batterie_initiale, float x, float y);
Paquet* creer_paquet(int id, float valeur, long timestamp);
void produire_paquet(Capteur* capteur);
float calculer_distance(float x1, float y1, float x2, float y2);
float calculer_energie_transmission(float distance);
int envoyer_paquet(Capteur* capteur, Paquet* paquet);
void liberer_memoire(Capteur* capteur);
void afficher_etat(Capteur* capteur, int iteration);

#endif
