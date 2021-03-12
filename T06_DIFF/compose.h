#ifndef COMPOSEH
#define COMPOSEH
/* FILE NAME   : compose.h
 * PURPOSE     : Header module of reading tree.
 * PROGRAMMER  : Matvei Kreinin.
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include "tree.h"

const int FILE_START = 3;

namespace mk5 {

  class compose {
    

    compose  operator= (const compose& copy) = delete;
    compose(const compose & copy) = delete;

    int num_lines = 0;
    mk5::line* lines = nullptr, buf = {};
  
  public:
    compose() { ; }
    ~compose() {
      if (buf.str != nullptr) {
        free(buf.str);
        free(lines);
      }

    }
    
    vertex* read_file(const char* FileName, int &size_tree);

    vertex* read_tree_str(char* expr, int &size_tree);

  private:
    vertex* read_vertex_file(int &cur_line, mk5::line* line, int &size_tree);

    vertex* read_vertex_str(mk5::line &expr, int &cur_symb, int &size_tree);
  };
}
#endif