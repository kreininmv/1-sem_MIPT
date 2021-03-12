/* FILE NAME   : T06DIFF.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include <iostream>
#include "diff.h"

int main(void) {
  
  srand(500 - 30*30 + 1);
  try {
    mk5::differenciator matan;
    char command[] = {"cos(x^5) - ln(x) + sin(x)*x^x + sin(cos(x))"};
    char myvar[] = {"x"};
    matan.love_math(command, myvar);
  }
  catch(syntax_error &error) {
    fprintf(stderr, "ACHTUNG!!!\n\n"
                    "FILE: %s\n"
                    "FUNCTION: %s\n"
                    "LINE: %d\n"
                    "ERROR TYPE: %s\n"
                    "SYNTAX ERROR IN: %s",
                    error.file_name, error.function_name, error.num_line, error.type_error, error.descr);
  }
  catch (std::exception &err) {
    fprintf(stderr, "PROBLEM: %s\n", err.what());
  }
  catch (...) {
    fprintf(stderr, "I don't know what is it\n");
  }
  

  return 0;
}/* End of 'main' function */