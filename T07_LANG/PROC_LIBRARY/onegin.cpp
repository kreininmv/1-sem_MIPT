#include "onegin.h"

/*! Normalize lower to uppercase case symbol.
 * \param[in]  symbol - char element;
 * \param[out] normalizing symbol;
 */
char NormalizeToUpper(const char element)
{
  if (element >= 'a' && element <= 'z')
    return element - ('a' - 'A');

  return element;
} /* End of 'Normalize' function */

/*! Skip trash in mk5::line function.
 * \param[in]  point to the mk5::line - mk5::line *mk5::line;
 * \param[in]  mode of skipping  - const char *mode;
 * \param[out] none.
 */
void SkipTrash(mk5::line *line, int mode)
{
  if (mode == FROM_END)
    while (IsTrash(line->str[line->len]) && line->len != 0)
      line->len--;
  else if (mode == FROM_BEGIN)
    while (IsTrash(*(line->str)) && *(line->str) != 0)
      line->str++;

}/* End of 'SkipTrash' function */

/*! Checking symbol is letter function.
 * \param[in]  symbol - const char element;
 * \param[out] true or false;
 */
int IsTrash(const char element)
{
  if (element < 'A' || element > 'z' || ('Z' < element && element < 'a'))
    return 1;

  return 0;
}/* End of 'IsTrash' function */

/*! Put point to begin of strings in buffer function.
 * \param[in]  buffer                   - const mk5::line buf;
 * \param[in]  array of strings         - mk5::line **lines;
 * \param[in]  size of array of strings - int count_of_strings;
 * \param[out] none.
 */
void PutPointsToStrings(const mk5::line buf, mk5::line **lines, int count_of_strings)
{
  int cur_sym = 0, past_cur_sym = 0;
  int i_sym = 0;
  for (i_sym = 0; i_sym < count_of_strings - 1; i_sym++)
  {
    (*lines)[i_sym].str = &(buf.str[cur_sym]);
    past_cur_sym = cur_sym;
    while (buf.str[cur_sym] != '\n')
      cur_sym++;
    if (buf.str[cur_sym - 1] == '\r')
      buf.str[cur_sym - 1] = 0;
    else
      buf.str[cur_sym] = 0;
    
    cur_sym++;
    
    (*lines)[i_sym].len = cur_sym - past_cur_sym;
  }

  /* Last string */
  (*lines)[count_of_strings - 1].str = &(buf.str[cur_sym]);
  past_cur_sym = cur_sym;
  while (buf.str[cur_sym] != 0)
    cur_sym++;
  (*lines)[count_of_strings - 1].len = cur_sym + 1 - past_cur_sym;

}/* End of 'PutPointToStrings' function */

/*! Get length of file function.
 * \param[in]  file - FILE *file;
 * \param[out] lenth of file.
 */
int GetLengthOfFile(FILE *file)
{
  fseek(file, 0, SEEK_END);
  return ftell(file);
}/* End of 'GetLenthOfFile' function */

/*! Count of symbols in buffet function.
 * \param[in]  point to the buffer -  const char* buf;
 * \param[in]  symbols that need to count - const char sym;
 * \param[ouy] number of symbols.
 */
int CountSymbols(const char* buf, const char sym)
{
  int count_of_sym = 0, i_sym = 0;

  while (buf[i_sym++] != 0)
    if (buf[i_sym] == sym)
      count_of_sym++;

  return count_of_sym;
}/* End of 'CountSymbols' function */

/*! Read strings from file function.
 * \param[in]  point to array of strings  - mk5::line **str;
 * \param[in]  name of file               - char *FileName;
 * \param[in]  buffer of file             - mk5::line *buf;
 * \param[out] count of strokes.
 */
int ReadStrBuf(mk5::line **lines, const char *FileName, mk5::line *buf)
{
  FILE *file = fopen(FileName, "rb");

  if (file == NULL)
  {
    printf("We can't find the file...\n");
    return 0;
  }

  buf->len = GetLengthOfFile(file) + 1;

  rewind(file);

  buf->str = (char*)calloc(buf->len, sizeof(char));
  fread(buf->str, sizeof(char), buf->len - 1, file);

  fclose(file);

  buf->str[buf->len - 1] = 0;
  int count_of_strings = CountSymbols(buf->str, '\n') + 1;

  *lines = (mk5::line*) calloc(count_of_strings, sizeof(mk5::line));
  PutPointsToStrings(*buf, lines, count_of_strings);

  return count_of_strings;
} /* End of 'ReadStrBuf' function */

/*! Read strokes from file function.
 * \warning deprecated!.
 * \param[in]  point to array of strings  - mk5::line **str;
 * \param[in]  name of file               - char *FileName;
 * \param[out] count of strings.
 */
int ReadStr(mk5::line **str, const char *FileName)
{
  int size = 0;
  FILE *F = fopen(FileName, "r");

  if (F == NULL)
  {
    printf("We can't find the file...\n");
    assert(0);
  }

  int tmp = 0;
  while ((tmp = fgetc(F)) != EOF)
    if (tmp == '\n')
      size++;

  size++;

  rewind(F);

  *str = (mk5::line *)calloc(size, sizeof(mk5::line));

  long lencur = 0;

  for (int i = 0; i < size; i++)
  {
    int lenstr = 0;

    do
    {
      tmp = fgetc(F);
      lenstr++;
    } while (tmp != '\n' && tmp != EOF);

    lenstr++;
    fseek(F, lencur * sizeof(char), SEEK_SET);
    lencur += lenstr;

    (*str)[i].len = lenstr;
    (*str)[i].str = (char *)calloc(lenstr, sizeof(char));
    fgets((*str)[i].str, lenstr, F);
  }

  fclose(F);

  return size;
} /* End of 'ReadStr' function */

/*! Write buffer in the file function.
 * \param[in]  structure of string  - mk5::line str;
 * \param[in]  name of file         - char *FileName;
 * \param[in]  mode of open file    - const char *open_mode;
 * \param[out] none.
 */
void WriteStrBuf(mk5::line str, const char *FileName, const char *open_mode)
{
  FILE *F = fopen(FileName, open_mode);

  if (F == NULL)
  {
    printf("We can't create the file...\n");
    abort();
  }

  fwrite(str.str, str.len, sizeof(char), F);

  fclose(F);
}/* End of 'WriteStrBuf' function */

/*! Write array of strings to the file function.
 * \param[in]  array of strings  - mk5::line *str;
 * \param[in]  name of file      - char *FileName;
 * \param[in]  size of array     - int size;
 * \param[in]  mode of open file - const char *open_mode;
 * \param[out] none.
 */
void WriteStr(mk5::line *lines, const char *FileName, size_t size, const char *open_mode)
{
  FILE *F = fopen(FileName, open_mode);

  if (F == NULL)
  {
    printf("We can't create the file...\n");
    abort();
  }

  for (size_t i_line = 0; i_line < size; i_line++)
    fwrite(lines[i_line].str, lines[i_line].len, sizeof(char), F);
  // \"%s\" - использовать для отладки

  fclose(F);
}/* End of 'WriteStr' function */


/*! Unit test function.
 * \param[in]  array of strings - mk5::line *str;
 * \param[in]  size of array    - int size;
 * \param[out]
 */
void Unit(mk5::line *lines, size_t size)
{
  FILE *F = fopen("log.txt", "a+");

  if (F == NULL)
  {
    printf("We can't create the file...\n");
    assert(0);
  }

  fclose(F);
} /* End of 'Unit' function */
