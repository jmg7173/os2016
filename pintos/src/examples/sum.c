#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  int i;
  int num[4];

  if(argc != 5)
    {
      printf ("usage : sum num1 num2 num3 num4\n");
      return EXIT_FAILURE;
    }

  for(i = 0; i<4; i++)
    num[i] = atoi(argv[i+1]);

  printf("%d %d\n",
	 pibonacci(num[0]),
	 sum_of_four_integers(num[0],num[1],num[2],num[3]));
  return EXIT_SUCCESS;
}
