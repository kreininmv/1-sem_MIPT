#ifndef TOKENH
#define TOKENH


#include "line.h"

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>

namespace mk5 {
  
  enum type_tok {
    TOK_NUL  = 0,
    TOK_NUM  = 1, 
    TOK_WORD = 2,
    TOK_SYM  = 3
  };

  class token {
  public:
    double value = NAN;
    line name;
    int line = 0, pos = 0;
    
    type_tok type = TOK_NUL;
  
    token(void) = default;
    
    token(char *nm, int ln, int ps, type_tok tp, int ln_nm) :
      name(nm, ln_nm), line(ln), pos(ps), type(tp)
    {;}

    token(double vl, int ln, int ps) :
      value(vl), line(ln), pos(ps), type(TOK_NUM)
    {;}

    ~token() {}

    token & operator= (const token& copy) {
      type = copy.type;
      line = copy.line;
      pos = copy.pos;
      name = copy.name;
      value = copy.value;

      return *this;
    }

  private:
    token(const token& copy) = delete;

  };
}

struct syntax_error: public  std::runtime_error {
  const char* file_name;
  const char* function_name;
  int   num_line = -1;
  const char* type_error;
  const char* descr;

  syntax_error(const char* file, const char* function, int line, const char* err, char* des) :
    file_name(file),
    function_name(function),
    num_line(line),
    type_error(err),
    descr(des),
    std::runtime_error(err)
  {
  }

  virtual const char * what()
  {
    return type_error;
  }
};

#endif