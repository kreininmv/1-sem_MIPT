/* FILE NAME   : disassembler.cpp
 * PURPOSE     : Module of function disassembler's functions.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include <stdlib.h>
#include <iostream>

#include "disasm.h"

/* Add new label to array of labels function.
 * \param[in]  retreat from start that writed in codes - const double lable
 * \param[out] number of label in array.
 */
int mk5::disassembler::FindLabel(const double lable)
{
  for (int i_label = 0; i_label < num_label; i_label++)
    if ((label[i_label].retreat_from_start == (int)lable))
      return i_label;

  return -1;
}/* End of 'FindLabel' function */

/* Add new label to array of labels function.
 * \param[in]  string - mk5::line& Name
 * \param[out] none.
*/
int mk5::disassembler::AddLabel(const int retreat_from_start)
{
  sprintf(label[num_label].name, "label%d", num_label);
  label[num_label].retreat_from_start = retreat_from_start;
  num_label++;
 
  return 1;
}/* End of 'AddLabel' function */

/*! Read commands from file unequal buf and add it in calls stack function.
 * \param[in]  name of file       - const char* FileName
 * \param[out] none.
 */
int mk5::disassembler::RInstFileAsme(const char* FileName)
{
  const char* exp = ".mk5asme";
  if (strcmp((FileName + strlen(FileName) - strlen(exp)), exp))
  {
    printf("This file doesn't have expansion '%s' !!!\n", exp);
    return 0;
  }

  char Name[MAX_PATH] = "";
  strcat(Name, "..\\Codes\\");
  strcat(Name, FileName);
  FILE* file = fopen(Name, "rb");
  
  if (file == NULL)
    return 0;

  //Read header
  fread(&header, sizeof(header), 1, file);
  len_code = header.num_instr;

  if (header.num_instr < 0 || header.signature != 0x1EEF)
  {
    fclose(file);
    return 0;
  }

  code = (byte *)calloc(header.size_of_instr, sizeof(byte));
  
  fread(code, sizeof(char), header.size_of_instr, file);
  
  fclose(file);

  return 1;
}/* End of 'RInstFileAsme' function */

/* Write instructions in tex file.
 * \param[in]  name of file - const char* FileName
 * \param[out] none.
 */
int mk5::disassembler::WInstrFileTex(const char* FileName)
{
  char Name[MAX_PATH] = "";
  const char* exp = ".mk5";
  strcat(Name, "..\\Codes\\");
  strcat(Name, FileName);
  strcat(Name, exp);

  FILE *file = fopen(Name, "wb");

  if (file == NULL || len_code == 0)
    return 0;

  const char zero = 0;
  const int size_str = 50;
  char *instr = (char *)calloc(size_str, sizeof(char));
  byte cur_instr = 0;

  double el = NAN;

  // Fill array of labels
  for (int cur_len_buf = 0; cur_len_buf < header.size_of_instr;)
  {
    cur_instr = *(code + cur_len_buf);
   
    if (cur_instr == INSTR_label)
      AddLabel(cur_len_buf);
    else if (cur_instr == INSTR_push || cur_instr == INSTR_hack)
      cur_len_buf += sizeof(double);

    cur_len_buf += sizeof(char);
  }

  for (int cur_len_buf = 0; cur_len_buf < header.size_of_instr;)
  {
    cur_instr = *(code + cur_len_buf);
    
    
    if (cur_instr != INSTR_label)
      GetStringOfInstruction(cur_instr, instr);

    cur_len_buf += sizeof(char);
    
    if (cur_instr == INSTR_label)
      fprintf(file, "%s:", label[FindLabel(cur_len_buf - sizeof(char))].name);
    else if (IsLabel(cur_instr) || cur_instr == INSTR_call)
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      fprintf(file, "%s :%s", instr, label[FindLabel(el)].name);

      cur_len_buf += sizeof(double);
    }
    else if (cur_instr == INSTR_push || cur_instr == INSTR_hack)
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      fprintf(file, " %lf", el);

      cur_len_buf += sizeof(double);
    }
    else if (cur_instr == INSTR_pushram || cur_instr == INSTR_popram)
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      fprintf(file, "%s%lf]", instr, el);

      cur_len_buf += sizeof(double);
    }
    else if (IsArg(cur_instr))
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      if (el == 0)
        fprintf(file, "%s]", instr);
      else
        fprintf(file, "%s+%lf]", instr, el);
      cur_len_buf += sizeof(double);
    }
    else
      fprintf(file, "%s", instr);

    if (cur_len_buf != header.size_of_instr)
      fprintf(file, "\n");

    memcpy(instr, &zero, size_str);
  }

  fclose(file);
  free(instr);

  return 1;
}/* End of 'WInstrFileTex' function */

