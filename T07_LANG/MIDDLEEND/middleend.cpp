#include "middleend.h"
#include "func_var.h"

char func_name[][MAX_STR] = {"sin", "cos", "sqrt", "ln", "exp"};

#define CUR node
#define L   node->left
#define R   node->right
#define R_L node->right->left
#define L_R node->left->right
#define L_L node->left->left
#define R_R node->right->right

#define TYPE node->type
#define L_TYPE node->left->type
#define R_TYPE node->right->type

#define R_VALUE node->right->value
#define L_VALUE node->left->value
#define VALUE node->value

#define NAME node->name
#define L_NAME node->left->name
#define R_NAME node->right->name

#define NUM_N node->num
#define L_NUM_N node->left->num
#define R_NUM_N node->right->num

#define KILL_L delete node->left
#define KILL_R delete node->right


int mk5::middleend::simplify_vertex(vertex* node) {
  if (node == nullptr)
    return 0;
  switch (TYPE) {
  case OPER:
    switch ((int)VALUE) {
    case '+':
    {
      if (L_TYPE == NUM && R_TYPE == NUM) {
        TYPE = NUM;
        VALUE = L_VALUE + R_VALUE;;
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (L_TYPE == NUM && L_VALUE == 0) {
        KILL_L;
        NUM_N = R_NUM_N;
        NAME = R_NAME;
        TYPE = R_TYPE;
        VALUE = R_VALUE;
        L = R_L;

        vertex* right = R;
        R = R_R;

        delete right;

        return 1;
      }
      else if (R_TYPE == NUM && R_VALUE == 0) {
        KILL_R;
        NUM_N = L_NUM_N;
        NAME = L_NAME;
        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;

        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }

      return simplify_vertex(L) + simplify_vertex(R);
    }
    case '*':
    {
      if (L_TYPE == NUM && R_TYPE == NUM) {
        TYPE = NUM;
        VALUE = R_VALUE * L_VALUE;
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if ((L_TYPE == NUM && L_VALUE == 0) || (R_TYPE == NUM && R_VALUE == 0)) {
        TYPE = NUM;
        VALUE = 0;
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if ((L_TYPE == NUM && L_VALUE == 1) || (R_TYPE == NUM && R_VALUE == 1)) {
        if ((R_TYPE == NUM && R_VALUE == 1)) {
          KILL_R;
          NAME = L_NAME;
          NUM_N = L_NUM_N;
          TYPE = L_TYPE;
          R = L_R;
          VALUE = L_VALUE;
          vertex *left = L;
          L = L_L;
          delete left;

          return 1;
        }
        else {
          KILL_L;
          NAME = R_NAME;
          NUM_N = R_NUM_N;
          TYPE = R_TYPE;
          L = R_L;
          VALUE = R_VALUE;
          vertex *right = R;
          R = R_R;
          delete right;

          return 1;
        }

        return 1;
      }

      return simplify_vertex(L) + simplify_vertex(R);
    }
    case '/':
    {
      if (L_TYPE == NUM && R_TYPE == NUM) {
        TYPE = NUM;
        VALUE = L_VALUE / R_VALUE;
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (R_TYPE == NUM && R_VALUE == 1) {
        KILL_R;
        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;
        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }

      return simplify_vertex(L) + simplify_vertex(R);
    }
    case '-':
    {
      if (L_TYPE == NUM && R_TYPE == NUM) {
        TYPE = NUM;
        VALUE = L_VALUE - R_VALUE;;
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (R_TYPE == NUM && R_VALUE == 0) {
        KILL_R;
        NAME = L_NAME;
        NUM_N = L_NUM_N;
        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;
        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }

      return simplify_vertex(L) + simplify_vertex(R);
    }
    case '^':
    {
      if (L_TYPE == NUM && R_TYPE == NUM) {
        TYPE = NUM;
        VALUE = pow(L_VALUE, R_VALUE);
        KILL_L;
        KILL_R;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (R_TYPE == NUM && R_VALUE == 1) {
        KILL_R;
        NUM_N = L_NUM_N;
        NAME = L_NAME;
        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;
        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }
      else if (L_TYPE == NUM && L_VALUE == 1) {
        KILL_R;
        KILL_L;
        TYPE = NUM;
        VALUE = 1;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (L_TYPE == NUM && L_VALUE == 0) {
        KILL_R;
        KILL_L;
        TYPE = NUM;
        VALUE = 0;
        L = nullptr;
        R = nullptr;

        return 1;
      }

      return simplify_vertex(L) + simplify_vertex(R);
    }
    default:
      return simplify_vertex(R) + simplify_vertex(L);
    }
  }
  return 0;
}

mk5::vertex* find_diff(mk5::vertex* node, mk5::vertex* parent) {

  if (node == nullptr)
    return nullptr;

  if (node->type == STD_FUNC && strncmp(node->name.str, "diff", 4) == 0)
    return parent;

  if (node->left != nullptr) {
    mk5::vertex* ver_ret = find_diff(node->left, node);
    if (ver_ret != nullptr)
      return ver_ret;
  }

  if (node->right != nullptr) {
    mk5::vertex* ver_ret = find_diff(node->right, node);
    if (ver_ret != nullptr)
      return ver_ret;
  }

  return nullptr;
}

int mk5::middleend::loop_diff(void) {
  
  while (1)
  {
    vertex* node_diff = find_diff(mytree.root->left, mytree.root);
    if (node_diff != nullptr && find_diff(node_diff->left, node_diff) != nullptr) {
      variable var_diff(node_diff->left->left->name, 0);
      vertex* already_diff = diff(node_diff->left->right, var_diff);
      mytree.delete_tree(node_diff->left);
      node_diff->left = already_diff;
    }
    else if (node_diff != nullptr) {
      variable var_diff(node_diff->right->left->name, 0);
      vertex* already_diff = diff(node_diff->right->right, var_diff);
      mytree.delete_tree(node_diff->right);
      node_diff->right = already_diff;
    }
    vertex* diff_vert = node_diff;

    node_diff = find_diff(mytree.root->right, mytree.root);
    if (node_diff != nullptr && find_diff(node_diff->left, node_diff) != nullptr) {
      variable var_diff(node_diff->left->left->name, 0);
      vertex* already_diff = diff(node_diff->left->right, var_diff);
      mytree.delete_tree(node_diff->left);
      node_diff->left = already_diff;
    }
    else if (node_diff != nullptr) {
      variable var_diff(node_diff->right->left->name, 0);
      vertex* already_diff = diff(node_diff->right->right, var_diff);
      mytree.delete_tree(node_diff->right);
      node_diff->right = already_diff;
    }

    if (node_diff == nullptr && diff_vert == nullptr)
      break;
  }

  return 1;
}

mk5::vertex* mk5::middleend::diff(vertex* node, variable &var_diff) {
  

  switch (node->type) {
  case NUM:
    return new vertex(0.0);
  case OPER:
    switch ((int)node->num) {
    #define OPERATOR(type, diff) \
    case type:                   \
      diff                       
      
    #include "operations.h"
    #undef OPERATOR
    }

    return new vertex(node);
  case VAR:
    if (node->name == var_diff.name)
      return new vertex(1);
    return new vertex(0.0);
  
  case STD_FUNC:
    if (0)
      ;
    #define FUNCTION(id, diff)                                                   \
    else if (strncmp(node->name.str, func_name[id], strlen(func_name[id])) == 0) \
      diff
    
    #include "functions.h"
    #undef FUNCTION
    return new vertex(node);
  default:
    return new vertex(node);;
  }
}

char choose_random(void) {
  int num = rand() % 5;
  
  if (num == 0)
    return '+';
  if (num == 1)
    return '-';
  if (num == 2)
    return '*';
  if (num == 3)
    return '/';

  return '^';
}

int make_some_random(mk5::vertex* node) {

  if (node->type == OPER && node->num != '=')
    node->num = choose_random();
  if (node->type == NUM)
    node->value = 30 * rand();
  if (node->type == POL_OP && (strncmp(node->name.str, "return", 6) != 0)) {
    if (rand() % 2)
      node->name = mk5::line("if", 2);
    else
      node->name = mk5::line("while", 5);
  }

  if (node->left != nullptr)
    make_some_random(node->left);
  if (node->right != nullptr)
    make_some_random(node->right);

  return 0;
}

int mk5::middleend::simplify_write(const char* name) {
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
  
  loop_diff();

  while (simplify_vertex(mytree.root))
    ;
  
  while (1)
  {
    printf("\nDo u wanna some trash???))))\n"
      "1. Yes\n"
      "2. No\n");
    int n = getchar();
    if (n == '1' || n == '2')
    {
      printf("MUAHAHHAHAHAHAHAHHA\n");
      make_some_random(mytree.root);
      break;
    }
    else if (n == '0')
      break;
  }

  mytree.write(file_name);

  return 1;
}