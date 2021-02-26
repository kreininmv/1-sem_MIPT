/* FILE NAME   : T03DISASM.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>

#include "disasm.h"

/*! Main function.
 * \param[in]  none.
 * \param[out] none.
 */
int main(void)
{
  SetDbgMemHooks();
  mk5::disassembler mydisassembler;
  mydisassembler.InstFromAsme("solveq.mk5asme");
 

  return 0;
}/* End of 'main' function */