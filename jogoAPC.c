// Matrícula: 241020760
// Rafael Medeiros de Alencar

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

typedef struct {
    char nome[20];
    int pontuacao;
} dadosjogador;

char nickname[20], arquivo[20];
char* arquivos[] = { "../iniciante.txt", "../intermediario.txt", "../avancado.txt" };
int matrizint[10][10], matriz[10][10], linhas[10], colunas[10];
int size, vidas, fase = 0, arq = 0, pos = 0, morreu = 0, pont = 0, nvjog = 1, qtdjog = 0;
dadosjogador ranking[100], jogadoratual;

FILE* fp;
FILE* fr;

void clear();
void StartLevel();
void PrintMatriz();
int Jogar();
int TmnLevel(char txt);
void StartRanking();
void AttRanking();
void SaveRanking();
int StoreRanking();

int main() {

     clear();
    printf("Seja bem vindo ao Sudoku ao Contrario!\n\nDigite seu nickname: ");
    scanf("%[^\n]s", nickname);
    StartRanking(); //Se abriu meu jogo já merece entrar no ranking. Todos são vencedores!

    int opcao = 0;
    while(opcao != 5){
        clear();
        printf("|| OPCOES ||\n\n");
        printf("1 - Jogar\n2 - Configuracoes\n3 - Instrucoes\n4 - Ranking\n5 - Sair\n\n");
        printf("Dificuldade: %d  - Fase: %d\n", arq+1, fase+1); // Nao estava no roteiro, coloquei pra me auxiliar nos testes. Acabei gostando e deixei
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        if(opcao == 1){
            // Jogar
            clear();
            StartLevel();  
            Jogar();
            AttRanking();
            SaveRanking();
        }
        else if(opcao == 2){
            // Configurações
            OP: clear(); // Clamo benevolência, meus prezados monitores e/ou professora, mas foi uma otimização necessária
            printf("*** CONFIGURACOES ***\n\n");
            printf("1 - Zerar Ranking\n2 - Modo de Dificuldade\n3 - Voltar ao menu principal\n\n");
            printf("Digite a opcao desejada: ");
            
            int config;
            scanf("%d", &config);
            if(config == 1){
                printf("Confirma reinicializar o ranking? (S/N) ");
                char a;
                getchar();
                scanf("%c", &a);

                if(a == 'N' || a == 'n'){
                    config = 0;
                    opcao = 2;
                    goto OP; // Novamente, perdão! Mas foi inevitável
                }
                else if(a == 'S' || a == 's'){
                    fclose(fr);
                    fr = fopen("ranking.bin", "w+b");
                    int um = 1;
                    fwrite(&um, sizeof(int), 1, fr);
                    memset(ranking, 0, sizeof(ranking));
                    qtdjog = 0;
                    StartRanking();
                    /*Tomei a liberdade de recriar o ranking já contendo o jogador atual da sessão com 0 pontos, como
                    normalmente é feito em outros jogos.*/ 
                }
            }
            else if(config == 2){
                DIF: clear();
                printf("*** ESCOLHA O MODO DE JOGO ***\n\n");
                printf("1 - Iniciante\n2 - Intermediario\n3 - Avancado\n4 - Retornar\n\n");
                printf("Digite a opcao desejada: ");

                int selectdif;
                scanf("%d", &selectdif);
                if(selectdif == 4) goto OP;
                else if(selectdif > 4){
                    printf("Opcao invalida. Tecle <enter> e escolha novamente.");
                    getchar();
                    getchar();
                    goto DIF;
                }
                else{
                    printf("\n\nConfigurado para modo ");
                    if(selectdif == 1) printf("INICIANTE.\n");
                    else if(selectdif == 2) printf("INTERMEDIARIO.\n");
                    else if(selectdif == 3) printf("AVANCADO.\n");
                    
                    arq = selectdif-1;
                    fase = 0;
                    morreu = 0;
                    fclose(fp);

                    printf("Tecle <enter> para continuar:");
                    getchar();
                    getchar();
                }

            }
        }
        else if(opcao == 3){
            // Instruções
            clear();
            printf("\n\nCOMO JOGAR O SUDOKU AO CONTRARIO\n\n");
            printf("O objetivo deste jogo e simples, mas exige atencao e estrategia.\n\n");
            printf("Acima e ao lado esquerdo do tabuleiro, existem alguns numeros que\ncorrespondem a soma da coluna abaixo ou da linha ao lado.\n");
            printf("Voce deve informar as posicoes dos numeros que devem ser apagados para\nque restem apenas os numeros que, somados, resultem nessa soma.\n");
            printf("Mas tome cuidado! Caso seja informado um numero que nao deve ser apagado, voce perdera 1 vida!\n\n");
            printf("Agora voce ja entendeu como tudo funciona! Pressione <enter> para retornar ao menu\n");
            getchar();
            getchar();
            //perdão, mas eu nao quis fazer uma função auxiliar pras intruções
        }
        else if(opcao == 4){
            // Ranking
            // Pelas imagens do roteiro, nao precisava dar clear no terminal na opção do ranking
            printf("\nRANKING:\n\n");
            for(int i = 0; i < qtdjog; i++){
                printf("%d - %s\n", ranking[i].pontuacao, ranking[i].nome);
            }
            printf("\nTecle <enter> para continuar>");
            getchar();
            getchar();
            
        }
    }

    StoreRanking();
    fclose(fp);
    return 0;
}

