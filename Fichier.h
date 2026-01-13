#ifndef FICHIERS_H
#define FICHIERS_H

#include "structures.h"

void sauvegarder_etat(Capteur* capteur);
Capteur* charger_etat();
void ecrire_log(FILE* log_file, int iteration, Capteur* capteur);

#endif
