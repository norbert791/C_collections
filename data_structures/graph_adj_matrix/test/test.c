#include <stdio.h>
#include <stdlib.h>

extern void vector_main_test(void);
extern void matrix_main_test(void);
extern void graph_main_test(void);

int main(void)
{
  vector_main_test();
  matrix_main_test();
  graph_main_test();
  setbuf(stdout, NULL);
  printf("Tests passed!\n");
  return 0;
}