void clear(){
    system(CLEAR);  
}

void StartLevel() {
    // Abre o arquivo atual
    if(fase == 0 || morreu == 1) fp = fopen(arquivos[arq], "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", arquivos[arq]);
        return;
    }
    if(morreu == 1){
        fseek(fp, pos+1, SEEK_CUR);
        morreu = 0;
    }

    // Limpa as matrizes e somas antes de carregar os dados
    memset(matriz, 0, sizeof(matriz));
    memset(matrizint, 0, sizeof(matrizint));
    memset(linhas, 0, sizeof(linhas));
    memset(colunas, 0, sizeof(colunas));

    int faseatual;
    if(fase == 0) faseatual = 1;
    else faseatual = fase;
    char ch;

    // Avança até a fase correta no arquivo
    while (faseatual < fase && fscanf(fp, "%c", &ch) != EOF) {
        if (ch == '*') {
            fscanf(fp, "\n");
            faseatual++;
        }
    }

    // Verifica se a fase foi encontrada
    if (faseatual < fase) {
        fclose(fp);
        arq++;

        // Se for a última fase do arquivo, vai para o próximo arquivo
        if (arq >= 3) {
            printf("PARABÉNS!!! VOCÊ TERMINOU (ZEROU) O JOGO!\n");
            arq = 0;
            fase = 0;
        } else {
            fase = 0;  // Reinicia a fase para 1 ao passar para o próximo arquivo
            StartLevel();  // Recursão para iniciar o próximo arquivo
        }
        return;
    }

    // Determina o tamanho da matriz lendo a primeira linha
    size = 0;
    if(arq == 0) size = 4;
    else if(arq == 1) size = 6;
    else if(arq == 2) size = 7;

    // Lê o restante da matriz
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%c", &ch);
            matriz[i][j] = ch - '0';
        }
        fscanf(fp, "\n");  // Pula o \n
    }

    // Lê as somas das colunas e linhas
    char linha_colunas[50], linha_linhas[50];

    // Pula possíveis quebras de linha adicionais
    //fscanf(fp, "%*[\n]");

    // Carrega as somas das colunas
    if (fscanf(fp, "%s", linha_colunas) != 1) {
        printf("Erro ao ler somas das colunas.\n");
        fclose(fp);
        return;
    }
    for (int i = 0; i < size; i++) {
        colunas[i] = (linha_colunas[i * 2] - '0') * 10 + (linha_colunas[i * 2 + 1] - '0');
    }

    pos = ftell(fp); //!!!!!!! -NAO MEXER EM HIPOTESE ALGUMA- !!!!!!!!!!!

    // Carrega as somas das linhas
    if (fscanf(fp, "%s", linha_linhas) != 1) {
        printf("Erro ao ler somas das linhas.\n");
        fclose(fp);
        return;
    }
    for (int i = 0; i < size; i++) {
        linhas[i] = (linha_linhas[i * 2] - '0') * 10 + (linha_linhas[i * 2 + 1] - '0');
    }
    fscanf(fp, "%*c");

    // Lê a matriz de comparação
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%c", &ch); 
            matrizint[i][j] = ch - '0';
        }
        fscanf(fp, "%*c");
    }
    
    fscanf(fp, "*\n");

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

        // Verifica se a posição é inválida
        if (matrizint[linha][coluna] == 1) {
            vidas--;
            printf("Voce perdeu uma vida! Tecle <enter> para prosseguir\n");
            getchar();
            getchar();
            continue;
        } 
        // Se a posição é válida:
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
            printf("Parabens! Voce completou a fase! Tecle <enter> para voltar ao menu\n");
            getchar();
            fase++;
            if(arq == 0) jogadoratual.pontuacao += 50;
            else if(arq == 1) jogadoratual.pontuacao += 100;
            else if(arq == 2) jogadoratual.pontuacao += 200;
            return 1;
        
        }
        PrintMatriz();
    }

    if (vidas == 0) {
        printf("Game Over! Voce perdeu todas as suas vidas.\n");
        printf("Print <enter> para retornar ao menu inicial.\n");
        getchar();
        fclose(fp);
        morreu = 1;
        return 0;
    }
    return 0;
}

