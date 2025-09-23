#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Todas as Funções

void shuffle(char *, int);

void swap(char *, char *);

int validar_jogada(char **, int , int , int , int, int, char);

int existe_jogada_valida(char **, int, int, int, char);

int insert(char **, int, int, int, char);

int vitoria(char **, int, int);

int pontosx(char **, int);

int main() {
    
    // Gerador Aleatório
    srand(time(NULL)); 

    int n, nxn, i, j;
    char **v;
    char simbolos[] = {'|', '/', '-', '\\'};
    int num_simbolos = 4;

    // Tamanho do Tabuleiro
    printf("Digite o tamanho do tabuleiro (N x N)\n");
    scanf("%d", &n);
    nxn = n * n;
    
    while (nxn % num_simbolos != 0 && n < 3) {
        printf("O numero de casas (N*N) deve ser divisivel por 4.\n");
        printf("Digite um tamanho de tabuleiro valido (ex: 2, 4, 6...)\n");
        scanf("%d", &n);
        nxn = n * n;
    }
    
    // Alocação dinâmica da matriz
    v = (char **) malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        v[i] = (char *) malloc(n * sizeof(char));
    }

    // Criando um array temporário com a quantidade exata de cada símbolo
    char *temp_array = (char *)malloc(nxn * sizeof(char));
    int simbolos_por_tipo = nxn / num_simbolos;
    int index = 0;

    for (i = 0; i < num_simbolos; i++) {
        for (j = 0; j < simbolos_por_tipo; j++) {
            temp_array[index++] = simbolos[i];
        }
    }

    // Embaralhando o array temporário
    shuffle(temp_array, nxn);

    // Copiando os valores embaralhados para a matriz 2D
    index = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            v[i][j] = temp_array[index++];
        }
    }

    // Ate quantas vezes se pode jogar os donuts
    int donuts_jogo = 0;

    // Guardar ultima linha, coluna e simbolo
    int ultimo_i = -1, ultimo_j = -1;
    char ultimo_simbolo = '?';

    // Vitoria jogo da velha 
    int vtr = 0;

    // Ate quajtas peças se pode jogar
    do {

        // Visual
        printf("\n");
        for (i = 0; i < n + 1; i++) {
            if (i == 0) {
                printf(" \t");
            } else {
                printf("%d\t", i - 1); // Ajustado para começar do 0
            }
        }
        printf("\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < n + 1; j++) {
                if (j == 0) {
                    printf("%d\t", i);
                } else {
                    printf("%c\t", v[i][j - 1]);
                }
            }
            printf("\n");
        }
        printf("\n");

        // Para o loop se houve uma vitoria pelas regras do jogo da velha
        if (vtr == 1 ) {
            int letra;
            if ((donuts_jogo - 1) % 2 == 0) {
                letra = 1;
            }
            else {
                letra = 2;
            }
            printf("Vitoria do jogador %d", letra);
            break;
        }

        // Qual jogador vai jogar
        do {
            if (donuts_jogo % 2 == 0) {
                printf("Vez do Jogador 1 (O proximo jogador so ira jogar quando voce fizer um lance valido)\n");
                printf("Escreva a linha e depois a coluna desejada\n");
                scanf("%d %d", &i, &j);
            }
            else {
                printf("Vez do Jogador 2 (O proximo jogador so ira jogar quando voce fizer um lance valido)\n");
                printf("Escreva a linha e depois a coluna\n");
                scanf("%d %d", &i, &j);
            }
        } while (!validar_jogada(v, n, i, j, ultimo_i, ultimo_j, ultimo_simbolo));

        // Guardar ultima linha, coluna e simbolo
        ultimo_i = i;
        ultimo_j = j;
        ultimo_simbolo = v[i][j];

        // Digita no tabuleiro o X e o O
        if (donuts_jogo % 2 == 0) {
            v[i][j] = 'X';
        }
        else {
            v[i][j] = 'O';
        }

        // Função insert
        insert(v, n, i, j, v[i][j]);

        // Que pessoa esta jogando
        int pessoa;
        if (donuts_jogo % 2 == 0) {
            pessoa = 1;
        }
        else {
            pessoa = 2;
        }

        // 
        if(vitoria(v, n, pessoa) == 1) {
            vtr = 1;
        }

        // Conta mais um donuts no jogo
        donuts_jogo++;
    } while (donuts_jogo < nxn - 2);

    // Vitoria por pontos
    if (donuts_jogo == nxn - 2) {
        int px, po;

        // Quantos pontos o jogador 1 e 2 tem
        px = pontosx(v, n);
        po = (n - 2) - px;

        if (px < po) {
            printf("O jogador 2 ganhou.\n");
        }
        else if (px > po) {
            printf("O jogador 1 ganhou.\n");
        }
        else {
            printf("Impate tecnico\n");
        }
        printf("Pontos do jogador 1: %d\nPontos do Jogador 2: %d", px, po);
    }

    // Libera a memória alocada
    free(temp_array);
    for (i = 0; i < n; i++) {
        free(v[i]);
    }
    free(v);

    return 0;
}

