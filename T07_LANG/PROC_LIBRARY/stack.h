#ifndef STACKH
#define STACKH
/* FILE NAME   : STACK.h
 * PURPOSE     : Header T02STACK module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 06.10.2020.
 * NOTE        : None.
 */

#include <windows.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



static  long goose_silent = 0x1E1E1E;
static  long goose_sleap = 0xEFEFEF;
#define MAX_SIZE             50

//Сменить название case_of_switch
#define CASE_OF_SWITCH(enum_constant, stack, log_file)                                                                       \
  case enum_constant:                                                                                                        \
    fprintf(log_file, "Stack (%s) point to data[%d]; point to stack[%d];\n", #enum_constant, (int)stack->data, (int)stack);  \
    break;                                                                                                                   \

#define FPRINTF_ENUM(enum_constant, log_file, text)       \
    fprintf(log_file, "%s %s\n", text, #enum_constant);   \

#ifndef DEBUG
#define  assert_stack(stack)                            \
                 assert(stack);                           \
                 assert(stack->data);                     \
                 assert(stack->count >= 0);               \
                 assert(stack->size >= 0);                \
                 assert(stack->count <= stack->size);     \
                 assert(stack->goose1 == goose_silent);   \
                 assert(stack->goose2 == goose_silent);   \

#elif
#define  assert_stack(stack);
#endif

#define stack_verification(stack) StackVerification(stack, __LINE__, __FILE__, __func__);
#define StackConstr(stack, size) StackConstruct(&stack, size, #stack);
#define Dump(stack) DumpName(&stack, #stack)

enum error_code
{
  INITIALIZED = 0,
  STACK_NULL = 1,
  DATA_NULL = 2,
  COUNT_NEGATIVE = 3,
  SIZE_NEGATIVE = 4,
  COUNT_BIGGER_SIZE = 5,
  STACK_EQUAL_DATA = 6,
  CLEAR = 7,
  FIRST_STRUCT_GOOSE_DEAD = 8,
  SECOND_STRUCT_GOOSE_DEAD = 9,
  FIRST_DATA_GOOSE_DEAD = 10,
  SECOND_DATA_GOOSE_DEAD = 11
};

namespace mk5 //IFK namespace
{
  /* Structure of stack */
  struct stack
  {
  public:
    long goose1 = goose_sleap;
    int count = -1;
    int size = -1;
    double *data = NULL;
    error_code error = CLEAR;
    char *Name = NULL;
    long goose2 = goose_sleap;
  };
}

/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpName(mk5::stack *thou, const char *Name);

/* Verification of stack function.
 * \param[in]  point to stack                - mk5::stack* thou;
 * \param[in]  line from where it called     - int line;
 * \param[in]  file from where it called     - char* file;
 * \param[in]  function from where it called - char* func;
 * \param[out] none.
 */
void StackVerification(mk5::stack* thou, int line, const char *file, const char *func);

/* Checks errors of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] error_code.
 */
error_code StackCheckError(mk5::stack* thou);

/* Print all information about stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void StackPrint(mk5::stack* thou, FILE* log_file);

/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpF(mk5::stack *thou, FILE* log_file);

/*! Init part of reallocated memory function.
  * \param[in]  poin to the part of stack             - mk5::stack* thou;
  * \param[in]  size of part stack for initialization - int size;
  * \param[out] none.
  */
void StackPartInit(mk5::stack* thou);

/*! Reallocated memory of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] none.
 */
bool StackRealloc(mk5::stack* thou);

/*! Delete and return last element of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] last element.
 */
double StackPop(mk5::stack* thou);

/*! Add element to stack function.
 * \param[in]  point to stack        - mk5::stack* thou;
 * \param[in]  value to add to stack - double value;
 * \param[out] none.
 */
void StackPush(mk5::stack* thou, double value);

/*! Deinitialization of stakc function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] none.
 */
void StackDestruct(mk5::stack *thou);

/*! Initialization of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  size of stack  - int size;
 * \param[out] true or false.
 */
error_code StackConstruct(mk5::stack *thou, const int size, const char* Name);

/* Print all information about stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void StackPrint(mk5::stack* thou, FILE* log_file);

/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpName(mk5::stack *thou, const char *Name);

/* Notification about error with stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  point to file  - FILE* log_file;
 * \param[out] none.
 */
void DumpF(mk5::stack *thou, FILE* log_file);

/* Verification of stack function.
 * \param[in]  point to stack                - mk5::stack* thou;
 * \param[in]  line from where it called     - int line;
 * \param[in]  file from where it called     - char* file;
 * \param[in]  function from where it called - char* func;
 * \param[out] none.
 */
void StackVerification(mk5::stack* thou, int line, const char* file, const char* func);

/* Checks errors of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] error_code.
 */
error_code StackCheckError(mk5::stack* thou);

/*! Init part of reallocated memory function.
  * \param[in]  poin to the part of stack             - mk5::stack* thou;
  * \param[in]  size of part stack for initialization - int size;
  * \param[out] none.
  */
void StackPartInit(mk5::stack* thou);

/*! Reallocated memory of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] true or false.
 */
bool StackRealloc(mk5::stack* thou);

/*! Delete and return last element of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] last element.
 */
double StackPop(mk5::stack* thou);

/*! Add element to stack function.
 * \param[in]  point to stack        - mk5::stack* thou;
 * \param[in]  value to add to stack - double value;
 * \param[out] none.
 */
void StackPush(mk5::stack* thou, double value);

/*! Deinitialization of stakc function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[out] none.
 */
void StackDestruct(mk5::stack *thou);

/*! Initialization of stack function.
 * \param[in]  point to stack - mk5::stack* thou;
 * \param[in]  size of stack  - size_t size;
 * \param[out] true or false.
 */
error_code StackConstruct(mk5::stack *thou, const int size, const char *Name);

#endif