/*! Convert buf to code function.
 * \param[in]  name of file.mk5assemblere - const char* FileName
 * \param[out] none.
 */
int mk5::disassembler::Listing(const char* FileName)
{
  int mode_listing = (FileName[strlen(FileName) - 1] == 'b') ? sizeof(double) : sizeof(char);

  char Name[MAX_PATH] = "";
  strcat(Name, "..\\Codes\\");
  strcat(Name, "Listing(");
  strcat(Name, FileName);
  strcat(Name, ")");

  FILE *file = fopen(Name, "wb");
  if (file == NULL)
    return 0;

  /* Constants for clearing strings and for size of string */
  const char zero = 0;
  const int size_str = 50;
  /* For print name of labels */
  mk5::line name_label = {};
  name_label.str = (char *)calloc(size_str, sizeof(char));
  name_label.len = size_str;
  /* For getting instructions from array of codes */
  char *instr = (char *)calloc(size_str, sizeof(char));
  byte cur_instr = 0;

  fprintf(file, "HEADER:\n"
    "signature:     %.4X\n"
    "version:       %d\n"
    "num_instr:     %d\n"
    "size_of_instr: %d", header.signature, header.version, header.num_instr, header.size_of_instr);

  double el = NAN;
  // Fill array of labels 
  for (int cur_len_buf = 0; cur_len_buf < header.size_of_instr;)
  {
    if (mode_listing == sizeof(char))
      cur_instr = *(code + cur_len_buf);
    else
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      cur_instr = (byte)el;
    }

    if (GetNumCommand(cur_instr) == INSTR_push || GetNumCommand(cur_instr) == INSTR_hack)
      cur_len_buf += sizeof(double);
    else if (GetNumCommand(cur_instr) == INSTR_label)
      AddLabel(cur_len_buf);
  }

  //Writing
  for (int cur_len_buf = 0; cur_len_buf < header.size_of_instr;)
  {
    fprintf(file, "\n");

    cur_instr = *(code + cur_len_buf);

    GetStringOfInstruction(cur_instr, instr);
    cur_len_buf += mode_listing;
    
    if (IsLabel(cur_instr))
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));
      FindLabel(el);
      fprintf(file, "%04x %02x ", cur_len_buf, cur_instr);
      
      for (int i = 0; i < 8; i++)
          fprintf(file, "%02X ", *(code + cur_len_buf + i));

      fprintf(file, "| %s %5.lf", instr, el);
    }
    else if (cur_instr == INSTR_push || cur_instr == INSTR_hack)
    {
      memcpy(&el, code + cur_len_buf, sizeof(double));

      fprintf(file, "%04X %02X | ", cur_len_buf, cur_instr);
      for (int i = 0; i < 8; i++)
        fprintf(file, "%02X ", *(code + cur_len_buf + i));
      fprintf(file, "| %s %5.lf; PUSH", instr, el);

      cur_len_buf += sizeof(double);
    }
    else
    {
      fprintf(file, "%04X %02X                            ", cur_len_buf, cur_instr);
      fprintf(file, " %s; ", instr);
    }

    memcpy(instr, &zero, size_str);
  }

  fclose(file);
  free(instr);

  return 1;
}/* End of 'Listing' function */

/* Convert unequal buffer codes to text codes function.
 * \param[in]  name of file - const char* FileName
 * \param[out] none
 */
int mk5::disassembler::InstFromAsme(const char* FileName)
{
  char Name[MAX_PATH] = "";
  
  RInstFileAsme(FileName);
  strncat(Name, FileName, strlen(FileName) - strlen(".mk5asme"));
  WInstrFileTex(Name);

  return 1;
}/* End of 'InstFromAsme' function */