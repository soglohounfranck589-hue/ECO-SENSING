#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX_BUFFER 5
#define E_ELEC 0.05f
#define E_AMP 0.01f
#define STATION_X 0.0f
#define STATION_Y 0.0f
#define FILENAME_SAVE "save.bin"
#define FILENAME_LOG "log.txt"

typedef struct Paquet {
    int id;
    float valeur;
    long timestamp;
    struct Paquet *suivant;
} Paquet;

typedef struct Capteur {
    float batterie;
    float x, y;
    Paquet *buffer_tete;
    int buffer_usage;
    int dernier_id;
} Capteur;

#endif
