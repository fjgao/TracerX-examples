/* Obtained from http://www.mrtc.mdh.se/projects/wcet/benchmarks.html,
 * with bugfix, LLBMC and KLEE harnessing, and correctness assertion
 * added */

/* $Id: insertsort.c,v 1.2 2005/04/04 11:34:58 csg Exp $ */

/*************************************************************************/
/*                                                                       */
/*   SNU-RT Benchmark Suite for Worst Case Timing Analysis               */
/*   =====================================================               */
/*                              Collected and Modified by S.-S. Lim      */
/*                                           sslim@archi.snu.ac.kr       */
/*                                         Real-Time Research Group      */
/*                                        Seoul National University      */
/*                                                                       */
/*                                                                       */
/*        < Features > - restrictions for our experimental environment   */
/*                                                                       */
/*          1. Completely structured.                                    */
/*               - There are no unconditional jumps.                     */
/*               - There are no exit from loop bodies.                   */
/*                 (There are no 'break' or 'return' in loop bodies)     */
/*          2. No 'switch' statements.                                   */
/*          3. No 'do..while' statements.                                */
/*          4. Expressions are restricted.                               */
/*               - There are no multiple expressions joined by 'or',     */
/*                'and' operations.                                      */
/*          5. No library calls.                                         */
/*               - All the functions needed are implemented in the       */
/*                 source file.                                          */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/*  FILE: insertsort.c                                                   */
/*  SOURCE : Public Domain Code                                          */
/*                                                                       */
/*  DESCRIPTION :                                                        */
/*                                                                       */
/*     Insertion sort for 10 integer numbers.                            */
/*     The integer array a[] is initialized in main function.            */
/*                                                                       */
/*  REMARK :                                                             */
/*                                                                       */
/*  EXECUTION TIME :                                                     */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#ifdef LLBMC
#include <llbmc.h>
#else
#include <klee/klee.h>
#endif

#ifdef DEBUG
int cnt1, cnt2;
#endif

#define ARRAY_SIZE 11

unsigned int a[ARRAY_SIZE];

int main()
{
  int  i,j, temp;

  /* We replace the following concrete input with symbolic ones */
  // a[0] = 0;   /* assume all data is positive */
  // a[1] = 11; a[2]=10;a[3]=9; a[4]=8; a[5]=7; a[6]=6; a[7]=5;
  // a[8] =4; a[9]=3; a[10]=2;

#ifdef LLBMC
  for (int i = 0; i < ARRAY_SIZE; ++i) {
    a[i] = __llbmc_nondef_unsigned_int();
  }
#else
  klee_make_symbolic(a, ARRAY_SIZE * sizeof(unsigned int), "a");
#endif

  i = 2;
  while (i <= ARRAY_SIZE - 1) {
#ifdef DEBUG
      cnt1++;
#endif
      j = i;
#ifdef DEBUG
	cnt2=0;
#endif
    while (j > 0 && a[j] < a[j - 1]) {
#ifdef DEBUG
	cnt2++;
#endif
	temp = a[j];
	a[j] = a[j-1];
	a[j-1] = temp;
	j--;
      }
#ifdef DEBUG
	printf("Inner Loop Counts: %d\n", cnt2);
#endif
      i++;
    }
#ifdef DEBUG
    printf("Outer Loop : %d ,  Inner Loop : %d\n", cnt1, cnt2);
#endif

#ifdef LLBMC
  __llbmc_assert(a[0] <= a[1] && \
                 a[1] <= a[2] && \
                 a[2] <= a[3] && \
                 a[3] <= a[4] && \
                 a[4] <= a[5] && \
                 a[5] <= a[6] && \
		 a[6] <= a[7] && \
		 a[7] <= a[8] && \
		 a[8] <= a[9] && \
		 a[9] <= a[10]);
#else
  klee_assert(a[0] <= a[1] && \
              a[1] <= a[2] && \
              a[2] <= a[3] && \
              a[3] <= a[4] && \
              a[4] <= a[5] && \
              a[5] <= a[6] && \
	      a[6] <= a[7] && \
	      a[7] <= a[8] && \
	      a[8] <= a[9] && \
	      a[9] <= a[10]);
#endif
    return 1;
}


