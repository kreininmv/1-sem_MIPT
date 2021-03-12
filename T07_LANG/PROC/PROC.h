#ifndef PROCH
#define PROCH
/* FILE NAME   : PROC.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */
 
#include "stack.h"
#include "onegin.h"
#include "utils.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define FRAME_W 256
#define FRAME_H 256

void Display(void);
void Timer(void);
void Keyboard(unsigned char Key, int x, int y);
void Copy(double *RAM);
extern int handle;

namespace mk5
{
  class proc
  {
  private:
    mk5::program_header header = {};

    mk5::stack stack = {};
    mk5::stack calls = {};

    byte *code = nullptr;
    long len_code = -1;

    double ax = NAN,
           bx = NAN,
           cx = NAN,
           dx = NAN;

    /*! Switch function by enum in stack function.
     * \param[in]  number of current instruction - size_t& cur_instr
     * \param[out] none.
     */
    int SwitchCommandEnum(size_t& cur_instr);
  
  public:
    double RAM[65536];
    proc()
    {
      StackConstr(stack, 2);
      StackConstr(calls, 2);
    }
    
    ~proc()
    {
      //MessageBox(NULL, "", "", 0);
      StackDestruct(&calls);
      StackDestruct(&stack);
      if (code != nullptr)
        free(code);
    }
    proc(const proc& copy) = delete;
    proc operator= (const proc& copy) = delete;

   /*! Do commands from processor function.
     * \param[in]  none.
     * \param[out] none.
     */
    int DoInstruction(void);

    /*! Read commands from file unequal buf and add it in calls stack function.
     * \param[in]  name of file - const char* FileName
     * \param[out] none.
     */
    int RInstFileAsme(const char* FileName);

  };
}
#endif