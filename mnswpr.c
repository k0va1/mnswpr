#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_SUCCESS 0

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 20
#define MINES_COUNT 30

typedef enum { OPEN, CLOSE, MARK_AS_MINE } CellStatus;
typedef struct {
  char value;
  CellStatus status;
} Cell;

typedef struct {
  Cell cels[FIELD_WIDTH][FIELD_HEIGHT];
} Field;

typedef enum { FINISHED, ACTIVE } GameStatus;

typedef struct {
  Field* field;
  GameStatus status;
  int mines_placed;
} Game;

Field* createField() {
  Field *f = malloc(sizeof(Field));
  srand(time(0));

  for(int i=0; i<FIELD_HEIGHT; ++i) {
    for(int j=0; j<FIELD_WIDTH; ++j) {
      f->cels[i][j].value = 0;
      f->cels[i][j].status = CLOSE;
    }
  }

  int mines_placed = 0;
  while(mines_placed < MINES_COUNT) {
    int pos_x = rand()%FIELD_WIDTH;
    int pos_y = rand()%FIELD_HEIGHT;

    if (f->cels[pos_x][pos_y].value == 0) {
      f->cels[pos_x][pos_y].value = -1;
      mines_placed++;
    }
  }

  return f;
}
void printField(Field *f, int is_debug) {
  printf("INFO: i=rows, j=cols\n\n");
  printf("  0 1 2 3 4 5 6 7 8 9\n");
  for(int i=0; i<FIELD_HEIGHT; ++i) {
    printf("%i ", i);
    for(int j=0; j<FIELD_WIDTH; ++j) {
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
}


void batchOpen(Field* f, int i, int j) {
  if (i < 0 || i > FIELD_HEIGHT-1 || j < 0 || j > FIELD_WIDTH-1) {
    return;
  }
  if ( f->cels[i][j].status == OPEN ) {
    return;
  }

  if (f->cels[i][j].value == 0) {
    f->cels[i][j].status = OPEN;
    batchOpen(f, i-1, j);
    batchOpen(f, i, j+1);
    batchOpen(f, i, j-1);
    batchOpen(f, i+1, j);
    batchOpen(f, i+1, j+1);
    batchOpen(f, i+1, j-1);
    batchOpen(f, i-1, j-1);
    batchOpen(f, i-1, j+1);
  } else {
    f->cels[i][j].status = OPEN;
  }
}

Game start() {
  Game game;
  Field* f = createField();
  game.field = f;
  game.status = ACTIVE;
  game.mines_placed = 0;

  return game;
}

void calcWeights(Field *f) {
  for(int i=0; i<FIELD_HEIGHT; ++i) {
    for(int j=0; j<FIELD_WIDTH; ++j) {
      int is_mine = f->cels[i][j].value == -1;
      if (is_mine) {
        // north
        if(i>0) {
          if (f->cels[i-1][j].value != -1){
            f->cels[i-1][j].value++;
          }
        }
        // n-e
        if(i != 0 && j < FIELD_WIDTH-1) {
          if (f->cels[i-1][j+1].value != -1){
            f->cels[i-1][j+1].value++;
          }
        }
        // n-w
        if(i != 0 && j > 0) {
          if (f->cels[i-1][j-1].value != -1){
            f->cels[i-1][j-1].value++;
          }
        }
        // south
        if(i < FIELD_HEIGHT -1) {
          if (f->cels[i+1][j].value != -1) {
            f->cels[i+1][j].value++;
          }
        }
        // s-e
        if(i < FIELD_HEIGHT -1 && j < FIELD_WIDTH-1) {
          if (f->cels[i+1][j+1].value != -1) {
            f->cels[i+1][j+1].value++;
          }
        }
        // s-w
        if(i < FIELD_HEIGHT-1 && j > 0) {
          if (f->cels[i+1][j-1].value != -1) {
            f->cels[i+1][j-1].value++;
          }
        }
        // east
        if(j < FIELD_WIDTH-1) {
          if (f->cels[i][j+1].value != -1) {
            f->cels[i][j+1].value++;
          }
        }
        // west
        if(j>0) {
          if (f->cels[i][j-1].value != -1) {
            f->cels[i][j-1].value++;
          }
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  Game game = start();
  Field* f = game.field;
  printField(f, 0);
  printf("\n");
  calcWeights(f);
  printField(f, 1);

  char *input = NULL;
  size_t len = 0;

  while (game.status != FINISHED) {
    printf("Mines left: %i\n", MINES_COUNT - game.mines_placed);
    printf("Enter cell index to open, prefix `f` to put a flag(i,j): ");
    getline(&input, &len, stdin);
    char command = input[0];

    if (command == 'f') {
      int i = input[1] - '0'; // it converts char to int index
      int j = input[3] - '0';
      f->cels[i][j].status = MARK_AS_MINE;
      game.mines_placed++;
      printField(f, 0);
    } else {
      int i = input[0] - '0'; // it converts char to int index
      int j = input[2] - '0';

      if(f->cels[i][j].value == -1) {
        game.status = FINISHED;
        printf("GAME OVER\n");
      } else if (f->cels[i][j].value == 0) {
        // implement batch open algorithm
        batchOpen(f, i, j);
        /*f->cels[i][j].status = OPEN;*/
        printField(f, 0);
      } else {
        f->cels[i][j].status = OPEN;
        printField(f, 0);
      }
    }
  }

  return EXIT_SUCCESS;
}
