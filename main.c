#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct array_Ty array_Ty;
struct array_Ty {
    int sommet;
    struct list_Ty *list;
};

typedef struct list_Ty list_Ty;
struct list_Ty {
    float valeur;
    int sommet;
    struct list_Ty *next;
};

typedef struct unpointing_node_list {
    int val;
    struct l_node * list_sommet;
    struct unpointing_node_list * next;
}unpointing_node_list;

typedef struct l_node {
    int sommet;
    struct l_node * next;
}l_node;

int nombreSommets;
int sommetsAdditionnels = 0;
char *ptr;
struct list_Ty *list;
struct list_Ty *l;

#define MAXCHAR 1000000
struct array_Ty* CreateMatrix(char *filePath){
    // Ouvrir le fichier du graphe
    FILE *file;
    struct array_Ty* Matrix;
    char line[MAXCHAR];

    // Erreur de chargement du fichier
    file = fopen(filePath, "r");
    if (file == NULL) perror("fopen in gm_write_dot");

    fgets(line, MAXCHAR, file);
    fgets(line, MAXCHAR, file);

    unpointing_node_list * nodeList = NULL;
    unpointing_node_list * nodePrv = NULL;
    l_node * l_sommet = NULL;
    printf("Creating Matrix : 1/3 ... ");

    //Chercher les noeuds qui ne pointent vers aucuns noeuds puis les stockés dans une liste
    for (int i = 0; i < nombreSommets; i++) {
        fgets(line, MAXCHAR, file);
        ptr = strtok(line, " ");
        ptr = strtok(NULL, " ");
        int s = atoi(ptr);
        if (s == 0 ) {
            if (nodeList == NULL) {
                nodeList = malloc(sizeof(*nodeList));
                nodeList->val = i+1;
                nodeList->list_sommet = NULL;
                nodeList->next = NULL;
            } else if(nodeList->next == NULL){
                nodePrv = malloc(sizeof(*nodePrv));
                nodePrv->val = i+1;
                nodePrv->next = NULL;
                nodePrv->list_sommet = NULL;
                nodeList->next = nodePrv;
            } else{
                nodePrv->next = malloc(sizeof(*nodePrv));
                nodePrv = nodePrv->next;
                nodePrv->val = i+1;
                nodePrv->list_sommet = NULL;
                nodePrv->next = NULL;
            }
        }
    }
    fclose(file);

    printf("2/3 ... ");

    //    //Chercher les noeuds qui ne sont pointés par aucuns noeuds puis les stockés dans une liste
    file = fopen(filePath, "r");
    if (file == NULL) perror("fopen in gm_write_dot");
    fgets(line, MAXCHAR, file);
    fgets(line, MAXCHAR, file);

    for(int i = 0; i < nombreSommets; i++){
        fgets(line, MAXCHAR, file);
        ptr = strtok(line, " ");
        int sommet = atoi(ptr);
        ptr = strtok(NULL, " ");
        int s = atoi(ptr);
        for(int j = 0; j < s; j++){
            ptr = strtok(NULL, " ");
            nodePrv = nodeList;
            while (nodePrv  && nodePrv->val <= atoi(ptr)) {
                if (atoi(ptr) == nodePrv->val){
                    if(nodePrv->list_sommet == NULL){
                        l_sommet = malloc(sizeof(*l_sommet));
                        l_sommet->sommet = sommet;
                        l_sommet->next = NULL;
                        nodePrv->list_sommet = l_sommet;
                    } else{
                        l_sommet = nodePrv->list_sommet;
                        while (l_sommet->next != NULL) l_sommet = l_sommet->next;
                        l_sommet->next = malloc(sizeof(*l_sommet));
                        l_sommet = l_sommet->next;
                        l_sommet->sommet = sommet;
                        l_sommet->next = NULL;
                    }
                }
                nodePrv = nodePrv->next;
            }
            l_sommet = NULL;
            ptr = strtok(NULL, " ");
        }
    }
    fclose(file);

