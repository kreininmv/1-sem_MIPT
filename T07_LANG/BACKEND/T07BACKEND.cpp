#include "backend.h"

int main(int argc, char *argv[])
{
  try {
    mk5::backend bcknd;
    //bcknd.write_asm("mytest.tree");
    if (argc == 2)
      bcknd.write_asm(argv[1]);
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
  printf("BACKEND\n");
  return 0;
}