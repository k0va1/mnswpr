#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 10
#define MINES_COUNT 10

typedef enum { OPEN, CLOSE, MARK_AS_MINE } CellStatus;
typedef struct {
  int value;
  CellStatus status;
} Cell;

typedef struct {
  Cell cels[FIELD_WIDTH][FIELD_HEIGHT];
} Field;

typedef enum { PENDING, ACTIVE, FINISHED } GameStatus;

typedef struct {
  Field *field;
  GameStatus status;
  int mines_placed;
} Game;

Field *createField() {
  Field *f = malloc(sizeof(Field));
  srand(time(0));

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      f->cels[i][j].value = 0;
      f->cels[i][j].status = CLOSE;
    }
  }

  int mines_placed = 0;
  while (mines_placed < MINES_COUNT) {
    int pos_x = rand() % FIELD_WIDTH;
    int pos_y = rand() % FIELD_HEIGHT;

    if (f->cels[pos_x][pos_y].value == 0) {
      f->cels[pos_x][pos_y].value = -1;
      mines_placed++;
    }
  }

  return f;
}
void printField(Field *f, int is_debug) {
  printf("   0 1 2 3 4 5 6 7 8 9\n");
  printf("======================\n");
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    printf("%i| ", i);
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      if (is_debug) {
        if (f->cels[i][j].value == -1) {
          printf("* ");
        } else {
          printf("%i ", f->cels[i][j].value);
        }
      } else {
        if (f->cels[i][j].status == OPEN) {
          printf("%i ", f->cels[i][j].value);
        } else if (f->cels[i][j].status == MARK_AS_MINE) {
          printf("F ");
        } else {
          printf("x ");
        }
      }
    }
    printf("\n");
  }
  printf("======================\n");
}

int isEndOfTheGame(Game *g) {
  int result = 0;

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      if (g->field->cels[i][j].status == OPEN ||
          g->field->cels[i][j].status == MARK_AS_MINE) {
        result = 1;
      } else {
        return 0;
      }
    }
  }

  return result;
}

int isInvalidCoordinates(int i, int j) {
  return i < 0 || i > FIELD_HEIGHT - 1 || j < 0 || j > FIELD_WIDTH - 1;
}

void batchOpen(Field *f, int i, int j) {
  // check that we are in table bounds
  if (isInvalidCoordinates(i, j)) {
    return;
  }
  if (f->cels[i][j].status == OPEN) {
    return;
  }

  if (f->cels[i][j].value == 0) {
    f->cels[i][j].status = OPEN;
    batchOpen(f, i - 1, j);
    batchOpen(f, i, j + 1);
    batchOpen(f, i, j - 1);
    batchOpen(f, i + 1, j);
    batchOpen(f, i + 1, j + 1);
    batchOpen(f, i + 1, j - 1);
    batchOpen(f, i - 1, j - 1);
    batchOpen(f, i - 1, j + 1);
  } else {
    f->cels[i][j].status = OPEN;
  }
}

void updateCell(Field *f, int i, int j) {
  if (isInvalidCoordinates(i, j)) {
    return;
  }
  if (f->cels[i][j].value != -1) {
    f->cels[i][j].value++;
  }
}

void updateCellNeighbours(Field *f, int i, int j) {
  updateCell(f, i - 1, j);
  updateCell(f, i, j + 1);
  updateCell(f, i, j - 1);
  updateCell(f, i + 1, j);
  updateCell(f, i + 1, j + 1);
  updateCell(f, i + 1, j - 1);
  updateCell(f, i - 1, j - 1);
  updateCell(f, i - 1, j + 1);
}

void calcWeights(Field *f) {
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      int isMine = f->cels[i][j].value == -1;
      if (isMine) {
        updateCellNeighbours(f, i, j);
      }
    }
  }
}

Game start() {
  Game game;
  Field *f = createField();
  calcWeights(f);
  printField(f, 0);

  game.field = f;
  game.status = PENDING;
  game.mines_placed = 0;

  return game;
}

int main(int argc, char *argv[]) {
  Game game = start();
  Field *f = game.field;

  char *input = NULL;
  size_t len = 0;

  while (game.status != FINISHED) {
    int gameEnded = isEndOfTheGame(&game);
    if (gameEnded) {
      printf("You win!\n");
      return EXIT_SUCCESS;
    }

    printf("Mines left: %i\n", MINES_COUNT - game.mines_placed);
    printf("Enter cell index to open, prefix `f` to put a flag(i,j): ");
    getline(&input, &len, stdin);
    char command = input[0];
    game.status = ACTIVE;

    if (command == 'f') {
      input = &input[1];
      char * ptr = strtok(input, ",");
      int i = atoi(ptr);
      int j = atoi(input);
      printf("%i", isInvalidCoordinates(i, j));
      if (isInvalidCoordinates(i, j)) {
        printf("ERROR: Invalid coordinates: i=%i, j=%i\n", i, j);
        continue;
      }
      f->cels[i][j].status = MARK_AS_MINE;
      game.mines_placed++;
      printField(f, 0);
    } else if (command == 'q') {
      printf("Exiting\n");
      return EXIT_SUCCESS;
    } else {
      char * ptr = strtok(input, ",");
      int i = atoi(ptr);
      int j = atoi(input);
      printf("%i", isInvalidCoordinates(i, j));
      if (isInvalidCoordinates(i, j)) {
        printf("ERROR: Invalid coordinates: i=%i, j=%i\n", i, j);
        continue;
      }

      if (f->cels[i][j].value == -1) {
        game.status = FINISHED;
        printf("GAME OVER\n");
        printField(f, 1);
      } else if (f->cels[i][j].value == 0) {
        batchOpen(f, i, j);
        printField(f, 0);
      } else {
        if (f->cels[i][j].status == MARK_AS_MINE) {
          game.mines_placed--;
        }
        f->cels[i][j].status = OPEN;
        printField(f, 0);
      }
    }
  }

  return EXIT_SUCCESS;
}
