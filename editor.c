#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M_PI 3.14159265358979323846

struct seta{    
    double pontos[6];    
    int dia;
};
struct entidade{
    char cidade[11];
    int hora;
    double antes;
    double durante;
    double depois;};

void printEntidade(struct entidade e){
    printf("\ncidade: %s\nhora: %d\nantes: %.2lf\ndurante: %.2lf\ndepois: %.2lf\n", 
    e.cidade,e.hora,e.antes,e.durante,e.depois);
}

struct entidade gerarEntidade(char *linha){
    struct entidade temp;
    sscanf(linha,"%s,%d,%lf,%lf,%lf\n",
        &temp.cidade,
        &temp.hora,
        &temp.antes,
        &temp.durante,
        &temp.depois
    );
    return temp;
}

double valores[8][24][3];

int indiceDaCidade(char nome[11]){
    if(strcmp(nome,"APODI")==0) return 0;
    if(strcmp(nome,"MOSSORO")==0) return 1;
    if(strcmp(nome,"CAICO")==0) return 2;
    if(strcmp(nome,"IPANGUACU")==0) return 3;
    if(strcmp(nome,"MACAU")==0) return 4;
    if(strcmp(nome,"SANTA CRUZ")==0) return 5;
    if(strcmp(nome,"CALCANHAR")==0) return 6;
    if(strcmp(nome,"NATAL")==0) return 7;
}

const double coordenadas[8][2]={
    {227.0,297.0},
    {374.0,210.0},
    {447.0,536.0},
    {510.0,292.0},
    {572.0,181.0},
    {723.0,471.0},
    {860.0,191.0},
    {940.0,368.0}
};

void gerarElementos(FILE *origem,char tipo){
    char linha[500];
    FILE *info = fopen("tratados.csv","r");

    while(fgets(linha,500,origem)!=NULL){
        if(contem(linha,'@')){
            while(fgets(linha,50,info)!=NULL){
                struct entidade ent = gerarEntidade(linha);
                if(tipo=='m'){
                    printf("\t\t\t\t\t\t\t");
                    struct seta tri = gerarSeta(ent);
                    printSeta(tri);
                }
                else if (tipo == 't'){
                    struct seta tri = gerarSetaTabela(ent);
                    printf("\t\t\t\t");
                    printSeta(tri);
                }
            }            
        }else{            
            printf("%s",linha);
        }
    }
    fclose(info);
}

void gerarMapa(){
    FILE *origem = fopen("MapaChuvaPressao.html","r");
    gerarElementos(origem,'m');
    fclose(origem);    
}

int main(){
    gerarMapa();
    return 0;
}
