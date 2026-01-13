#include "structures.h"
#include "fichiers.h"

void sauvegarder_etat(Capteur* capteur) {
    FILE* fichier = fopen(FILENAME_SAVE, "wb");
    if (fichier == NULL) {
        printf("Erreur ouverture fichier sauvegarde\n");
        return;
    }

    fwrite(capteur, sizeof(Capteur), 1, fichier);

    Paquet* courant = capteur->buffer_tete;
    while (courant != NULL) {
        fwrite(courant, sizeof(Paquet), 1, fichier);
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("Sauvegarde dans %s\n", FILENAME_SAVE);
}

Capteur* charger_etat() {
    FILE* fichier = fopen(FILENAME_SAVE, "rb");
    if (fichier == NULL) {
        printf("Pas de sauvegarde\n");
        return NULL;
    }

    Capteur* capteur = (Capteur*)malloc(sizeof(Capteur));
    if (capteur == NULL) {
        fclose(fichier);
        return NULL;
    }

    fread(capteur, sizeof(Capteur), 1, fichier);
    capteur->buffer_tete = NULL;

    Paquet* dernier = NULL;
    for (int i = 0; i < capteur->buffer_usage; i++) {
        Paquet* paquet = (Paquet*)malloc(sizeof(Paquet));
        fread(paquet, sizeof(Paquet), 1, fichier);
        paquet->suivant = NULL;

        if (capteur->buffer_tete == NULL) {
            capteur->buffer_tete = paquet;
        } else {
            dernier->suivant = paquet;
        }
        dernier = paquet;
    }

    fclose(fichier);
    printf("Charge depuis %s\n", FILENAME_SAVE);
    return capteur;
}

void ecrire_log(FILE* log_file, int iteration, Capteur* capteur) {
    if (log_file == NULL) return;
    fprintf(log_file, "Temps: %d | Batterie: %.2fJ | Paquets en attente: %d\n",
            iteration, capteur->batterie, capteur->buffer_usage);
}
