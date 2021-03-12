#pragma once
#ifndef TREEH
#define TREEH
/* FILE NAME   : tree.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin.
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "onegin.h"

#define NDEBUG

/* Debug memory allo cation support */
#ifndef NDEBUG 
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h> 

#define SetDbgMemHooks()                                           \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))      

static class __Dummy
{
public:
  /* Class constructor */
  __Dummy(VOID)
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __ooppss;

#endif /* _DEBUG */ 

#ifndef NDEBUG
#  ifdef _CRTDBG_MAP_ALLOC 
#    define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#  endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

const int MAX_STR = 130;
const int COMMAND_LEN = 500;
typedef char data_type;
const char ZERO = 0;

/*! Get string from consol.
 * \param[in]  point to the string - char* str
 * \param[out] none
 */
void getstr(char* str);

enum type_vertex {
  nill = 0,
  VAR  = 1,
  NUM  = 2,
  OPER = 3,
  FUNC = 4
};

enum operation {
#define OPERATOR(name, num, diff, eval, tex, diff_tex) name = num,
#include "operations.h"
  LAST_OPER
#undef OPERATOR
};

enum function {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) my_##name = num,
#include "functions.h"
  LAST_FUNC
#undef FUNCTION
};

const int NO_VARIABLE = -1;

const int NO_PARENT = 0;
const int POW_PARENT = '^';
const int NO_POW_PARENT = 1;


const int MAX_VAR = 100;


namespace mk5 {

  class vertex {
  public:
    vertex *left         = nullptr, 
           *right        = nullptr;

    type_vertex  type   = nill;
    double value         = NAN;
   
    vertex() = default;

    vertex(const vertex* copy){
      value = copy->value;
      type = copy->type;

      if (copy->left != nullptr)
        left = new vertex(copy->left);

      if (copy->right != nullptr)
        right = new vertex(copy->right);
    }

    vertex(vertex* left_add, vertex* right_add, double value_add, type_vertex type_vertex_add ) 
      : left(left_add), right(right_add), value(value_add), type(type_vertex_add) {
      
    }
  };

  class tree {
  public:
    vertex* root = nullptr;
    int size_tree = 0;
    char dump_name[MAX_STR] = "";

  private:
    tree operator= (const tree& copy) = delete;
    tree(const tree& copy) = delete;

  public:
    tree() = default;
    ~tree() {
      if (root != nullptr)
        delete_tree(root);

      root = nullptr;
      size_tree = 0;
    }

    /*! Make picture with a graphviz.
     * \param[in]  none.
     * \param[out] none.
     */
    int dump(void);

    /*! Write tree in the file.
     * \param[in]  name of file        - const char* FileName
     * \param[out] none
     */
    int write(const char* FileName);

    /*! Build tree by the information in the file
     * \param[in]  name of file - const char* FileName
     * \param[out] none
     */
    int read(const char* FileName);

    /*! Delete all vertexes in the tree
     * \param[in]  point to the root - vertex* node
     * \param[out] none
     */
    void delete_tree(vertex* node);
  private:
    /*! Print information of cur node.
     * \param[in]  point to node    - vetex* node
     * \param[in]  num of vertexes  - int num_vertex
     * \param[in]  file for writing - FILE* file
     * \param[out] none.
     */
    int print_vertex_dump(vertex* node, int num_vertex, FILE* file);

    /*! Write cur node in the file
     * \param[in]  point to the node - vertex* node
     * \param[in]  num of ' '        - int indent_size
     * \param[in]  file for writing  - FILE* file
     * \param[out] none
     */
    int write_vertex(vertex* node, int indent_size, FILE* file);
  };
}
#endif