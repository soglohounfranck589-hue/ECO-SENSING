#include "structures.h"
#include "capteur.h"
#include "fichiers.h"

int main() {
    srand(time(NULL));

    FILE* log_file = fopen(FILENAME_LOG, "w");
    if (log_file == NULL) {
        printf("Erreur creation fichier log\n");
        return EXIT_FAILURE;
    }

    Capteur* capteur;
    int choix;

    printf("====Eco-Sensing Simulator====\n\n");
   // printf("CONSEIL: Pour voir l'alerte 'Memoire saturee', entrez:\n");
    //printf("Batterie: 50.0 (pour durer longtemps)\n");
    //printf( "Position: 10.0, 10.0 (assez eloignee)\n\n");
     printf("\n");
    printf("1. Nouvelle simulation\n2. Charger sauvegarde\nChoix: ");
    scanf("%d", &choix);
    getchar(); // Absorber le \n

    if (choix == 2) {
        capteur = charger_etat();
        if (capteur == NULL) {
            printf("Creation d'une nouvelle simulation...\n");
            choix = 1;
        }
    }

    if (choix == 1) {
        float batterie, x, y;
        printf("Batterie initiale (J): ");
        scanf("%f", &batterie);
        printf("Position X: ");
        scanf("%f", &x);
        printf("Position Y: ");
        scanf("%f", &y);
        capteur = initialiser_capteur(batterie, x, y);
        getchar(); // Absorber le \n
    }

    int iteration = 0;
    int paquets_transmis = 0;

    // Laisser les 5 premiers cycles SANS envoi pour remplir le buffer
    int cycles_sans_envoi = 5;

    while (capteur->batterie > 0.001f) {
        iteration++;
        printf("\n\n");
        printf("CYCLE %d\n", iteration);
        printf("\n");

        // 1. PRODUIRE un nouveau paquet
        produire_paquet(capteur);

        // 2. ESSAYER D'ENVOYER un paquet
        // Ne pas envoyer pendant les premiers cycles pour remplir le buffer
        if (iteration > cycles_sans_envoi) {
            if (capteur->buffer_tete != NULL) {
                printf("\nTentative d'envoi d'un paquet...\n");
                if (envoyer_paquet(capteur, capteur->buffer_tete)) {
                    paquets_transmis++;
                    // Supprimer le paquet envoyé
                    Paquet* envoye = capteur->buffer_tete;
                    capteur->buffer_tete = envoye->suivant;
                    free(envoye);
                    capteur->buffer_usage--;
                    printf("Paquet envoye et supprime du buffer\n");
                } else {
                    printf("Echec d'envoi - Batterie insuffisante\n");
                    capteur->batterie = 0;
                }
            }
        } else {
            printf("\n\n",cycles_sans_envoi);
        }

        // 3. AFFICHER ÉTAT DÉTAILLÉ
        afficher_etat(capteur, iteration);

        // 4. ÉCRIRE DANS LE LOG
        ecrire_log(log_file, iteration, capteur);

        // 5. SAUVEGARDER PÉRIODIQUEMENT
        if (iteration % 3 == 0) {
            sauvegarder_etat(capteur);
        }

        // 6. PAUSE POUR LIRE LES RESULTATS
        if (iteration < 10) { // Seulement pour les premiers cycles
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();
        } else {
            printf("\n--- Suite automatique (3 sec) ---\n");
            sleep(3); // Pour Unix/Linux
            // Pour Windows, remplacez par: Sleep(3000);
        }
    }

    // FIN DE SIMULATION
    printf("\n\n");
    printf("SIMULATION TERMINEE\n");
    printf("\n\n");
    printf("RESULTATS FINAUX :\n");
    printf(" Iterations totales : %d\n", iteration);
    printf(" Paquets transmis   : %d\n", paquets_transmis);
    printf(" Batterie finale    : %.4f\n", capteur->batterie);
    printf(" Paquets restants   : %d\n", capteur->buffer_usage);

    // Dernière sauvegarde
    sauvegarder_etat(capteur);

    // Nettoyage
    fclose(log_file);
    liberer_memoire(capteur);

    printf("\nFichiers crees : log.txt et save.bin\n");
    printf("Appuyez sur Entree pour quitter...");
    getchar();

    return EXIT_SUCCESS;
}
