
/*fonctions utiles comprises dans cjson.h ou stdio.h:


fseek(nom fichier, valeur entre 0 et 2, position dans le fichier debut, courant ou fin) : se deplacer

ftell(nom_fichier) : donne la taille du fichier
buffer : variable dite buffer qui indique la taille du fichier

fread(buffer, size_t blocSize(taille du bloc à lire), size_t blocCount(nombre de bloc à lire), FILE * stream )


fprintf(file, "%s", objet_char); : apres avoir creer le langage formaté depuis json; affiche l'objet

fgetc(file) : permet d'obtenir la totalité des caractères du fichiers

snprintf(.............................................
*/


#include <stdio.h>    
#include <stdlib.h>  
#include <string.h>
#include "fichier_h/constantes_structures.h"
#include "fichier_h/save.h"

// Fonction principale pour lire, afficher, modifier la seed, et retourner sa valeur
int read_write_seed(const char *nom_fichier, int nouvelle_seed) {
    FILE *file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier pour lecture.\n");
        return -1;
    }

    // Lire le contenu du fichier
    char buffer[256];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[255] = '\0'; // Sécurité
    fclose(file);

    // Affiche le contenu actuel
    printf("Contenu JSON avant modification : %s\n", buffer);

    // Extraire la valeur actuelle de seed (simple parsing naïf)
    int valeur_seed = 0;
    sscanf(buffer, "{ \"seed\": %d }", &valeur_seed);
    printf("Valeur actuelle de seed : %d\n", valeur_seed);

    // Remplacer avec la nouvelle valeur
    FILE *file_write = fopen(nom_fichier, "w");
    if (file_write == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier pour écriture.\n");
        return -1;
    }

    fprintf(file_write, "{ \"seed\": %d }", nouvelle_seed);
    fclose(file_write);

    printf("Nouvelle valeur de seed écrite dans le fichier : %d\n", nouvelle_seed);
    return nouvelle_seed;
}



void save(const char *filename, int seed, int wave, int vie, int argent, Tower *towers, int tower_count) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Erreur ouverture fichier sauvegarde\n");
        return;
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"seed\": %d,\n", seed);
    fprintf(f, "  \"wave\": %d,\n", wave);
    fprintf(f, "  \"vie\": %d,\n", vie);
    fprintf(f, "  \"argent\": %d,\n", argent);
    fprintf(f, "  \"towers\": [\n");
    for (int i = 0; i < tower_count; i++) {
        fprintf(f, "    { \"x\": %d , \"y\": %d, \"level\": %d }%s\n",
                towers[i].x, towers[i].y, towers[i].level,
                i == tower_count - 1 ? "" : ",");
    }
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");

    fclose(f);
}



int load_game(const char *filename, int *seed, int *wave, int *vie, int *argent, Tower **towers1, int *tower_count,int t_size) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Fichier de sauvegarde introuvable\n");
        return 0;
    }

    char buffer[2048];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, f);
    buffer[2047] = '\0';
    fclose(f);

    // Parsing très basique
    sscanf(buffer, " { \"seed\": %d , \"wave\": %d, \"vie\": %d, \"argent\": %d", seed, wave, vie, argent);

    Tower *towers = malloc(sizeof(Tower) * 100); // adapt size if needed
    if (!towers) {
        printf("Erreur allocation mémoire pour towers\n");
        return 0;
    }

    *tower_count = 0;
    char *ptr = strstr(buffer, "\"towers\": [");
    if (ptr) {
        /*ptr = strchr(ptr, '[') + 1;
        while (*ptr && *ptr != ']') {
            int x, y, level;
            if (sscanf(ptr, " { \"x\": %d , \"y\": %d, \"level\": %d }", &x, &y, &level) == 3) {
                towers[*tower_count] = (Tower){ .x = x, .y = y, .level = level, .damage = 1+level, .range = 48+level, .speed_damage = 30, .cooldown = 0 };
                (*tower_count)++;
            }
            ptr = strchr(ptr, '}');
            if (!ptr) break;
            ptr++;
        }*/
        ptr = strchr(ptr, '[') + 1;
while (*ptr && *ptr != ']') {
    int x, y, level;
    //int fake_money =100000000;
    if (sscanf(ptr, " { \"x\": %d , \"y\": %d, \"level\": %d }", &x, &y, &level) == 3) {
        towers[*tower_count] = (Tower){ .x = x, .y = y, .level = level, .damage = 1+level, .range = t_size*4, .speed_damage = 30, .cooldown = 0 };
		//for (int e; e<level;e++){
		//upgrade_s(towers,*tower_count,x,y);}
        (*tower_count)++;
    }
    ptr = strchr(ptr, '}');
    if (!ptr) break;
    ptr++;
    // Skip commas, spaces, etc. to reach next object
    while (*ptr && *ptr != '{' && *ptr != ']') ptr++;
}

    }

    *towers1 = towers;
    return 1;
}

void load_seed(const char *filename, int *seed) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Fichier de sauvegarde introuvable\n");
        return;
    }

    char buffer[2048];
    fread(buffer, sizeof(char), sizeof(buffer)-1, f);
    buffer[2047] = '\0';
    fclose(f);

    // Parsing très basique
    sscanf(buffer, " { \"seed\": %d", seed);
}



