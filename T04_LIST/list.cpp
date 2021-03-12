/* FILE NAME   : list.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.11.2020.
 * NOTE        : None.
 */

#include "list.h"

/* Check the value for correcntess.
 * \brief
 * \version
 * \authors
 * \param[in]  value - const data_type& value
 * \param[out] true or false
 */
int mk5::list::IsValue(const data_type& value) {
  
  return isnan(value);
}/* End of 'IsValue' function */

/* Add data to the vertex.
 * \param[in]  data for add      - const void* data
 * \param[in]  physical position - const int pos
 * \param[in]  size of data      - const int size_of
 * \param[out] none.
 */
int mk5::list::emit(const void* data, const int pos, const int size_of) {

  char *element = (char *)data;
  memcpy(&(vertex_arr[pos].data), element, size_of);

  return 1;
}/* End of 'emit' function */

/*! Add vertex to list of free vertexes.
 * \param[in]  physical position - const int pos
 * \param[out] none.
 */
int mk5::list::free_vertex(const int pos){
  
  emit(&POISON, pos, sizeof(POISON));
  vertex_arr[pos].prev = -1;

  if (free == -1)
    vertex_arr[pos].next = -1;
  else
    vertex_arr[pos].next = free;
  
  free = pos;

  return 1;
}/* End of 'free_vertex' function */

/* Allocate some memory for the list.
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::list::alloc_memory(void)
{
  if (free != -1)
    return 0;
  if (capacity <= 0)
    return 0;

  vertex *arr = new vertex[capacity * 2 + 1];
  for (int i_vertex = capacity + 1; i_vertex < capacity * 2 + 1; i_vertex++) {
    arr[i_vertex].next = i_vertex + 1;
    arr[i_vertex].prev = -1;
    arr[i_vertex].data = POISON;
  }
  arr[capacity * 2].next = -1;
  free = capacity + 1;
  
  memcpy(arr, vertex_arr, sizeof(vertex) * (capacity + 1));
  delete[] vertex_arr;
  vertex_arr = arr;
  capacity *= 2;

  return 1;
}/* End of 'alloc_memory' function */

