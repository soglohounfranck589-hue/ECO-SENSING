#include "structures.h"
#include "capteur.h"

// FONCTION 1: creer_paquet - DOIT ÊTRE PRÉSENTE
Paquet* creer_paquet(int id, float valeur, long timestamp) {
    Paquet* nouveau = (Paquet*)malloc(sizeof(Paquet));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire pour le paquet\n");
        exit(EXIT_FAILURE);
    }

    nouveau->id = id;
    nouveau->valeur = valeur;
    nouveau->timestamp = timestamp;
    nouveau->suivant = NULL;

    return nouveau;
}

// FONCTION 2: initialiser_capteur
Capteur* initialiser_capteur(float batterie_initiale, float x, float y) {
    Capteur* capteur = (Capteur*)malloc(sizeof(Capteur));
    if (capteur == NULL) {
        printf("Erreur d'allocation memoire pour le capteur\n");
        exit(EXIT_FAILURE);
    }

    capteur->batterie = batterie_initiale;
    capteur->x = x;
    capteur->y = y;
    capteur->buffer_tete = NULL;
    capteur->buffer_usage = 0;
    capteur->dernier_id = 0;

    return capteur;
}

// FONCTION 3: produire_paquet
void produire_paquet(Capteur* capteur) {
    // Générer données
    float valeur_mesure = (float)(rand() % 10001) / 100.0f;
    long timestamp = time(NULL);

    // Créer paquet
    capteur->dernier_id++;
    Paquet* nouveau = creer_paquet(capteur->dernier_id, valeur_mesure, timestamp);

    // SLIDING WINDOW
    if (capteur->buffer_usage >= MAX_BUFFER) {
        Paquet* ancien = capteur->buffer_tete;
        printf("ALERTE : Memoire saturee. Suppression du paquet ID [%d] pour liberer de l'espace.\n", ancien->id);

        capteur->buffer_tete = ancien->suivant;
        free(ancien);
        capteur->buffer_usage--;
    }

    // Ajouter à la fin
    if (capteur->buffer_tete == NULL) {
        capteur->buffer_tete = nouveau;
        capteur->buffer_usage = 1;
    } else {
        Paquet* courant = capteur->buffer_tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
        capteur->buffer_usage++;
    }
    nouveau->suivant = NULL;

    printf("Nouveau paquet cree : ID=%d, Valeur=%.2f\n", nouveau->id, nouveau->valeur);
}

// FONCTION 4: calculer_distance
float calculer_distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// FONCTION 5: calculer_energie_transmission
float calculer_energie_transmission(float distance) {
    return E_ELEC + (E_AMP * distance * distance);
}

// FONCTION 6: envoyer_paquet
int envoyer_paquet(Capteur* capteur, Paquet* paquet) {
    float distance = calculer_distance(capteur->x, capteur->y, STATION_X, STATION_Y);
    float energie = calculer_energie_transmission(distance);

    if (capteur->batterie >= energie) {
        capteur->batterie -= energie;
        printf("Paquet ID=%d envoye. Distance=%.2fm, Energie=%.4fJ, Batterie=%.2fJ\n",
               paquet->id, distance, energie, capteur->batterie);
        return 1;
    }
    printf("Echec envoi paquet ID=%d - Batterie insuffisante\n", paquet->id);
    return 0;
}

// FONCTION 7: liberer_memoire
void liberer_memoire(Capteur* capteur) {
    Paquet* courant = capteur->buffer_tete;
    while (courant != NULL) {
        Paquet* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    free(capteur);
}

// FONCTION 8: afficher_etat
void afficher_etat(Capteur* capteur, int iteration) {
    printf("\n=== Iteration %d ===\n", iteration);
    printf("Batterie: %.2fJ | Position: (%.1f, %.1f)\n", capteur->batterie, capteur->x, capteur->y);
    printf("Paquets en attente: %d/%d\n", capteur->buffer_usage, MAX_BUFFER);

    printf("Buffer: ");
    Paquet* courant = capteur->buffer_tete;
    while (courant != NULL) {
        printf("[ID=%d:%.1f] ", courant->id, courant->valeur);
        courant = courant->suivant;
    }
    printf("\n");
}
