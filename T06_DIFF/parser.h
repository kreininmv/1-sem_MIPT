#pragma once
#ifndef PARSERH
#define PARSERH
/* FILE NAME   : tree.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "tree.h"
#include <iostream>

double isFunction(mk5::line *str);

struct syntax_error: public  std::runtime_error{
  const char* file_name;
  const char* function_name;
  int   num_line = -1;
  const char* type_error;
  const char* descr;

  syntax_error(const char* file, const char* function, int line, const char* err, const char* des) : 
    file_name(file),
    function_name(function),
    num_line(line),
    type_error(err),
    descr(des), 
    std::runtime_error(err) 
  { 
  }
  
  virtual const char const* what()
  {
    return type_error;
  }
};

struct variable {
  char name[MAX_STR] = {};
  double value = NAN;
};

namespace mk5 {
  class var {
  public:
    variable table_var[MAX_VAR] = {};
    int num_var = 0;

    const char* get_var_name(int serial_number);

    double get_var_val(int serial_number);

    int find_var(line &var_for_find);

    int add_var(line &var_for_add);
    int add_value(void);
    
    var() = default;
    ~var() { ; }

    var operator= (const var & copy) = delete;
    var(const var & copy) = delete;

  };
}

namespace mk5 {
  class parser {
    char *str = nullptr;
    int len_str = 0;

  public:
    var myvar;

    vertex* GetG(const char *s);

    parser() = default;
    ~parser() {
      
      if (str != nullptr) {
        str -= len_str;
        delete[] str;
      }
      
      str = nullptr;
    }

    parser operator= (const parser & copy) = delete;
    parser(const parser& copy) = delete;

  private:
    vertex* GetE(void);

    vertex* GetT(void);

    vertex* GetW(void);

    vertex* GetP(void);

    vertex* GetN(void);

    vertex* GetId(void);
  };
}
#endif