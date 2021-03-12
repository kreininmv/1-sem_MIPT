/* FILE NAME   : parser.cpp
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "parser.h"
#include "onegin.h"

#define ASSERT_ERROR(x, err)                                       \
if (*str != x)                                                     \
{                                                                  \
  throw syntax_error(__FILE__, __func__, __LINE__, err, str);      \
}                                                             


double isFunction(mk5::line *str) {
  int x = 0;
  if (0) 
    x++;
#define FUNCTION(name, num, diff, eval, tex, diff_tex)   \
  else if (!strncmp(str->str, #name, str->len)) {        \
    return num;                                          \
  }
#include "functions.h"
#undef FUNCTION

  return NAN;
}

const char* mk5::var::get_var_name(int serial_number) {
  if (serial_number >= num_var || serial_number < 0)
    return nullptr;

  return table_var[serial_number].name;
}

double mk5::var::get_var_val(int serial_number) {
  if (serial_number >= num_var || serial_number < 0)
    return NAN;

  return table_var[serial_number].value;
}

int mk5::var::find_var(line &var_for_find) {
  for (int i_var = 0; i_var < num_var; i_var++)
    if (strncmp(var_for_find.str, table_var[i_var].name, var_for_find.len) == 0)
      return i_var;

  return -1;
}

int mk5::var::add_var(line &var_for_add) {
  int var_number = find_var(var_for_add) ;

  if (var_number != -1)
    return var_number;

  strcat(table_var[num_var++].name, var_for_add.str);
  
  return num_var - 1;
}

int mk5::var::add_value(void) {

  for (int i_var = 0; i_var < num_var; i_var++) {
    printf("\nAdd value of this var %s :", table_var[i_var].name);
    scanf("%lf", &table_var[i_var].value);
  }
  
  return 1;
}

//Parser
mk5::vertex* mk5::parser::GetG(const char *s) {
  len_str = strlen(s);
  str = new char[len_str];

  int i_sym = 0, cur_sym_str = 0;
  while (s[i_sym] != 0)
    if (s[i_sym] != ' ')
      str[cur_sym_str++] = s[i_sym++];
    else
      i_sym++;

  str[cur_sym_str] = 0;
  len_str = cur_sym_str;

  vertex* node = GetE();

  ASSERT_ERROR(0, "not finishing with '\0' ");
 
  return node;
}

mk5::vertex* mk5::parser::GetE(void) {

  vertex* val = GetT();

  while (*str == '+' || *str == '-')
  {
    char oper = *str;
    str++;

    vertex* val2 = GetT();

    if (oper == '+')
      val = new vertex(val, val2, '+', OPER);
    else
      val = new vertex(val, val2, '-', OPER);
  }

  return val;
}

mk5::vertex* mk5::parser::GetT(void) {

  vertex* val = GetW();

  while (*str == '*' || *str == '/') {
    char oper = *str;
    str++;

    vertex* val2 = GetW();

    if (oper == '*')
      val = new vertex(val, val2, '*', OPER);
    else
      val = new vertex(val, val2, '/', OPER);
  }

  return val;
}

mk5::vertex* mk5::parser::GetW(void) {

  vertex* val = GetP();

  while (*str == '^') {
    str++;

    vertex* val2 = GetP();

    val = new vertex(val, val2, '^', OPER);
  }

  return val;
}

mk5::vertex* mk5::parser::GetP(void) {
  if (*str == '(') {
    str++;
    vertex* val = GetE();

    ASSERT_ERROR(')', "no closing bracket ')'");

    str++;

    return val;
  }
  else
    return GetN();
}

mk5::vertex* mk5::parser::GetN(void) {

  if ('0' <= *str && *str <= '9') {

    double val = 0;

    while ('0' <= *str && *str <= '9') {
      val = val * 10 + *str - '0';
      str++;
    }

    if (*str == '.')
    {
      str++;
      int counter = 0;

      while ('0' <= *str && *str <= '9') {
        val += (*str - '0') / pow(10, counter);
        counter++;
        str++;
      }
    }

    return new vertex(nullptr, nullptr, val, NUM);
  }
  return GetId();
}

mk5::vertex* mk5::parser::GetId(void) {
  line command = {0, new char[MAX_STR]};

  char *start_in = str;
  while (('a' <= *str && *str <= 'z') || ('A' <= *str && *str <= 'Z')) {
    command.str[command.len++] = *str;
    str++;
  }
  command.str[command.len] = '\0';

  if (*str == '(') {
    double value = isFunction(&command);
    delete[] command.str;
    if (isnan(value)) {
      str = start_in;
      ASSERT_ERROR(256, "there is no such function");
    }

    str++;
    vertex* val2 = GetE();
    ASSERT_ERROR(')', "no closing bracket ')'");
    
    str++;
    return new vertex(nullptr, val2, value, FUNC);
  }
  else 
    myvar.add_var(command);
  
  vertex* vertex_return = new vertex(nullptr, nullptr, myvar.add_var(command), VAR);
  delete[] command.str;
  return vertex_return;
}