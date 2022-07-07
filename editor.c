#include <math.h>
#include <string.h>
#include <time.h>
#include "constantes.h"

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

void marcarCidades(){
    printf("\t\t<g id=\"marcas\" stroke=\"gray\" fill=\"pink\" stroke-width=\"0.5\">\n");
    for(int i=0;i<8;i++){
        printf("\t\t\t<g id=\"marca_%s\">\n",cidade[i]);
        printf("\t\t\t\t<title>%s</title>\n",cidade[i]);
        printf("\t\t\t\t<circle cx=\"%g\" cy=\"%g\" r=\"%d\"/>\n",
        coordenadas[i][0],
        coordenadas[i][1],
        10);
        printf("\t\t\t</g>\n");
        char msg[50];
        sprintf(msg,"Marcou cidade %d",i);
        LOG(msg);
    }
    printf("\t\t</g>\n");
}

void gerarRaios(int i){  
    for(int raio=10;raio<=60;raio+=10){
        printf("\t\t\t<g id=\"raio_%d_%s\">\n",raio,cidade[i]);            
        printf("\t\t\t\t<title>%d mBar</title>\n",980+raio*4/3);
        printf("\t\t\t\t<circle cx=\"%g\" cy=\"%g\" r=\"%d\"/>\n",
            coordenadas[i][0],
            coordenadas[i][1],
            raio);   
        printf("\t\t\t</g>\n");
        char msg[50];
        sprintf(msg,"desenhou raio %d em %d",raio,i);
         LOG(msg); 
    }
}

void gerarTracos(int i){
    double raio = 60;
    for(double hora=0;hora<24;hora++){
        double ang = (hora-6)*M_PI/12;
        printf("\t\t<g id=\"tra%s%d\">\n",cidade[i],hora);
        printf("<title>%g h</title>\n",hora);
        printf("\t\t\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"/>\n",
            coordenadas[i][0] + 10*cos(ang) ,
            coordenadas[i][1] + 10*sin(ang) ,
            coordenadas[i][0] + raio*cos(ang),
            coordenadas[i][1] + raio*sin(ang));
        if(((int)hora)%6==0){
            printf("<text x=\"%g\" y=\"%g\" font-family=\"sans-serif\" stroke-width=\"0\" fill=\"gray\" dominant-baseline=\"central\" text-anchor=\"middle\" font-size=\"4\">%gh</text>\n",
                coordenadas[i][0] + (raio+7)*cos(ang),
                coordenadas[i][1] + (raio+7)*sin(ang),
                hora);
        }
        printf("\t\t</g>\n");
        char msg[50];
        sprintf(msg,"desenhou traco %g em %d",ang,i);
        LOG(msg);
    }
}

