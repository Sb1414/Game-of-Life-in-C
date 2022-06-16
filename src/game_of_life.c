#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#define MAXROWS 25
#define MAXCOLUMNS 80

int currentBoard[MAXROWS][MAXCOLUMNS];
static struct termios stored_settings;

int aliveNeighbors(int board[MAXROWS][MAXCOLUMNS], int row, int column);
int aliveNeighborsInRow(int board[MAXROWS][MAXCOLUMNS], int testRow, int row, int column);
void initializeBoard();
void printBoardCurrentState();
void setBoardNextState();
int speedTime(int *speed);
void inputCase(int option);
void inputFromFile(char *string);
void clrscr();
void set_keypress(void);
void reset_keypress(void);

int main() {
    char option[64];
    int sleepingTime;
    clrscr();
    speedTime(&sleepingTime);
    clrscr();
    printf("Начало игры!\n");
    printf("Для регулировки скорости используйте + и -\n");
    printf("Для выхода из игры нажмите q\n");
    printf("Выберите начальное положение организмов:\n");
    printf(" 1. Цветы\n");
    printf(" 2. Глейдеры\n");
    printf(" 3. Ружьё\n");
    printf(" 4. Абстракт\n");
    printf(" 5. Много организмов\n");
    printf(" 6. Рандомное расположение организмов\n");
    while (atoi(option) < 1 || atoi(option) > 6) {
        fgets(option, sizeof(option), stdin);
    }
    inputCase(atoi(option));

    fd_set rfds;
    struct timeval tv;
    int retval;
    set_keypress();
    while (1) {
        char c = '\0';
        clrscr();
        printBoardCurrentState();
        setBoardNextState();
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        retval = select(2, &rfds, NULL, NULL, &tv);
        if (retval) {
            c = getc(stdin);
        }
        if ((c == 'q') || (c == 'Q'))
            break;
        if (c == '-')
            if (sleepingTime < 125000)
            sleepingTime = sleepingTime + 5000;
        if (c == '+')
            if (sleepingTime >= 25000)
                sleepingTime = sleepingTime - 5000;
        usleep(sleepingTime);
        }
  return 0;
}

int speedTime(int *speed) {
    char opt[64];
    printf("Выберите скорость игры от 1 до 5\n");
    do {
        fgets(opt, sizeof(opt), stdin);
        if (atoi(opt) == 1) {
            *speed = 1000 * 100;
            break;
        } else if (atoi(opt) == 2) {
            *speed = 1000 * 75;
            break;
        } else if (atoi(opt) == 3) {
            *speed = 1000 * 50;
            break;
        } else if (atoi(opt) == 4) {
            *speed = 1000 * 25;
            break;
        } else if (atoi(opt) == 5) {
            *speed = 1000 * 1;
            break;
        }
    } while (atoi(opt) < 1 || atoi(opt) > 5);
    return *speed;
}

void inputCase(int option) {
    switch (option) {
        case 1:
            inputFromFile("figures1.txt");
            break;
        case 2:
            inputFromFile("figures2.txt");
            break;
        case 3:
            inputFromFile("figures3.txt");
            break;
        case 4:
            inputFromFile("figures4.txt");
            break;
        case 5:
            inputFromFile("figures5.txt");
            break;
        case 6:
            initializeBoard();
    }
}

void inputFromFile(char *string) {
    FILE *f;
    f = fopen(string, "r");
    char **str = (char**)malloc(MAXCOLUMNS * MAXROWS * sizeof(char) + MAXROWS * sizeof(char*));
    char *ptr = (char*) (str + MAXROWS);
    int n = 0;
    while (!feof(f)) {
        str[n] = ptr + MAXCOLUMNS * n;
        fgets(str[n], MAXCOLUMNS, f);
        n++;
    }

    for (int i = 0; i < MAXROWS; i++) {
            for (int j = 0; j < MAXCOLUMNS; j++) {
                if (str[i][j] == 'O') {
                    currentBoard[i][j] = 1;
                } else {
                    currentBoard[i][j] = 0;
                }
            }
        }
    for (int i = 0; i < n - 1; i++) {
        puts(str[i]);
    }
    free(str);
        fclose(f);
}

void clrscr() {
    system("clear");
}

int aliveNeighbors(int board[MAXROWS][MAXCOLUMNS], int row, int column) {
    int aliveNeighbors = 0;
    int testRow = 0;

    if (row == 0) {
        testRow = MAXROWS - 1;
    } else {
        testRow = row - 1;
    }

    aliveNeighbors += aliveNeighborsInRow(board, testRow, row, column);

    testRow = row;

    aliveNeighbors += aliveNeighborsInRow(board, testRow, row, column);

    if (row == MAXROWS - 1) {
        testRow = 0;
    } else {
        testRow = row + 1;
    }

    aliveNeighbors += aliveNeighborsInRow(board, testRow, row, column);

    return aliveNeighbors;
}

int aliveNeighborsInRow(int board[MAXROWS][MAXCOLUMNS], int testRow, int row, int column) {
    int aliveNeighbors = 0;
    int testColumn = 0;

    if (column  == 0) {
        testColumn = MAXCOLUMNS - 1;
    } else {
        testColumn = column - 1;
    }

    aliveNeighbors += board[testRow][testColumn];

    testColumn = column;

    if (testRow != row) {
        aliveNeighbors += board[testRow][testColumn];
    }

    if (column == MAXCOLUMNS - 1) {
        testColumn = 0;
        } else {
            testColumn = column + 1;
        }

    aliveNeighbors += board[testRow][testColumn];

    return aliveNeighbors;
}

void initializeBoard() {
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLUMNS; j++) {
            currentBoard[i][j] = rand() & 1;
        }
    }
}

void printBoardCurrentState() {
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLUMNS; j++) {
            if (currentBoard[i][j]) {
                printf("█");
            } else {
                printf(" ");
            }
        }

        printf("\n");
    }
}

void setBoardNextState() {
    int nextBoard[MAXROWS][MAXCOLUMNS];

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLUMNS; j++) {
            int neighbors = 0;

            neighbors = aliveNeighbors(currentBoard, i, j);

            if ((currentBoard[i][j] && neighbors == 2) || neighbors == 3) {
                nextBoard[i][j] = 1;
            } else {
                nextBoard[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLUMNS; j++) {
            currentBoard[i][j] = nextBoard[i][j];
        }
    }
}

void set_keypress(void) {
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON & ~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    return;
}

void reset_keypress(void) {
    tcsetattr(0, TCSANOW, &stored_settings);
    return;
}
