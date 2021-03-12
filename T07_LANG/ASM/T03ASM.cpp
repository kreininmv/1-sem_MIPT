/* FILE NAME   : T03ASM.cpp
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

#include "asm.h"

/*! Main function.
 * \param[in]  none.
 * \param[out] none.
 */
int main(int argc, char *argv[])
{
  SetDbgMemHooks();
  mk5::assembler myassembler;
  //myassembler.InstToAsme("test.tree");
  if (argc == 2)
    myassembler.InstToAsme(argv[1]);
  else
    printf("DOLBAEB\n");

  printf("ASM\n");
  return 0;
}/* End of 'main' function */