// Função para embaralhar um array
void shuffle(char *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

// Função para trocar dois caracteres
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Função para validar jogada
int validar_jogada(char **v, int n, int i, int j, int ultimo_i, int ultimo_j, char ultimo_simbolo) {

    // 1. Limite das peças
    if (i < 0 || i >= n || j < 0 || j >= n) {
        printf("Jogada Fora do Tabuleiro\n");
        return 0;
    }

    // 2. Verifica se a casa está ocupada
    if (v[i][j] == 'X' || v[i][j] == 'O') {
        printf("Esta casa ja esta ocupada\n");
        return 0;
    }
    
    // Se for a primeira jogada, pode em qualquer lugar
    if (ultimo_i == -1 && ultimo_j == -1) {
    return 1;
    }

    // Se ainda existem jogadas possíveis seguindo a regra
    if (existe_jogada_valida(v, n, ultimo_i, ultimo_j, ultimo_simbolo)) {

    // 3. Verificar restrição pelo símbolo da última jogada
        switch (ultimo_simbolo) {
            case '|': // vertical
                if (j == ultimo_j) return 1;
            break;
            case '-': // horizontal
                if (i == ultimo_i) return 1;
            break;
            case '/': // diagonal secundária
                if (i + j == ultimo_i + ultimo_j) return 1;
            break;
            case '\\': // diagonal principal
                if (i - j == ultimo_i - ultimo_j) return 1;
            break;
        }
    }
    else {
        // Se não existe jogada possível, libera qualquer casa vazia
        return 1;
    }
    return 0;
}

// Função para determinar se existe jogada valida
int existe_jogada_valida(char **v, int n, int ultimo_i, int ultimo_j, char ultimo_simbolo) {

    // Analisa todas as casas da matriz
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < n ; j++) {

            // Analise se a casa está vazia (sem X ou O)
            if (v[i][j] == 'X' || v[i][j] == 'O') {
            continue; 
            }

            // Verificamos se ela respeita a restrição do símbolo da última jogada
            switch (ultimo_simbolo) {
                case '|': if (j == ultimo_j) return 1; 
                break;
                case '-': if (i == ultimo_i) return 1; 
                break;
                case '/': if (i + j == ultimo_i + ultimo_j) return 1; 
                break;
                case '\\': if (i - j == ultimo_i - ultimo_j) return 1; 
                break;
            }
        }
    }

    // Nenhuma jogada possível seguindo o símbolo
    return 0; 
}

// Função para determinar se houve insert
int insert(char **v, int n, int i, int j, char valor) {
    int alterou = 0;
    char adversario = (valor == 'X') ? 'O' : 'X';

    // 4 direções (cada uma cobre os dois lados)
    int direcoes[4][2] = {
        {1, 0},   // vertical
        {0, 1},   // horizontal
        {1, 1},   // diagonal principal
        {1, -1}   // diagonal secundária
    };

    // Garente dodas as possibilidades de insert ao mesmo tempo
    for (int d = 0; d < 4; d++) {
        int dx = direcoes[d][0];
        int dy = direcoes[d][1];

        // procurar extremidade 1 (para trás)
        int x1 = i, y1 = j;
        do {
            x1 -= dx;
            y1 -= dy;
        } while (x1 >= 0 && x1 < n && y1 >= 0 && y1 < n && v[x1][y1] == valor);

        // procurar extremidade 2 (para frente)
        int x2 = i, y2 = j;
        do {
            x2 += dx;
            y2 += dy;
        } while (x2 >= 0 && x2 < n && y2 >= 0 && y2 < n && v[x2][y2] == valor);

        // verificar se as extremidades são do adversário
        if (x1 >= 0 && x1 < n && y1 >= 0 && y1 < n &&
            x2 >= 0 && x2 < n && y2 >= 0 && y2 < n) {

            if (v[x1][y1] == adversario && v[x2][y2] == adversario) {
                v[x1][y1] = valor;
                v[x2][y2] = valor;

                // Fala que ouve insert
                printf("Ouve um insert por (%d, %d) que alterou as regioes (%d, %d) e (%d, %d)\n", i, j, x1, y1, x2, y2);

                alterou = 1;
            }
        }
    }

    return alterou;
}

// Função que analisa se houve vitorial igual no jogo na velha
// Função que analisa se houve vitoria (versão corrigida)
int vitoria(char **v, int n, int pessoa) {
    char simbolo;
    int i, j;

    // Define o símbolo do jogador atual
    if (pessoa == 1) {
        simbolo = 'X';
    } else {
        simbolo = 'O';
    }

    // 1. Verifica todas as linhas e colunas
    for (i = 0; i < n; i++) {

        // Contadores zerados para CADA linha e coluna
        int contador_linha = 0;
        int contador_coluna = 0;
        for (j = 0; j < n; j++) {

            // Conta para a linha 'i'
            if (v[i][j] == simbolo) {
                contador_linha++;
            }

            // Conta para a coluna 'i'
            if (v[j][i] == simbolo) {
                contador_coluna++;
            }
        }

        // Se a contagem de uma linha ou coluna inteira for igual a 'n', é vitória
        if (contador_linha == n) {
            return 1;
        }
        if (contador_coluna == n) {
            return 1;
        }
    }

    // 2. Verifica as duas diagonais separadamente
    int contador_diag_p = 0;
    int contador_diag_s = 0;
    for (i = 0; i < n; i++) {

        // Diagonal principal
        if (v[i][i] == simbolo) {
            contador_diag_p++;
        }

        // Diagonal secundária
        if (v[i][n - 1 - i] == simbolo) {
            contador_diag_s++;
        }
    }

    if (contador_diag_p == n) {
        return 1;
    }
    if (contador_diag_s == n) {
        return 1;
    }

    // Se nenhuma condição de vitória foi atingida
    return 0;
}

// Função que determina quem ganhou por pontos
int pontosx(char **v, int n) {
    int contadorx = 0;

    // Analisa todas as celulas da matriz
    for (int i = 0 ; i < n ; i++) {
        for (int j = 0 ; j < n ; j++) {

            // Conta quantas vezes o X apareceu
            if(v[i][j] == 'X') {
                contadorx++;
            }
        }
    }
    return contadorx;
}