#ifndef LANGH
#define LANGH

#include "token.h"
#include "parser.h"

const int TAB = 2;

namespace mk5 {

  class rev_frontend {
    char *buf = nullptr;
    int buf_len = 0;
    parser myparser;
    tree mytree;
    int cur_func = GLOBAL;
    
    int indent_size = 0;


    int build_code(vertex* node, FILE* file);


    int write_pol_op(vertex* node, FILE* file);
    
    int write_var(vertex* node, FILE* file);

    int write_param(vertex* node, FILE *file);

    int write_oper(vertex* node, FILE* file);

    int write_cmp(vertex* node, FILE* file);

    int write_concatenation(vertex* node, FILE* file);

    int write_function(vertex* node, FILE* file);

    int write_function_declaration(vertex* node, FILE* file);

    int write_std_func(vertex* node, FILE* file);

  public:
    rev_frontend(void) {};

    ~rev_frontend(void) {
      if (buf != nullptr)
        delete[] buf;
      buf = nullptr;
    }

    int read_tree(const char* name);

  private:
    rev_frontend operator= (const rev_frontend& copy) = delete;
    rev_frontend(const rev_frontend& copy) = delete;

  };
}

#endif