#include "backend.h"

int mk5::backend::write_cond(vertex* node, FILE *file) {
  write_vert_asm(node->left, file);
  write_vert_asm(node->right, file);

  if (node->num == '>')
    fprintf(file, "jbe :");
  else if (node->num == '<')
    fprintf(file, "jae :");
  else if (strncmp(node->name.str, ">=", 2) == 0)
    fprintf(file, "jb :");
  else if (strncmp(node->name.str, "<=", 2) == 0)
    fprintf(file, "ja :");
  else if (strncmp(node->name.str, "==", 2) == 0)
    fprintf(file, "jne :");
  else if (strncmp(node->name.str, "!=", 2) == 0)
    fprintf(file, "je :");
  

  return 1;
}

int mk5::backend::write_num(vertex* node, FILE *file) {
  fprintf(file, "push %lf\n", node->value);
  
  return 1;
}

int mk5::backend::write_var_pop(vertex* node, FILE *file) {

  variable var(node->name, cur_func);
  int num = myparser.variables.find(var);

  if (num != -1)
    fprintf(file, "pop [%d]\n", num);
  else
    fprintf(file, "pop [dx+%d]\n", -myparser.functions[cur_func].vars.find(var));

  return 1;
}

int mk5::backend::write_var_push(vertex* node, FILE *file) {

  variable var(node->name, cur_func);
  int num = myparser.variables.find(var);

  if (num != -1)
    fprintf(file, "push [%d]\n", num);
  else
    fprintf(file, "push [dx+%d]\n", -myparser.functions[cur_func].vars.find(var));

  return 1;
}