/*! Make picture with a graphviz.
 * \param[in]  none
 * \param[out] none
*/
int mk5::list::dump(void) {
  
  FILE *file = fopen("gr_all.dot", "wb");
  
  if (file == NULL)
    return 0;

  fprintf(file, "digraph G{\n");
  fprintf(file, "node [color=\"#000000\", shape=record];\n struct0 [style=\"filled\", fillcolor=\"#FF019A\"];\n  ");
  fprintf(file, "struct0 [label=\"<f1> ZERO\"];\n  ");

  fprintf(file, "node [color=\"#000000\", shape=record];\n  structzero [style=\"filled\", fillcolor=\"#FF019A\"];\n  ");
  fprintf(file, "structzero [label=\"<f1> INVALID_ZERO\"];\n  ");
  fprintf(file, "edge[color = \"white\"];\n");
  for (int i_vert = 1; i_vert <= capacity; i_vert++) {

    if (vertex_arr[i_vert].prev != -1 && size > 1 && (0
      || (i_vert != vertex_arr[vertex_arr[i_vert].prev].next && i_vert != head)
      || (i_vert != vertex_arr[vertex_arr[i_vert].next].prev && i_vert != tail) 
      || (i_vert == head && i_vert != vertex_arr[vertex_arr[i_vert].next].prev)
      || (i_vert == tail && i_vert != vertex_arr[vertex_arr[i_vert].prev].next)
      )) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"RED\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ",
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }
    else if (i_vert == head || i_vert == tail) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"#675BF4\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ",
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }
    else if (vertex_arr[i_vert].next != -1 && vertex_arr[i_vert].prev != -1) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"#89F09B\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ", 
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }  
    else  {
      fprintf(file, "node[color=\"#000000\", shape=record];\n  struct%i[style=\"filled\", fillcolor=\"#F3F372\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ",
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }

    if (i_vert + 1 < capacity && vertex_arr[vertex_arr[i_vert].next].prev != -1)
      fprintf(file, "struct%i -> struct%i;\n  ", i_vert, i_vert + 1);
    else if (i_vert + 1 < capacity && vertex_arr[i_vert].prev == -1)
      fprintf(file, "struct%i -> struct%i;\n  ", i_vert, i_vert + 1);
  }

  fprintf(file, "edge[color = \"black\"];\n");
  
  for (int i_vert = 1; i_vert <= capacity; i_vert++) {
    
    if (vertex_arr[i_vert].next == -1)
      fprintf(file, "struct%i:f2 -> structzero;\n  ", i_vert);
    else
      fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", i_vert, vertex_arr[i_vert].next);
    if (vertex_arr[i_vert].prev == -1)
      fprintf(file, "struct%i:f0 -> structzero;\n  ", i_vert);
    else
      fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", i_vert, vertex_arr[i_vert].prev);
  }
  
  fprintf(file, "}");
  fclose(file);
  if (system("C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\Graphviz\\bin\\dot.exe -Tpdf"
            " C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T04LIST\\T04LIST\\gr_all.dot "
            "> C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T04LIST\\T04LIST\\Graphs\\all_out.pdf"))
    assert(0);

  
  file = fopen("gr_list.dot", "wb");

  if (file == NULL)
    return 0;

  fprintf(file, "digraph G{\n  rankdir=LR;\n  node [shape=record];\n  ");
  fprintf(file, "node [color=\"#000000\", shape=record];\n  struct0 [style=\"filled\", fillcolor=\"#FF019A\"];\n  ");
  fprintf(file, "struct0 [label=\"<f1> ZERO\"];\n  ");

  for (int i_vert = 1; i_vert <= capacity; i_vert++) {

    if (vertex_arr[i_vert].prev != -1 && size > 1 && (0
      || (i_vert != vertex_arr[vertex_arr[i_vert].prev].next && i_vert != head)
      || (i_vert != vertex_arr[vertex_arr[i_vert].next].prev && i_vert != tail)
      || (i_vert == head && i_vert != vertex_arr[vertex_arr[i_vert].next].prev)
      || (i_vert == tail && i_vert != vertex_arr[vertex_arr[i_vert].prev].next)
      )) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"RED\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ",
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }
    else if (i_vert == head || i_vert == tail) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"#675BF4\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ",
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }
    else if (vertex_arr[i_vert].next != -1 && vertex_arr[i_vert].prev != -1) {
      fprintf(file, "node [color=\"#000000\", shape=record];\n  struct%i [style=\"filled\", fillcolor=\"#89F09B\"];\n  ", i_vert);
      fprintf(file, "struct%i [label=\"<f0> prev (%i)|<f1> num (%i)|<f2> next (%i)|<f3> data (%lf)\"];\n  ", 
        i_vert, vertex_arr[i_vert].prev, i_vert, vertex_arr[i_vert].next, vertex_arr[i_vert].data);
    }

    if (vertex_arr[i_vert].next != -1 && vertex_arr[i_vert].prev != -1) {
      fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", i_vert, vertex_arr[i_vert].next);
      fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", i_vert, vertex_arr[i_vert].prev);
    }
  }
  fprintf(file, "}");

  fclose(file);
  if (system("C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\Graphviz\\bin\\dot.exe -Tpdf " 
             " C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T04LIST\\T04LIST\\gr_list.dot"
             " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T04LIST\\T04LIST\\Graphs\\list_out.pdf"))
    assert(0);
 
  return 1;
}/* End of 'dump' function*/

/*! Print code error and what it is problem.
 * \param[in]  code of error                     - int code_error
 * \param[in]  file where triggered verification - const char* file
 * \param[in]  line where triggered verification - const int line
 * \param[in]  num of free vertexes              - const int count_free
 * \param[in]  num of busy vertexes              - const int count_busy
 * \param[out] none
 */
