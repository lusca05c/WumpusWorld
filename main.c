//Código WUMPUS (em C) —---------------👍
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define GBOARD_SIZE 4
#define VAZIO "-----"
#define CAVE "cave"
#define WUMPUS "womp"
#define BRISA "breeze"
#define FEDO "scretch"
#define ESGOTO "breeze_scretch"
#define GOLD "Minéru"

/*
* Validacoes a baixo
*/

int isValid(int i, int j) {
    return i >= 0 && i < GBOARD_SIZE && j >= 0 && j < GBOARD_SIZE;
}


void placeBrisa(char *gBoard[GBOARD_SIZE][GBOARD_SIZE], int i, int j) {
    if (isValid(i, j) && gBoard[i][j] != CAVE) {
        gBoard[i][j] = BRISA;
    }
}


int isEffectValid(char *gBoard[GBOARD_SIZE][GBOARD_SIZE], int i, int j) {
    return isValid(i, j) && gBoard[i][j] != CAVE && gBoard[i][j] != WUMPUS;
}


int isGoldGoal(char *gBoard[GBOARD_SIZE][GBOARD_SIZE], int i, int j) {
    return gBoard[i][j] != FEDO && gBoard[i][j] != BRISA && gBoard[i][j] != ESGOTO && isEffectValid(gBoard, i, j);
}

typedef struct agentType{
    char * name;
    int pontuation;
    int positionX;
    int positionY;
    int alive;
    int back;
   
}agentType;

void applyFedorOrEsgoto(char *gBoard[GBOARD_SIZE][GBOARD_SIZE], int i, int j) {
    if (isEffectValid(gBoard, i, j)) {
        if (gBoard[i][j] == BRISA)
            gBoard[i][j] = ESGOTO;
        else
            gBoard[i][j] = FEDO;
    }
}


void haveCave(char *gBoard[GBOARD_SIZE][GBOARD_SIZE]) {
    srand(time(NULL));
    int i, j;


    do {
        i = rand() % GBOARD_SIZE;
        j = rand() % GBOARD_SIZE;
    } while ((i == 0 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 0) || gBoard[i][j] == CAVE);
    gBoard[i][j] = CAVE;


    placeBrisa(gBoard, i - 1, j);  // Acima
    placeBrisa(gBoard, i + 1, j);  // Abaixo
    placeBrisa(gBoard, i, j - 1);  // Esquerda
    placeBrisa(gBoard, i, j + 1);  // Direita
}


void applyGold (char * gBoard[GBOARD_SIZE][GBOARD_SIZE]){
    srand(time(NULL));
    int i,j;
   
    do {
        i = rand() % GBOARD_SIZE;
        j = rand() % GBOARD_SIZE;
    } while (!isGoldGoal(gBoard, i, j) || (i == 0 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 0));
   
    gBoard[i][j] = GOLD;
}


void haveWumpus(char * gBoard[GBOARD_SIZE][GBOARD_SIZE]){
   srand(time(NULL));
    int i, j;
   
    do {
        i = rand() % GBOARD_SIZE;
        j = rand() % GBOARD_SIZE;
    } while ((i == 0 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 0) || gBoard[i][j] == CAVE || gBoard[i][j] == WUMPUS);


    gBoard[i][j] = WUMPUS;


    applyFedorOrEsgoto(gBoard, i - 1, j);  // Acima
    applyFedorOrEsgoto(gBoard, i + 1, j);  // Abaixo
    applyFedorOrEsgoto(gBoard, i, j - 1);  // Esquerda
    applyFedorOrEsgoto(gBoard, i, j + 1);  // Direita
   
}


void generategBoard(agentType * agent, char *gBoard[GBOARD_SIZE][GBOARD_SIZE]) {
    
    for (int x = 0; x < GBOARD_SIZE; x++) {
        for (int y = 0; y < GBOARD_SIZE; y++) {
            gBoard[x][y] = VAZIO;
        }
    }

    
    for (int x = 0; x < GBOARD_SIZE; x++) {
        for (int y = 0; y < GBOARD_SIZE; y++) {
            if (x == 0 && y == 0)
                gBoard[x][y] = agent->name;
            else
                gBoard[x][y] = VAZIO;
        }
    }

    for (int i = 0; i < 2; i++) {
        haveCave(gBoard);
    }
    haveWumpus(gBoard);
    applyGold(gBoard);
   
}


