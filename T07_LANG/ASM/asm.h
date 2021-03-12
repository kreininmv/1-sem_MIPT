#ifndef ASMH
#define ASMH
/* FILE NAME   : asm.h
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

  class assembler 
  {
  private:
    program_header header = {};
    int num_label = 0;
    program_label label[len_label];

    byte* code = nullptr;
    
    long len_code = -1;

  public:
    //—делать
    //$ онструктор копировани€, каким образом перехватывать копирование объекта? (написание конструктора)
    //$ќператор присваивани€, тоже решить копировани€ с ним
    //Ёпитимь€: правило трех (если объ€влен деструктор или конструктор копировани€ или оператор присваивани€, то нужон объ€вл€ть и все остальные) 
    //»диома:   RAII (хочешь получить что-то, то это что-то сразу и инициализируетс€, хочешь освободить, то это и удалитс€)
    //ѕеределать риды в конструкторы или нет (видимо нет)
    //Ёпитимь€: кто написал boost noncopyable (ƒэвид јбрахам)

    assembler()
    {
    }

    assembler(const assembler& copy) = delete;
    /*
    {
      header.signature = copy.header.signature;
      header.version = copy.header.version;
      header.num_instr = copy.header.num_instr;
      header.size_of_instr = copy.header.size_of_instr;
      len_code = copy.len_code;

      if (len_code <= 0)
        code = cur_instr;
      else
      {
        code = (double *)calloc(len_code, sizeof(double));
        memcpy(code, copy.code, sizeof(double) * len_code);
      }
    }
    */
    assembler operator= (const assembler& copy) = delete;
    /*
    {
      if (this == &copy)
        return *this;

      header.signature = copy.header.signature;
      header.version = copy.header.version;
      header.num_instr = copy.header.num_instr;
      header.size_of_instr = copy.header.size_of_instr;
      len_code = copy.len_code;

      if (len_code <= 0)
        code = cur_instr;
      else
      {
        code = (double *)calloc(len_code, sizeof(double));
        memcpy(code, copy.code, sizeof(double) * len_code);
      }-

      return *this;
    }
    */
    ~assembler()
    {
      if (code != nullptr)
      {
        //printf("%p : free()\n", code);
        free(code);
        code = nullptr;
      }

      len_code = -1;
      header.signature = 0x1EEF;
      header.version = 3;
      header.num_instr = 0;
      header.size_of_instr = 0;
    }

    /* Convert text codes to assembler enequal buffer codes function.
     * \param[in]  name of file - const char* FileName
     * \param[out] none
     */
    int InstToAsme(const char* FileName);

  private:
    /* Find by string a label in array of labels function.
     * \param[in]  string - mk5::line& Name
     * \param[out] none.
     */
    int FindLabelStr(const mk5::line& Name);

    /* Find by num a label in array of labels function.
     * \param[in]  retreat from start that writed in codes - const double lable
     * \param[out] number of label in array.
     */
    int FindLabelNum(const double lable);
    
    /* Add new label to array of labels function.
     * \param[in]  string                         - mk5::line& Name
     * \param[in]  retrat from beggining of array - const int retreat_from_start
     * \param[out] none.
     */
    int AddLabel(const mk5::line& Name, const int retreat_from_start);

     /*! Add argument to array of codes.
     * \param[in]  argument    - double value
     * \param[in]  instruction - byte instr
     * \param[out] none.
     */
    int emitArg(double value, byte instr);

    /*! Read commands from file and add it in array of codes function.
     * \param[in]  name of file   - const char* FileName
     * \param[in]  arrat of lines - mk5::line** lines
     * \param[in]  buffer         - mk5::line& buf
     * \param[out] none.
     */
    int RInstFileTex(const char* FileName, mk5::line** lines, mk5::line& buf);

    /*! Write commands in unequal buf in file function.
     * \param[in]  name of file       - const char* FileName
     * \param[out] none.
     */
    int WInstFileAsme(const char* FileName);

    /*! Convert buf to code function.
     * \param[in]  name of file - const char* FileName
     * \param[out] none.
     */
    int Listing(const char* FileName);
  };
}

/* Add element to array and extend counter function.
 * \param[in]  array                      - byte* arr
 * \param[in]  point to counter           - long counter
 * \param[in]  point to element for add   - void* element
 * \param[in]  size of element for adding - long size
 * \param[out] none.
 */
void emit(byte* arr, int* counter, void* element, long size);
#endif