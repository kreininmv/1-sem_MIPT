#ifndef DISASMH
#define DISASMH
/* FILE NAME   : DISASM.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include "stack.h"
#include "onegin.h"
#include "utils.h"

namespace mk5
{
  const size_t len_label = 30;

  class disassembler
  {
  private:
    program_header header = {};
    int num_label = 0;
    program_label label[len_label];

    byte *code = NULL;
    long len_code = -1;

  public:
    disassembler(){}
    
    disassembler(const disassembler& copy) = delete;
    
    disassembler operator= (const disassembler& copy) = delete;

    ~disassembler()
    {
      if (code != NULL)
      {
        //printf("%p : free()\n", code);
        free(code);
        code = NULL;
      }

      len_code = -1;
      header.signature = 0x1EEF;
      header.version = 3;
      header.num_instr = 0;
      header.size_of_instr = 0;
    }

    /* Convert unequal buffer codes to text codes function.
     * \param[in]  name of file - const char* FileName
     * \param[out] none
     */
    int InstFromAsme(const char* FileName);

  private:

    /* Add new label to array of labels function.
     * \param[in]  retreat from start that writed in codes - const double lable
     * \param[out] number of label in array.
     */
    int FindLabel(const double lable);

    /* Add new label to array of labels function.
     * \param[in]  retreat from begin - const int retreat_from_start
     * \param[out] none.
     */
    int AddLabel(const int retreat_from_start);

    /*! Read commands from file unequal buf and add it in calls stack function.
     * \param[in]  name of file       - const char* FileName
     * \param[out] none.
     */
    int RInstFileAsme(const char* FileName);

    /* Write instructions in tex file.
     * \param[in]  name of file - const char* FileName
     * \param[out] none.
     */
    int WInstrFileTex(const char* FileName);
    
    /*! Convert buf to code function.
     * \param[in]  name of file - const char* FileName
     * \param[out] none.
     */
    int Listing(const char* FileName);
  };
}
#endif