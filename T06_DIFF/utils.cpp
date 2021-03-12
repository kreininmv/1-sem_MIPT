/* FILE NAME   : utils.cpp
 * PURPOSE     : Module of auxiliary.
 * PROGRAMMER  : Matvei Kreinin.
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include "parser.h"

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

//Just need to save it 
//class calculator {
//
//  char *str = nullptr;
//  int len_str = 0;
//
//public:
//  int GetG(const char *s) {
//    len_str = strlen(s);
//    str = new char[len_str];
//
//    int i_sym = 0, cur_sym_str = 0;
//    while (s[i_sym] != 0)
//      if (s[i_sym] != ' ')
//        str[cur_sym_str++] = s[i_sym++];
//      else
//        i_sym++;
//
//    str[cur_sym_str] = 0;
//    len_str = cur_sym_str;
//
//    int val = GetE();
//
//    if (*str != 0)
//      ;  //ERROR("doesn't end in zero");
//
//    return val;
//  }
//
//private:
//  int GetE(void) {
//    char *unary_oper = str;
//    int val = GetT();
//
//    if (unary_oper == str)
//      val = 0;
//
//    while (*str == '+' || *str == '-') {
//      char oper = *str;
//      str++;
//      int val2 = GetT();
//
//      if (oper == '+')
//        val += val2;
//      else
//        val -= val2;
//    }
//
//    return val;
//  }
//
//  int GetT(void) {
//
//    int val = GetW();
//
//    while (*str == '*' || *str == '/') {
//
//      char oper = *str;
//      str++;
//      int val2 = GetW();
//
//      if (oper == '*')
//        val *= val2;
//      else
//        val /= val2;
//
//    }
//
//    return val;
//  }
//
//  int GetW(void) {
//    int val = GetP();
//
//    while (*str == '^') {
//
//      str++;
//      int val2 = GetP();
//
//      val = pow(val, val2);
//    }
//
//    return val;
//  }
//
//  int GetP(void) {
//    if (*str == '(') {
//      str++;
//      int val = GetE();
//
//      if (*str != ')') {
//        ;//ERROR;
//      }
//      str++;
//
//      return val;
//    }
//    else
//      return GetN();
//  }
//
//  int GetN(void) {
//
//    int val = 0;
//    char *p = str;
//
//    while ('0' <= *str && *str <= '9') {
//      val = val * 10 + *str - '0';
//      str++;
//    }
//    if (str == p)
//      ;//ERROR;
//    return val;
//  }
//
//
//};