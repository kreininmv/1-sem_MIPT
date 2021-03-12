/* FILE NAME   : vertex.cpp
 * PURPOSE     : Module of vertex class.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "akin.h"

/*! Add data to the vertex
 * \param[in]  point of data for add - const void* data_add
 * \param[out] none.
 */
int emit(void* source, void* data_add, int size) {

  memcpy(source, data_add, size);
  return 1;
}/* End of 'emit' function */

/*! Get string from console.
 * \param[in]  point to the string - char* str
 * \param[out] none
 */
void getstr(char* str) {
  
  int i = 0;
  char c;

  while ((c = getchar()) != '\n')
    if (str != nullptr && i < MAX_STR - 1)
      str[i++] = c;

  if (str != nullptr && i < MAX_STR)
    str[i] = 0;

}/* End of 'getstr' function */