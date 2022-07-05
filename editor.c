#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define M_PI 3.14159265358979323846

void LOG(char *mensagem){
    FILE *logger = fopen("log.txt","a");
    unsigned long tempo = (unsigned long)time(NULL);
    char marcaTempo[30];
    int s = tempo%60;
    tempo/=60;
    int m = tempo%60;
    tempo/=60;
    int h = tempo%24;    
    sprintf(marcaTempo,"%dh %dm %ds",h,m,s);
    fprintf(logger,"%s > %s\n",marcaTempo,mensagem);
    fclose(logger);
}

const char cidade[8][12] ={
    "APODI",
    "MOSSORO",
    "CAICO",
    "IPANGUACU",
    "MACAU",
    "SANTA CRUZ",
    "CALCANHAR",
    "NATAL"
};

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
        char msg[50];
        sprintf(msg,"Marcou cidade %d",i);
        LOG(msg);
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
            char msg[50];
            sprintf(msg,"desenhou raio %d em %d",raio,i);
            LOG(msg); 
        }

        for(double ang=0;ang<6.28;ang+=M_PI/12){
            printf("\t\t\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"/>\n",
            coordenadas[i][0],
            coordenadas[i][1],
            coordenadas[i][0] + raio*cos(ang),
            coordenadas[i][1] + raio*sin(ang)
            );
            char msg[50];
            sprintf(msg,"desenhou traco %g em %d",ang,i);
            LOG(msg);
        }
    }
    printf("\t\t</g>\n");
}

void partes(char *linha,char partes[][10]){
    int i=0,j=0,k=0;;
    while(linha[j]!='\0' && linha[j]!='\n'){
        while(linha[j]!=',' && linha[j]!='\n'){
            partes[i][k++] = linha[j++];
        }
        partes[i][k]='\0';
        LOG(partes[i]);
        j++;
        i++;
        k=0;
    }    
}

void lerDados(double valores[8][3][24]){
    FILE *dados = fopen("tratado2.csv","r");    
    LOG("Lendo dados");
    int ind[3];
    double pres[3];
    char linha[200];
    while(fgets(linha,200,dados)!=NULL){
        char colunas[6][10];
        partes(linha,colunas);  
        LOG(linha);
        int i=0;
        for(int j=0;j<3;j++){
            ind[j]=atoi(colunas[i++]);
        }        
        for(int j=0;j<3;j++){
            valores[ind[0]][j][ind[1]] = (atof(colunas[i++]) - 980)*2;      
            char msg[50];
            sprintf(msg,"string:\t%s\tdouble:\t%g\n"
                ,colunas[i-1]
                ,valores[ind[0]][j][ind[1]]);
            LOG(msg);
        }
        char msg[50];
        sprintf(msg,"antes:\t%g\tdurante:\t%g\tdepois:\t%g"
            ,valores[ind[0]][0][ind[1]]
            ,valores[ind[0]][1][ind[1]]
            ,valores[ind[0]][2][ind[1]]);
        LOG(msg);
    }
    fclose(dados);
}

void gerarLinhas(){
    double dados[8][3][24];
    LOG("gerando linhas");
    for(int i=0;i<8;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<24;k++){
                dados[i][j][k]=0;
            }
        }
    }
    LOG("gerou matriz nula");
    char cores[3][10] = {"red","blue","azure"};
    LOG("Definiu as cores");
    lerDados(dados);
    printf("\t\t<g id=\"linhas\" fill=\"none\" stroke-width=\"2\">\n");
    char markers[3][3] = {"mS","mX","mC"};
    char desc[3][8] = {"Antes","Durante","Depois"};
    for(int cid=0;cid<8;cid++){
        for(int pres=0;pres<3;pres++){
            for(int h=0;h<24;h++){
                if(dados[cid][pres][h]>0){                    
                    printf("\t\t\t<g stroke=\"%s\">\n",cores[pres]);
                    printf("\t\t\t\t<title id=\"tit%g\">%s: %dh > %g (%s)</title>\n",
                        cid*10000+pres*100+h,
                        cidade[cid],
                        h,
                        dados[cid][pres][h]*2+980,
                        desc[pres]);
                    double pos;
                    if(h!=23 && dados[cid][pres][h+1]>0){
                        pos = dados[cid][pres][h+1];
                    }else if(h!=0 && dados[cid][pres][h-1]>0){
                        pos = dados[cid][pres][h-1];
                    }else{
                        pos = dados[cid][pres][h];
                    }
                    printf("\t\t\t\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" />\n",
                    coordenadas[cid][0]+dados[cid][pres][h]*cos((M_PI*h)/12),
                    coordenadas[cid][1]+dados[cid][pres][h]*sin((M_PI*h)/12),
                    coordenadas[cid][0]+pos*cos((M_PI*(h+1))/12),
                    coordenadas[cid][1]+pos*sin((M_PI*(h+1))/12));
                }
                printf("\t\t\t</g>\n");
            }   
        }
    }
    printf("\t\t<g>\n");
}

void gerarMapa(){
    FILE *origem = fopen("view/index.html","r");
    char linha[200];
    LOG("iniciou o programa");
    while(fgets(linha,200,origem)!=NULL){
        if(linha[0]=='@'){
            LOG("achou o arroba");
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
