#include "la_common.h"

/************************************************************************/
/* Auxiliary functions							*/
/************************************************************************/
void *alloc(size_t size)
{
  void *ptr;

  if ((ptr = malloc(size)) == NULL)
  {
    printf("Out of memory!\n");
    exit(-1);
  }
  else return ptr;
}

unsigned int log_base2(unsigned int x)
{
  unsigned int y;	
  asm ( "\tbsr %1, %0\n"	
      : "=r"(y)	
      : "r" (x)	
  );	
  return y;
}

int max(int a, int b)
{
  return ((a < b) ? b : a);
}

int min(int a, int b)
{
  return ((a > b) ? b : a);
}

// The highest power of 2 that divides n with no remainder 
unsigned int highest_pwrOf2_divides(unsigned int x) 
{ 
    return (x & (~(x - 1))); 
} 

// The highest power of 2 less than or equal to n
unsigned int highest_pwrOf2(unsigned int x) 
{ 
    return 1 << (unsigned int)floor(log_base2(x));
} 

// Performs the regular modulo operation with just a bit operation. y must be a power of 2
unsigned int mod_pwr2(int x, int y){
	return x & (y - 1);
}

double get_elapsed_time(struct timespec start, struct timespec end){

  long seconds = end.tv_sec - start.tv_sec;
  long nanoseconds = end.tv_nsec - start.tv_nsec;
  double elapsed_time = seconds + (nanoseconds * 0.000000001);

  return elapsed_time;
}
/************************************************************************/