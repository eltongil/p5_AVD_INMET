#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define M_PI 3.14159265358979323846

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

void marcarCidades(){
    printf("\t\t<g id=\"marcas\" stroke=\"black\" fill=\"red\" stroke-width=\"1\";>");
    for(int i=0;i<8;i++){
        printf("\t\t\t<circle cx=\"%g\" cy=\"%g\" r=\"%d\"/>\n",
        coordenadas[i][0],
        coordenadas[i][1],
        10);
    }
    printf("\t\t</g>\n");
}

void gerarGrades(){
    printf("\t\t<g id=\"grades\" stroke=\"black\" stroke-width=\"1\" fill=\"none\">\n");
    double raio = 80;
    for(int i=0;i<8;i++){    
        for(int raio=20;raio<=80;raio+=20){
            printf("\t\t\t<circle cx=\"%g\" cy=\"%g\" r=\"%d\"/>\n",
            coordenadas[i][0],
            coordenadas[i][1],
            raio);    
        }

        for(double ang=0;ang<2*M_PI;ang+=M_PI/12){
            printf("\t\t\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"/>\n",
            coordenadas[i][0],
            coordenadas[i][1],
            coordenadas[i][0] + raio*cos(ang),
            coordenadas[i][1] + raio*sin(ang)
            );
        }
    }
    printf("\t\t</g>\n");
}

void lerDados(double valores[8][3][24]){
    FILE *dados = fopen("tratado.csv","r");    
    int ind[3];
    double pres[3];
    char linha[100];
    while(fgets(linha,100,dados)!=NULL){
        sscanf(linha,"%d,%d,%d,%lf,%lf,%lf",
        &ind[0],
        &ind[1],
        &ind[2],
        &pres[0],
        &pres[1],
        &pres[2]
        );
        for(int i=0;i<3;i++){
            valores[ind[0]][i][ind[1]] = pres[i] - 980;
        }        
    }
    fclose(dados);
}

void gerarLinhas(){
    double dados[8][3][24];
    char cores[3][10] = {"red","blue","azure"};
    lerDados(dados);
    printf("\t\t<g id=\"linhas\" stroke-width=\"1\">\n");
    for(int cid=0;cid<8;cid++){
        for(int pres=0;pres<3;pres++){
            printf("\t\t\t<polyline points=\"");
            for(int h=0;h<24;h++){
                printf("%g,%g ",
                coordenadas[cid][0]+dados[cid][pres][h]*cos((M_PI/24)*h),
                coordenadas[cid][1]+dados[cid][pres][h]*sin((M_PI/24)*h));
            }   
            printf("\" stroke-color=\"%s\"/>\n",cores[pres]);
        }
    }
    printf("\t\t<g>\n");
}

void gerarMapa(){
    FILE *origem = fopen("view/index.html","r");
    char linha[200];
    while(fgets(linha,200,origem)!=NULL){
        if(linha[0]=='@'){
            marcarCidades();
            gerarGrades();
            gerarLinhas();
        }else{
            printf("%s",linha);
        }
    }
    fclose(origem);    
}

int main(){
    gerarMapa();
    return 0;
}
