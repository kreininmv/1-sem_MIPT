#include "frontend-1.h"

int mk5::rev_frontend::read_tree(const char* name) {
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
  //printf("%d\n", buf);
  mytree.root = myparser.read_tree(buf);

  char new_file_name[MAX_STR] = "";
  strcat(new_file_name, "..\\PROGRAMS\\");
  strcat(new_file_name, "new_");
  strncat(new_file_name, name, strlen(name) - strlen(".tree"));
  strcat(new_file_name, ".txt");
  

  file = fopen(new_file_name, "wb");
  if (file == nullptr)
    return 0;
  for (int i = 0; i < myparser.functions.cur_size; i++)
    for (int j = 0; j < myparser.functions[i].num_arg; j++)
      myparser.functions[i].vars[j].init = IsInit;

  build_code(mytree.root, file);

  fclose(file);

  return 1;
}

int mk5::rev_frontend::write_param(vertex* node, FILE *file) {

  if (node == nullptr)
    return 1;

  if (node->type == CONCATENATION && node->right != nullptr)
  {
    build_code(node->right, file);
    if (node->left != nullptr && node->left->right != nullptr)
      fprintf(file, ", ");
  }
  
  if (node->type == CONCATENATION && node->left != nullptr)
    write_param(node->left, file);
  
  if (node->type != CONCATENATION)
  {
    build_code(node, file);
    if ( node->left != nullptr       && node->left->right != nullptr
      && node->type != CONCATENATION && node->left->left != nullptr) {
      fprintf(file, ", ");
      build_code(node->left, file);
    }
  }

  return 1;
}

int mk5::rev_frontend::write_var(vertex* node, FILE* file) {
  
  variable var(node->name, 0);
  int num = myparser.variables.find(var);
  
  if (num == -1) {
    num = myparser.functions[cur_func].vars.find(var);
    if (myparser.functions[cur_func].vars[num].init == NoInit) {
      fprintf(file, "var ");
      myparser.functions[cur_func].vars[num].init = IsInit;
    }
  }
  else if (myparser.variables[num].init == NoInit)
  {
    fprintf(file, "var ");
    myparser.variables[num].init = IsInit;
  }

  //fprintf(file, "");
  node->name.print_file(file);
  //fprintf(file, "");

  return 1;
}

int mk5::rev_frontend::write_pol_op(vertex* node, FILE* file) {
  
  
  if (strncmp(node->name.str, "return", 6) == 0) {
    fprintf(file, "\n%*c", indent_size, ' ');
    node->name.print_file(file);

    if (node->left != nullptr) {
      fprintf(file, " ");
      build_code(node->left, file);
    }

    fprintf(file, ";\n");
  }
  else {
    fprintf(file, "%*c", indent_size, ' ');
    node->name.print_file(file);

    fprintf(file, " (");
    build_code(node->left, file);
    fprintf(file, ")\n%*c{\n", indent_size, ' ');
    indent_size += TAB;
    build_code(node->right, file);
    indent_size -= TAB;
    fprintf(file, "%*c}\n\n", indent_size, ' ');
  }

  return 1;
}

int mk5::rev_frontend::write_oper(vertex* node, FILE* file) {
  
  if (node->num == '=')
    fprintf(file, "%*c", indent_size, ' ');

  if (node->num == '*' || node->num == '/' || node->num == '^') {
    if (node->left->type != VAR && node->left->type != NUM && node->left->type == FUNC && node->left->type == STD_FUNC) {
      fprintf(file, "(");
      build_code(node->left, file);
      fprintf(file, ")");
    }
    else
      build_code(node->left, file);
    
    fprintf(file, " %c ", node->num);
    if (node->right->type != VAR && node->right->type != NUM && node->right->type == FUNC && node->right->type == STD_FUNC) {
      fprintf(file, "(");
      build_code(node->right, file);
      fprintf(file, ")");
    }
    else
      build_code(node->right, file);
  }
  else {
    build_code(node->left, file);
    fprintf(file, " %c ", node->num);
    build_code(node->right, file);
  }


  return 1;
}

int mk5::rev_frontend::write_std_func(vertex* node, FILE* file) {

  if (strncmp(node->name.str, "print", 5) == 0 || strncmp(node->name.str, "scan", 4) == 0)
    fprintf(file, "%*c", indent_size, ' ');

  node->name.print_file(file);
  fprintf(file, "(");
  build_code(node->left, file);
  fprintf(file, ")");

  return 1;
}

int mk5::rev_frontend::write_cmp(vertex* node, FILE* file) {
  build_code(node->left, file);
  fprintf(file, " ");
  node->name.print_file(file);
  fprintf(file, " ");
  build_code(node->right, file);

  return 1;
}

int mk5::rev_frontend::write_concatenation(vertex* node, FILE* file) {
  if (node->left != nullptr)
    build_code(node->left, file);
  if (node->left != nullptr &&  node->left->type != POL_OP)
    fprintf(file, ";\n");
  if (node->right != nullptr)
    build_code(node->right, file);

  return 1;
}

int mk5::rev_frontend::write_function(vertex* node, FILE* file) {
  if (node->right != nullptr)
  {
    fprintf(file, "\nfunction ");
    function func(node->name);
    cur_func = myparser.functions.find(func);
  }

  node->name.print_file(file);
  fprintf(file, "(");
  write_param(node->left, file);
  fprintf(file, ")");
  
  if (node->right != nullptr) {
    fprintf(file, "\n%*c\n", indent_size, '{');
    indent_size += TAB;
    build_code(node->right, file);
    indent_size -= TAB;
    fprintf(file, "%*c\n\n", indent_size, '}');
  }

  return 1;
}

int mk5::rev_frontend::write_function_declaration(vertex* node, FILE* file) {
  if (node->left != nullptr)
    build_code(node->left, file);
  if (node->right != nullptr)
    build_code(node->right, file);

  return 1;
}

int mk5::rev_frontend::build_code(vertex* node, FILE* file)
{

  switch (node->type) {
  case FUNCTION_DECLARATION:
    write_function_declaration(node, file);
    break;
  case CONCATENATION:
    write_concatenation(node, file);
    break;
  case FUNC:
    write_function(node, file);
    break;
  case VAR:
    write_var(node, file);
    break;
  case NUM:
    fprintf(file, "%.lf", node->value);
    break;
  case POL_OP:
    write_pol_op(node, file);
    break;
  case STD_FUNC:
    write_std_func(node, file);
    break;
  case CMP:
    write_cmp(node, file);
    break;
  case OPER:
    write_oper(node, file);
    break;
  default:
    return 0;
  }
  return 1;
}