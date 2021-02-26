/* FILE NAME   : T02STACK.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 06.10.2020.
 * NOTE        : None.
 */

 /* Plan:
  1. Push, pop, constructor, desctuctor
  2. Verificator, assert, damp
  3. Canareiki, hash
  */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stack>

#include "STACK.h"


/*! Main function.
 * \param[in]  none.
 * \param[out] none.
 */
int main(void)
{

  mk5::stack stk = {};
  mk5::stack stk2 = {};

  stk.error = StackConstr(stk, 50);
  stk2.error = StackConstr(stk2, 50);
  stk.goose1 = 5; //Ошибка

  StackPush(&stk, 20.5);
  StackPush(&stk, 24.5);
  StackPush(&stk, 25656);
  Dump(stk);

  stk.count = 0;

  double* trap = (double*)calloc(10, sizeof(double));
  free(trap);
  stk2.data = trap;
  StackDestruct(&stk2);
  /*
  for (int i = 0; i < 1000000000000; i++) {
    StackPush(&stk, 20.5);
    printf("%p\n", stk.data);
  }
  */


  StackDestruct(&stk);
  StackDestruct(&stk2);
  return 0;
}/* End of 'main' function */