#ifndef LISTH
#define LISTH
/* FILE NAME   : list.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.11.2020.
 * NOTE        : None.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

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

#ifndef NDEBUG
#define VERIFICATION {                                                                                                                 \
                       int count_free = 0, count_busy = 0;                                                                             \
                       print_verification(verification(count_free, count_busy), __FILE__, __LINE__, __func__, count_free, count_busy); \
                     }
#else
#define VERIFICATION ;
#endif

typedef double data_type;
const data_type POISON = NAN;

enum code_error {
  all_ok           = 0,
  head_tail        = 1,
  next_of_prev     = 2,
  prev_of_next     = 3,
  size_of_list     = 4,
  size_of_all_list = 5
};

namespace mk5 {
  
  struct vertex {
    data_type data = NAN;
    int next = -1, prev = -1;
  };
  
  class list {
  private:
    vertex *vertex_arr = nullptr;
    int capacity = -1, size = -1;
    int head = -1, tail = -1, free = -1;
    
  public:
    /* Class constructor */
    list(size_t size_list) : 
      vertex_arr(new vertex[size_list + 1]), 
      capacity(size_list) {

      clear();
    }/* End of 'list' constructor */

    /* Class destructor */
    ~list() {
      //Залить ядом перед освобождением.
      if (vertex_arr != nullptr) {
        delete[] vertex_arr;
        vertex_arr = nullptr;
      }

      capacity = head = tail = 0;
    }/* End of '`list' constructor */

    /*! Checking list for correctness
     * \param[in]  variable for counting free vertexes - int& count_free
     * \param[in]  variavle for counting busy vertexes - int& couny_busy
     * \param[out] code eror
     */
    int verification(int& count_free, int& count_busy);
    
    /*! Make picture with a graphviz.
     * \param[in]  none.
     * \param[out] none.
     */
    int dump(void);

    /* Clear all vertexes in list (doesn't free memory).
     * \param[in]  none.
     * \param[out] none.
     */
    void clear(void);

    /* Sort all vertexes by his logical index, but without memory allocation.
     * \param[in]  none.
     * \param[out] none.
     */
    int sort_list_memory(void);

    /* Sort all vertexes by his logical index
     * \param[in]  none.
     * \param[out] none.
     */
    int sort_list(void);

    /*! Add to front of list.
     * \param[in]  value - const data_type value
     * \param[out] none.
     */
    int push_front(const data_type value);

    /*! Add element to end of list.
     * \param[in]  value - const data_type value
     * \param[out] none.
    */
    int push_back(const data_type value);
    
    /*! Delete from front of list.
     * \param[in]  none.
     * \param[out] none.
     */
    int pop_back(void);

    /*! Delete element from end of list.
     * \param[in]  none.
     * \param[out] none.
    */
    int pop_front(void);

    /*! Getting physical index of tail.
     * \param[in]  none.
     * \param[out] adress of tail.
     */
    int end(void);

    /*! Getting physical index of head.
     * \param[in]  none.
     * \param[out] adress of head.
     */
    int begin(void);

    /*! Get physical adress of element that stayed before.
     * \param[in]  pos - const size_t pos
     * \param[out] adress
     */
    int rback(const size_t pos);

    /*! Get physical adress of element that stayed after.
     * \param[in]  pos - const size_t pos
     * \param[out] adress
     */
    int rfront(const size_t pos);

    /*! Get data by logical position in list.
     * \param[in]  logical position - int pos
     * \param[out] data
     */
    data_type get_data(int pos);

    /*! Find logical position of value in list.
     * \param[in]  value - data_type value
     * \param[out]
     */
    int get_index(const data_type value);

    /*! Insert value before of physical position.
     * \param[in]  pos of insert - const int pos
     * \param[in]  value         - const data_type value
     * \param[out] return of newposition
     */
    int emplace_back(const int pos, const data_type value);

    /*! Insert value after of physical position.
     * \param[in]  pos of insert - const int pos
     * \param[in]  value         - const data_type value
     * \param[out] return of new position
     */
    int emplace_front(const int pos, const data_type value);

    /*! Insert value after of physical position.
     * \param[in]  pos of insert - const int pos
     * \param[out] return of new position
     */
    int pop_pos_front(const int pos);
    
    /*! Delete value after of physical position.
     * \param[in]  pos of insert - const int pos
     * \param[out] return of new position
     */
    int pop_pos_back(const int pos);

    /*! Delete cuurent value of physical position.
     * \param[in]  pos of delete - const int pos
     * \param[out] code error
     */
    int pop_pos(const int pos);

    private:

    /* Allocate some memory for the list.
     * \param[in]  none.
     * \param[out] none.
     */
    int alloc_memory(void);

    /* Check the value for correcntess.
     * \param[in]  value - const data_type& value
     * \param[out] true or false
     */
    int IsValue(const data_type& value);
    
    /*! Add vertex to list of free vertexes.
     * \param[in]  physical position - const int pos
     * \param[out] none.
     */
    int free_vertex(const int pos);

    /* Add data to the vertex.
     * \param[in]  data for add      - const void* data
     * \param[in]  physical position - const int pos
     * \param[in]  size of data      - const int size_of
     * \param[out] none.
     */
    int emit(const void* data, const int pos, const int size_of);

    /*! Print code error and what it is problem.
     * \param[in]  code of error                     - int code_error
     * \param[in]  file where triggered verification - const char* file
     * \param[in]  line where triggered verification - const int line
     * \param[in]  num of free vertexes              - const int count_free
     * \param[in]  num of busy vertexes              - const int count_busy
     * \param[out] none
     */
    void print_verification(const int code_error, const char* file, const int line, const char* func, const int count_free, const int count_busy);
  };
}

#endif