void mk5::list::print_verification(const int code_error, const char* file, const int line, const char* func, const int count_free, const int count_busy) {
 
  
  switch (code_error) {
  case head_tail:
    fprintf(stderr, "Something is going wrong and your head or tail doesn't point on zero element.\n"
      "View the picture of graph.\n");

    break;
  case next_of_prev:
    fprintf(stderr, "Your graph is broken. (next of prev != cur).\n"
      "You need to check more information in picture\n");
    
    break;
  case prev_of_next:
    fprintf(stderr, "Your graph is broken.(prev of next != cur).\n"
      "You need to check more information in picture\n");
    
    break;
  case size_of_list:
    fprintf(stderr, "You have problems with a size of graph!\n"
      "Size (%i), size that counted by me (%i)\n", size, count_busy);
    
    break;
  case size_of_all_list:
    fprintf(stderr, "Your graph is broken. (count of free and busy elements not equal capacity).\n"
      "You need to check more information in picture\n"
      "Size (%i), size that counted by me (%i)", count_free + count_busy, size);

    break;
  default:
    return;
  }
  fprintf(stderr, "FILE: %s\nLINE: %i\nFUNC: %s\n", file, line, func);
  dump();
  getchar();
  abort();

}/* End of 'print_ferivication*/

/*! Checking list for correctness
 * \param[in]  variable for counting free vertexes - int& count_free
 * \param[in]  variavle for counting busy vertexes - int& couny_busy
 * \param[out] code eror
 */
int mk5::list::verification(int& count_free, int& count_busy) {
  
  if (vertex_arr[head].prev != 0 || vertex_arr[tail].next != 0)
    return head_tail;
  
  int cur_vertex = head;
  while (cur_vertex != 0) {
    if (cur_vertex != vertex_arr[vertex_arr[cur_vertex].prev].next && cur_vertex != head && size > 1) 
      return next_of_prev;
    if (cur_vertex != vertex_arr[vertex_arr[cur_vertex].next].prev && cur_vertex != tail && size > 1) 
      return prev_of_next;

    count_busy++;
    cur_vertex = vertex_arr[cur_vertex].next;
  }

  if (count_busy != size && tail != head)
    return size_of_list;

  int i_prev = 1;
  for (; i_prev <= capacity; i_prev++)
    count_free += (vertex_arr[i_prev].prev == -1) ? 1 : 0;

  if ((count_free + count_busy != capacity) && head != tail)
    size_of_all_list;

  return all_ok;
}/* End of 'verification' function */

/* Clear all vertexes in list (doesn't free memory).
 * \param[in]  none.
 * \param[out] none.
 */
void mk5::list::clear(void) {
  
  vertex_arr[0].data = vertex_arr[0].next = vertex_arr[0].prev = 0;
  
  size = 0 ;
  head = tail = 1;

  if (capacity > 1)
    free = 2;

  vertex_arr[head].next = vertex_arr[head].prev = 0;
  
  for (int i_free = 2; i_free <= capacity; i_free++) {
    vertex_arr[i_free].next = i_free + 1;
    emit(&POISON, i_free, sizeof(POISON));
  }

  vertex_arr[capacity].next = -1;
  
  VERIFICATION;
}/* End of 'clear' function */

template <typename type_data>
/* Comparing two vertexes, that function needed for sorting elements by indexes.
 * \param[in]  first vertex  - const mk5::vertex& first
 * \param[in]  second vertex - const mk5::vertex& second
 * \param[out] return -1 if (first > second)
 *             return  1 if (first < second)
 */
int compare_vertex(const type_data* first, const type_data* second) {
  
  if (second->prev == -1 && first->prev == -1)
    return 0;

  if (second->prev == -1)
    return -1;

  if (first->prev == -1)
    return 1;

  return first->next - second->next;
}/* End of 'compare_vertex' function */

template <typename type_data>
/*! Swap two elements function.
 * \param[in]  point to the first element  - type_data *first;
 * \param[in]  point to the second element - type_data *second;
 * \param[out] none.
 */
void Swap(type_data *first, type_data *second) {
  type_data tmp = *first;
  *first = *second;
  *second = tmp;
}/* End of 'Swap' function */

