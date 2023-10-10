#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_SUCCESS 0

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 10
#define MINES_COUNT 10

typedef struct {
  short value;
} Cell;

typedef struct {
  Cell cels[FIELD_WIDTH][FIELD_HEIGHT];
} Field;


Field createField() {
  Field f;
  srand(time(0));

  for(int i=0; i<FIELD_HEIGHT; ++i) {
    for(int j=0; j<FIELD_WIDTH; ++j) {
      f.cels[i][j].value = 0;
    }
  }

  int mines_placed = 0;
  while(mines_placed < MINES_COUNT) {
    int pos_x = rand()%FIELD_WIDTH;
    int pos_y = rand()%FIELD_HEIGHT;

    if (f.cels[pos_x][pos_y].value == 0) {
      f.cels[pos_x][pos_y].value = -1;
      mines_placed++;
    }
  }

  return f;
}

void printField(Field *f) {
  for(int i=0; i<FIELD_HEIGHT; ++i) {
    for(int j=0; j<FIELD_WIDTH; ++j) {
      printf("%d\t", f->cels[i][j].value);
    }
    printf("\n");
  }
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
  Field f = createField();
  printField(&f);
  printf("\n");
  calcWeights(&f);
  printField(&f);

  return EXIT_SUCCESS;
}


