/* FILE NAME   : asm.cpp
 * PURPOSE     : Module of function assembler's functions.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include <stdlib.h>
#include "asm.h"

/* Add element to array and extend counter function.
 * \param[in]  array                      - byte* arr
 * \param[in]  point to counter           - long counter
 * \param[in]  point to element for add   - void* element
 * \param[in]  size of element for adding - long size
 * \param[out] none.
 */
void emit(byte* arr, int* counter, void* element, long size)
{
  memcpy(arr + *counter, element, size);
  *counter += size;
} /* End of 'emit' function */

/*! Add argument to array of codes.
 * \param[in]  argument    - double value
 * \param[in]  instruction - byte instr
 * \param[out] none.
*/
int mk5::assembler::emitArg(double value, byte instr)
{
  
  if (!isnan(value) && IsArg(instr))
  {
    char element_buf[8] = {};

    memcpy(element_buf, &value, sizeof(double));
    header.num_instr++;
    emit(code, &header.size_of_instr, (char *)element_buf, sizeof(double));
    return 1;
  }

  return 0;
}/* End of 'emitArg' function */

/*! Add new label to array of labels function.
 * \param[in]  string - mk5::line& Name
 * \param[out] none.
 */
int mk5::assembler::FindLabelStr(const mk5::line& Name)
{
  for (int i_label = 0; i_label < num_label; i_label++)
    if (!strncmp(label[i_label].name, Name.str + 1, strlen(Name.str) - 1))
      return i_label;
  
  return 0;
}/* End of 'FindLabelStr' function */

/*! Add new label to array of labels function.
 * \param[in]  retreat from start that writed in codes - const double lable
 * \param[out] number of label in array.
 */
int mk5::assembler::FindLabelNum(const double lable) {
  for (int i_label = 0; i_label < num_label; i_label++)
    if ((label[i_label].retreat_from_start == (int)lable))
      return i_label;

  return -1;
}/* End of 'FindLabelNum' function */

/*! Add new label to array of labels function.
 * \param[in]  string                         - mk5::line& Name
 * \param[in]  retrat from beggining of array - const int retreat_from_start
 * \param[out] none.
*/
int mk5::assembler::AddLabel(const mk5::line& Name, const int retreat_from_start) {
  label[num_label].len_name = strlen(Name.str);
  label[num_label].retreat_from_start = retreat_from_start;
  strncat(label[num_label].name, Name.str, label[num_label].len_name - 1);
  num_label++;

  return 1;
  //Начал писать добавления меток в массив структуры, ПРОДОЛЖИ ОТСЮДА! У тебя всё получится!
}/* End of 'AddLabel' function */

/*! Recognition what writing in string
 * \param[in]  first  instruction - byte& first_instr
 * \param[in]  second instruction - const mk5::line& second_instr
 * \param[in]  value              - double& value
 * \param[out] none
 */
byte RecognizeStr(byte& first_instr, const mk5::line& second_instr, double& value) {
  
  char rgst = *(second_instr.str + 1);
  sscanf(second_instr.str + 1, "%lf", &value);
  
  if (first_instr == INSTR_pop)
    first_instr += 1;

  if (!isnan(value)) {
    first_instr += 64;
    return 1;
  }
  else if (*(second_instr.str + 3) != '+')
    value = 0;
  else
    sscanf(second_instr.str + 4, "%lf", &value);

  if (rgst == 'a')
    first_instr += 128 + 64 + INSTR_ax;
  else if (rgst == 'b')
    first_instr += 128 + 64 + INSTR_bx;
  else if (rgst == 'c')
    first_instr += 128 + 64 + INSTR_cx;
  else if (rgst == 'd')
    first_instr += 128 + 64 + INSTR_dx;

  return 1;
}/* End of 'RecognizeStr' function */

/*! Read commands from file and add it in array of codes function.
 * \param[in]  name of file   - const char* FileName
 * \param[in]  arrat of lines - mk5::line** lines
 * \param[in]  buffer         - mk5::line& buf
 * \param[out] none.
 */