template <typename type_data>
/*! Sort array of string function.
 * \param[in]  array                        - type_data *Arr;
 * \param[in]  size of array                - int N;
 * \param[in]  point for comparing function - int (*Compare)(char *, char *);
 * \param[out] none.
 */
void QuickSort(type_data *Arr, size_t num_of_elements, int(*Compare)(const type_data *, const type_data *)) {
  size_t left = 0, right = num_of_elements - 1;
  type_data x = Arr[num_of_elements / 2];

  if (num_of_elements < 2)
    return;

  while (left <= right)
  {
    while (Compare(&x, &Arr[left]) > 0)
      left++;
    while (Compare(&Arr[right], &x) > 0)
      right--;

    if (left <= right)
    {
      if (left != right)
        Swap(Arr + left, Arr + right);
      left++;
      right--;
    }
  }

  QuickSort(Arr, right + 1, Compare);
  QuickSort(Arr + left, num_of_elements - left, Compare);
}/* End of 'QuickSort' function */

/* Sort all vertexes by his logical index, but without memory allocation.
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::list::sort_list_memory(void) {

  VERIFICATION;

  for (int count_vertexes = size, cur_vertex = tail; cur_vertex != 0; cur_vertex = vertex_arr[cur_vertex].prev, count_vertexes--)
    vertex_arr[cur_vertex].next = count_vertexes;

  QuickSort<vertex>(vertex_arr + 1, capacity, compare_vertex<vertex>);

  head = 1;
  tail = size;
  if (tail != capacity)
    free = size + 1;
  else
    free = -1;

 
  for (int cur_vertex = head; cur_vertex <= tail; cur_vertex++) {
    vertex_arr[cur_vertex].next = cur_vertex + 1;
    vertex_arr[cur_vertex].prev = cur_vertex - 1;
  }
  vertex_arr[tail].next = 0;

  for (int cur_vertex = free; cur_vertex <= capacity; cur_vertex++) {
    vertex_arr[cur_vertex].next = cur_vertex + 1;
    vertex_arr[cur_vertex].prev = -1;
  }
  vertex_arr[capacity].next = -1;

  VERIFICATION;

  return 1;
}/* End of 'sort_list_memory' function */

/* Sort all vertexes by his logical index
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::list::sort_list(void) {

  VERIFICATION;

  mk5::vertex *arr = new vertex[capacity + 1];
 
  arr[0] = vertex_arr[0];
  
  for (int i = 1; i <= capacity; i++) {
    arr[i].next = i + 1;
    arr[i].prev = -1;
    arr[i].data = POISON;
  }
  arr[capacity].next = arr[capacity].prev = -1;

  int i_phys = 0, cur_vertex = head;
  while (cur_vertex != 0) {
    arr[++i_phys].prev = i_phys - 1;
    arr[i_phys].data = vertex_arr[cur_vertex].data;
    
    cur_vertex = vertex_arr[cur_vertex].next;
  }

  head = 1;
  tail = size;
  arr[head].prev = 0;
  arr[tail].prev = tail - 1;
  arr[tail].next = 0;
  
  delete[] vertex_arr;
  vertex_arr = arr;
  
  VERIFICATION;
  
  return TRUE;
}/* End of 'sort_list' function */

/*! Add to front of list function.
 * \param[in]  value - const data_type value
 * \param[out] none
 */
int mk5::list::push_front(const data_type value) {
  
  if (IsValue(value))
    return 0;

  VERIFICATION;
  if (size == 0) {
    emit(&value, head, sizeof(value));
    
    size++;

    return head;
  }
  
  if (free == -1)
    alloc_memory();

  int newpos = free;
  free = vertex_arr[free].next;
  
  vertex_arr[newpos].prev = 0;
  vertex_arr[newpos].next = head;
  vertex_arr[head].prev = newpos;
  
  emit(&value, newpos, sizeof(value));
  size++;

  head = newpos;

  VERIFICATION;
  return newpos;
} /* End of 'push_front' function */

