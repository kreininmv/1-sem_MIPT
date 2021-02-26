/* FILE NAME   : UTILS.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 06.10.2020.
 * NOTE        : None.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stack>

#include "stack.h"

/* Print all information about stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void StackPrint(mk5::stack* thou, FILE* log_file)
{
  fprintf(log_file, "{\n");

  if (thou->goose1 == goose_sleap)
  {
    FPRINTF_ENUM(goose_sleap, log_file, "  long goose1 =");
  }
  else if (thou->goose1 == goose_silent)
  {
    FPRINTF_ENUM(goose_silent, log_file, "  long goose1 =");
  }
  else
    fprintf(log_file, "  long goose1 = %d\n", thou->goose1);

  fprintf(log_file, "  int count   = %d\n", thou->count);
  fprintf(log_file, "  int  size   = %d\n", thou->size);
  fprintf(log_file, "  int* data   [%d]\n", (int)thou->data);
  fprintf(log_file, "  {\n");

  int size = (abs(thou->size) > 0) ? abs(thou->size) : MAX_SIZE;
  fprintf(log_file, "     goose_data1 %lf\n", *(thou->data - 1));

  for (int i_cur = 0; i_cur < MAX_SIZE && i_cur < size; i_cur++)
  {
    if (!isnan(thou->data[i_cur]))
      fprintf(log_file, "    *");
    else
      fprintf(log_file, "     ");

    fprintf(log_file, "[%d] - %lf\n", i_cur, thou->data[i_cur]);
  }

  fprintf(log_file, "     goose_data2 %lf\n", *(thou->data + size));
  fprintf(log_file, "  }\n");

  if (thou->goose2 == goose_sleap)
  {
    FPRINTF_ENUM(goose_sleap, log_file, "  long goose2 =");
  }
  else if (thou->goose2 == goose_silent)
  {
    FPRINTF_ENUM(goose_silent, log_file, "  long goose2 =");
  }
  else
    fprintf(log_file, "long goose2 = %d\n", thou->goose2);
  fprintf(log_file, "}\n\n");
}/* End of 'StackPrint' function */


/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpName(mk5::stack *thou, const char *Name)
{
  thou->error = StackCheckError(thou);

  FILE *log_file = fopen(Name, "w");

  switch (thou->error)
  {
    CASE_OF_SWITCH(STACK_NULL, thou, log_file);
    CASE_OF_SWITCH(DATA_NULL, thou, log_file);
    CASE_OF_SWITCH(COUNT_NEGATIVE, thou, log_file);
    CASE_OF_SWITCH(SIZE_NEGATIVE, thou, log_file);
    CASE_OF_SWITCH(COUNT_BIGGER_SIZE, thou, log_file);
    CASE_OF_SWITCH(STACK_EQUAL_DATA, thou, log_file);
    CASE_OF_SWITCH(CLEAR, thou, log_file);
    CASE_OF_SWITCH(INITIALIZED, thou, log_file);
    CASE_OF_SWITCH(FIRST_STRUCT_GOOSE_DEAD, thou, log_file);
    CASE_OF_SWITCH(SECOND_STRUCT_GOOSE_DEAD, thou, log_file);
  default:
    fprintf(log_file, "Stack (SOMETHING GOING WRONG) [%d], but i don't know what is...\n", (int)thou->data);
    break;
  }
  StackPrint(thou, log_file);

  fclose(log_file);
} /* End of 'DumpName' function */

/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpF(mk5::stack *thou, FILE* log_file)
{
  thou->error = StackCheckError(thou);

  switch (thou->error)
  {
    CASE_OF_SWITCH(STACK_NULL, thou, log_file);
    CASE_OF_SWITCH(DATA_NULL, thou, log_file);
    CASE_OF_SWITCH(COUNT_NEGATIVE, thou, log_file);
    CASE_OF_SWITCH(SIZE_NEGATIVE, thou, log_file);
    CASE_OF_SWITCH(COUNT_BIGGER_SIZE, thou, log_file);
    CASE_OF_SWITCH(STACK_EQUAL_DATA, thou, log_file);
    CASE_OF_SWITCH(CLEAR, thou, log_file);
    CASE_OF_SWITCH(SECOND_STRUCT_GOOSE_DEAD, thou, log_file);
    CASE_OF_SWITCH(FIRST_DATA_GOOSE_DEAD, thou, log_file);
    CASE_OF_SWITCH(SECOND_DATA_GOOSE_DEAD, thou, log_file);
    CASE_OF_SWITCH(INITIALIZED, thou, log_file);
  default:
    fprintf(log_file, "Stack (SOMETHING GOING WRONG) [%d], but i don't know what is...\n", (int)thou->data);
    break;
  }
  StackPrint(thou, log_file);
} /* End of 'DumpF' function */


/* Verification of stack function.
 * \param[in]  point to stack                - mk5::stack* thou;
 * \param[in]  line from where it called     - int line;
 * \param[in]  file from where it called     - char* file;
 * \param[in]  function from where it called - char* func;
 * \param[out] none.
 */
