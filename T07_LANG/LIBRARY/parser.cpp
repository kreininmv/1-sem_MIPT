/* FILE NAME   : parser.cpp
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "parser.h"

#define ASSERT_ERROR(expr, err)                                         \
if (expr)                                                               \
{                                                                       \
  throw syntax_error(__FILE__, __func__, __LINE__, err, ptr->name.str); \
}                                                              

#define CHECK_SYM(sym) (ptr->name.str[0] != sym)

#define CHECK_WORD(x) (strncmp(x, ptr->name.str, ptr->name.len) == 0)
                       
#define TOKEN_IF        "if"
#define TOKEN_WHILE     "while"
#define TOKEN_VAR       "var"
#define TOKEN_PRINT     "print"
#define TOKEN_RETURN    "return"
#define TOKEN_FUNCTION  "function"

int mk5::parser::CountLeft(vertex* node) {
  if (node == nullptr)
    return 0;
  if (node->left == nullptr)
    return 1;
  
  return 1 + CountLeft(node->left);
}

mk5::vertex* mk5::parser::read_vertex(void) {

  if (*buf != '{')
  {
    //printf("\n%d", buf);
    return nullptr;
  }
  else
    buf++;

  vertex* node = nullptr;
  if (isdigit(*buf)) {
    int add_pos = 0;
    double num = NAN;
    sscanf(buf, "%lg%n", &num, &add_pos);
    buf += add_pos;
    node = new vertex(num);
    node->left = read_vertex();
    node->right = read_vertex();
  }
  else if (isalpha(*buf)) {
    int new_pos = 0;
    do
    {
      new_pos++;
    } while (isalpha((unsigned char)buf[new_pos]) || isdigit(buf[new_pos]) || buf[new_pos] == '-' || buf[new_pos] == '_');

    if (strncmp(buf, "function-declaration", strlen("function-declaration")) == 0) {
      node = new vertex(FUNCTION_DECLARATION, nullptr, nullptr);
      buf += new_pos + 1;

      int new_function = 0;
      do {
        new_function++;
      } while (isalpha((unsigned char)buf[new_function]) || isdigit(buf[new_function]) || buf[new_pos] == '-' || buf[new_pos] == '_');

      if (strncmp(buf, "nil", strlen("nil")) == 0)
      {
        delete node;
        node = nullptr;
        buf += new_function;
      }
      else
      {
        line func_name(buf + 1, new_function - 1);
        function func(func_name, 0);
        buf += new_function;

        int num_func = functions.find(func);
        if (num_func == -1)
        {
          functions.add(func);
          num_func = functions.cur_size - 1;
        }
        cur_func = num_func;
        vertex* arguments = read_vertex();
        functions[num_func].num_arg = CountLeft(arguments);
        vertex* body = read_vertex();

        buf++;

        node->right = read_vertex();
        node->left = new vertex(FUNC, func_name.str, func_name.len, num_func, arguments, body);
      }
    }
    else if (strncmp(buf, "nil", strlen("nil")) == 0)
    {
      node = nullptr;
      buf += new_pos;
    }
    else if (strncmp(buf, "if", 2) == 0) {
      count_if++;
      node = new vertex(POL_OP, buf, new_pos, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();
    }
    else if (strncmp(buf, "while", 5) == 0) {
      count_while++;
      node = new vertex(POL_OP, buf, new_pos, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();
    }
    else if (strncmp(buf, "return", 6) == 0) {
      count_while++;
      node = new vertex(POL_OP, buf, new_pos, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();
    }
    else if (strncmp(buf, "concatenation", strlen("concatenation")) == 0){
      node = new vertex(CONCATENATION, nullptr, nullptr);
      buf += new_pos;

      node->left = read_vertex();
      node->right = read_vertex();
    }
    else {
      line name_var(buf, new_pos);
      variable new_var(name_var, cur_func);
      int num_func = GLOBAL;
      if (cur_func == GLOBAL) {
        num_func = variables.find(new_var);
        if (num_func == -1)
          variables.add(new_var);
      }
      else {
        num_func = functions[cur_func].vars.find(new_var);
        if (num_func == -1) {
          functions[cur_func].vars.add(new_var);
        }
      }

      node = new vertex(VAR, buf, new_pos, 0, nullptr, nullptr);
      buf += new_pos;

      node->left = read_vertex();
      node->right = read_vertex();
    }
  }
  else if (*buf == '$') {
    int new_pos = 0;
    do
    {
      new_pos++;
    } while (isalpha((unsigned char)buf[new_pos]) || isdigit(buf[new_pos]) || buf[new_pos] == '-' || buf[new_pos] == '_');
    
    //printf("%d\n", buf);
    if (strncmp(buf + 1, "sin", strlen("sin")) == 0   || strncmp(buf + 1, "cos", strlen("cos")) == 0   || strncmp(buf + 1, "print", strlen("print")) == 0 
     || strncmp(buf + 1, "diff", strlen("diff")) == 0 || strncmp(buf + 1, "scan", strlen("scan")) == 0 || strncmp(buf + 1, "sqrt", strlen("sqrt")) == 0
     || strncmp(buf + 1, "ln", 2) == 0) {
      node = new vertex(STD_FUNC, buf + 1, new_pos - 1, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();
    }
    else {
      node = new vertex(FUNC, buf + 1, new_pos - 1, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();

      vertex* arg = node->left;
      while (1)
      {
        if (arg == nullptr || arg->right)
          break;

        node->num++;
        if (arg->left == nullptr)
          break;

        arg = arg->left;
      }

      function func(node->name, node->num);
      int num_func = functions.find(func);
      if (num_func == -1) {
        functions.add(func);
        node->num = functions.cur_size - 1;
      }
    }
  }
  else {
    int new_pos = 0;
    do
    {
      new_pos++;
    } while (buf[new_pos] != '{');
    
    if (strncmp(buf, "<=", 2) == 0 || strncmp(buf, "!=", 2) == 0 || strncmp(buf, ">=", 2) == 0 || strncmp(buf, "==", 2) == 0) {
      node = new vertex(CMP, buf, new_pos, 0, nullptr, nullptr);
      buf += new_pos;
      node->left = read_vertex();
      node->right = read_vertex();
    }
    else if (new_pos == 1) {
      node = new vertex(OPER, *buf, nullptr, nullptr);
      buf += new_pos;

      node->left = read_vertex();
      node->right = read_vertex();
    }
  }

  buf++;

  return node;
}

//*Parser
mk5::vertex* mk5::parser::GetG(token *arr, int size){
  ptr = arr;
  token_size = size;

  vertex* root = GetDec();

  return root;
}

mk5::vertex* mk5::parser::GetDec(void) {
  
  vertex* root = new vertex(FUNCTION_DECLARATION, nullptr, nullptr);
  if (strncmp(ptr->name.str, TOKEN_FUNCTION, ptr->name.len) == 0)
    root->left = GetCreateFunc();
  //else if (strncmp(ptr->name.str, TOKEN_VAR, ptr->name.len) == 0)
  //  root->left = GetCreateVar();
  else
    ASSERT_ERROR(1, "You write some shit, idiot");
  
  vertex* cur = new vertex(FUNCTION_DECLARATION, nullptr, nullptr);
  root->right = cur;

  while (ptr->name.str[0] != 0) {

    if (strncmp(ptr->name.str, TOKEN_FUNCTION, ptr->name.len) == 0)
      cur->left = GetCreateFunc();
    //else if (strncmp(ptr->name.str, TOKEN_VAR, ptr->name.len) == 0)
    //  cur->left = GetCreateVar();
    else
      ASSERT_ERROR(1, "You write some shit, idiot");
    
    cur->right = new vertex(FUNCTION_DECLARATION, nullptr, nullptr);
    cur = cur->right;
  }

  return root;
}

mk5::vertex* mk5::parser::GetCreateVar(void) {
  ptr++;

  vertex* var = new vertex(VAR, ptr->name.str, ptr->name.len, add_var(ptr->name), nullptr, nullptr);
  ptr++;

  if (ptr->type == TOK_SYM) {
    ptr++;
    vertex* node = new vertex(OPER, '=', var, GetE());
    ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
    ptr++;

    return node;
  }

  ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
  ptr++;

  return var;
}

mk5::vertex* mk5::parser::GetStandartFunc(void) {
  vertex* node = nullptr;
  
  if (ptr->name.str != nullptr && strncmp(ptr->name.str, "sin", 3) == 0) 
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
  else if (ptr->name.str != nullptr && strncmp(ptr->name.str, "cos", 3) == 0)
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
  else if (ptr->name.str != nullptr && strncmp(ptr->name.str, "sqrt", 4) == 0)
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
  else if (ptr->name.str != nullptr && strncmp(ptr->name.str, "ln", 2) == 0)
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
  else if (ptr->name.str != nullptr && strncmp(ptr->name.str, "scan", 4) == 0) {
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
    ptr++;
    ASSERT_ERROR(CHECK_SYM('('), "You don't have right bracket ')' ");
    ptr++;
    node->left = GetVar();
    ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
    ptr++;
    ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
    ptr++;

    return node;
  }
  else if (ptr->name.str != nullptr && strncmp(ptr->name.str, "diff", 4) == 0) {
    node = new vertex(STD_FUNC, ptr->name.str, ptr->name.len, 1, nullptr, nullptr);
    ptr++;
    ASSERT_ERROR(CHECK_SYM('('), "You don't have right bracket ')' ");
    ptr++;
    node->left = GetVar();
    ASSERT_ERROR(CHECK_SYM(','), "You don't have ',' in diff standart function");
    ptr++;
    node->right = GetE();
    ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
    ptr++;

    return node;
  }

    
  
  if (node == nullptr)
    return nullptr;

  ptr++;

  ASSERT_ERROR(CHECK_SYM('('), "You don't have right bracket ')' ");
  ptr++;

  node->left = GetE();

  ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
  ptr++;
  
  return node;
}

mk5::vertex* mk5::parser::GetFunc(void) {
  
  if (ptr->name.str[0] == ')')
    return nullptr;

  int num = -1;
  
  function func(ptr->name);

  num = functions.find(func);
  ptr++;

  if (num == -1) {
    functions.add(func);
    num = functions.cur_size - 1;
  }

  ASSERT_ERROR(CHECK_SYM('('), "You don't have left bracket '(' ");
  ptr++;
  vertex* arguments = nullptr;
  
  arguments = GetE();
  
  if (arguments == nullptr) {
    ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
    ptr++;
    functions[num].num_arg = 0;
    return  new vertex(FUNC, functions[num].name.str, functions[num].name.len, num, nullptr, nullptr);
  }

  //Если их больше одного
  //Это функция по запихиванию переменных в левое поддерево функции при её непосредственном вызове
  vertex* cur = new vertex(CONCATENATION, nullptr, arguments);
  arguments = cur;
  int arg_count = 1;
  
  while(ptr->name.str[0] != ')')
  {
    ASSERT_ERROR(CHECK_SYM(','), "You don't have right bracket ')' ");
    ptr++;
    
    cur->left = new vertex(CONCATENATION, nullptr, GetE());
    cur = cur->left;
    arg_count++;
  }

  ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
  ptr++;
  functions[num].num_arg = arg_count;

  return  new vertex(FUNC, functions[num].name.str, functions[num].name.len, num, arguments, nullptr);
}

mk5::vertex* mk5::parser::GetVar( void ) {
  
  if (ptr->name.str == nullptr)
    return nullptr;

  int num = -1;
  variable var(ptr->name, cur_func);
  vertex* node = nullptr;

  if (cur_func == GLOBAL) {
    num = variables.find(var);
    
    if (num == -1)
      return nullptr;

    node = new vertex(VAR, variables[num].name.str, variables[num].name.len, num, nullptr, nullptr);
  }
  else {
    num = functions[cur_func].vars.find(var);
    
    if (num == -1)
      return nullptr;
    
    node = new vertex(VAR, functions[cur_func].vars[num].name.str, functions[cur_func].vars[num].name.len, num, nullptr, nullptr);
  }

  ptr++;

  return node;
}

int mk5::parser::add_var(line& name) {
  variable var(name, cur_func);
  
  int num = -2;
  
  num = variables.find(var);

  if (cur_func != GLOBAL)
    num = functions[cur_func].vars.find(var);

  ASSERT_ERROR(num != -1, "Variable redeclaration");
  
  if (cur_func == GLOBAL) {
    variables.add(var);
    return variables.cur_size - 1;
  }
  else {
    functions[cur_func].vars.add(var);
    return functions[cur_func].vars.cur_size - 1;
  }

  return -1;
}

mk5::vertex* mk5::parser::CreateArgs(void) {
  #define CREATE_VAR  \
   new vertex(VAR, ptr->name.str, ptr->name.len, add_var(ptr->name), nullptr, nullptr)
  

  ASSERT_ERROR(CHECK_SYM('('), "You don't have left bracket '(' ");
  ptr++;
  int arg_count = 0;

  vertex* arguments = nullptr;
  
  if (ptr->type == TOK_WORD) {
    arguments = CREATE_VAR;
    ptr++;
    arg_count++;
    vertex* cur = new vertex(CONCATENATION, nullptr, arguments);
    
    arguments = cur;

    while (ptr->name.str[0] == ',') {
      ptr++;
      arg_count++;
      cur->left = new vertex(CONCATENATION, nullptr, CREATE_VAR);
      ptr++;
      cur = cur->left;
    }
  }

  ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
  ptr++;
#undef CREATE_VAR
  
  if (arguments != nullptr)
    arguments->value = arg_count;

  return arguments;
}

mk5::vertex* mk5::parser::GetCondition(void) {
  ASSERT_ERROR(CHECK_SYM('('), "You don't have left bracket '(' ");
  ptr++;

  vertex* left = GetE();

  if (ptr->name.str[0] == '!') {
    ptr++;
    if (ptr->name.str[0] == '=') {
      ptr++;
      vertex* node = new vertex(CMP, "!=", 2, 0, left, GetE());
     
      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }

  }
  else if (ptr->name.str[0] == '>') {
    ptr++;
    
    if (ptr->name.str != nullptr && ptr->name.str[0] == '=' ) {
      ptr++;
      vertex* node = new vertex(CMP, ">=", 2, 0, left, GetE());

      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }
    else {
      vertex* node = new vertex(CMP, ">", 1, 0, left, GetE());

      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }
  }
  else if (ptr->name.str[0] == '<') {
    ptr++;
    if (ptr->name.str != nullptr && ptr->name.str[0] == '=') {
      ptr++;
      vertex* node = new vertex(CMP, "<=", 2, 0, left, GetE());
      
      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }
    else {
      vertex* node = new vertex(CMP, "<", 1, 0, left, GetE());
      
      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }
  }
  else if (ptr->name.str[0] == '=') {
    ptr++;
    if (ptr->name.str[0] == '=') {
      ptr++;
      vertex* node = new vertex(CMP, "==", 2, 0, left, GetE());
      
      ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
      ptr++;

      return node;
    }
  }

  ASSERT_ERROR(1, "Inccorrect comprasion");

  return nullptr;
}

mk5::vertex* mk5::parser::GetIf(void)
{
  ptr++;
  vertex* left = GetCondition();
  vertex* right = GetBody();
  return new vertex(POL_OP, "if", 2, 0, left, right);
}

mk5::vertex* mk5::parser::GetWhile(void)
{
  ptr++;
  vertex* left = GetCondition();
  vertex* right = GetBody();
  
  return new vertex(POL_OP, "while", 5, 0, left, right);
}

mk5::vertex* mk5::parser::GetPrint(void)
{
  ptr++;

  ASSERT_ERROR(CHECK_SYM('('), "You don't have right bracket ')' ");
  ptr++;
  
  vertex* node = new vertex(STD_FUNC, "print", 5, 0, GetE(), nullptr);

  ASSERT_ERROR(CHECK_SYM(')'), "You don't have right bracket ')' ");
  ptr++;

  ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
  ptr++;

  return node;
}

mk5::vertex* mk5::parser::GetReturn(void)
{
  ptr++;

  if (ptr->name.str != nullptr && CHECK_SYM(';') == false) {
    ptr++;
    return new vertex(POL_OP, "return", 6, 0, nullptr, nullptr);
  }

  vertex* node = new vertex(POL_OP, "return", 6, 0, GetE(), nullptr);

  ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
  ptr++;
  return node;
}

mk5::vertex* mk5::parser::GetEqual(void) {
 
  vertex* var = GetVar();
  
  if (var == nullptr) {
    return nullptr;
  }
  ASSERT_ERROR(CHECK_SYM('='), "You don't have '=' ");
  ptr++;
  vertex* node = new vertex(OPER, '=', var, GetE());
  ASSERT_ERROR(CHECK_SYM(';'), "You don't have ';' ");
  ptr++;

  return node;
}
mk5::vertex* mk5::parser::GetWord(void)
{
  vertex* node = nullptr;
  if (strncmp(ptr->name.str, TOKEN_IF, strlen(TOKEN_IF)) == 0)
    node = GetIf();
  else if (strncmp(ptr->name.str, TOKEN_WHILE, strlen(TOKEN_WHILE)) == 0)
    node = GetWhile();
  else if (strncmp(ptr->name.str, TOKEN_PRINT, strlen(TOKEN_PRINT)) == 0)
    node = GetPrint();
  else if (strncmp(ptr->name.str, TOKEN_RETURN, strlen(TOKEN_RETURN)) == 0)
    node = GetReturn();
  else if (strncmp(ptr->name.str, TOKEN_VAR, strlen(TOKEN_VAR)) == 0)
    node = GetCreateVar();
  else {
    node = GetStandartFunc();
    if (node == nullptr)
      node = GetEqual();
    if (node == nullptr) {
      node = GetFunc();
      ASSERT_ERROR(CHECK_SYM(';'), "You don't have ;");
      ptr++;
    }
  }
  return node;
}

mk5::vertex* mk5::parser::GetBody(void) {
  ASSERT_ERROR(CHECK_SYM('{'), "You don't have left bracу '{' ");
  ptr++;
  vertex* root = new vertex(CONCATENATION, nullptr, nullptr);
  vertex* node = nullptr;

  if (ptr->type == TOK_WORD)
    node = GetWord();

  root->left = node;

  vertex* cur = root;
  
  while (ptr->type == TOK_WORD){
    node = GetWord();

    cur->right = new vertex(CONCATENATION, node, nullptr);
    cur = cur->right;
  }

  ASSERT_ERROR(CHECK_SYM('}'), "You don't have right brace '}' ");
  ptr++;
  return root;
}

mk5::vertex* mk5::parser::GetCreateFunc(void) {

  ptr++;

  line name = ptr->name;
  ptr++;
  
  function func_add(name, 0);
  int num_func = functions.find(func_add);
  if (num_func == -1) {
    functions.add(func_add);
    num_func = functions.cur_size - 1;
  }

  cur_func = num_func;

  vertex* arguments = CreateArgs();
  int num_args = 0;
  if (arguments != nullptr)
    num_args = (int)arguments->value;
    
  functions[num_func].num_arg = num_args;

  vertex* func = new vertex(FUNC, name.str, name.len, num_func, arguments, GetBody());

  return func;
}

mk5::vertex* mk5::parser::GetE(void) {

  vertex* val = GetT();

  while (ptr->name.str[0] == '+' || ptr->name.str[0] == '-')
  {
    char oper = ptr->name.str[0];
    ptr++;

    vertex* val2 = GetT();

    if (oper == '+')
      val = new vertex(OPER, '+', val, val2);
    else                        
      val = new vertex(OPER, '-', val, val2);
  }

  return val;
}

mk5::vertex* mk5::parser::GetT(void) {

  vertex* val = GetW();

  while (ptr->name.str[0] == '*' || ptr->name.str[0] == '/') {
    char oper = ptr->name.str[0];
    ptr++;

    vertex* val2 = GetW();

    if (oper == '*')
      val = new vertex(OPER, '*', val, val2);
    else
      val = new vertex(OPER, '/', val, val2);
  }

  return val;
}

mk5::vertex* mk5::parser::GetW(void) {

  vertex* val = GetP();

  while (ptr->name.str[0] == '^') {
    ptr++;

    vertex* val2 = GetP();

    val = new vertex(OPER, '^', val, val2);
  }

  return val;
}

mk5::vertex* mk5::parser::GetP(void) {
  
  if (ptr->type == TOK_NUM)
    return GetN();

  if (ptr->name.str[0] == '(') {
    ptr++;
    vertex* val = GetE();

    ASSERT_ERROR(CHECK_SYM(')'), "No closing bracket ')'");

    ptr++;

    return val;
  }

  return GetId();
}

mk5::vertex* mk5::parser::GetN(void) {
  
  vertex* num = new vertex(ptr->value);
  ptr++;

  return num;
}

mk5::vertex* apply(mk5::vertex* cur, mk5::vertex* node) {
  if (cur != nullptr)
    if (node != nullptr) {
      node->right = cur;
      return node;
    }
    else
      return cur;

  return nullptr;
}

mk5::vertex* mk5::parser::GetId(void) {

  int num = -1;
  vertex* node = nullptr;
  vertex* cur = nullptr;
  vertex* check = nullptr;

  if (ptr->name.str[0] == '-') {
    ptr++;
    node = new vertex(OPER, '-', new vertex(0.0), nullptr);
  }
  
  if (ptr->type == TOK_NUM) {
    cur = GetN();
    node->right = cur;
    return node;
  }
    

  cur = GetStandartFunc();
  
  check = apply(cur, node);
  if (check != nullptr)
    return check;
  cur = GetVar();
  
  check = apply(cur, node);
  if (check != nullptr)
    return check;

  cur = GetFunc();
  
  return apply(cur, node);
}