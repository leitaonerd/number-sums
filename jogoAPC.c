#include <stdio.h>
#include <string.h>

char nickname[30], arquivo[20];
char* arquivos[] = { "iniciante.txt", "intermediario.txt", "avancado.txt" };
int matrizint[10][10], matriz[10][10], size, vidas, fase = 0, arq = 0; pos = 0;
int linhas[10], colunas[10];  // Variáveis globais para armazenar as somas de linhas e colunas

FILE* fp;

void clear();
void StartLevel();
void PrintMatriz();
int Jogar();
int TmnLevel(char txt);

int main() {
    fp = fopen("iniciante.txt", "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    clear();
    printf(".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n");
    printf("Ola, seja bem vindo ao Jogo das Somas!\n\nDigite seu nickname: ");
    scanf("%[^\n]s", nickname);

    int opcao = 0;
    while(opcao != 5){
        clear();
        printf("|| OPCOES ||\n\n");
        printf("1 - Jogar\n2 - Configuracoes\n3 - Instrucoes\n4 - Ranking\n5 - Sair\n\n");
        printf("Fase: %d\n", fase+1);
        printf("Digite a opcao desejada\n");
        scanf("%d", &opcao);

        if(opcao == 1){
            // Jogar
            clear();
            StartLevel();  
            fase += (Jogar()); 
        }
        else if(opcao == 2){
            // Configurações
        }
        else if(opcao == 3){
            // Instruções
            clear();
            printf("\n\nINSTRUCOES SOBRE O JOGO DAS SOMAS\n\n");
            printf("O objetivo deste jogo e simples, mas exige atencao e estrategia.\n\n");
            printf("Acima e ao lado esquerdo do tabuleiro, existem alguns numeros que\ncorrespondem a soma da coluna abaixo ou da linha ao lado.\n");
            printf("Voce deve informar as posicoes dos numeros que devem ser apagados para\nque restem apenas os numeros que, somados, resultem nessa soma.\n");
            printf("Mas tome cuidado! Caso seja informado um numero que nao deve ser apagado, voce perdera 1 vida!\n\n");
            printf("Agora voce ja entendeu como tudo funciona! Pressione <enter> para retornar ao menu\n");
            getchar();
            getchar();
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
    // Abre o arquivo atual
    FILE* fp = fopen(arquivos[arq], "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", arquivos[arq]);
        return;
    }

    // Limpa as matrizes e somas antes de carregar os dados
    memset(matriz, 0, sizeof(matriz));
    memset(matrizint, 0, sizeof(matrizint));
    memset(linhas, 0, sizeof(linhas));
    memset(colunas, 0, sizeof(colunas));

    int faseatual = 1;
    char ch;

    // Avança até a fase correta no arquivo
    while (faseatual < fase && fscanf(fp, "%c", &ch) != EOF) {
        if (ch == '*') {
            faseatual++;
        }
    }

    // Verifica se a fase foi encontrada
    if (faseatual < fase) {
        fclose(fp);
        arq++;

        // Se for a última fase do arquivo, vai para o próximo arquivo
        if (arq >= 3) {
            printf("Parabéns! Você completou todas as fases disponíveis.\n");
            arq = 0;
            fase = 1;
        } else {
            fase = 1;  // Reinicia a fase para 1 ao passar para o próximo arquivo
            StartLevel();  // Recursão para iniciar o próximo arquivo
        }
        return;
    }

    // Determina o tamanho da matriz lendo a primeira linha
    size = 0;
    while (fscanf(fp, "%c", &ch) == 1 && ch != '\n') {
        matriz[0][size++] = ch - '0';  // Primeira linha
    }

    // Lê o restante da matriz
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%c", &ch);
            matriz[i][j] = ch - '0';
        }
        fscanf(fp, "%*c");  // Pula o \n
    }

    // Lê as somas das colunas e linhas
    char linha_colunas[50], linha_linhas[50];

    // Pula possíveis quebras de linha adicionais
    fscanf(fp, "%*[\n]");

    // Carrega as somas das colunas
    if (fscanf(fp, "%s", linha_colunas) != 1) {
        printf("Erro ao ler somas das colunas.\n");
        fclose(fp);
        return;
    }
    for (int i = 0; i < size; i++) {
        colunas[i] = (linha_colunas[i * 2] - '0') * 10 + (linha_colunas[i * 2 + 1] - '0');
    }

    // Carrega as somas das linhas
    if (fscanf(fp, "%s", linha_linhas) != 1) {
        printf("Erro ao ler somas das linhas.\n");
        fclose(fp);
        return;
    }
    for (int i = 0; i < size; i++) {
        linhas[i] = (linha_linhas[i * 2] - '0') * 10 + (linha_linhas[i * 2 + 1] - '0');
    }

    // Lê a matriz de comparação
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, " %c", &ch);  // Espaço antes de %c para ignorar whitespace
            matrizint[i][j] = ch - '0';
        }
    }

    // Fecha o arquivo
    fclose(fp);

    // Inicializa as vidas
    vidas = 5;

    // Imprime na tela
    PrintMatriz();
}



void PrintMatriz() {
    clear();
    
    printf("    ");
    for (int i = 0; i < size; i++) {
        if(colunas[i] == -1) printf("   ");
        else printf("%3d", colunas[i]);
    }
    printf("\n    ");
    for (int i = 0; i < size; i++) {
        printf("___");
    }
    printf("_\n");
    
    for (int i = 0; i < size; i++) {
        if(linhas[i] == -1) printf("   |");
        else printf("%3d|", linhas[i]);
        for (int j = 0; j < size; j++) {
            if (matriz[i][j] == -1) {
                printf("   ");  
            } else {
                printf("%3d", matriz[i][j]);
            }
        }
        printf("\n");
    }
    printf("*** Voce tem %d vidas ***\n", vidas);
}

int Jogar() {
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

        // Ajusta indice para 0
        linha--;
        coluna--;

        // Verifica se já foi apagada
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
                if (matriz[linha][i] != -1 && matrizint[linha][i] != 1)  
                    linha_completa = 0;
                if (matriz[i][coluna] != -1 && matrizint[i][coluna] != 1) 
                    coluna_completa = 0;
            }

            // Apaga o valor correspondente se a linha ou coluna estiver completa
            if (linha_completa) {
                linhas[linha] = -1;
                if (coluna_completa) {
                    colunas[coluna] = -1;  
                    printf("Parabens! Voce completou a linha %d e a coluna %d! Tecle <enter>\n", linha + 1, coluna + 1);
                    getchar();
                }
                else{
                printf("Parabens! Voce completou a linha %d! Tecle <enter>\n", linha + 1);
                getchar();
                }
            }
            else if (coluna_completa) {
                colunas[coluna] = -1;  
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
            printf("Parabens! Voce completou o jogo! Tecle <enter> para voltar ao menu\n");
            getchar();
            return 1;
            break;
        }
        PrintMatriz();
    }

    if (vidas == 0) {
        printf("Game Over! Voce perdeu todas as suas vidas.\n");
        return 0;
    }
    
}


