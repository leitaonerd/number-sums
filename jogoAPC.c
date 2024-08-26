#include <stdio.h>
#include <string.h>

char nickname[30], arquivo[20];
int matrizint[10][10], matriz[10][10];

FILE* fp;
fp = fopen("iniciante.txt", "r");

void clear();
void StartLevel();
int TmnLevel(char txt);

int main() {

    clear();
    printf("Olá, seja bem vindo ao jogo!\n\n Digite seu nickname: ");
    scanf("%[^\n]s", nickname);

    int opcao = 0;
    while(opcao != 5){
        clear();
        printf("1 - Jogar\n2 - Configurações\n3 - Instruções\n4 - Ranking\n5 - Sair\n");
        printf("Digite a opção desejada\n");
        scanf("%d", &opcao);

        if(opcao == 1){
            //Jogar
            clear();
            StartLevel()


        }
        else if(opcao == 2){
            //Configurações

        }
        else if(opcao == 3){
            //Intruções

        }
        else if(opcao == 4){
            //Ranking

        }
    }

    fclose(fp);
    return 0;
}

void clear(){
    system("cls");
}

void StartLevel(){
    //Load Matriz Numerica
    int i;
    char tmp = '0';
    for(i = 0; tmp != '\n', i++){
        fscanf(arquivo, "%c", &tmp);
        matriz[0][i] = tmp - '0';
    }
    int tmn_matriz = i;

    for(i = 1; i < tmn_matriz; i++){
        for(int j = 0; j < tmn_matriz; j++){
            fscanf(arquivo, "%c", &tmp);
            matriz[i][j] = tmp - '0';
        }
    }

    //Load somas de linhas e colunas
    //Colunas
    char colunastmp[20];
    int colunas[8];

    fscanf(arquivo, "%s", colunastmp);
    int idx = 0;
    for(int i = 0; i < strlen(colunastmp); i+=2){
        int soma = 0;
        soma += (colunastmp[i]-'0')*10;
        soma += (colunastmp[i+1]-'0');

        colunas[idx] = soma;
        idx++;
    }

    //Linhas
    char linhastmp[20];
    int linhas[8];

    fscanf(arquivo, "%s", linhastmp);
    idx = 0;
    for(int i = 0; i < strlen(linhastmp); i+=2){
        int soma = 0;
        soma += (linhastmp[i]-'0')*10;
        soma += (linhastmp[i+1]-'0');

        linhas[idx] = soma;
        idx++;
    }

    //Load Matriz de referecia
    tmp = '0';
    for(i = 0; tmp != '\n', i++){
        fscanf(arquivo, "%c", &tmp);
        matrizint[0][i] = tmp - '0';
    }
    int tmn_matriz = i;

    for(i = 1; i < tmn_matriz; i++){
        for(int j = 0; j < tmn_matriz; j++){
            fscanf(arquivo, "%c", &tmp);
            matrizint[i][j] = tmp - '0';
        }
    }

    //print na tela
    printf("   ");
    for(i = 0; i < idx; i++){
        printf("%3d", colunas[i]);
    }
    printf("\n");
    printf("   -------------\n");
    for(i = 0; i < idx; i++){
        printf("%3d|", linhas[idx]);
        for(int j = 0; j < idx; j++){
            printf("%3d", matriz[idx][j]);
        }
        printf("\n");
    }    
}
