#include <stdio.h>
#include <stdlib.h>

extern void vector_main_test(void);

int main(void)
{
  vector_main_test();
  setbuf(stdout, NULL);
  printf("Tests passed!\n");
  return 0;
}