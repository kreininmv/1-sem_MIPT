#ifndef MIDDLEENDH
#define MIDDLEENDH

#include "parser.h"

namespace mk5 {
  class middleend {
    parser myparser;
    char *buf = nullptr;
    tree mytree;
    int simplify_vertex(vertex* node);
    vertex* diff(vertex* node,  variable &var_diff);

    int loop_diff(void);
    
  private:
    middleend operator= (const middleend& copy) = delete;
    middleend(const middleend& copy) = delete;
  public:
    middleend() { ; }
    ~middleend() {
      if (buf != nullptr)
        delete[] buf;
      buf = nullptr;
    }
    int simplify_write(const char* name);
  };
}

#endif
