#ifndef LANGH
#define LANGH

#include "token.h"
#include "parser.h"

namespace mk5 {
  
  class frontend {
    char *buf = nullptr;
    int buf_len = 0;
    token* tok = nullptr;
    int tok_size = 0;
    parser myparser;
    tree mytree;

  public:
    frontend(void) {};
    
    ~frontend(void) {
      if (tok != nullptr)
        delete[] tok;
      if (buf != nullptr)
        delete[] buf;

      tok_size = buf_len  = 0;
      tok = nullptr;
      buf = nullptr;
    }

    int read_file(const char* name);

    int build_tree(const char* name) {
      mytree.root = myparser.GetG(tok, tok_size);
      strcat(mytree.dump_name, "TEST");
      
      char file_name[MAX_STR] = {"..\\PROGRAMS\\"};
      strncat(file_name, name, strlen(name) - strlen(".txt"));
      strcat(file_name, ".tree");
      mytree.write(file_name);
      return 1;
    }

  private:
    frontend operator= (const frontend& copy) = delete;
    frontend(const frontend& copy) = delete;
  
  };
}

#endif