#ifndef TYPES_ASSEMBLAGE_H
#define TYPES_ASSEMBLAGE_H

#include "Grid.h"

typedef struct {
    char symbole;
    double puissance;
} TypeAssemblage;

extern const char *palette[16];

int couleur_type(char c);

void definir_types(TypeAssemblage *types, int *nb_types);
void remplir_grille_aleatoire(Grid *G, TypeAssemblage *types, int nb_types);

void calculer_carte_thermique(Grid *G, TypeAssemblage *types, int nb_types,
                              double **T);

void diffusion_thermique(Grid *G, double **T, int iterations);

void evaluer_thermique(Grid *G, double **T,
                       double *Tmin, double *Tmax,
                       double *deltaT, double *grad_max);

/* Nouveaux affichages */
void afficher_thermique_ascii(Grid *G, double **T);
void afficher_thermique_couleur(Grid *G, double **T);

#endif
