/*
 * Copyright 2016 National University of Singapore
 */
#include <klee/klee.h>
#include <assert.h>

int main() {
  char a[6] = "hello";
  unsigned i;

  klee_make_symbolic(&i, sizeof(i), "i");

  klee_assume(i <= 1);
  a[i] = 0;

  assert(a[3] != 0);

  return 0;
} 


