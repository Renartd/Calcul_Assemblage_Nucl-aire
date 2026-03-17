#include <stdio.h>
#include <string.h>
#include "TypesAssemblage.h"
#include "Grid.h"

/* ---------------------------------------------------------
   Lecture sécurisée d’une ligne
   --------------------------------------------------------- */
static void lire_ligne(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ---------------------------------------------------------
   Choix du combustible
   --------------------------------------------------------- */
static CombustibleType choisir_combustible(void) {
    int choix = 0;
    printf("Type de combustible :\n");
    printf(" 1) Thorium (Th-232)\n");
    printf(" 2) Uranium 238 (U-238)\n");
    printf(" 3) Uranium 235 (U-235)\n");
    printf(" 4) Plutonium 239 (Pu-239)\n");
    printf(" 5) MOX (mélange Pu/U)\n");
    printf("Votre choix : ");

    if (scanf("%d", &choix) != 1) {
        lire_ligne();
        return COMB_U235;
    }
    lire_ligne();

    switch (choix) {
        case 1: return COMB_THORIUM;
        case 2: return COMB_U238;
        case 3: return COMB_U235;
        case 4: return COMB_PU239;
        case 5: return COMB_MOX;
        default: return COMB_U235;
    }
}

/* ---------------------------------------------------------
   Définition des types d’assemblages
   --------------------------------------------------------- */
void definir_types(TypeAssemblage *types, int *nb_types) {
    int n = 0;
    printf("Nombre de types d’assemblages : ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Valeur invalide, utilisation de 3 types par défaut.\n");
        n = 3;
        lire_ligne();
    } else {
        lire_ligne();
    }

    for (int i = 0; i < n; i++) {
        printf("\n=== Type %d ===\n", i + 1);

        printf("Symbole (caractère) : ");
        if (scanf(" %c", &types[i].symbole) != 1)
            types[i].symbole = 'A' + i;
        lire_ligne();

        printf("Puissance thermique (MW) : ");
        if (scanf("%lf", &types[i].puissance) != 1)
            types[i].puissance = 1.0;
        lire_ligne();

        printf("Stock maximal (nombre d’assemblages) : ");
        if (scanf("%d", &types[i].stock_max) != 1)
            types[i].stock_max = 100;
        lire_ligne();

        types[i].combustible = choisir_combustible();

        /* Initialisation enrichissements */
        types[i].enrichissement_principal = 0.0;
        types[i].enrichissement_mox_pu    = 0.0;
        types[i].enrichissement_mox_u235  = 0.0;
        types[i].enrichissement_mox_u238  = 0.0;

        if (types[i].combustible == COMB_MOX) {
            printf("Enrichissement MOX - Pu (%%) : ");
            if (scanf("%lf", &types[i].enrichissement_mox_pu) != 1)
                types[i].enrichissement_mox_pu = 7.0;
            lire_ligne();

            printf("Enrichissement MOX - U235 (%%) : ");
            if (scanf("%lf", &types[i].enrichissement_mox_u235) != 1)
                types[i].enrichissement_mox_u235 = 3.0;
            lire_ligne();

            printf("Enrichissement MOX - U238 (%%) : ");
            if (scanf("%lf", &types[i].enrichissement_mox_u238) != 1)
                types[i].enrichissement_mox_u238 = 90.0;
            lire_ligne();
        } else {
            printf("Enrichissement principal (%%) : ");
            if (scanf("%lf", &types[i].enrichissement_principal) != 1)
                types[i].enrichissement_principal = 3.0;
            lire_ligne();
        }
    }

    *nb_types = n;
}

/* ---------------------------------------------------------
   Nom lisible du combustible
   --------------------------------------------------------- */
static const char *nom_combustible(CombustibleType c) {
    switch (c) {
        case COMB_THORIUM: return "Thorium";
        case COMB_U238:    return "U238";
        case COMB_U235:    return "U235";
        case COMB_PU239:   return "Pu239";
        case COMB_MOX:     return "MOX";
        default:           return "Inconnu";
    }
}

/* ---------------------------------------------------------
   Sauvegarde dans assemblage.txt (avec légende lisible)
   --------------------------------------------------------- */
void sauver_assemblage(const char *nom_fichier, Grid *G, TypeAssemblage *types, int nb_types) {
    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        perror("Erreur ouverture fichier assemblage");
        return;
    }

    /* Grille */
    fprintf(f, "# Grille d’assemblage\n");
    fprintf(f, "%d\n", G->size);

    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++)
            fputc(G->g[i][j], f);
        fputc('\n', f);
    }

    /* Types */
    fprintf(f, "\n# Types d’assemblages\n");
    fprintf(f, "%d\n", nb_types);

    /* === LÉGENDE LISIBLE POUR HUMAIN === */
    fprintf(f, "# symbole  puissance(MW)  stock  combustible  enrich_principal  mox_pu  mox_u235  mox_u238\n");

    for (int i = 0; i < nb_types; i++) {
        TypeAssemblage *t = &types[i];
        fprintf(f,
                "%c %.6f %d %s %.6f %.6f %.6f %.6f\n",
                t->symbole,
                t->puissance,
                t->stock_max,
                nom_combustible(t->combustible),
                t->enrichissement_principal,
                t->enrichissement_mox_pu,
                t->enrichissement_mox_u235,
                t->enrichissement_mox_u238);
    }

    fclose(f);
}
