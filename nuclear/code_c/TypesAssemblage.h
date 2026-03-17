#ifndef TYPES_ASSEMBLAGE_H
#define TYPES_ASSEMBLAGE_H

#include "Grid.h"

/* Types de combustible */
typedef enum {
    COMB_THORIUM = 0,   /* Th-232 */
    COMB_U238,
    COMB_U235,
    COMB_PU239,
    COMB_MOX
} CombustibleType;

/* Description d'un type d'assemblage */
typedef struct {
    char   symbole;          /* Symbole dans la grille */
    double puissance;        /* Puissance thermique */
    int    stock_max;        /* Stock total disponible */

    CombustibleType combustible;

    /* Enrichissement simple (U235, U238, Th, Pu selon le type) */
    double enrichissement_principal;

    /* Enrichissements MOX */
    double enrichissement_mox_pu;
    double enrichissement_mox_u235;
    double enrichissement_mox_u238;

} TypeAssemblage;

/* Saisie interactive */
void definir_types(TypeAssemblage *types, int *nb_types);

/* Export grille + description des types */
void sauver_assemblage(const char *nom_fichier, Grid *G, TypeAssemblage *types, int nb_types);

#endif
