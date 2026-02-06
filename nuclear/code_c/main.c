#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Grid.h"
#include "TypesAssemblage.h"
#include "MonteCarlo.h"

static double **alloc_double_grid(int n) {
    double **m = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        m[i] = malloc(n * sizeof(double));
    return m;
}

static void free_double_grid(double **m, int n) {
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

int main(void) {
    srand((unsigned int)time(NULL));

    int rayon;
    printf("Rayon du cœur (max 25) : ");
    if (scanf("%d", &rayon) != 1) return 1;

    Grid G = generer_grille_circulaire(rayon);

    printf("\n=== Cœur généré ===\n");
    afficher_core(G);

    TypeAssemblage types[16];
    int nb_types = 0;
    definir_types(types, &nb_types);

    remplir_grille_aleatoire(&G, types, nb_types);

    printf("\n=== Remplissage aléatoire ===\n");
    afficher_grille(G);

    double **Tfield = alloc_double_grid(G.size);

    double Tmin, Tmax, deltaT, grad_max;

    calculer_carte_thermique(&G, types, nb_types, Tfield);
    diffusion_thermique(&G, Tfield, 5);
    evaluer_thermique(&G, Tfield, &Tmin, &Tmax, &deltaT, &grad_max);

    printf("\n=== Carte thermique (ASCII) ===\n");
    afficher_thermique_ascii(&G, Tfield);

    printf("\n=== Carte thermique (couleur) ===\n");
    afficher_thermique_couleur(&G, Tfield);

    printf("\n=== Évaluation initiale ===\n");
    printf("Tmin = %.4f\n", Tmin);
    printf("Tmax = %.4f\n", Tmax);
    printf("ΔT   = %.4f\n", deltaT);
    printf("Gradient max = %.4f\n", grad_max);

    printf("\n=== Monte Carlo (Metropolis) ===\n");
    monte_carlo_metropolis(&G, types, nb_types, 2000, 0.5, Tfield);
    afficher_grille(G);

    printf("\n=== Recuit simulé ===\n");
    recuit_simule(&G, types, nb_types, 5000, 1.0, 0.999, Tfield);
    afficher_grille(G);

    free_double_grid(Tfield, G.size);
    free_int_grid(G.core, G.size);
    free_char_grid(G.g, G.size);

    return 0;
}