    //Calculer le nombre de sommets additionnel à créer
    nodePrv = nodeList;
    while (nodePrv) {
        l_sommet = nodePrv->list_sommet;
        if (l_sommet) l_sommet = l_sommet->next;
        while (l_sommet){
            sommetsAdditionnels += 1;
            l_sommet = l_sommet->next;
        }
        nodePrv = nodePrv->next;
    }

    printf("3/3\n");

    //Creer la matrice du backSpace-------------------------------------------------------------
    file = fopen(filePath, "r");
    if (file == NULL) perror("fopen in gm_write_dot");

    if (fgets(line, MAXCHAR, file) != NULL){
        Matrix = malloc((nombreSommets + sommetsAdditionnels) * sizeof(*Matrix));
    }
    fgets(line, MAXCHAR, file);
    nodePrv = nodeList;

    int i =0;
    fgets(line, MAXCHAR, file);
    ptr = strtok(line, " ");
    int n = atoi(ptr);
    while (n <= nombreSommets){
        Matrix[i].sommet = n;
        Matrix[i].list = NULL;
        ptr = strtok(NULL, " ");

        int s = atoi(ptr);
        if (s == 0 ) {
            while (nodePrv->val < n) nodePrv = nodePrv->next;
            if (nodePrv->val == n) {
                if (nodePrv->list_sommet == NULL) i++;
                else l_sommet = nodePrv->list_sommet;
                while (l_sommet != NULL){
                    Matrix[i].sommet = n;
                    Matrix[i].list = malloc(sizeof(struct list_Ty));
                    Matrix[i].list->sommet = l_sommet->sommet;
                    Matrix[i].list->valeur = 1;
                    Matrix[i].list->next = NULL;
                    l_sommet = l_sommet->next;
                    i++;
                }
            }
        } else {
            for (int j = 0; j < s; j++) {
                if (!Matrix[i].list) {
                    list = malloc(sizeof(struct list_Ty));
                    Matrix[i].list = list;
                }
                else {
                    list->next = malloc(sizeof(struct list_Ty));
                    list = list->next;
                }
                ptr = strtok(NULL, " ");
                list->sommet = atoi(ptr);
                ptr = strtok(NULL, " ");
                list->valeur = atof(ptr);
                list->next = NULL;
            }
            i++;
        }
        if (fgets(line, MAXCHAR, file) != NULL){
            ptr = strtok(line, " ");
            n = atoi(ptr);
        } else break;
    }
    fclose(file);
    printf("Matrix Created successfully\n");
    return Matrix;
}

void multiplyConstByMatrix(array_Ty* matrix, float val, int size){
    for (int i = 0; i < size; ++i) {
        l = matrix[i].list;
        while (l != NULL) {
            l ->valeur *= val;
            l = l->next;
        }
    }
}

void addConstToVector(float *vector, float val, int size){
    for (int i = 0; i < size; ++i){
        vector[i] += val;
    }
}

void multiplyVectorByMatrix(array_Ty* matrix, const float *vector, float *result, int size){
    for (int i = 0; i < size; ++i) result[i] = 0;
    for (int i = 0; i < size; ++i) {
        l = matrix[i].list;
        while (l != NULL) {
            result[l->sommet - 1] += vector[i] * l->valeur;
            l = l->next;
        }
    }
}

float getSomeOfVector(const float* X, int size){
    float sum = 0;
    for (int i = 0; i < size; ++i){
        sum += X[i];
    }
    return sum;
}

float getDiffVectors(float v1[], float v2[], int size){
    float Diff = 0;
    for (int i = 0; i < size; ++i) {
        Diff += fabsf(v1[i]-v2[i]);
    }
    return Diff;
}

void copyVectors(float *v1, float *v2, int size){
    for(int i = 0; i < size; ++i){
        v1[i] = v2[i];
        v2[i] = 0;
    }
}

void printVector(float *vct, int size){
    for(int i = 0; i < size; i++){
        printf("%f - ", vct[i]);
    }
    printf("\n");
}

