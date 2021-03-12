#ifndef FUNC_VARH
#define FUNC_VARH

#include "line.h"
#include "stock.h"

const int GLOBAL = -1;
const char IsInit = 1;
const char NoInit = 0;

enum type_vertex {
  nil                  = 0,
  VAR                  = 1,
  NUM                  = 2,
  OPER                 = 3,
  FUNC                 = 4,
  STD_FUNC             = 5,
  CMP                  = 6,
  POL_OP               = 7,
  CONCATENATION        = 8,
  FUNCTION_DECLARATION = 9
};

namespace mk5 {

  class vertex {
  public:
    type_vertex  type = nil;
    double value = NAN;
    char num = 0;
    vertex *left = nullptr;
    vertex *right = nullptr;
    line name;

    vertex() = default;

    //CONCATENATION and FUNCTION_DECLARATION constructor
    vertex(type_vertex tpe, vertex* l, vertex* r) {
      left = l;
      right = r;
      type = tpe;
      if (tpe == CONCATENATION)
        name = line("concatenation", strlen("concatenation"));
      else if (tpe == FUNCTION_DECLARATION)
        name = line("function-declaration", strlen("function-declaration"));
    }
    
    //num constructor
    vertex(double vl) {
      type = NUM;
      value = vl;
    }

    //function, var and CMP_OP constructor
    vertex(type_vertex tp, char *nm, int name_len, int var_num, vertex* lt, vertex* rt) {
      type = tp;
      name = line(nm, name_len);
      num = (char)var_num;
      left = lt;
      right = rt;
    }
    
    //functioan and var constructor with const
    vertex(type_vertex tp, const char *nm, int name_len, int var_num, vertex* lt, vertex* rt) {
      type = tp;
      name = line(nm, name_len);
      num = (char)var_num;
      left = lt;
      right = rt;
    }

    //operator constructor
    vertex(type_vertex tp, char op, vertex* l, vertex* r) {
      left = l;
      right = r;
      num = op;
      type = OPER;
    }

    //copy constuctor
    vertex(const vertex* copy) {
      value = copy->value;
      type = copy->type;
      if (copy->name.str != nullptr)
        name = copy->name;
      num = copy->num;

      if (copy->left != nullptr)
        left = new vertex(copy->left);

      if (copy->right != nullptr)
        right = new vertex(copy->right);
    }
  };

  class variable {
  public:
    line name;
    int num_func = GLOBAL;
    char init = NoInit;

    variable(void) { }

    variable(line &name_var, int num) {
      name = name_var;
      num_func = num;
    }

    ~variable() { }

    variable & operator=(variable& copy) {
      name = copy.name;
      num_func = copy.num_func;
      copy.init = copy.init;

      return *this;
    }

    bool operator==(variable& copy) {
      if (copy.name == name)
        return true;

      return false;
    }
  };

  class function {
  public:
    line name;
    int num_arg = 0;
    stock<variable> vars;

    function(void) { ; }
    ~function() { ; }

    function & operator=(function& copy) {
      name = copy.name;
      num_arg = copy.num_arg;
      vars = copy.vars;

      return *this;
    }

    bool operator==(function& copy) {
      if (copy.name == name)
        return true;

      return false;
    }

    explicit function(line &nm) {
      name = nm;
      num_arg = 0;
    }

    explicit function(line &nm, int num) {
      name = nm;
      num_arg = num;
    }

    void create_func(line &nm, int arg) {
      num_arg = arg;
      name = nm;
    }
  };
}

#endif