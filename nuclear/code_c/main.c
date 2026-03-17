#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Grid.h"
#include "TypesAssemblage.h"
#include "Thermique.h"
#include "PlacementProgressif.h"
#include "Affichage.h"

int main(void) {
    srand((unsigned int)time(NULL));

    int rayon = 5;
    printf("Rayon du cœur (par défaut 5) : ");
    if (scanf("%d", &rayon) != 1 || rayon <= 0) {
        rayon = 5;
    }

    Grid *G = generer_grille_circulaire(rayon);

    printf("\n=== Grille circulaire générée (taille = %d) ===\n", G->size);
    afficher_core(G);

    TypeAssemblage types[32];
    int nb_types = 0;
    definir_types(types, &nb_types);

    /* Placement symétrique identique au Python */
    remplir_grille_symetrique(G, types, nb_types);

    /* === THERMIQUE === */
    int n = G->size;
    double **T = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        T[i] = malloc(n * sizeof(double));

    double Tmin, Tmax, deltaT, grad_max;

    calculer_carte_thermique(G, types, nb_types, T);
    diffusion_thermique(G, T, 40);
    evaluer_thermique(G, T, &Tmin, &Tmax, &deltaT, &grad_max);

    /* === LÉGENDE COULEUR === */
    printf("\n=== Légende des couleurs ===\n");
    for (int t = 0; t < nb_types; t++) {
        int idx = couleur_type(types[t].symbole);
        const char *col = (idx == -1 ? "⬜" : palette[idx]);
        printf("  %c → %s\n", types[t].symbole, col);
    }

    /* === AFFICHAGE ASCII === */
    printf("\n=== Grille ASCII ===\n");
    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++)
            printf("%c ", G->g[i][j]);
        printf("\n");
    }

    /* === AFFICHAGE COULEUR === */
    printf("\n=== Grille en couleurs ===\n");
    afficher_grille(G);

    /* === EXPORT === */
    printf("\nSauvegarde dans assemblage.txt...\n");
    sauver_assemblage("assemblage.txt", G, types, nb_types);
    printf("Fichier assemblage.txt écrit.\n");

    return 0;
}