/*! Add element to end of list function
 * \param[in]  value - const data_type value
 * \param[out] none.
*/
int mk5::list::push_back(const data_type value) {
  
  if (IsValue(value))
    return 0;

  VERIFICATION;
  if (size == 0) {
    emit(&value, tail, sizeof(value));
    size++;

    return tail;
  }

  if (free == -1)
    alloc_memory();

  int newpos = free;
  free = vertex_arr[free].next;
  
  vertex_arr[newpos].next = 0;
  vertex_arr[newpos].prev = tail;
  vertex_arr[tail].next = newpos;
  
  emit(&value, newpos, sizeof(value));
  size++;

  tail = newpos;

  VERIFICATION;

  return newpos;
}/* End of 'push_back' function */

/*! Delete element from end of list function
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::list::pop_back(void) {
  
  VERIFICATION;

  if (size == 0)
    return 0;

  if (size == 1) {
    emit(&POISON, tail, sizeof(POISON));
    
    size--;

    return 1;
  }
  
  int new_tail = vertex_arr[tail].prev;
  vertex_arr[new_tail].next = 0;
  
  free = tail;
  vertex_arr[tail].next = free;
  vertex_arr[tail].prev = -1;
  
  emit(&POISON, tail, sizeof(POISON));
  size--;
  
  tail = new_tail;

  VERIFICATION;

  return 1;
}/* End of 'pop_back' function */

/*! Delete from front of list function.
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::list::pop_front(void) {

  VERIFICATION;

  if (size == 0)
    return 0;

  if (size == 1) {
    emit(&POISON, head, sizeof(POISON));
    size--;

    return 1;
  }
  
  int new_head = vertex_arr[head].next;
  vertex_arr[new_head].prev = 0;
  
  vertex_arr[head].next = free;
  free = head;
  vertex_arr[head].prev = -1;
  
  emit(&POISON, head, sizeof(POISON));
  size--;

  head = new_head;

  VERIFICATION;

  return 1;
}/* End of 'pop_front' function */

/*! Getting physical index of head.
 * \param[in]  none.
 * \param[out] adress of head.
 */
int mk5::list::begin(void) {
  return head; 
}
/* End of 'GetAdressHead' function */

/*! Getting physical index of tail.
 * \param[in]  none.
 * \param[out] adress of tail.
 */
int mk5::list::end(void) { 
  return tail; 
}
/* End of 'GetAdressTail' function */

/*! Get physical adress of element that stayed before.
 * \param[in]  pos - const size_t pos
 * \param[out] adress
 */
int mk5::list::rback(const size_t pos) {
  return vertex_arr[pos].prev;
}/* End of 'rback' function */

/*! Get physical adress of element that stayed after.
 * \param[in]  pos - const size_t pos
 * \param[out] adress
 */
int mk5::list::rfront(const size_t pos) { 
  return vertex_arr[pos].next; 
}
/* End of 'rfront' function */

/*! Get data by logical position in list.
 * \param[in]  logical position - int pos
 * \param[out] data
 */
data_type mk5::list::get_data(int pos) {
  
  VERIFICATION;

  int cur_vertex = head;
  while (cur_vertex != 0 && pos > 0) {
    cur_vertex = vertex_arr[cur_vertex].next;
    pos--;
  }

  VERIFICATION;

  return vertex_arr[cur_vertex].data;
}/* End of 'get_data' function */

/*! Find logical position of value in list.
 * \param[in]  value - const data_type value
 * \param[out] 
 */
int mk5::list::get_index(const data_type value) {
  
  if (IsValue(value))
    return 0;

  VERIFICATION;

  int pos = head;
  
  while (vertex_arr[pos].data != value && pos > 0) {
    pos = vertex_arr[pos].next;
  }
  
  VERIFICATION;

  return pos;
}/* End of 'get_index' function */

/*! Insert value before of physical position.
 * \param[in]  pos of insert - const int pos
 * \param[in]  value         - const data_type value
 * \param[out] return of new position
 */
