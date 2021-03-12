/* FILE NAME   : utils.cpp
 * PURPOSE     : Supporting functions.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 18.10.2020.
 * NOTE        : None.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/*! Checking is it command function.
 * \param[in]  command - double command
 * \param[in]  string  - char* name
 * \param[out] none..
 */
int GetStringOfInstruction(double instr, char* instr_name)
{
  instructions cur_instr = (instructions)((int)instr);

  switch (cur_instr)
  {
  #define DEF_INSTR(name, num, code)          \
    case INSTR_##name:                        \
      strcat(instr_name, #name);              \
      return 1;

  #define DEF_INSTRARG(name, arg, num, code)  \
    case INSTR_##name##arg:                   \
    {                                         \
      strcat(instr_name, #name);              \
      strcat(instr_name, " ");                \
      strcat(instr_name, #arg);               \
        return 1;                             \
    }

  #define DEF_LABEL(name, num, code)          \
    {                                         \
      strcat(instr_name, #name);              \
      return 1;                               \
    }

  #define DEF_INSTRARGRAM(name, arg, num, code)              \
    case INSTR_##name##arg##ram:                            \
    {                                                       \
      strcat(instr_name, #name);                            \
      strcat(instr_name, " [");                             \
      strcat(instr_name, #arg);                             \
      return 1;                                             \
    }

  #define DEF_INSTRRAM(name, num, code)   \
    case INSTR_##name##ram:               \
    {                                     \
      strcat(instr_name, #name);          \
      strcat(instr_name, " [");           \
      return 1;                           \
    }


  #include "instruction.h"
  
  default:
    strcat(instr_name, "Unknown instruction");
    return 0;
  
  #undef DEF_LABEL
  #undef GLUENAME
  #undef DEF_INSTRRAM
  #undef DEF_INSTRARGRAM
  #undef DEF_INSTR
  #undef DEF_INSTRARG
  }
}/* End of 'GetNumCommand' function */

/*! Checking is it command function.
 * \param[in]  command - double command
 * \param[out] number of command.
 */
int GetNumCommand(double instruction)
{
  int cur_instr = (int)instruction;
  switch (cur_instr)
  {
#define DEF_INSTR(name, num, code) \
    case INSTR_##name:             \
      return cur_instr;
#define DEF_INSTRARG(name, arg, nun, code) \
    case INSTR_##name##arg:                \
      return cur_instr;
#define DEF_LABEL(name, num, code) \
    case INSTR_##name:
      return cur_instr;
#define DEF_INSTRRAM(name, num, code)     \
   case INSTR_##name##ram:                \
      return cur_instr;
#define DEF_INSTRARGRAM(name, arg, num, code)              \
    case INSTR_##name##arg##ram:                          \
      return cur_instr;

#include "instruction.h"

  default:
    return 0;

#undef DEF_INSTRRAM
#undef DEF_INSTRARGRAM
#undef DEF_LABEL
#undef DEF_INSTR
#undef DEF_INSTRARG
    return 0;
  }
}/* End of 'GetNumCommand' function */

/*! Compare two strings function
 * \param[in]  point to first string  - const char* text
 * \param[in]  point to second string - const char* str
 * \param[out] if str include str (return 1), else return 0.
 */
int IsStrInclText(const char* text, const char* str)
{
  return !strncmp(text, str, strlen(str));
}/* End of 'IsStrInclText' function */

/*! Select a command function.
 * \param[in]  command - mk5::line command
 * \param[out] code of command.
 */
instructions SwitchCommandText(mk5::line command)
{
  //SkipTrash(&command, FROM_BEGIN);
  
  if (0);
#define DEF_INSTR(name, num, code)              \
  else if(IsStrInclText(#name, command.str))    \
    return INSTR_##name;

#define GLUENAME(name) #name

#define DEF_INSTRARG(name, arg, num, code)                 \
  else if(IsStrInclText(GLUENAME(arg##name), command.str)) \
    return INSTR_##name##arg;

#define DEF_LABEL(name, num, code)                        \
  else if (':' == command.str[strlen(command.str)-1])     \
    return INSTR_##name;

#define DEF_INSTRRAM(name, num, code)                     \
  else if(0)                                              \
    return INSTR_##name##ram;                             \

#define DEF_INSTRARGRAM(name, arg, num, code)              \
  else if (0)                                             \
    return INSTR_##name##arg##ram;


#include "instruction.h"

#undef DEF_INSTRRAM
#undef DEF_INSTRARGRAM
#undef DEF_LABEL
#undef DEF_INSTR
#undef DEF_INSTRARG

  return INSTR_error;
} /* End of 'SwitchCommand' function */

/*! Checks the instruction for label function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
*/
int IsLabel(const byte instr)
{
  if (instr != INSTR_jmp && instr != INSTR_ja && instr != INSTR_jae && instr != INSTR_je
                         && instr != INSTR_jb && instr != INSTR_jbe && instr != INSTR_jne)
    return 0;

  return 1;
}/* End of 'IsLabel' function */

/*! Checks the instruction for register function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
 */
int IsRegister(const byte instr)
{
  if (instr != INSTR_ax && instr != INSTR_bx && instr != INSTR_cx && instr != INSTR_dx)
    return 0;

  return 1;
}/* End of 'IsRegister' function */


/*! Checks the instruction for an argument function
 * \param[in]  instruction - const byte instr
 * \param[out] true or false
 */
int IsArg(const byte instr)
{
  if ( instr != INSTR_push      && instr != INSTR_jmp       && instr != INSTR_ja        && instr != INSTR_jae        && instr != INSTR_je
    && instr != INSTR_jb        && instr != INSTR_jbe       && instr != INSTR_jne       && instr != INSTR_call       && instr != INSTR_pushram
    && instr != INSTR_pushaxram && instr != INSTR_pushbxram && instr != INSTR_pushcxram && instr != INSTR_pushdxram  && instr != INSTR_popram
    && instr != INSTR_popaxram  && instr != INSTR_popbxram  && instr != INSTR_popcxram  && instr != INSTR_popdxram)
    return 0;
  
  return 1;
}/* End of 'IsArg' fucntion */

/*! Skip string if is comment or empty function.
 * \param[in]  string - mk5::line& line
 * \param[out] true or false
 */
int SkipString(const mk5::line& line)
{
  if (*line.str == 0 || *line.str == '\n' || (*line.str == '/' && *(line.str + 1) == '/'))
    return 1;

  return 0;
}/* End of 'SkipString' function */