void StartRanking(){
    // Abre o arquivo do ranking ou cria um novo
    fr = fopen("ranking.bin", "r+b");
    if(fr == NULL){
        fopen("ranking.bin", "w+b");
        int um = 0;
        fwrite(&um, sizeof(int), 1, fr); // salva um int no inicio do arquivo com a quantidade de jogadores no ranking
    }
    else{
        fread(&qtdjog, sizeof(int), 1, fr); // lê um int no inicio do arquivo com a quantidade de jogadores no ranking
        fread(&ranking, sizeof(dadosjogador), qtdjog, fr); //
    }

    // Verifica se o nickname atual já está no ranking
    nvjog = 1;
    for(int i = 0; i < qtdjog; i++){
        if(strcmp(ranking[i].nome, nickname) == 0){
            jogadoratual = ranking[i]; // Caso esteja, carrega para o programa os dados relacionados
            nvjog = 0;
            break;
        }
    }

    // Se for um novo jogador, armazena dados atuais no ranking 
    //OBS: mostrar os dados do jogador atual desde o inicio da sessão foi uma escolha que eu tomei por conta própria
    if(nvjog == 1){
        strcpy(jogadoratual.nome, nickname);
        strcpy(ranking[qtdjog].nome, nickname);
        jogadoratual.pontuacao = pont;
        qtdjog++;
    }
    fclose(fr);
}

void AttRanking(){
    // Procura o jogador atual no vetor do ranking
    /*OBS: eu tentei fazer isso de um jeito mais simples e otimizado, mas por incapacidade e/ou cansaço não fui capaz...
    Fui obrigado a escolher funcionalidade antes de elegância*/
    int a;
    for(a = 0; a < qtdjog; a++){
        if(strcmp(ranking[a].nome, nickname) == 0){
            break;
        }
    }
    // Atualiza a pontuacao
    ranking[a].pontuacao = jogadoratual.pontuacao;

    // Ordena o ranking
    /*Bubblesort otimizado, faltou tempo para implementar o quicksort adequadamente e dado o tamanho
    esperado para este ranking, não deve ser um problema*/ 
    int ok = 0;
    for(int j = 0; j < qtdjog && ok == 0; j++){
        ok = 1;
        for(int i = 0; i < qtdjog-j-1; i++){
            if(ranking[i].pontuacao < ranking[i+1].pontuacao){
                ok = 0;
                dadosjogador aux = ranking[i];
                ranking[i] = ranking[i+1];
                ranking[i+1] = aux;
            }
        }
    }
}

void SaveRanking(){
    // Abre o arquivo do ranking
    fr = fopen("ranking.bin", "r+b");
    // Escreve os dados e fecha
    fwrite(&qtdjog, sizeof(int), 1, fr);
    fwrite(&ranking, sizeof(dadosjogador), qtdjog, fr);
    fclose(fr);
}

int StoreRanking(){
    // Retira players com 0 pontos
    if(jogadoratual.pontuacao == 0 && qtdjog == 1){
        qtdjog = 0;
        
        fr = fopen("ranking.bin", "w+b");
        fwrite(&qtdjog, sizeof(int), 1, fr);
        fclose(fr);
        return 0;
    }
    else if(jogadoratual.pontuacao == 0) qtdjog -= 1;

    // Escreve os dados e fecha
    fr = fopen("ranking.bin", "r+b");
    fwrite(&qtdjog, sizeof(int), 1, fr);
    fwrite(&ranking, sizeof(dadosjogador), qtdjog, fr);
    fclose(fr);
    
    return 0;
}