int mk5::backend::write_vert_asm(vertex* node, FILE *file) {

  switch (node->type) {
  case FUNCTION_DECLARATION:
    if (node->left != nullptr)
      write_vert_asm(node->left, file);
    if (node->right != nullptr)
      write_vert_asm(node->right, file);
    break;
  case CONCATENATION:
    if (node->left != nullptr)
      write_vert_asm(node->left, file);
    if (node->right != nullptr)
      write_vert_asm(node->right, file);
    break;
  case NUM:
    write_num(node, file);
    break;
  case OPER:
    if (node->num == '=') {
      write_vert_asm(node->right, file);
      write_var_pop(node->left, file);
      break;
    }

    write_vert_asm(node->left, file);
    write_vert_asm(node->right, file);

    if (node->num == '+')
      fprintf(file, "add\n");
    else if (node->num == '*')
      fprintf(file, "mul\n");
    else if (node->num == '-')
      fprintf(file, "sub\n");
    else if (node->num == '/')
      fprintf(file, "div\n");
    else if (node->num == '^')
      fprintf(file, "pow\n");
    break;
  case VAR:
  {
    write_var_push(node, file);
    break;
  }
  case STD_FUNC:
  {
    if (strncmp(node->name.str, "scan", 4) == 0) {
      fprintf(file, "in\n");
      node = node->left;
      write_var_pop(node, file);
    }
    else if (strncmp(node->name.str, "sin", 3) == 0) {
      write_vert_asm(node->left, file);
      fprintf(file, "sin\n");
    }
    else if (strncmp(node->name.str, "cos", 3) == 0) {
      write_vert_asm(node->left, file);
      fprintf(file, "cos\n");
    }
    else if (strncmp(node->name.str, "print", 5) == 0) {
      write_vert_asm(node->left, file);
      fprintf(file, "out\n");
    }
    else if (strncmp(node->name.str, "sqrt", 4) == 0) {
      write_vert_asm(node->left, file);
      fprintf(file, "sqrt\n");
    }
    else if (strncmp(node->name.str, "ln", 2) == 0) {
      write_vert_asm(node->left, file);
      fprintf(file, "ln");
    }
    break;
  }
  case FUNC:
  {
    function func(node->name);
    int num_func = myparser.functions.find(func);

    if (node->right != nullptr) {
      //При объявлении функции
      node->name.print_file(file);
      fprintf(file, ":\n");

      fprintf(file, "\n//Parametres for moving dx to variables in new function:\n");
      fprintf(file, "push %d\n", myparser.functions[num_func].vars.cur_size);
      fprintf(file, "push dx\n");
      fprintf(file, "add\n");
      fprintf(file, "pop dx\n");
      fprintf(file, "////////////////\n\n\n");
      
      //Аргументы функции
      fprintf(file, "//POP ARGS TO THE VARS FROM STACK:");
      for (int i = 0; i < myparser.functions[num_func].num_arg; i++)
        fprintf(file, "\npop [dx+%d]", -i);
      fprintf(file, "\n//ENDED POPING PARAMS TO THE CALLING VARS\n\n");
      cur_func = num_func;


      write_vert_asm(node->right, file);
    }
    else {
      
      vertex* arg = node->left;
      fprintf(file, "\n//CALL '");
      node->name.print_file(file);
      fprintf(file, "'\n\n");
      fprintf(file, "//ARGUMENTS:\n");
      while(1)
      {
        if (arg == nullptr)
          break;

        if (arg->right == nullptr)
          break;

        write_vert_asm(arg->right, file);
        
        if (arg->left == nullptr)
          break;
        arg = arg->left;
      }
      fprintf(file, "///////////\n");
      fprintf(file, "call :");
      node->name.print_file(file);
      fprintf(file, "\n//Parametres for moving dx to variables after calling function:\n");
      
      fprintf(file, "push dx\n");
      fprintf(file, "push %d\n", myparser.functions[num_func].vars.cur_size);
      fprintf(file, "sub\n");
      fprintf(file, "pop dx\n\n\n");
    }

  }
  case POL_OP:
  {
    if (strncmp(node->name.str, "if", node->name.len) == 0) {
      fprintf(file, "\n//It's start of condition for IF\n");
      write_cond(node->left, file);
      int num_if = i_if;
      i_if++;
      fprintf(file, "if%d\n", num_if);
      fprintf(file, "\n//It's end of condition for IF\n");
      fprintf(file, "\n//It's START BODY OF IF\n");
      write_vert_asm(node->right, file);
      fprintf(file, "//It's END BODY OF IF\n");
      fprintf(file, "if%d:\n", num_if);
    }
    else if (strncmp(node->name.str, "while", node->name.len) == 0) {
      
      int num_while = i_while;
      i_while += 2;

      fprintf(file, "\n//That's for repeat action\n");
      fprintf(file, "while%d:\n", num_while + 1);
      fprintf(file, "////////////////////////////\n");
      fprintf(file, "\n//It's start of condition for WHILE\n");
      write_cond(node->left, file);
      fprintf(file, "while%d\n", num_while);
      fprintf(file, "//It's end of condition for WHILE\n");
      fprintf(file, "\n//It's START BODY OF WHILE\n");
      write_vert_asm(node->right, file);
      fprintf(file, " while%d\n", num_while + 1);
      fprintf(file, "//It's END BODY OF WHILE\n");
      fprintf(file, "while%d:\n", num_while);

    }
    else if (strncmp(node->name.str, "return", node->name.len) == 0) {
      if (node->left != nullptr)
        write_vert_asm(node->left, file);
      if (strncmp(myparser.functions[cur_func].name.str, "main", 4) == 0)
      {
        fprintf(file, "hlt\n\n");
      }
      else
      fprintf(file, "rtrn\n\n");
    }
    break;
  }
  default:
      fprintf(stderr, "TU PROEBALSYA MATVEI\n");
      break;
  }
  return 0;
}

int mk5::backend::write_asm(const char* name) {
  char file_name[MAX_STR] = "";
  strcat(file_name, "..\\PROGRAMS\\");
  strcat(file_name, name);
  FILE *file = fopen(file_name, "rb");
  if (file == nullptr)
    return 0;

  fseek(file, 0, SEEK_END);
  int buf_len = ftell(file) + 1;

  rewind(file);

  buf = new char[buf_len];
  fread(buf, sizeof(char), buf_len - 1, file);

  fclose(file);
  buf[buf_len - 1] = 0;
  int new_buf_len = 0;
  for (int i = 0; i < buf_len; i++)
    if (isspace(buf[i]) == 0)
      buf[new_buf_len++] = buf[i];
  buf[new_buf_len - 1] = 0;

  mytree.root = myparser.read_tree(buf);

  memcpy(file_name, &ZERO, MAX_STR);
  strcat(file_name, "..\\PROGRAMS\\");
  strncat(file_name, name, strlen(name) - strlen(".tree"));
  strcat(file_name, ".mk5");
  file = fopen(file_name, "wb");
  if (file == nullptr)
    return 0;
  fprintf(file, "push 0\npush 0\n");
  fprintf(file, "pop dx\npop ax\n");
  fprintf(file, "call :main\n\nhlt\n\n");
  
  write_vert_asm(mytree.root, file);

  fclose(file);

  return 1;
}