void PrintMatrix(array_Ty* matrix, int size){
    for (int i = 0; i < size; i++){
        l = matrix[i].list;
        printf("-%d-", matrix[i].sommet);
        while (l != NULL){
            printf("|%d <-> %.4f| ", l->sommet, l->valeur);
            l = l->next;
        }
        printf("\n\n");
    }
}

int getNombreSommets(char* filePath){
    FILE *file;
    char line[MAXCHAR];
    char root[50];

    file = fopen(filePath, "r");
    if (file == NULL) perror("fopen in gm_write_dot");

    if (fgets(line, MAXCHAR, file) != NULL){
        return atoi(line);
    }
}

void initializeVector(float *vector, float valIni, int size){
    if (valIni == -1){
        float a = (float )1/(float)size;
        for (int i = 0; i < size; i++) vector[i] = a;
    } else for (int i = 0; i < size; i++) vector[i] = valIni;
}

void convergence(struct array_Ty *Matrix, float * Vector, float *result, float epsilon, float alpha){
    int itr = 0;
    float inter = 0;
    float diff = 10;

    //Multiplier la matrice par alpha
    multiplyConstByMatrix(Matrix, alpha, nombreSommets + sommetsAdditionnels);
    while (diff > epsilon){
        //Multiplier la matrice par le vecteur X
        multiplyVectorByMatrix(Matrix, Vector, result, nombreSommets + sommetsAdditionnels);

        //Calculer e^t e 1/N
        inter = getSomeOfVector(Vector, nombreSommets + sommetsAdditionnels);
        inter = inter / (float )(nombreSommets + sommetsAdditionnels);
        inter = inter * (1- alpha);

        //l'ajouter au matrice
        addConstToVector(result, inter, nombreSommets + sommetsAdditionnels);
        itr++;
        printf("---------------- %d\n", itr);

        //calculer epsilon
        diff = getDiffVectors(Vector, result, nombreSommets + sommetsAdditionnels);
        printf("%f\n ", diff);
        copyVectors(Vector, result, nombreSommets + sommetsAdditionnels);
    }
}

//Additionner les pertinances des copis des pages pour reduire la taille du vecteur de partinance à la taille initial
void reduceVector(float * Vector, array_Ty* matrix){
    int j = 0;
    int sommet;
    float som = 0.0f;
    for (int i = 0; i < nombreSommets; i++){
        sommet = matrix[j].sommet;
        while(matrix[j].sommet == sommet){
            som += Vector[j];
            j++;
        }
        Vector[i] = som;
        som = 0.0f;
    }
}

int main() {
    //---------------------Déclaration & initialisation-----------------------------------------------------------------
    clock_t t;
    char* filePath = "C:\\Users\\OsmOlr\\Desktop\\mod&sim_data\\Exemple.txt";
    float alpha = 0.85f;
    nombreSommets = getNombreSommets(filePath);

    t = clock();
    //---------------------Back Space-----------------------------------------------------------------------------------

    array_Ty* Matrix = CreateMatrix(filePath);

    float *X = malloc((nombreSommets + sommetsAdditionnels) * sizeof(float ));
    initializeVector(X, -1, nombreSommets + sommetsAdditionnels);
    float *result = malloc((nombreSommets + sommetsAdditionnels) * sizeof(float ));
    initializeVector(result, 0, nombreSommets + sommetsAdditionnels);

    //------------------------------------------------------------------------------------------------------------------
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nLa creation de structure de la matrice a pris %f seconds\n", time_taken);
    t = clock();
    //------------------------------------------------------------------------------------------------------------------

    convergence(Matrix, X, result, 0.000001f, alpha);
    printf("Nouveaux noeuds crees : %d\n", sommetsAdditionnels);

    printVector(X, nombreSommets+ sommetsAdditionnels);

    reduceVector(X, Matrix);

    printf("\n vecteur de pertinance: ");
    printVector(X, nombreSommets);

    //---------------------fin surfer aléatoire-------------------------------------------------------------------------
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\nLa convergence a pris %f seconds\n", time_taken);

}

