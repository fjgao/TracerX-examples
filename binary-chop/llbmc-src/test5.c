/* Generated by CIL v. 1.3.7 */
/* print_CIL_Input is true */
#include<stdio.h>
#define LLBMC 1
#ifdef LLBMC
#include "llbmc.h"
#else
#include <klee/klee.h>
#endif

#include <stdio.h> 
#include <assert.h>
#include <math.h>
#include <stdlib.h>
/* compiler builtin: 
   void *__builtin_alloca(unsigned int  ) ;  */
int wcet;
int BOUND  =    10;
int m_flush  =    1;
int m_PipedOutputStream  =    2;
int m_close  =    3;
int m_write  =    4;
int m_connect_1  =    5;
int m_connect_2  =    6;
int kappa  =    0;

int main(void) 
{ wcet = 0;
int dummy ;
  int q ;
  int FLAG ;
  int i ;
  int method_id ;
  int this_sink ;
  int this_sinkConnected ;
  int (*symb)[19] ;
  unsigned int __lengthofsymb ;
  void *tmp ;
  int P1 ;
  int P2 ;
  int P3 ;
  int P4 ;
  int FLAG___0 ;

  {
  dummy = 1;
  q = 0;
  FLAG = 0;
  i = 0;
  this_sink = 0;
  this_sinkConnected = 0;
  __lengthofsymb = (unsigned int )BOUND;
  tmp = __builtin_alloca(sizeof(*symb) * __lengthofsymb);
  symb = (int (*)[19])tmp;
  #ifdef LLBMC
  for (int i = 0; i < BOUND; ++i) {
  for (int j = 0; j < 19; ++j) {
    symb[i][j] = __llbmc_nondef_int();
  }
  }
  #else
  klee_make_symbolic(symb, (sizeof(int ) * (unsigned int )BOUND) * 19U, "symb");
  #endif
  
 #ifdef LLBMC
  P1 = __llbmc_nondef_int();
  P2 = __llbmc_nondef_int();
  P3 = __llbmc_nondef_int();
  P4 = __llbmc_nondef_int();
  #else
  klee_make_symbolic(& P1, sizeof(int ), "P1");
  klee_make_symbolic(& P2, sizeof(int ), "P2");
  klee_make_symbolic(& P3, sizeof(int ), "P3");
  klee_make_symbolic(& P4, sizeof(int ), "P4");
 #endif
 
  FLAG___0 = 0;
  while (FLAG___0 < BOUND) {
    if (q == 0) {
      if ((*(symb + FLAG___0))[1]) {
        if (dummy == 1) {
          method_id = 2;
          if (dummy != 1) {
            goto ERROR;
          }
          q = 2;
          this_sink = 0;
          this_sinkConnected = 0;
        }
        goto __Cont;
      }
      goto __Cont;
    }
    if (q == 1) {
      if ((*(symb + FLAG___0))[2]) {
        if (dummy == 1) {
          method_id = 4;
          if (this_sink != 0) {
            goto ERROR;
          }
          q = 1;
          break;
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[3]) {
        if (P2 != 1) {
          if (P1 != 0) {
            method_id = 5;
            if (P2 != 1) {
              if (this_sink == 0) {
                if (P1 != 0) {
                  goto ERROR;
                }
              }
            }
            q = 1;
            break;
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[4]) {
        if (P2 != 1) {
          if (! (P1 != 0)) {
            goto _L;
          }
        } else {
          _L: /* CIL Label */ 
          method_id = 6;
          if (dummy != 1) {
            goto ERROR;
          }
          q = 1;
          break;
        }
        goto __Cont;
      }
      goto __Cont;
    }
    if (q == 2) {
      if ((*(symb + FLAG___0))[5]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___0;
          } else {
            if (dummy != 1) {
              _L___0: /* CIL Label */ 
              method_id = 1;
              if (dummy != 1) {
                goto ERROR;
              }
              q = 2;
              this_sink = this_sink;
              this_sinkConnected = this_sinkConnected;
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[6]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___2;
          } else {
            if (dummy != 1) {
              _L___2: /* CIL Label */ 
              if (this_sink == 0) {
                goto _L___1;
              } else {
                if (dummy != 1) {
                  _L___1: /* CIL Label */ 
                  method_id = 1;
                  if (dummy != 1) {
                    goto ERROR;
                  }
                  q = 2;
                  this_sink = this_sink;
                  this_sinkConnected = this_sinkConnected;
                }
              }
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[7]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___3;
          } else {
            if (dummy != 1) {
              _L___3: /* CIL Label */ 
              method_id = 3;
              if (dummy != 1) {
                goto ERROR;
              }
              q = 2;
              this_sink = this_sink;
              this_sinkConnected = this_sinkConnected;
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[8]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___5;
          } else {
            if (dummy != 1) {
              _L___5: /* CIL Label */ 
              if (this_sink == 0) {
                goto _L___4;
              } else {
                if (dummy != 1) {
                  _L___4: /* CIL Label */ 
                  method_id = 3;
                  if (dummy != 1) {
                    goto ERROR;
                  }
                  q = 2;
                  this_sink = this_sink;
                  this_sinkConnected = this_sinkConnected;
                }
              }
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[9]) {
        if (dummy == 1) {
          method_id = 4;
          if (this_sink != 0) {
            goto ERROR;
          }
          q = 1;
          break;
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[10]) {
        if (P2 != 1) {
          if (P1 != 0) {
            method_id = 5;
            if (this_sink != 0) {
              if (P1 != 0) {
                if (P2 != 1) {
                  if (P1 != 0) {
                    goto ERROR;
                  }
                }
              }
            }
            q = 3;
            this_sink = P1;
            this_sinkConnected = 1;
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[11]) {
        if (P2 != 1) {
          if (! (P1 != 0)) {
            goto _L___6;
          }
        } else {
          _L___6: /* CIL Label */ 
          method_id = 6;
          if (dummy != 1) {
            goto ERROR;
          }
          q = 1;
          break;
        }
        goto __Cont;
      }
      goto __Cont;
    }
    if (q == 3) {
      if ((*(symb + FLAG___0))[12]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___7;
          } else {
            if (dummy != 1) {
              _L___7: /* CIL Label */ 
              method_id = 1;
              if (dummy != 1) {
                goto ERROR;
              }
              q = 3;
              this_sink = this_sink;
              this_sinkConnected = this_sinkConnected;
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[13]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___9;
          } else {
            if (dummy != 1) {
              _L___9: /* CIL Label */ 
              if (this_sink == 0) {
                goto _L___8;
              } else {
                if (dummy != 1) {
                  _L___8: /* CIL Label */ 
                  method_id = 1;
                  if (dummy != 1) {
                    goto ERROR;
                  }
                  q = 3;
                  this_sink = this_sink;
                  this_sinkConnected = this_sinkConnected;
                }
              }
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[14]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___10;
          } else {
            if (dummy != 1) {
              _L___10: /* CIL Label */ 
              method_id = 3;
              if (dummy != 1) {
                goto ERROR;
              }
              q = 3;
              this_sink = this_sink;
              this_sinkConnected = this_sinkConnected;
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[15]) {
        if (dummy == 1) {
          if (this_sink != 0) {
            goto _L___12;
          } else {
            if (dummy != 1) {
              _L___12: /* CIL Label */ 
              if (this_sink == 0) {
                goto _L___11;
              } else {
                if (dummy != 1) {
                  _L___11: /* CIL Label */ 
                  method_id = 3;
                  if (dummy != 1) {
                    goto ERROR;
                  }
                  q = 3;
                  this_sink = this_sink;
                  this_sinkConnected = this_sinkConnected;
                }
              }
            }
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[16]) {
        if (dummy == 1) {
          method_id = 4;
          if (this_sink == 0) {
            goto ERROR;
          }
          q = 3;
          this_sink = this_sink;
          this_sinkConnected = this_sinkConnected;
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[17]) {
        if (P2 != 1) {
          if (P1 != 0) {
            method_id = 5;
            if (P2 != 1) {
              if (this_sink == 0) {
                if (P1 != 0) {
                  goto ERROR;
                }
              }
            }
            q = 1;
            break;
          }
        }
        goto __Cont;
      }
      if ((*(symb + FLAG___0))[18]) {
        if (P2 != 1) {
          if (! (P1 != 0)) {
            goto _L___13;
          }
        } else {
          _L___13: /* CIL Label */ 
          method_id = 6;
          if (dummy != 1) {
            goto ERROR;
          }
          q = 1;
          break;
        }
        goto __Cont;
      }
      goto __Cont;
    }
    __Cont: /* CIL Label */ 
    FLAG___0 = FLAG___0 + 1;
  }
  ERROR: 
  tracerx_check();
  return (0);
}
}
