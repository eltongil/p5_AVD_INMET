#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define total_linhas 197203

char** partes(char *linha){
    char **ret;
    ret = malloc(sizeof(char*)*6);
    for(int i=0;i<6;i++){
        ret[i] = malloc(sizeof(char)*10);
        int j=0;
        while(*linha!=',' && *linha!='\n'){
            ret[i][j++]=*linha++;
        }
        ret[i][j]='\0';
        *linha++;
    }
    return ret;
}

typedef struct row{
    char local[11];
    int ano;
    int dia;
    int hora;
    double chuva;
    double p_atual;
    double p_antes;
    double p_depois;
}*medida;

void printRow(struct row r){
    printf("%s,%02d,%.02f,%.02f,%.02f\n",r.local,r.hora,r.p_antes,r.p_atual,r.p_depois);
}

void printRowp(struct row *r){
    printRow(*r);
}

struct row *novaRow(char *linha){
    struct row *nova = (struct row *) malloc(sizeof(struct row));
    char **colunas = partes(linha);
    strncpy(nova->local,colunas[0],11);
    nova->ano = atoi(colunas[1]);
    nova->dia = atoi(colunas[2]);
    nova->hora = atoi(colunas[3]);
    nova->chuva = atof(colunas[4]);
    nova->p_atual = atof(colunas[5]);
    for(int i=0;i<6;i++){
        free(colunas[i]);
    }
    free(colunas);
    return nova;
}

int main(){
    FILE *origem  = fopen("base.csv","r");
    FILE *saida = fopen("tratado.csv","w");
    char linha[100];
    medida dados[total_linhas];
    int indice = 0;
    while(fgets(linha,100,origem)){
        dados[indice++] = novaRow(linha);
    }
    for(indice=0;indice<total_linhas;indice++){
        if(indice%1000==0)
            printf("\r%d%%",100*indice/total_linhas);
        if(dados[indice]->chuva>1){
            double media = dados[indice]->p_atual;
            int medidasComChuva = 1;
            //antes
            dados[indice]->p_antes = dados[indice-1]->p_atual;
            //depois
            while(dados[indice+medidasComChuva]->chuva>1){
                media+=dados[indice+medidasComChuva]->p_atual;
                medidasComChuva++;
            }
	    dados[indice]->p_depois = dados[indice+medidasComChuva]->p_atual;
	    //atual
            dados[indice]->p_atual = media/medidasComChuva;
	    //arquivo
            fprintf(saida,
                    "%s,%d,%d,%g,%g,%g\n",
                    dados[indice]->local,
                    dados[indice]->hora,
		    medidasComChuva,
                    dados[indice]->p_antes,
                    dados[indice]->p_atual,
                    dados[indice]->p_depois);
	    //pula horas já consideradas
	    indice+=medidasComChuva-1;
        }
    }
    
    for(indice=0;indice<total_linhas;indice++){
        free(dados[indice]);
    }
    fclose(origem);
    fclose(saida);
    return 0;
}
