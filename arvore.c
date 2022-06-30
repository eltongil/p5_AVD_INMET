#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** partes(char* linha){
    int cont=0;
    char **part = malloc(sizeof(char*)*5);
    for(int i=0;i<5;i++){
        part[i] = malloc(sizeof(char)*30);
        int j=0;
        while(linha[cont]!=',' && linha[cont]!='\n' && linha[cont]!='\0'){
            part[i][j]=linha[cont];
            j++;
            cont++;
        }
        part[i][j]='\0';
        cont++;
    }
}

typedef struct horario{
    double pressao[3];
    /*0 antes
    1 durante
    2 depois*/
}*hora;

typedef struct cidade{
    char nome[11];
    hora horario[24];
}*local;

typedef struct raiz{
    local cidades[8];
}*RAIZ;

RAIZ novaRaiz(){
    RAIZ nova = malloc(sizeof(RAIZ));
    for(int i=0;i<8;i++){
        strcpy(nova->cidades[i]->nome," ");
    }
    return nova;
}

void addHora(local onde,char **parts){
    for(int i=0;i<3;i++){
        onde->horario[atoi(parts[1])]->pressao[i] = atof(parts[i+2]);
    }
}

void addNo(RAIZ root,char* linha){
    char **parts = partes(linha);
    for(int i=0;i<8;i++){
        if(strcmp(root->cidades[i]->nome," ")==0){
            strcpy(root->cidades[i]->nome,parts[0]);
            addHora(root->cidades[i],parts);
            break;
        }else if(strcmp(root->cidades[i]->nome,parts[0])==0){
            addHora(root->cidades[i],parts);
            break;
        }
    }
}

void freeAll(RAIZ root){
    for(int i=0;i<8;i++){
        for(int j=0;j<24;j++){
            free(root->cidades[i]->horario[j]);
        }
        free(root->cidades[i]);
    }
    free(root);
}

int main(){
    RAIZ base = novaRaiz();

    FILE *origem = fopen("calculado.csv","r");
    FILE *destino = fopen("arvore.csv","w");

    char linha[250];
    while(fgets(linha,250,origem)!=NULL){
        puts(linha);
    }
    freeAll(base);
    return 0;
}