void StackVerification(mk5::stack* thou, int line, const char* file, const char* func)
{
  if ((thou->error = StackCheckError(thou)) == INITIALIZED)
    return;

  char *Name = (char *)calloc(strlen(thou->Name) + 24, sizeof(char));
  if (thou->Name == NULL)
    strcat(Name, "log_stack(NAMENULL).txt");
  else
  {
    strcat(Name, "log_stack(");
    strcat(Name, thou->Name);
    strcat(Name, ").txt");
  }
  FILE* log_file = fopen(Name, "a+");
  fprintf(log_file, "VERIFICATION ERROR: LINE# %d in FILE: %s IN FUCTION %s\n", line, file, func);
  DumpF(thou, log_file);
  fclose(log_file);
  abort();
}/* End of 'StackVerification' function */

/* Checks errors of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] error_code.
 */
error_code StackCheckError(mk5::stack* thou)
{
  if (thou == NULL)
    return STACK_NULL;
  if (thou->goose1 == goose_sleap && thou->goose2 == goose_sleap
    && thou->data == NULL && thou->size == -1
    && thou->count == -1 && thou->error == CLEAR)
    return CLEAR;
  if (thou->goose2 != goose_silent)
    return SECOND_STRUCT_GOOSE_DEAD;
  if (thou->data == NULL)
    return DATA_NULL;
  if (thou->count < 0)
    return COUNT_NEGATIVE;
  if (thou->size < 0)
    return SIZE_NEGATIVE;
  if (!isnan(*(thou->data - 1)))
    return FIRST_DATA_GOOSE_DEAD;
  if (!isnan(*(thou->data + thou->size)))
    return SECOND_DATA_GOOSE_DEAD;

  return INITIALIZED;
}/* End of 'StackVerification' function */

/*! Init part of reallocated memory function.
  * \param[in]  poin to the part of stack             - mk5::stack* thou;
  * \param[in]  size of part stack for initialization - int size;
  * \param[out] none.
  */
void StackPartInit(mk5::stack* thou)
{
  for (int i_el = thou->count; i_el < thou->size + 1; i_el++)
    thou->data[i_el] = NAN;
}/* End of 'StackPartInit' function */

/*! Reallocated memory of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] true or false.
 */
bool StackRealloc(mk5::stack* thou)
{
  stack_verification(thou);

  if (thou->count >= thou->size)
  {
    if (thou->size == 0)
      thou->size = 1;
    else
      thou->size *= 2;

    thou->data = (double *)realloc(--thou->data, sizeof(double) * (thou->size + 2));
    thou->data++;
    if (thou->data == NULL)
      return false;

    StackPartInit(thou);
    return true;
  }

  if (4 * thou->count < thou->size)
  {
    thou->size /= 2;
    thou->data = (double *)realloc(--thou->data, sizeof(double) * (thou->size + 2));
    thou->data++;
    if (thou->data == NULL)
      return false;

    StackPartInit(thou);
    return true;
  }

  return false;
}/* End of 'StackRealloc' function */

/*! Delete and return last element of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] last element.
 */
double StackPop(mk5::stack* thou)
{
  stack_verification(thou);

  if (thou->count <= 0)
    return NAN;

  double el = thou->data[--thou->count];
  thou->data[thou->count] = NAN;
  if (4 * thou->count < thou->size)
    StackRealloc(thou);

  stack_verification(thou);
  return el;
}/* End of 'StackPop' function */

/*! Add element to stack function.
 * \param[in]  point to stack        - mk5::stack* thou;
 * \param[in]  value to add to stack - double value;
 * \param[out] none.
 */
void StackPush(mk5::stack* thou, double value)
{
  stack_verification(thou);

  if (thou->count == thou->size)
    StackRealloc(thou);

  thou->data[thou->count++] = value;

  stack_verification(thou);
}/* End of 'StackPush' function */

/*! Deinitialization of stakc function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] none.
 */
void StackDestruct(mk5::stack *thou)
{
  if (thou->error == CLEAR)
    return;

  thou->data--;

  if (thou->data != NULL)
    free(thou->data);

  thou->data = NULL;
  thou->count = -1;
  thou->size = -1;
  thou->goose1 = thou->goose2 = goose_sleap;
  thou->error = CLEAR;

}/* End of 'StackDestruct' function */

/*! Initialization of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  size of stack  - size_t size;
 * \param[out] true or false.
 */
error_code StackConstruct(mk5::stack *thou, const int size, const char *Name)
{
  if (thou->size >= 0 || thou->error != CLEAR || thou->goose2 == goose_silent
    || thou->goose1 == goose_silent || thou->count >= 0
    || thou->data != NULL)
    StackDestruct(thou);

  if (StackCheckError(thou) != CLEAR)
    return StackCheckError(thou);

  thou->data = (double *)calloc(size + 2, sizeof(double));
  thou->size = size;
  thou->count = 0;
  thou->goose1 = thou->goose2 = goose_silent;
  thou->Name = (char *)Name;

  if (thou->data == NULL)
    return DATA_NULL;
  
  thou->data[0] = thou->data[size + 1] = NAN;
  StackPartInit(thou);
  thou->data++;
  thou->error = INITIALIZED;

  return INITIALIZED;
}/* End of 'StackConstruct' function */