int mk5::assembler::RInstFileTex(const char* FileName, mk5::line** lines, mk5::line& buf) {
  //Add directory to name of file
  char Name[MAX_PATH] = "";
  strncat(Name, "..\\Codes\\", MAX_PATH);
  strncat(Name, FileName, MAX_PATH);
  
  //Reading buffer and get num of strings
  int num_strings = ReadStrBuf(lines, Name, &buf);
  len_code = num_strings * 2;
  
  //Alocated memory for array of codes
  code = (byte *)calloc(len_code * sizeof(double), sizeof(byte));

  //Variables for working with reading commands from buffer
  mk5::line first_instr = {}, second_instr = {};
  first_instr.len = second_instr.len = MAX_PATH;
  //Alocated memory for them
  first_instr.str = (char *)calloc(first_instr.len, sizeof(char));
  second_instr.str = (char *)calloc(second_instr.len, sizeof(char));
  
  //Value need for get number from buffer
  double value = NAN;
  
  //Element buf needed to copy value in it, for write it to array of codes
  byte zero = 0, cur_instr = 0;
  header.size_of_instr = 0;

  //Feel array of labels
  for (int i_string = 0; i_string < num_strings; i_string++) {
    sscanf((*lines)[i_string].str, "%s", first_instr.str);

    //Skip empty strings or comments
    if (SkipString(first_instr))
      continue;
    
    //Get code of instruction from string
    cur_instr = SwitchCommandText(first_instr);
    
    //Checking how much I need to move in the array
    if (IsLabel(cur_instr) || cur_instr == INSTR_call)
      header.size_of_instr += sizeof(double);
    else if (cur_instr == INSTR_label)
      AddLabel(first_instr, header.size_of_instr); //Adding new label to array of labels 
    else if (cur_instr == INSTR_push || cur_instr == INSTR_pop) {
      value = NAN;
      sscanf((*lines)[i_string].str, "%s %lf", first_instr.str, &value);
      if (!isnan(value))
        header.size_of_instr += sizeof(double);
    }

    header.size_of_instr += sizeof(char);
    
    //Feeling string by zeros
    memcpy(first_instr.str, &zero, first_instr.len);
  }

  //Put it zero before new circle
  header.size_of_instr = 0;
  int IsOk = 1;

  for (int i_string = 0; i_string < num_strings; i_string++) {
    value = NAN;
    sscanf((*lines)[i_string].str, "%s %lf", first_instr.str, &value);

    //Skip empty strings
    if (SkipString(first_instr))
      continue;

    cur_instr = SwitchCommandText(first_instr);

    if (cur_instr == INSTR_error)
      IsOk = 0;

    if (IsLabel(cur_instr) || cur_instr == INSTR_call) {
      sscanf((*lines)[i_string].str, "%s %s", first_instr.str, second_instr.str);
      value = label[FindLabelStr(second_instr)].retreat_from_start;
    }
    else if (cur_instr == INSTR_push || cur_instr == INSTR_pop) {
      if (isnan(value) && INSTR_push) {
        sscanf((*lines)[i_string].str, "%s %s", first_instr.str, second_instr.str);
        int rgst = SwitchCommandText(second_instr);
        if (cur_instr == INSTR_push && IsRegister(rgst))
          cur_instr = INSTR_pushax + (rgst - INSTR_ax);
        else if (cur_instr == INSTR_pop && IsRegister(rgst))
          cur_instr = INSTR_popax + (rgst - INSTR_ax);
        else
          RecognizeStr(cur_instr, second_instr, value);
      }
    }

    //Add instruction to array of codes
    header.num_instr++;
    emit(code, &header.size_of_instr, &cur_instr, sizeof(byte));
    emitArg(value, cur_instr);
    
    /* Fill memory zero */
    memcpy(first_instr.str, &zero, MAX_PATH);
    memcpy(second_instr.str, &zero, MAX_PATH);
  }

  len_code = header.num_instr;
  code = (byte *)realloc(code, header.size_of_instr); 

  free(first_instr.str);
  free(second_instr.str);
  
  return IsOk;
}/* End of 'RInstFileTex' function */

/*! Write commands in unequal buf in file function.
 * \param[in]  name of file       - const char* FileName
 * \param[out] none.
 */
int mk5::assembler::WInstFileAsme(const char* FileName) {
  //Write in File
  char Name[MAX_PATH] = "";
  strcat(Name, "..\\Codes\\");
  const char* exp = ".mk5asme";
  strcat(Name, FileName);
  strcat(Name, exp);

  FILE *file = fopen(Name, "wb");
  if (file == NULL)
    return 0;

  //Write header
  fwrite(&header, sizeof(header), 1, file);
  fwrite(code, sizeof(byte), header.size_of_instr, file);
  
  fclose(file);

  return 1;
}/* End of 'WInstFileAsme' function */

