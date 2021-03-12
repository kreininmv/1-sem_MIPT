#pragma once
#ifndef ONEGINH
#define ONEGINH 
/* FILE NAME   : onegin.h
 * PURPOSE     : Header T01ONEGIN module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 09.10.2020.
 * NOTE        : None.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>

  /*! Mode of Skip trash. */
enum MODE {
  FROM_BEGIN = 1,
  FROM_END = 2
};

/*! Structure of stoke.
 * Len of stroke                   - int len;
 * Poin to first element of stroke - char *str;
 */
namespace mk5 //IFK namespace
{
  struct line
  {
    int len = 0;
    char *str = nullptr;
  };
}

/* Normalize uppercase to lower case symbols.
 * \param[in]  symbol - char element;
 * \param[out] normalizing symbol;
 */
char NormalizeToUpper(const char element);

/* Skip trash in mk5::line function.
 * \param[in]  point to the mk5::line - mk5::line *mk5::line;
 * \param[in]  mode of skipping       - int mode;
 * \param[out] none.
 */
void SkipTrash(mk5::line *line, int mode);

/* Checking symbol is letter function.
 * \param[in]  symbol - const char element;
 * \param[out] true or false;
 */
int IsTrash(const char element);

/* Get length of file function.
 * \param[in]  file - FILE *file;
 * \param[out] lenth of file.
 */
int GetLengthOfFile(FILE *file);

/* Put point to begin of strings in buffer function.
 * \param[in]  buffer                   - const mk5::line buf;
 * \param[in]  array of strings         - mk5::line **lines;
 * \param[in]  size of array of strings - int count_of_strings;
 * \param[out] none.
 */
void PutPointsToStrings(const mk5::line buf, mk5::line **lines, int count_of_strings);

/* Count of symbols in buffet function.
 * \param[in]  point to the buffer -  const char* buf;
 * \param[in]  symbols that need to count - const char sym;
 * \param[ouy] number of symbols.
 */
int CountSymbols(const char* buf, const char sym);

/* Read strokes from file function.
 * \param[in]  point to array of strings  - mk5::line **str;
 * \param[in]  name of file               - char *FileName;
 * \param[in]  buffer of file             - mk5::line *buf;
 * \param[out] count of strokes.
 */
int ReadStrBuf(mk5::line **str, const char *FileName, mk5::line *buf);

/* Read strings from file function.
 * \param[in]  point to array of strings  - mk5::line **str;
 * \param[in]  name of file               - char *FileName;
 * \param[out] count of strokes.
 */
int ReadStr(mk5::line **str, const char *FileName);

/* Write buffer in the file function.
 * \param[in]  structure of string  - mk5::line str;
 * \param[in]  name of file         - char *FileName;
 * \param[in]  mode of open file    - const char *open_mode;
 * \param[out] none.
 */
void WriteStrBuf(mk5::line str, const char *FileName, const char *open_mode);

/* Write array of strings to the file function.
 * \param[in]  array of strings  - mk5::line *str;
 * \param[in]  name of file      - char *FileName;
 * \param[in]  size of array     - int size;
 * \param[in]  mode of open file - const char *open_mode;
 * \param[out] none.
 */
void WriteStr(mk5::line *lines, const char *FileName, size_t size, const char *open_mode);


/* Unit test function.
 * \param[in]  array of strings - mk5::line *str;
 * \param[in]  size of array    - int size;
 * \param[out]
 */
void Unit(mk5::line *lines, size_t size);
#endif