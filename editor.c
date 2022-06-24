#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

struct seta{    
    double pontos[6];    
    int dia;
};
struct entidade{
    int cidade;
    int dia;
    double max;
    double med;
    double dir;};

void printEntidade(struct entidade e){
    printf("entidade: \ncidade: %d\ndia: %d\nmax: %.2lf\nmed: %.2lf\ndir: %.2lf\n", 
    e.cidade,e.dia,e.max,e.med,e.dir);
}
struct entidade gerarEntidade(char *linha){
    struct entidade temp;
    sscanf(linha,"%d,%d,%lf,%lf,%lf\n",
        &temp.cidade,
        &temp.dia,
        &temp.max,
        &temp.med,
        &temp.dir
    );
    return temp;
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

const double coordenadasTabela[8][2]={
    {150.7,59.3},
    {150.7,270.5},
    {248.1,59.3},
    {150.7,164.1},
    {248.1,164.1},
    {248.1,375.3},
    {150.7,375.3},
    {248.1,270.5}
};

struct seta gerarSeta(struct entidade ent){
    struct seta nova;
    nova.pontos[0] = coordenadas[ent.cidade][0];
    nova.pontos[1] = coordenadas[ent.cidade][1];
    double rad = ent.dir*M_PI/180.0;
    nova.pontos[2] = nova.pontos[0] + 6*ent.max*cos(rad) + 6*ent.med*sin(rad);
    nova.pontos[3] = nova.pontos[1] + 6*ent.max*sin(rad) + 6*ent.med*cos(rad);
    nova.pontos[4] = nova.pontos[0] + 6*ent.max*cos(rad) - 6*ent.med*sin(rad);
    nova.pontos[5] = nova.pontos[1] + 6*ent.max*sin(rad) - 6*ent.med*cos(rad);
    nova.dia = ent.dia;
    return nova;
}

struct seta gerarSetaTabela(struct entidade ent){
    struct seta nova;
    nova.pontos[0] = coordenadasTabela[ent.cidade][0];
    nova.pontos[1] = coordenadasTabela[ent.cidade][1];
    double rad = ent.dir*M_PI/180.0;
    nova.pontos[2] = nova.pontos[0] + ent.max*cos(rad) + ent.med*sin(rad);
    nova.pontos[3] = nova.pontos[1] + ent.max*sin(rad) + ent.med*cos(rad);
    nova.pontos[4] = nova.pontos[0] + ent.max*cos(rad) - ent.med*sin(rad);
    nova.pontos[5] = nova.pontos[1] + ent.max*sin(rad) - ent.med*cos(rad);
    nova.dia = ent.dia;
    return nova;
}

long long unsigned int getFill(int dia){
    return 16581375L*dia/366L;
}

void printSeta(struct seta s){
    printf("<polygon points=\"",s.dia);
    for(int i=0;i<6;i++){
        printf("%.2lf",s.pontos[i]);
        if(i%2){printf(" ");}
        else{printf(",");}        
    }
    printf("\" style=\"fill:#%06x01\"/>\n",
    getFill(s.dia));
}

int contem(char *fonte,char simbolo){
    while(*fonte!='\0' && *fonte!='\n'){
        if(*fonte==simbolo) return 1;
        fonte++;
    }
    return 0;
}

void gerarElementos(FILE *origem,char tipo){
    char linha[500];
    FILE *info = fopen("vp9.csv","r");

    while(fgets(linha,500,origem)!=NULL){
        if(contem(linha,'@')){
            while(fgets(linha,500,info)!=NULL){
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
    FILE *origem = fopen("MapaVento.html","r");
    gerarElementos(origem,'m');
    fclose(origem);    
}

void gerarTabela(){
    FILE *origem = fopen("TABLE2.svg","r");
    gerarElementos(origem,'t');
    fclose(origem);
}

int main(){
    gerarMapa();
    return 0;
}