/*! Convert buf to code function.
 * \param[in]  name of file.mk5assemblere - const char* FileName
 * \param[out] none.
 */
int mk5::assembler::Listing(const char* FileName) {
  char Name[MAX_PATH] = "";
  strcat(Name, "..\\Codes\\");
  strcat(Name, "Listing(");
  strcat(Name, FileName);
  strcat(Name, ")");

  FILE *file = fopen(Name, "wb");
  if (file == NULL)
    return 0;

  //Variables for writing
  const int size_str = 50;
  char *instr = (char *)calloc(size_str, sizeof(char));
  
  const char zero = 0;
  byte cur_instr = 0;

  fprintf(file, "HEADER:\n"
    "signature:     %.4X\n"
    "version:       %d\n"
    "num_instr:     %d\n"
    "size_of_instr: %d", header.signature, header.version, header.num_instr, header.size_of_instr);

  double el = NAN;

  for (int cur_len_buf = 0; cur_len_buf < header.size_of_instr;) {
    fprintf(file, "\n");
    
    cur_instr = *(code + cur_len_buf);

    if (cur_instr != INSTR_label)
      GetStringOfInstruction(cur_instr, instr);
    else
      strcat(instr, label[FindLabelNum(cur_len_buf)].name);
    
   
    cur_len_buf += sizeof(char);

  
    if (IsLabel(cur_instr) || cur_instr == INSTR_call) {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      int cur_label = FindLabelNum(el);
      fprintf(file, "%04x %02x | ", cur_len_buf, cur_instr);

      for (int i = 0; i < 8; i++)
        fprintf(file, "%02X ", *(code + cur_len_buf + i));

      fprintf(file, "| %s :%s   %5.lf ", instr, label[cur_label].name, el);
      
      cur_len_buf += sizeof(double);
    }
    else if (cur_instr == INSTR_push || cur_instr == INSTR_hack) {
      memcpy(&el, code + cur_len_buf, sizeof(double));

      fprintf(file, "%04X %02X | ", cur_len_buf, cur_instr);
      for (int i = 0; i < 8; i++)
        fprintf(file, "%02X ", *(code + cur_len_buf + i));
      fprintf(file, "| %s %5.lf ", instr, el);

      cur_len_buf += sizeof(double);
    }
    else if (cur_instr == INSTR_label) {
      fprintf(file, "%04X %02X                            ", cur_len_buf, cur_instr);
      fprintf(file, " %s: ", instr);
    }
    else if (cur_instr == INSTR_pushram || cur_instr == INSTR_popram) {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      fprintf(file, "%04X %02X | ", cur_len_buf, cur_instr);
      
      for (int i = 0; i < 8; i++)
        fprintf(file, "%02X ", *(code + cur_len_buf + i));

      fprintf(file, "| %s%lf] ", instr, el);
      cur_len_buf += sizeof(double);
    }
    else if (IsArg(cur_instr)) {
      fprintf(file, "%04X %02X | ", cur_len_buf, cur_instr);
      
      memcpy(&el, code + cur_len_buf, sizeof(double));
      if (el == 0)
        fprintf(file, "                        | %s] ", instr);
      else {
        for (int i = 0; i < 8; i++)
          fprintf(file, "%02X ", *(code + cur_len_buf + i));
        fprintf(file, "| %s+%lf] ", instr, el);
      }
      cur_len_buf += sizeof(double);
    }
    else {
      fprintf(file, "%04X %02X                            ", cur_len_buf, cur_instr);
      fprintf(file, " %s", instr);
    }

    memcpy(instr, &zero, size_str);
  }
  
  fclose(file);
  free(instr);

  return 1;
}/* End of 'Listing' function */

/* Convert text codes to assembler unequal buffer codes function.
 * \param[in]  name of file - const char* FileName
 * \param[out] none
 */
int mk5::assembler::InstToAsme(const char* FileName) {
  char Name[MAX_PATH] = "";
  mk5::line* lines, buf = {};

  if (RInstFileTex(FileName, &lines, buf)) {
    strncat(Name, FileName, strlen(FileName) - strlen(".mk5"));
    WInstFileAsme(Name);
    strcat(Name, ".mk5asme");
  }

  Listing(Name);

  free(buf.str);
  free(lines);

  return 1;
}/* End of 'InstToAsme' function */