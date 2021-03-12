#ifndef STOCKH
#define STOCKH

#include "line.h"

namespace mk5 {

  template <typename type_data>
  int standart_compare(type_data &a, type_data &b)
  {
    return !(a == b);
  }

  template <typename type_data>
  class stock {
  public:
    int cur_size;
    int max_size;
    type_data *arr;

  
    stock(void) :
      max_size(2),
      cur_size(0)
    {
      arr = new type_data[2];
    }

    stock (int size) : 
      max_size(size), 
      cur_size(0) 
    {
      arr = new type_data[size];
    }

    ~stock() {

      if (arr != nullptr)
        delete[] arr;

      cur_size = 0;
      max_size = 0;
    }

    stock<type_data> & operator= (const stock<type_data> & copy) {
      if (arr != nullptr)
        delete[] arr;
      arr = new type_data[copy.max_size];
      max_size = copy.max_size;
      cur_size = copy.cur_size;

      for (int i = 0; i < cur_size; i++)
        arr[i] = copy.arr[i];

      return *this;
    }

    void resize(int size) {
      type_data *bulc = new type_data[size];

      if (arr != nullptr)
      {
        for (int i = 0; i < cur_size; i++)
          bulc[i] = arr[i];

        delete[] arr;
      }
      arr = bulc;
      max_size = size;
    }

    void add(type_data& data) {
      if (cur_size >= max_size) {
        if (max_size == 0)
          max_size = 1;
        else
          max_size *= 2;
        resize(max_size);
      }
      if (arr == nullptr)
        resize(1);

      arr[cur_size++] = data;
    }

    int find(type_data& data, int(*compare)(type_data &a, type_data &b) = nullptr) {
      if (compare == nullptr)
        compare = standart_compare;

      for (int i = 0; i < cur_size; i++)
        if (compare(arr[i], data) == 0)
          return i;

      return -1;
    }

    type_data & operator[](const int index) {
      assert(index >= 0 && index <= max_size);

      return arr[index];
    }

    stock<type_data>(const stock<type_data> &copy) = delete;
    
  };
}

#endif