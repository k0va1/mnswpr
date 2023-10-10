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

  for(int i=0; i<FIELD_WIDTH; ++i) {
    for(int j=0; j<FIELD_HEIGHT; ++j) {
      f.cels[i][j].value = 0;
    }
  }

  int mines_placed = 0;
  while(mines_placed < MINES_COUNT) {
    int pos_x = rand()%FIELD_WIDTH;
    int pos_y = rand()%FIELD_HEIGHT;

    if (f.cels[pos_x][pos_y].value == 0) {
      f.cels[pos_x][pos_y].value = 1;
      mines_placed++;
    }
  }

  return f;
}

void printField(Field *f) {
  for(int i=0; i<FIELD_WIDTH; ++i) {
    for(int j=0; j<FIELD_HEIGHT; ++j) {
      printf("%d ", f->cels[i][j].value);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  Field f = createField();
  printField(&f);

  return EXIT_SUCCESS;
}


