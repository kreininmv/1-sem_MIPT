#ifndef BACKENDH
#define BACKENDH

#include "parser.h"

namespace mk5 {
  class backend {
    parser myparser;
    char *buf = nullptr;
    tree mytree;
    backend operator= (const backend& copy) = delete;
    backend(const backend& copy) = delete;
    int i_if = 0;
    int i_while = 0;

    int cur_func = GLOBAL;
 
    int write_cond(vertex* node, FILE *file);

    int write_num(vertex* node, FILE *file);

    int write_var_push(vertex* node, FILE *file);

    int write_var_pop(vertex* node, FILE *file);

    int write_vert_asm(vertex* node, FILE *file);
  public:
    backend() { ; }
    ~backend() {
      if (buf != nullptr)
        delete[] buf;
      buf = nullptr;
    }
    int write_asm(const char* name);
  };
}

#endif
