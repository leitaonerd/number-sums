#include <stdio.h>
#include <string.h>

char nickname[30], arquivo[20];
int matrizint[10][10], matriz[10][10], size, vidas;
int linhas[10], colunas[10];  // Variáveis globais para armazenar as somas de linhas e colunas

FILE* fp;

void clear();
void StartLevel();
void PrintMatriz();
void Jogar();
int TmnLevel(char txt);

int main() {
    fp = fopen("iniciante.txt", "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    clear();
    printf(".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n");
    printf("Ola, seja bem vindo ao jogo!\n\nDigite seu nickname: ");
    scanf("%[^\n]s", nickname);

    int opcao = 0;
    while(opcao != 5){
        clear();
        printf("|| OPCOES ||\n\n");
        printf("1 - Jogar\n2 - Configuracoes\n3 - Instrucoes\n4 - Ranking\n5 - Sair\n\n");
        printf("Digite a opcao desejada\n");
        scanf("%d", &opcao);

        if(opcao == 1){
            // Jogar
            clear();
            StartLevel();  // Inicializa o nível
            Jogar();  // Executa o jogo
        }
        else if(opcao == 2){
            // Configurações
        }
        else if(opcao == 3){
            // Instruções
        }
        else if(opcao == 4){
            // Ranking
        }
    }

    fclose(fp);
    return 0;
}

void clear(){
    system("cls");  
}

void StartLevel() {
    // Abre o arquivo
    FILE* fp = fopen("iniciante.txt", "r");
    if(fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Limpa as matrizes antes de carregar os dados
    memset(matriz, 0, sizeof(matriz));
    memset(matrizint, 0, sizeof(matrizint));
    memset(linhas, 0, sizeof(linhas));  // Limpa as somas das linhas
    memset(colunas, 0, sizeof(colunas));  // Limpa as somas das colunas

    // Variáveis para auxiliar a leitura
    char tmp;
    int i = 0, j = 0;

    // Inicializa o tamanho da matriz
    size = 0;

    // Lê a matriz principal e determina o tamanho
    while (fscanf(fp, "%c", &tmp) == 1 && tmp != '\n') {
        matriz[0][size++] = tmp - '0';  // Primeira linha
    }

    // Lê as próximas linhas da matriz
    for (i = 1; i < size; i++) {
        for (j = 0; j < size; j++) {
            fscanf(fp, "%c", &tmp);
            matriz[i][j] = tmp - '0';
        }
        fscanf(fp, "%*c");  // Pula o \n
    }

    // Carrega as somas das colunas
    char colunastmp[2 * size + 1];
    fscanf(fp, "%s", colunastmp);
    for (i = 0, j = 0; i < size; i++, j += 2) {
        colunas[i] = (colunastmp[j] - '0') * 10 + (colunastmp[j + 1] - '0');
    }

    // Carrega as somas das linhas
    char linhastmp[2 * size + 1];
    fscanf(fp, "%s", linhastmp);
    for (i = 0, j = 0; i < size; i++, j += 2) {
        linhas[i] = (linhastmp[j] - '0') * 10 + (linhastmp[j + 1] - '0');
    }

    // Carrega a matriz de comparação (matrizint)
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fscanf(fp, " %c", &tmp);  // Note the space before %c to skip any whitespace
            matrizint[i][j] = tmp - '0';
        }
    }

    // Lê o delimitador de nível '*' e prepara para o próximo nível (se necessário)
    fscanf(fp, "%*c");

    // Fecha o arquivo
    fclose(fp);
    
    vidas = 5;
    // Print na tela
    PrintMatriz();
}


void PrintMatriz() {
    clear();
    // Exibe os números das colunas no topo
    printf("    ");
    for (int i = 0; i < size; i++) {
        if(colunas[i] == -1) printf("   ");
        else printf("%3d", colunas[i]);
    }
    printf("\n    ");
    for (int i = 0; i < size; i++) {
        printf("---");
    }
    printf("\n");
    // Exibe a matriz
    for (int i = 0; i < size; i++) {
        if(linhas[i] == -1) printf("   |");
        else printf("%3d|", linhas[i]);
        for (int j = 0; j < size; j++) {
            if (matriz[i][j] == -1) {
                printf("   ");  // Exibe um espaço vazio se foi apagado
            } else {
                printf("%3d", matriz[i][j]);
            }
        }
        printf("\n");
    }
    printf("*** Voce tem %d vidas ***\n", vidas);
}

void Jogar() {
    int linha, coluna;
    vidas = 5;

    while(vidas > 0) {
        PrintMatriz();
        printf("Digite a linha e a coluna do elemento a ser apagado: ");
        scanf("%d %d", &linha, &coluna);

        // Verifica se os índices estão dentro do intervalo
        if (linha < 1 || linha > size || coluna < 1 || coluna > size) {
            printf("Posicao invalida! Pressione <enter> para tentar novamente.\n");
            getchar();
            getchar();
            continue;
        }

        // Ajusta para índice 0-baseado
        linha--;
        coluna--;

        // Verifica se a posição já foi apagada
        if (matriz[linha][coluna] == -1) {
            printf("Essa posicao ja foi apagada! Pressione <enter> para tentar de novo\n");
            getchar();
            getchar();
            continue;
        }

        // Verifica se a posição é uma bomba
        if (matrizint[linha][coluna] == 1) {
            vidas--;
            printf("Voce perdeu uma vida! Tecle <enter> para prosseguir\n");
            getchar();
            getchar();
            continue;
        } 
        else if (matrizint[linha][coluna] == 0) {
            matriz[linha][coluna] = -1;  // Marca como apagado
            printf("Muito bem! Tecle <enter> para continuar\n");
            getchar();
            getchar();

            // Verifica se a linha ou coluna está completa
            int linha_completa = 1, coluna_completa = 1;
            for (int i = 0; i < size; i++) {
                if (matriz[linha][i] != -1 && matrizint[linha][i] != 1)  // Ainda há números a serem apagados na linha
                    linha_completa = 0;
                if (matriz[i][coluna] != -1 && matrizint[i][coluna] != 1)  // Ainda há números a serem apagados na coluna
                    coluna_completa = 0;
            }

            // Apaga o valor correspondente se a linha ou coluna estiver completa
            if (linha_completa) {
                linhas[linha] = -1;  // Apaga a soma da linha
                printf("Parabens! Voce completou a linha %d! Tecle <enter>\n", linha + 1);
                getchar();
            }
            if (coluna_completa) {
                colunas[coluna] = -1;  // Apaga a soma da coluna
                printf("Parabens! Voce completou a coluna %d! Tecle <enter>\n", coluna + 1);
                getchar();
            }
        }

        // Verifica se o jogo acabou (todas as posições apagadas ou todas as vidas perdidas)
        int jogo_completo = 1;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (matrizint[i][j] == 0 && matriz[i][j] != -1 && matriz[i][j] != 1) {
                    jogo_completo = 0;
                    break;  // Pode sair do loop cedo se encontrar um número não apagado
                }
            }
            if (!jogo_completo) break;
        }
        if (jogo_completo) {
            printf("Parabens! Voce completou o jogo! Tecle <enter> para voltar\n");
            getchar();
            break;
        }
        PrintMatriz();
    }

    if (vidas == 0) {
        printf("Game Over! Voce perdeu todas as suas vidas.\n");
    }
    
}