void printgBoard(char *gBoard[GBOARD_SIZE][GBOARD_SIZE]) {
    printf("\n+-----------------------------------+\n"); // Linha superior


    for (int x = 0; x < GBOARD_SIZE; x++) {
        printf("|"); // Inicia a linha de cada linha do tabuleiro


        for (int y = 0; y < GBOARD_SIZE; y++) {
            // Alinha a célula e imprime uma borda separando as colunas
            printf(" %-3s|", gBoard[x][y]);
        }


        printf("\n+-----------------------------------+\n"); // Linha de separação após cada linha do tabuleiro
    }
}
#define MAX_PILHA 20

char * messagesSubmisses[GBOARD_SIZE][GBOARD_SIZE] = {0};

//Implementação de pilha da várzea

typedef struct {
    int x;
    int y;
} Position;

Position pilha[MAX_PILHA];
int top = -1;
void push(int x, int y) {
    if (top < MAX_PILHA - 1) {
        top++;
        pilha[top].x = x;
        pilha[top].y = y;
    }
}
Position pop() {
    if (top >= 0) return pilha[top--];
    Position invalid = {-1, -1};
    return invalid;
}

//


int visited[GBOARD_SIZE][GBOARD_SIZE] = {0}; // Identificar as posições visitadas
int saboMuch[GBOARD_SIZE][GBOARD_SIZE] = {0}; 
// 0 = desconhecido
// 1 = Andável
// 2 = possível buraco
// 3 = possível Wumpus
// 9 = Seguro

void updateSaboMuch(agentType *agent, char *gBoard[GBOARD_SIZE][GBOARD_SIZE]) {
    int x = agent->positionX, y = agent->positionY;
    visited[x][y] = 1;
    saboMuch[x][y] = 9; // Marca a posição atual do agente

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    int perigoBuraco = strcmp(gBoard[x][y], BRISA) == 0 || strcmp(gBoard[x][y], ESGOTO) == 0;
    int perigoWumpus = strcmp(gBoard[x][y], FEDO) == 0 || strcmp(gBoard[x][y], ESGOTO) == 0;

    // Propaga informações para as casas adjacentes
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (!isValid(nx, ny)) continue;

        if (saboMuch[nx][ny] == 0) {
            if (perigoBuraco) saboMuch[nx][ny] = 2;
            if (perigoWumpus && saboMuch[nx][ny] != 2) saboMuch[nx][ny] = 3;
            if (!perigoBuraco && !perigoWumpus) saboMuch[nx][ny] = 1;
        }

        if (saboMuch[nx][ny] == 2 && !perigoBuraco) saboMuch[nx][ny] = 1;
        if (saboMuch[nx][ny] == 3 && !perigoWumpus) saboMuch[nx][ny] = 1;
    }

    // Breve segurança nas casas adjacentes, se não houver perigo
    if (!perigoBuraco && !perigoWumpus) {
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (isValid(nx, ny)) saboMuch[nx][ny] = 1;
        }
    }

    // Realiza inferência com base nas casas visitadas
    for (int i = 0; i < GBOARD_SIZE; i++) {
        for (int j = 0; j < GBOARD_SIZE; j++) {
            if (!visited[i][j]) continue;

            int desconhecidas = 0;
            for (int k = 0; k < 4; k++) {
                int nx = i + dx[k], ny = j + dy[k];
                if (isValid(nx, ny) && saboMuch[nx][ny] == 0) {
                    desconhecidas++;
                    break;
                }
            }

            if (!desconhecidas && saboMuch[i][j] == 0) saboMuch[i][j] = 9; // Marca como seguro
        }
    }
}

void movimentAgent(agentType *agent, int i, int j, char * gBoard[GBOARD_SIZE][GBOARD_SIZE]) {

    if (messagesSubmisses[agent->positionX][agent->positionY] != NULL)
        gBoard[agent->positionX][agent->positionY] = messagesSubmisses[agent->positionX][agent->positionY];
    else
        gBoard[agent->positionX][agent->positionY] = VAZIO;

    messagesSubmisses[i][j] = gBoard[i][j];

    if (i != agent->positionX || j != agent->positionY)
        push(i, j);

    agent->positionX = i;
    agent->positionY = j;
    agent->pontuation -= 1;
    visited[i][j] = 1;
    saboMuch[i][j] = 9;

    gBoard[i][j] = agent->name;

    printf("andô dêi jeitu (%d, %d)\n", i, j);
    printf("Posição atual: (%d, %d), Pontuação: %d\n", agent->positionX, agent->positionY, agent->pontuation);
}


