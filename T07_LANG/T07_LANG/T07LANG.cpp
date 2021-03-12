/* FILE NAME   : T07LANG.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 14.12.2020.
 * NOTE        : None.
 */

#include "lang.h"

int main(int argc, char *argv[]) {
  
  try {
    mk5::frontend frnt;
    //frnt.read_file("mytest.txt");
    //frnt.build_tree("mytest.txt");
    if (argc == 2) {
      frnt.read_file(argv[1]);
      frnt.build_tree(argv[1]);
    }
    else      
      printf("DOLBAEB");
  }
  catch (syntax_error &error) {
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
  printf("FRONTEND\n");
  return 0;
}/* End of 'main' function */