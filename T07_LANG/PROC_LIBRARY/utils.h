#ifndef UTILSH
#define UTILSH
/* FILE NAME   : Utils.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 20.10.2020.
 * NOTE        : None.
 */

#include "stack.h"
#include "onegin.h"

typedef unsigned char byte;
//#define byte char

#ifndef ENUM_INSTRUCTIONS
#define ENUM_INSTRUCTIONS
enum instructions
{
#define DEF_INSTR(name, num, code)            INSTR_##name           = num,
#define DEF_INSTRARG(name, arg, num, code)    INSTR_##name##arg      = num,
#define DEF_LABEL(name, num, code)            INSTR_##name           = num,
#define DEF_INSTRARGRAM(name, arg, num, code) INSTR_##name##arg##ram = num,
#define DEF_INSTRRAM(name, num, code)         INSTR_##name##ram      = num,
#include "instruction.h"

#undef DEF_INSTRRAM
#undef DEF_INSTRARGRAM
#undef DEF_INSTR
#undef DEF_INSTRARG
#undef DEF_LABEL

  INSTR_MAX
};
#endif

namespace mk5
{
  struct program_label
  {
    char name[MAX_PATH] = "";
    int len_name = 0;
    int retreat_from_start = 0;
  };

  struct program_header
  {
    int signature = 0x1EEF;
    int version = 3;
    int num_instr = 0;
    int size_of_instr = 0;
  };
}

/*! Checking is it command function.
 * \param[in]  command - double command
 * \param[in]  string  - char* name
 * \param[out] none..
 */
int GetStringOfInstruction(double instr, char* instr_name);

/*! Checking is it command function.
 * \param[in]  command - double command
 * \param[out] number of command.
 */
int GetNumCommand(double instruction);

/*! Compare two strings function
 * \param[in]  point to first string  - const char* text
 * \param[in]  point to second string - const char* str
 * \param[out] if str include str (return 1), else return 0.
 */
int IsStrInclText(const char* text, const char* str);

/*! Checks the instruction for label function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
*/
int IsLabel(const byte instr);

/*! Checks the instruction for register function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
*/
int IsRegister(const byte instr);

/*! Checks the instruction for an argument function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
 */
int IsArg(const byte instr);

/*! Skip string if is comment or empty function.
 * \param[in]  string - mk5::line& line
 * \param[out] true or false
 */
int SkipString(const mk5::line& line);

/*! Select a command function.
 * \param[in]  command - mk5::line command
 * \param[out] code of command.
 */
instructions SwitchCommandText(mk5::line command);

/* Debug memory allocation support */
#  include <crtdbg.h> 
#  define SetDbgMemHooks()                                            \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))    \

#endif