void gerarGrades(){
    printf("\t\t<g id=\"grades\" stroke=\"gray\" stroke-width=\"0.5\" fill=\"none\">\n");
    for(int i=0;i<8;i++){  
        //gerarRaios(i);
        gerarTracos(i);
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
            valores[ind[0]][j][ind[1]] = (atof(colunas[i++]) - 975.0)*1.5;      
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

void marca(double x,double y,int id,double tam){
    printf("\t\t\t\t<");
    switch(id){
        case 1:
            printf("rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\"",x-tam/2,y-tam/2,tam,tam);
        break;
        case 2:
            printf("circle cx=\"%g\" cy=\"%g\" r=\"%g\"",x,y,tam/2);
        break;
        case 0:        
            printf("polygon points=\"%g,%g %g,%g %g,%g \"",
                x+tam/2,y+tam/2,
                x,y-tam/2,
                x-tam/2,y+tam/2);
        break;
        default:
        printf("ERRO");
    }
    printf("/>\n");
}

void radar(double x,double y,int hora,int tipo,double tam,double diff){
    double dy = cos(M_PI*(hora-6)/12)*tam/2;
    double dx = sin(M_PI*(hora-6)/12)*tam/2;
    double rx = cos(M_PI*(hora-6)/12)*diff*2;
    double ry = sin(M_PI*(hora-6)/12)*diff*2;
    printf("\t\t\t\t<");
    switch (tipo){
        case 0:
            printf("line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" ",
                x-dx,
                y+dy,
                x-dx+rx,
                y+dy+ry);
        break;
        case 1:
            printf("line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" stroke-width=\"0.7\"",
                x-dx,
                y+dy,
                x+dx,
                y-dy);
        break;
        case 2:
            printf("line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" ",
                x+dx,
                y-dy,
                x+dx+rx,
                y-dy+ry);
        break;
        default:
            printf("ERRO");
    }
    printf("/>\n");
}

void gerarTriangulos(double x,double y,int hora,int tipo,double tam,double diff){
    if(tipo==1){
        radar(x,y,hora,tipo,tam,diff);
    }else{
        double dy = cos(M_PI*(hora-6)/12)*tam/2;
        double dx = sin(M_PI*(hora-6)/12)*tam/2;
        double rx = cos(M_PI*(hora-6)/12)*diff*2;
        double ry = sin(M_PI*(hora-6)/12)*diff*2;
        printf("\t\t\t\t<polygon stroke=\"none\" fill=\"%s\"",cores[tipo]);
        if(tipo){
            printf("points=\"%g,%g %g,%g %g,%g\"",
                x,y,
                x+dx,y-dy,
                x+dx+rx, y-dy+ry
                );
        }else{
            printf("points=\"%g,%g %g,%g %g,%g\"",
                x,y,
                x-dx,y+dy,
                x-dx+rx,y+dy+ry
                );
        }
        printf("/>\n");
    }
}

void gerarLinhas(int tipo){
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
    lerDados(dados);
    printf("\t\t<g id=\"linhas\" fill=\"none\" stroke-width=\"1\">\n");
    for(int cid=0;cid<8;cid++){
        for(int pres=0;pres<3;pres++){
            for(int h=0;h<24;h++){
                printf("\t\t\t<g id=\"tit%d\" stroke=\"%s\">\n",cid*10000+pres*100+h,cores[pres]);                                            
                printf("\t\t\t\t<title>%s: %dh > %g (%s)</title>\n",                        
                    cidade[cid],
                    h,
                    dados[cid][pres][h]+980,
                    desc[pres]);   
                double cx = coordenadas[cid][0]+dados[cid][1][h]*cos((M_PI*(h-6))/12);
                double cy = coordenadas[cid][1]+dados[cid][1][h]*sin((M_PI*(h-6))/12);
                double diff = 0;
                if(pres!=1){
                    diff = dados[cid][pres][h] - dados[cid][1][h];
                }
                if(dados[cid][pres][h]>0){   
                    switch(tipo){ 
                        case 0:                 
                            marca(cx,cy,pres,5);
                        break;
                        case 1:                            
                            gerarTriangulos(cx,cy,h,pres,5,diff);
                        break;
                        default:                        
                    }   
                    printf("\t\t\t</g>\n");
                }
            }
        }
    }
    printf("\t\t</g>\n");
}


void legenda(){
    printf("\t\t<svg id=\"legenda\" fill=\"none\" stroke-width=\"4\">\n");
    for(int i=0;i<3;i++){
        printf("\t\t\t<g id=\"legenda_%s\" stroke=\"%s\">\n",desc[i],cores[i]);
        printf("\t\t\t\t<title>%s</title>\n",desc[i]);
        gerarTriangulos(20+i*70,50,0,i,25,25);
        printf("\t\t\t\t<text text-anchor=\"middle\" x=\"%d\" y=\"70\" stroke-width=\"1\" stroke=\"black\" fill=\"black\">%s</text>\n",30+i*70,desc[i]);
        printf("\t\t\t</g>\n");
    }
    printf("\t\t</svg>\n");
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
            gerarLinhas(1);
        }else if(linha[0]=='_'){
            legenda();
        }
        else{
            printf("%s",linha);
        }
    }
    fclose(origem);    
}

int main(){
    gerarMapa();
    return 0;
}
