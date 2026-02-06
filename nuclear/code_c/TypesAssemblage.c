#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Grid.h"
#include "TypesAssemblage.h"

/* Palette 16 couleurs */
const char *palette[16] = {
    "🟥","🟧","🟨","🟩","🟦","🟪","⬛","⬜",
    "🟫","🟧","🟦","🟩","🟪","🟥","🟨","⬜"
};

/* Accepte TOUS les caractères possibles */
int couleur_type(char c) {
    if (c == '-') return -1;
    return ((unsigned char)c) % 16;
}

void definir_types(TypeAssemblage *types, int *nb_types) {
    printf("Combien de types d’assemblages ? ");
    if (scanf("%d", nb_types) != 1) exit(1);

    for (int i = 0; i < *nb_types; i++) {
        printf("Symbole du type %d : ", i+1);
        scanf(" %c", &types[i].symbole);
        printf("Puissance thermique du type %c : ", types[i].symbole);
        scanf("%lf", &types[i].puissance);
    }
}

void remplir_grille_aleatoire(Grid *G, TypeAssemblage *types, int nb_types) {
    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++) {
            if (G->core[i][j]) {
                int k = rand() % nb_types;
                G->g[i][j] = types[k].symbole;
            } else {
                G->g[i][j] = '-';
            }
        }
    }
}

void calculer_carte_thermique(Grid *G, TypeAssemblage *types, int nb_types,
                              double **T) {
    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++) {
            if (!G->core[i][j]) {
                T[i][j] = 0.0;
                continue;
            }
            char c = G->g[i][j];
            double p = 0.0;
            for (int k = 0; k < nb_types; k++) {
                if (types[k].symbole == c) {
                    p = types[k].puissance;
                    break;
                }
            }
            T[i][j] = p;
        }
    }
}

void diffusion_thermique(Grid *G, double **T, int iterations) {
    int n = G->size;
    double **tmp = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        tmp[i] = malloc(n * sizeof(double));

    for (int it = 0; it < iterations; it++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (!G->core[i][j]) {
                    tmp[i][j] = 0.0;
                    continue;
                }
                double sum = T[i][j];
                int count = 1;
                if (i > 0 && G->core[i-1][j]) { sum += T[i-1][j]; count++; }
                if (i < n-1 && G->core[i+1][j]) { sum += T[i+1][j]; count++; }
                if (j > 0 && G->core[i][j-1]) { sum += T[i][j-1]; count++; }
                if (j < n-1 && G->core[i][j+1]) { sum += T[i][j+1]; count++; }
                tmp[i][j] = sum / count;
            }
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                T[i][j] = tmp[i][j];
    }

    for (int i = 0; i < n; i++)
        free(tmp[i]);
    free(tmp);
}

void evaluer_thermique(Grid *G, double **T,
                       double *Tmin, double *Tmax,
                       double *deltaT, double *grad_max) {
    *Tmin = 1e30;
    *Tmax = -1e30;
    *grad_max = 0.0;

    int n = G->size;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!G->core[i][j]) continue;
            double t = T[i][j];
            if (t < *Tmin) *Tmin = t;
            if (t > *Tmax) *Tmax = t;

            if (i < n-1 && G->core[i+1][j]) {
                double g = fabs(T[i+1][j] - t);
                if (g > *grad_max) *grad_max = g;
            }
            if (j < n-1 && G->core[i][j+1]) {
                double g = fabs(T[i][j+1] - t);
                if (g > *grad_max) *grad_max = g;
            }
        }
    }

    *deltaT = *Tmax - *Tmin;
}

/* ASCII thermique */
void afficher_thermique_ascii(Grid *G, double **T) {
    double Tmin = 1e30, Tmax = -1e30;
    int n = G->size;

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(G->core[i][j]) {
                if(T[i][j] < Tmin) Tmin = T[i][j];
                if(T[i][j] > Tmax) Tmax = T[i][j];
            }

    const char levels[] = " .:-=+*#%@";

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(!G->core[i][j]) {
                printf("  ");
                continue;
            }
            double x = (T[i][j] - Tmin) / (Tmax - Tmin + 1e-9);
            int idx = (int)(x * 9);
            printf("%c ", levels[idx]);
        }
        printf("\n");
    }
}

/* Couleur thermique bleu → rouge */
void afficher_thermique_couleur(Grid *G, double **T) {
    double Tmin = 1e30, Tmax = -1e30;
    int n = G->size;

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(G->core[i][j]) {
                if(T[i][j] < Tmin) Tmin = T[i][j];
                if(T[i][j] > Tmax) Tmax = T[i][j];
            }

    const char *thermo[] = {
        "🔵","🔷","🔹","🟦","🟩","🟨","🟧","🟥"
    };

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(!G->core[i][j]) {
                printf("⬜");
                continue;
            }
            double x = (T[i][j] - Tmin) / (Tmax - Tmin + 1e-9);
            int idx = (int)(x * 7);
            printf("%s", thermo[idx]);
        }
        printf("\n");
    }
}
