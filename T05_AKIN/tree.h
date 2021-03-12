#ifndef TREEH
#define TREEH
/* FILE NAME   : tree.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "onegin.h"



/* Debug memory allocation support */
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


#define NDEBUG
#ifndef NDEBUG
#define VERIFICATION                                                           \
{                                                                              \
  if (verification()) {                                                        \
    fprintf(stderr, "LINE: %d\nFILE:%s\nFUNC:%s"__LINE__, __FILE__, __func__); \
    assert(0);                                                                 \
  }                                                                            \
}                                                                               
#else
#define VERIFICATION ;
#endif /* _DEBUG */

#define MAX_STR 100
#define COMMAND_LEN 300
typedef char data_type;
const char ZERO = 0;

/*! Get string from consol.
 * \param[in]  point to the string - char* str
 * \param[out] none
 */
void getstr(char* str);

/*! Add data to the vertex
 * \param[in]  point of data for add - const void* data_add
 * \param[out] none.
 */
int emit(void* source, void* data_add, int size);

//left  - NO
//right - YES
enum status_vertex {
  clear              = 0,
  no_need_free_buf   = 1,
  no_need_free_stock = 2,
  need_free          = 3
};


namespace mk5 {

  class stock {
    char **array;
    size_t cur_size = 0, max_size = 0;
    size_t cur_size_arr = 0, max_size_arr = 0;

  public:
    stock(size_t size_arr, size_t num_blocks) : cur_size_arr(0), cur_size(0), max_size(num_blocks), max_size_arr(size_arr){
      array = new char *[max_size];
      array[cur_size] = new char[max_size_arr];
    }

    ~stock() {
      for (int i_block = 0; i_block < cur_size; i_block++)
        delete[] array[i_block];

      delete[] array;
    }
    stock operator= (const stock& copy) = delete;
    stock(const stock& copy) = delete;

    char* add(char* data_add, size_t size_data_add) {

      if (size_data_add + cur_size > max_size_arr) {
        cur_size_arr++;
        cur_size = 0;
        array[cur_size_arr] = new char[max_size_arr];
      }
      
      memcpy(array[cur_size_arr] + cur_size, data_add, size_data_add);

      cur_size += size_data_add;

      return (array[cur_size_arr] + cur_size - size_data_add);
    }

    int resize(void) {
      //if (max_size == 0)
      //  max_size = 256;
      //else
      //  max_size *= 2;
      //
      //char *new_bulc = new char[max_size];
      //for (int i_data = 0; i_data <= cur_size; i_data++)
      //  new_bulc[i_data] = array[i_data];
      //delete[] array;
      //
      //array = new_bulc;

      return 1;
    }
  };

  class vertex {
  public:
    vertex *left = nullptr, *right = nullptr;
    vertex* parent = nullptr;

    status_vertex status = clear;
    char *data   = nullptr;

    //std::nothrow google it
    //Зачем ввели, почему надо было её вводить, когда ей удобно применять и что она позволяет сделать если её употребить
    //std::nomeow

    vertex(data_type* data_add, status_vertex type);
 

    vertex(vertex* left_add, vertex* right_add, vertex* parent_add, data_type* data_add, status_vertex type);

    /*!Create new vertex and adding to the left
     * \param[in]  point to the vertex of adding - vertex * cur_vertex
     * \param[in]  data for adding - const data_type data
     * \param[in]  status of memory in vertex - status_vertex type
     * \param[out] none.
     */
    int push_left(data_type* data, status_vertex type);

    /*! Create new vertex and adding to the right
     * \param[in]  point to the vertex of adding - vertex* cur_vertex
     * \param[in]  data for adding               - const data_type data
     * \param[in]  status of memory in vertex    - status_vertex type
     * \param[out] none.
     */
    int push_right(data_type* data, status_vertex type);

    /*! Push to data to vertex
     * \param[in]  data for adding            - data_type* new_data
     * \param[in]  status of memory in vertex - status_vertex type
     * \param[out] none.
     */
    int push(data_type* new_data, status_vertex type);
  };

  class tree {
  public:
    vertex* root = nullptr;
    int size_tree = 0;
    char dump_name[MAX_STR] = "";

  private:
    line *lines = nullptr, buf = {};
    int num_lines = 0;

    tree operator= (const tree& copy) = delete;
    tree(const tree& copy)            = delete;

  public:
    tree() = default;
    ~tree() {
      if (root != nullptr)
        delete_tree(root);
      
      if (buf.str != nullptr) {
        free(buf.str);
        free(lines);
        buf.len = num_lines = 0;
        buf.str = nullptr;
        lines = nullptr;
      }

      root = nullptr;
      size_tree = 0;
    }

    int dump(void);

    int write(const char* FileName);
   
    int read(const char* FileName);
    
    void delete_tree(vertex* node);

    mk5::line* find_node(data_type* data);

    int init(data_type* data);
    
    int verification(void);

  private:
    int verification_node(vertex* son, vertex* father);

    int auxiliary_find_node(data_type* data, mk5::line *path, vertex* cur_root);

    int make_parent(vertex* son, vertex* father);

    int print_vertex_dump(vertex* node, int num_vertex, FILE* file);

    vertex* read_vertex(int &cur_line, mk5::line* line, vertex* parent);

    int write_vertex(vertex* node, int indent_size, FILE* file);
  };
}
#endif