void interactWithBoard(agentType *agent, char *gBoard[GBOARD_SIZE][GBOARD_SIZE]){
    int x = agent->positionX, y = agent->positionY;
    if(messagesSubmisses[x][y] == WUMPUS){
        agent->alive = 0;
        agent->pontuation = 0;
        printf("Assô!! O Wumpus matou o agente e ele perdeu todos os pontos!\n");
    }
    if(messagesSubmisses[x][y] == CAVE){
        agent->alive = 0;
        agent->pontuation = 0;
        printf("Ai morri! O agente caiu em um buraco e perdeu todos os pontos!\n");
    }
    if(messagesSubmisses[x][y] == GOLD){
        agent->pontuation += 100;
        printf("TÔ RICAAA! O agente encontrou o ouro e ganhou 100 pontos! Uau!\n");
    }
}

void returnPorPilha(agentType *agent, char *gBoard[GBOARD_SIZE][GBOARD_SIZE]) {
    printf("🔁 %s está retornando ao início...\n", agent->name);
    agent->back = 1;
    while (top >= 0) {
        Position pos = pop();
        if (pos.x == 0 && pos.y == 0) break;
        movimentAgent(agent, pos.x, pos.y, gBoard);
    }
    printf("🏁 %s retornou! Pontuação final: %d\n", agent->name, agent->pontuation);
}


void DecisionAgent(agentType *agent, char *gBoard[GBOARD_SIZE][GBOARD_SIZE]){
    int x = agent->positionX, y = agent->positionY;

    interactWithBoard(agent, gBoard);
    if (!agent->alive) return;

    // Se encontrou o ouro, retorna à posição inicial
    if (messagesSubmisses[x][y] != NULL && strcmp(messagesSubmisses[x][y], GOLD) == 0) {
        returnPorPilha(agent, gBoard);
        return;
    }

    updateSaboMuch(agent, gBoard);

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    // 1. Tentar mover para uma posição segura e não visitada
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny) && !visited[nx][ny] && saboMuch[nx][ny] == 1) {
            movimentAgent(agent, nx, ny, gBoard);
            return;
        }
    }

    // 2. Tentar mover para uma posição desconhecida
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny) && !visited[nx][ny] && saboMuch[nx][ny] == 0) {
            printf("🤨 Não sei se é seguro, mas vamo arriscar...\n");
            movimentAgent(agent, nx, ny, gBoard);
            return;
        }
    }

    // 3. Repetir uma casa segura já visitada, se necessário (ANTES do suicídio!)
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny) && visited[nx][ny] && saboMuch[nx][ny] == 1) {
            printf("🔄 Revisitando uma casa segura...\n");
            movimentAgent(agent, nx, ny, gBoard);
            return;
        }
    }

    // 4. Se só sobrou o caos
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny) && !visited[nx][ny] && (saboMuch[nx][ny] == 2 || saboMuch[nx][ny] == 3)) {
            printf("☠️ Tá doido, mas vamos nessa...\n");
            movimentAgent(agent, nx, ny, gBoard);
            return;
        }
    }

    // Se não houver mais movimentos possíveis, retornar
    printf("🔙 Nada mais pra fazer aqui. Voltando...\n");
    returnPorPilha(agent, gBoard);
}


int main() {

    
    char *gBoard[GBOARD_SIZE][GBOARD_SIZE];
        srand(time(NULL)); 

        printf("\n============= RODADA =============\n");
        agentType agent = {"rei_da_cacimbinha", 0, 0, 0, 1, 0};
        generategBoard(&agent, gBoard);
        printgBoard(gBoard);
        
        while (agent.alive && !agent.back) {
            DecisionAgent(&agent, gBoard);

            // Verifica se o agente morreu ou encontrou o ouro
            if (!agent.alive) {
                printf("O agente foi derrotado! 😢\n");
                break;
            }
            if (strcmp(gBoard[agent.positionX][agent.positionY], GOLD) == 0) {
                printf("O agente encontrou o ouro!\n");
                break;
            }
        }

        if (agent.back) {
            printf("🏁VICTORY! %s retornou ao início com a pontuação final: %d\n", agent.name, agent.pontuation);
        } else {
            printf("❌FRACASSO... Não conseguiu voltar ao início.\n");
        }

    return 0;
}

/*
Capacidade do agente de usar corretamente as percepções para fazer
inferências sobre o ambiente - 2,5 pts
● Eficiência (percentual de vezes que o agente encontra o ouro em 10 rodadas)
- 2,5 pts
● Habilidade do agente para retornar à entrada da caverna após coletar o ouro
(minimização dos passos) - 2,5 pts
● Tempo gasto para achar o ouro e retornar à posição inicial (comparação com
os colegas) - 2,5 pts
*/