int mk5::list::emplace_back(const int pos, const data_type value) {
  
  VERIFICATION;

  if (IsValue(value))
    return 0;

  if (pos < 1 || pos > capacity || vertex_arr[pos].prev == -1)
    return -1;

  if (pos == head)
    return push_front(value);

  if (free == -1)
    alloc_memory();

  int newpos = free;
  free = vertex_arr[free].next;

  vertex_arr[newpos].next = pos;
  vertex_arr[vertex_arr[pos].prev].next = newpos;
  vertex_arr[newpos].prev = vertex_arr[pos].prev;
  vertex_arr[pos].prev = newpos;
  
  emit(&value, newpos, sizeof(value));
  size++;

  VERIFICATION;

  return newpos;
}/* End of 'emplace_back' function */

/*! Insert value after of physical position.
 * \param[in]  pos of insert - const int pos
 * \param[in]  value         - const data_type value
 * \param[out] return of new position
 */
int mk5::list::emplace_front(const int pos, const data_type value) {

  VERIFICATION;
  
  if (IsValue(value))
    return 0;

  if (pos < 1 || pos > capacity || vertex_arr[pos].prev == -1)
    return -1;

  if (pos == tail)
    return push_back(value);

  if (free == -1)
    alloc_memory();

  int newpos = free;
  free = vertex_arr[free].next;

  vertex_arr[newpos].prev = pos;
  vertex_arr[newpos].next = vertex_arr[pos].next;
  vertex_arr[vertex_arr[pos].next].prev = newpos;
  vertex_arr[pos].next = newpos;
  
  emit(&value, newpos, sizeof(value));
  size++;

  VERIFICATION;

  return newpos;
}/* End of 'emplace_front' function */

/*! Delete value after of physical position.
 * \param[in]  pos of delete - const int pos
 * \param[out] code error
 */
int mk5::list::pop_pos_front(const int pos) {
  
  VERIFICATION;

  if (pos < 1 || pos > capacity || vertex_arr[pos].prev == -1)
    return -1;

  if (pos == tail) 
    return 0;

  int next = vertex_arr[pos].next;

  if (next == tail) {
    pop_back();
    
    return 1;
  }

  vertex_arr[pos].next = vertex_arr[next].next;
  vertex_arr[vertex_arr[next].next].prev = pos;
  
  vertex_arr[next].next = free;
  vertex_arr[next].prev = -1;
  free = next;
  emit(&POISON, next, sizeof(POISON));
  size--;
  
  VERIFICATION;

  return 1;
}/* End of 'pop_pos_front' function */

/*! Delete value before of physical position.
 * \param[in]  pos of delete - const int pos
 * \param[out] code error
 */
int mk5::list::pop_pos_back(const int pos) {
  
  VERIFICATION;

  if (pos == head)
    return 0; 

  if (pos < 1 || pos > capacity || vertex_arr[pos].prev == -1)
    return -1;

  int prev = vertex_arr[pos].prev;

  if (prev == head) {
    pop_front();
  
    return 1;
  }

  vertex_arr[vertex_arr[prev].prev].next = pos;
  vertex_arr[pos].prev = vertex_arr[prev].prev;

  vertex_arr[prev].next = free;
  free = prev;

  vertex_arr[prev].prev = -1;
  emit(&POISON, prev, sizeof(POISON));
  size--;

  VERIFICATION;

  return 1;
}
/* End of 'pop_pos_back' function */

/*! Delete current value of physical position.
 * \param[in]  pos of delete - const int pos
 * \param[out] code error
 */
int mk5::list::pop_pos(const int pos) {

  VERIFICATION;

  if (pos < 1 || pos > capacity || vertex_arr[pos].prev == -1)
    return -1;

  if (pos == head) {
    pop_front();
    
    return 1;
  }
  if (pos == tail) {
    pop_back();
    
    return 1;
  }
  int next = vertex_arr[pos].next;
  int prev = vertex_arr[pos].prev;

  vertex_arr[next].prev = prev;
  vertex_arr[prev].next = next;

  vertex_arr[pos].next = free;
  free = pos;

  vertex_arr[pos].prev = -1;
  emit(&POISON, pos, sizeof(POISON));
  size--;

  VERIFICATION;

  return 1;
}/* End of 'pop_pos' function */