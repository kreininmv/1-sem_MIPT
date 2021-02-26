/* FILE NAME   : PROC.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include <stdlib.h>

#include "PROC.h"

/*! Return size of retreat from beginning of array fuction.
 * \param[in]  array of labels  - const mk5::program_label* label
 * \param[in]  number of labels - int num_label
 * \param[in]  
 */
int FindLabel(const mk5::program_label* label,  int num_label, const mk5::line& string)
{
  for (int i_label = 0; i_label < num_label; i_label++)
    if (!strncmp(label[i_label].name, string.str + 1, strlen(string.str) - 1))
      return label[i_label].retreat_from_start;

  return -1;
}/* End of 'FindLabel' function */

/*! Switch function by enum in stack function.
 * \param[in]  number of current instruction            - size_t& cur_instr
 * \param[out] none.
 */
int mk5::proc::SwitchCommandEnum(size_t& cur_instr)
{
  double el = NAN;
  int cur = *(code + cur_instr);
  
  cur_instr += sizeof(char);
  
  switch (cur)
  {
#define DEF_INSTR(name, num, code)                     \
  case INSTR_##name:                                   \
    code                                               \
    printf("%i - %s (%lf)\n\n", cur_instr, #name, el); \
    return 1;

#define DEF_INSTRARG(name, arg, num, code)      \
  case INSTR_##name##arg:                       \
    code                                        \
    printf("%i - %s%s\n\n", cur_instr, #name, #arg);\
    return 1;

#define DEF_LABEL(name, num, code)            \
  case INSTR_##name:                          \
    code                                      \
    return 1;

#define DEF_INSTRARGRAM(name, arg, num, code) \
  case INSTR_##name##arg##ram:                \
    code                                      \
    printf("%i - %s [%s+%lf]\n\n", cur_instr, #name, #arg, el);\
    return 1;

#define DEF_INSTRRAM(name, num, code)            \
  case INSTR_##name##ram:                        \
    code                                         \
    printf("%i - %s [%lf]\n\n", cur_instr, #name, el);\
    return 1;

#include "instruction.h"

#undef DEF_INSTRARGRAM
#undef DEF_INSTRRAM
#undef DEF_LABEL
#undef DEF_INSTR
#undef DEF_INSTRARG
  default:
    return 0;
  }
}/* End of 'SwitchCommandEnum' function */

/*! Do commands from processor function.
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::proc::DoInstruction(void)
{
  size_t cur_instr = 0;
  while (SwitchCommandEnum(cur_instr))
  {
    //printf("CUR_INSTR: %i\nStack:\n", cur_instr);
    //for (int i = 0; i < stack.count; i++)
    //  printf("#%i - %lf\n", i, stack.data[i]);
    //
    //printf("Calls:\n");
    //for (int i = 0; i < calls.count; i++)
    //  printf("#%i - %lf\n", i, calls.data[i]);
    //
    //printf("ax - %lf\nbx - %lf\ncx - %lf\ndx - %lf\n", ax, bx, cx, dx);
    //getchar();
    ;
  }

  return 0;
}/* End of 'DoCommandsFromStack' function */


/*! Read commands from file unequal buf and add it in calls stack function.
 * \param[in]  name of file       - const char* FileName
 * \param[out] none.
 */
int mk5::proc::RInstFileAsme(const char* FileName)
{
  const char* exp = ".mk5asme";
  if (strcmp((FileName + strlen(FileName) - strlen(exp)), exp))
  {
    printf("This file doesn't have expansion '%s' !!!\n", exp);
    return 0;
  }

  char Name[MAX_PATH] = "";
  strncat(Name, "..\\Codes\\", MAX_PATH);
  strncat(Name, FileName, MAX_PATH);
  FILE* file = fopen(Name, "rb");

  if (file == NULL)
    return 0;

  //Read header
  fread(&header, sizeof(header), 1, file);

  if (header.num_instr < 0 || header.signature != 0x1EEF)
  {
    fclose(file);
    return 0;
  }

  len_code = header.num_instr;
  code = (byte *)calloc(header.size_of_instr, sizeof(byte));

  fread(code, sizeof(byte), header.size_of_instr, file);
  fclose(file);

  return 1;
}/* End of 'RInstFileAsme' function */