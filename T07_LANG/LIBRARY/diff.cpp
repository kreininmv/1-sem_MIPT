/* FILE NAME   : diff.cpp
 * PURPOSE     : Function module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include "diff.h"

extern mk5::var table_var[MAX_VAR] = {};
extern int num_var = 0;

const int num_introduction = 12;
const char introduction[][MAX_STR] = {"Ух какой интересный пример:",
                                      "Это выражение с легкостью сможет сделать натренированный пятиклассник:",
                                      "Совсем простенький пример, разве нет?:",
                                      "Это слишком простое выражение, тебе так не кажется?:",
                                      "Это очевиднейший пример:",
                                      "Если ты не можешь решить это, то я сочувствую тебе:",
                                      "Это -1 первая Великая теорема Ферма:",
                                      "Это загадка от древних шизов:",
                                      "Как это нельзя сделать за 30 секунд?:",
                                      "Это совсем несложный пример, попробуй сделать его сам:",
                                      "У меня не осталось слов, чтобы воодушевить тебя решить это выражение:",
                                      "Надеюсь, что Миша Павлов еще умеет решать такой пример:"};
const int num_answer = 12;
const char answer[][MAX_STR] = {"благодаря фмл 30 я могу получить из предыдущего выражения следующее:",
                                "это преобразование я произвожу по правилу, которое описано на странице 239 Математический анализ В.А.Зорич, и получаю:",
                                "\\includegraphics[scale=0.31]{image.jpg}:",
                                "спросив у знакомых из Чебышёвки (шучу, и так всё знал), я узнал, что предыдущее выражение переходит в следующее:",
                                "потанцевав с бубном я осуществил этот переход и получил:",
                                "я не буду объяснять, как я сделал этот переход и получил:",
                                "если немножка подумать, то можно понять, что из предыдущего легко получается:",
                                "под девиз: \"Делай, как считаешь нужным, а я проверю\", я и осуществил этот переход:",
                                "подумай и получи вот это:",
                                "применяю очередной финт ушами и получаю:",
                                "данное преобразование я выполнил по правилу Дмитрия Гущин, и получил:",
                                "этот переход осуществляется с помощью перехода сознания в потусторонний мир, и получил:"};

int mk5::differenciator::dump_graph(void) {
  if (mytree.root == nullptr) {
    fprintf(stderr, "Your tree is clear, I can't dump nothing...\nStop, I CAN)))\n");

    return 0;
  }
  char file_name[MAX_STR] = "", command[COMMAND_LEN] = "";

  strcat(file_name, mytree.dump_name);
  strcat(file_name, ".dot");

  FILE* file = fopen(file_name, "wb");

  if (file == nullptr)
    return 0;

  fprintf(file, "digraph G{\n");

  char command_name[MAX_STR] = "";
  get_name_diff(mytree.root->value, mytree.root->type, command_name);
  fprintf(file, "node [color=\"#000000\", shape=record];\n");
  fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#FF019A\", label=\"<f0>  LEFT |<f1> '%s' | <f2> RIGHT \"];\n", (int)mytree.root, command_name);

  fprintf(file, "edge[color = \"black\"];\n");

  if (mytree.root->right != nullptr) {
    fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", (int)mytree.root, (int)(mytree.root->right));

    write_vertex_dump(mytree.root->right, mytree.size_tree - 1, file);
  }

  if (mytree.root->left != nullptr) {
    fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", (int)mytree.root, (int)(mytree.root->left));

    write_vertex_dump(mytree.root->left, mytree.size_tree - 1, file);
  }


  fprintf(file, "}");
  fclose(file);

  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\Graphviz\\bin\\dot.exe -Tpdf C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\TREE\\");
  strcat(command, mytree.dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);


  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\TREE\\");
  strcat(command, mytree.dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);

  return 1;
}

int mk5::differenciator::write_vertex_tex(vertex* node, const int num_node, FILE* file, const int type_bracket)
{
  switch (node->type) {
  case NUM:
    if (node->value < 0)
      fprintf(file, " (%lg) ", node->value);
    else
      fprintf(file, " %lg ", node->value);
    break;
  case VAR:
    fprintf(file, " %s ", myparser.myvar.get_var_name((int)node->value));
    break;
  case OPER:
    switch ((int)node->value) {
#define OPERATOR(name, num, diff, eval, tex, diff_tex)     \
        case num:                                          \
         tex                                               \
         break;
#include "operations.h"
#undef OPERATOR
    }
    break;
  case FUNC:
    switch ((int)node->value) {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) \
        case num:                                      \
         tex                                           \
         break;
#include "functions.h"
#undef FUNCTION
    }
    break;
  default:
    fprintf(stderr, "Something is going wrong...\n");

    return 0;
  }

  return 1;
}

/*! Print information of cur node.
 * \param[in]  point to node   - vetex* node
 * \param[in]  num of vertexes - int num_vertex
 * \param[out] none.
 */
int mk5::differenciator::write_vertex_dump(vertex* node, const int num_node, FILE *file) {

  if (node->right != nullptr) {
    char command_name[MAX_STR] = "";
    get_name_diff(node->value, node->type, command_name);
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0>  LEFT |<f1> '%s' | <f2> RIGHT \"];\n", (int)node, command_name);
    fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", (int)node, (int)(node->right));

    write_vertex_dump(node->right, num_node - 1, file);
  }

  if (node->left != nullptr) {
    char command_name[MAX_STR] = "";
    get_name_diff(node->value, node->type, command_name);
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0>  LEFT |<f1> '%s' | <f2> RIGHT \"];\n", (int)node, command_name);
    fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", (int)node, (int)(node->left));

    write_vertex_dump(node->left, num_node - 1, file);
  }

  if (node->right == nullptr && node->left == nullptr) {
    char command_name[MAX_STR] = "";
    get_name_diff(node->value, node->type, command_name);
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#89F09B\", label=\"<f0>  LEFT |<f1> '%s' | <f2> RIGHT \"];\n", (int)node, command_name);
  }

  return 1;
}/* End of 'print' fucntion */

int mk5::differenciator::dump_tex(void) {
  if (mytree.root == nullptr) {
    fprintf(stderr, "Your tree is clear, I can't dump nothing...\nStop, I CAN)))\n");

    return 0;
  }
  char file_name[MAX_STR] = "";

  strcat(file_name, mytree.dump_name);
  strcat(file_name, ".tex");

  FILE* file = fopen(file_name, "wb");

  if (file == nullptr)
    return 0;

  fprintf(file, TEX_INTRODUCTION);

  do {
    fprintf(file, "\\begin{equation}\n");

    write_vertex_tex(mytree.root, mytree.size_tree, file, NO_POW_PARENT);

    fprintf(file, "\n\\end{equation}\n");
  } while (simplify_vertex(mytree.root, mytree.size_tree) != 0);


  fprintf(file, "\\end{center}\n"
    "\\end{document}\n");
  fclose(file);

  char command[COMMAND_LEN] = "";

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\iconv\\iconv.exe -t UTF-8 -f CP1251 < C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, file_name);

  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "pdflatex ");
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, file_name);

  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, mytree.dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);

  return 1;
}

mk5::vertex* mk5::differenciator::write_article(FILE *file, vertex* node, int &num_node, int var_diff, const int type_bracket) {
  switch (node->type) {
  case NUM:
    num_node++;
    return new vertex(nullptr, nullptr, 0, NUM);
  case OPER:
    switch ((int)node->value) {
#define OPERATOR(name, num, diff, eval, tex, diff_tex) \
          case num:                                    \
            diff_tex
#include "operations.h"
#undef OPERATOR
    default:
      fprintf(stderr, "\nUnknown type of operator (%d)\n", (int)node->value);
      return nullptr;
    }
  case FUNC:
    switch ((int)node->value) {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) \
          case num:                                    \
            diff_tex
#include "functions.h"
#undef FUNCTION
    default:
      fprintf(stderr, "\nUnknown function (%d)\n", (int)node->value);
      return nullptr;
    }
  case VAR:
    num_node++;
    if (var_diff == (int)node->value)
      return new vertex(nullptr, nullptr, 1, NUM);

    return new vertex(nullptr, nullptr, 0, NUM);

  default:
    fprintf(stderr, "\nOh my fucking god, all is going to hell\nUnknown type of vertex...\n");
    return nullptr;
  }

}

mk5::vertex* mk5::differenciator::diff(vertex* node, int &num_node, int var_diff) {
  switch (node->type) {
  case NUM:
    num_node++;
    return new vertex(nullptr, nullptr, 0, NUM);
  case OPER:
    switch ((int)node->value) {
#define OPERATOR(name, num, diff, eval, tex, diff_tex) \
          case num:                                    \
            diff
#include "operations.h"
#undef OPERATOR
    default:
      fprintf(stderr, "\nUnknown type of operator (%d)\n", (int)node->value);
      return nullptr;
    }
  case FUNC:
    switch ((int)node->value) {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) \
          case num:                                    \
            diff
#include "functions.h"
#undef FUNCTION
    default:
      fprintf(stderr, "\nUnknown function (%d)\n", (int)node->value);
      return nullptr;
    }
  case VAR:
    num_node++;
    if (var_diff == (int)node->value)
      return new vertex(nullptr, nullptr, 1, NUM);

    return new vertex(nullptr, nullptr, 0, NUM);

  default:
    fprintf(stderr, "\nOh my fucking god, all is going to hell\nUnknown type of vertex...\n");
    return nullptr;
  }
}

double mk5::differenciator::eval(const vertex* node) {

  switch (node->type) {
  case NUM:
    return node->value;
  case VAR:
    return myparser.myvar.get_var_val((int)node->value);
  case OPER:
    switch ((int)node->value) {
#define OPERATOR(name, num, diff, eval, tex, diff_tex) \
      case num:                                        \
        eval
#include "operations.h"
#undef OPERATOR
    default:
      fprintf(stderr, "\nUnknown type of operator (%d)\n", (int)node->value);
      return NAN;
    }
  case FUNC:
    switch ((int)node->value) {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) \
    case my_##name:                                    \
      eval
#include "functions.h"
#undef FUNCTION
    default:
      fprintf(stderr, "\nUnknown function (%d)\n", (int)node->value);
      return NAN;
    }
  default:
    fprintf(stderr, "\nOh my fucking god, all is going to hell\n");
    return NAN;
  }
}/* End of 'eval' function */


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

#define KILL_L delete node->left
#define KILL_R delete node->right


int mk5::differenciator::simplify_vertex(vertex* node, const int num_node) {
  switch (TYPE) {
  case NUM:
    break;
  case OPER:
    switch ((int)VALUE) {
    case ADD:
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
      else if (L_TYPE == VAR && R_TYPE == VAR && R_VALUE == L_VALUE) {
        VALUE = MUL;
        R_VALUE = 2;
        R_TYPE = NUM;

        return 1;
      }
      else if (L_TYPE == NUM && L_VALUE == 0) {
        KILL_L;

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

        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;
        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }

      return simplify_vertex(L, num_node) + simplify_vertex(R, num_node);
    }
    case MUL:
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
      else if (L_TYPE == VAR && R_TYPE == VAR && R_VALUE == L_VALUE) {
        VALUE = POW;
        R_VALUE = 2;
        R_TYPE = NUM;

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

      return simplify_vertex(L, num_node) + simplify_vertex(R, num_node);
    }
    case DIV:
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
      else if (L_TYPE == VAR && R_TYPE == VAR && R_VALUE == L_VALUE) {
        VALUE = 1;
        TYPE = NUM;
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

      return simplify_vertex(L, num_node) + simplify_vertex(R, num_node);
    }
    case SUB:
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
      else if (L_TYPE == VAR && R_TYPE == VAR && R_VALUE == L_VALUE) {
        KILL_L;
        KILL_R;
        TYPE = NUM;
        VALUE = 0;
        L = nullptr;
        R = nullptr;

        return 1;
      }
      else if (L_TYPE == NUM && L_VALUE == 0) {
        KILL_L;
        TYPE = FUNC;
        VALUE = my_neg;
        L = nullptr;

        return 1;
      }
      else if (R_TYPE == NUM && R_VALUE == 0) {
        KILL_R;

        TYPE = L_TYPE;
        VALUE = L_VALUE;
        R = L_R;
        vertex* left = L;
        L = L_L;

        delete left;

        return 1;
      }

      return simplify_vertex(L, num_node) + simplify_vertex(R, num_node);
    }
    case POW:
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

      return simplify_vertex(L, num_node) + simplify_vertex(R, num_node);
    }
    break;
    case FUNC:
      simplify_vertex(R, num_node);
      break;
    case VAR:
      break;
    }
  }
  return 0;
}

int mk5::differenciator::find_priority_lowest(vertex* node) {

  if (node->type != OPER || (node->type == OPER && node->value == POW))
    return '*';

  if (node->type == OPER && (node->value == '-' || node->value == '+'))
    return '+';

  int left_priority = 256;
  int right_priority = 256;

  if (node->left != nullptr)
    left_priority = find_priority_lowest(node->left);

  if (node->right != nullptr)
    right_priority = find_priority_lowest(node->right);

  if (node->right == nullptr && node->left == nullptr)
    return (int)node->value;

  if (left_priority == '+' || left_priority == '-' || right_priority == '+' || right_priority == '-')
    return '+';

  return '*';
}

int mk5::differenciator::find_var_vertex(vertex* node, int num_var) {

  if ((int)node->value == num_var && node->type == VAR)
    return 1;

  if (node->left != nullptr)
    if (find_var_vertex(node->left, num_var))
      return 1;

  if (node->right != nullptr)
    if (find_var_vertex(node->right, num_var))
      return 1;

  return 0;
}

long long factorial(int num) {
  if (num <= 1)
    return 1;
  else
    return num * factorial(num - 1);
}

mk5::vertex* mk5::differenciator::add_polynom(mk5::vertex* root, double value, int degree, int num_var_diff) {

  vertex* coeff       = new vertex(nullptr, nullptr, value / factorial(degree), NUM);
  vertex* var         = new vertex(nullptr, nullptr, num_var_diff, VAR);
  vertex* num         = new vertex(nullptr, nullptr, myparser.myvar.table_var[num_var_diff].value, NUM);
  vertex* sub         = new vertex(var, num, SUB, OPER);
  vertex* degree_vert = new vertex(nullptr, nullptr, degree, NUM);
  vertex* pow         = new vertex(sub, degree_vert, POW, OPER);
  vertex* mul         = new vertex(coeff, pow, MUL, OPER);

  return new vertex(mul, root, ADD, OPER);
}

mk5::vertex* mk5::differenciator::make_taylor(int degree, int num_var_diff) {
  vertex* root = new vertex(nullptr, nullptr, eval(mytree.root), NUM);

  for (int i_diff = 1; i_diff < degree; i_diff++) {

    vertex* new_root = diff(mytree.root, mytree.size_tree, num_var_diff);
    mytree.delete_tree(mytree.root);
    mytree.root = new_root;
    
    while (simplify_vertex(mytree.root, mytree.size_tree) != 0)
      ;

    root = add_polynom(root, eval(mytree.root), i_diff, num_var_diff);
  }
  return root;
}

int mk5::differenciator::make_graphic(FILE *file, int num_var_diff) {
  fprintf(file, "\\begin{tikzpicture}\n"
                "\\begin{axis}[title = График познания этого мира,\n"
                "xlabel = Эта XXX,\n"
                "ylabel = Эта YYY]\n"
                "\\addplot coordinates{\n");
  double value = myparser.myvar.table_var[num_var_diff].value;
  
  if (value == 0)
    myparser.myvar.table_var[num_var_diff].value = -1;
  else
    myparser.myvar.table_var[num_var_diff].value = value / 2.0;

  for (int i_point = 1; i_point < 50; i_point++) {
    if (value != 0)
      myparser.myvar.table_var[num_var_diff].value += value / 50.0;
    else
      myparser.myvar.table_var[num_var_diff].value += 1/25.0;
    
    double value_func = eval(mytree.root);

    if (!isnan(value_func))
      fprintf(file, "(%lf, %lf)\n", myparser.myvar.table_var[num_var_diff].value, value_func);
    
  }
  myparser.myvar.table_var[num_var_diff].value = value;
  
  fprintf(file, "};\n"
                "\\end{axis}\n"
                "\\end{tikzpicture}\n");

  return 1;
}

int mk5::differenciator::love_math(char *expr, char *var) {
  strcpy(mytree.dump_name, "DEBUG");
  //Считал дерево
  mytree.root = myparser.GetG(expr);
  //Запомнил изначальную функцию
  vertex* old_root = mytree.root;
  
  char file_name[MAX_STR] = "";

  strcat(file_name, mytree.dump_name);
  strcat(file_name, ".tex");

  FILE* file = fopen(file_name, "wb");

  if (file == nullptr)
    return 0;

  //Introdustion
  fprintf(file, TEX_INTRODUCTION);
  //Размер и переменная дифференцирования
  int num_tree = 0;
  mk5::line var_diff = {(int)strlen(var), var};
 
  //Пишу изначальное уравнение
  int num_var_diff = myparser.myvar.find_var(var_diff);
  fprintf(file, "\\textbf{Первая научная теория о том, как \\sout{расчленять} брать производную:}\n");
  fprintf(file, "\\begin{equation}\n");
  write_vertex_tex(mytree.root, mytree.size_tree, file, NO_POW_PARENT);
  fprintf(file, "\n\\end{equation}\n");

  //Пишу статью
  vertex* node = write_article(file, mytree.root, num_tree, num_var_diff, NO_POW_PARENT);

  //На выходе я получил производную
  mytree.size_tree = num_tree;
  mytree.root = node;

  fprintf(file, "\n\\newpage\n\\textbf{В итоге после всех преобразований я получил следующее выражение}:\n");
  fprintf(file, "\\begin{equation}\n");
  write_vertex_tex(mytree.root, mytree.size_tree, file, NO_POW_PARENT);
  fprintf(file, "\n\\end{equation}\n");

  fprintf(file, "\n\\textbf{Предлагаю немного это упростить}:\n");
  while (simplify_vertex(mytree.root, mytree.size_tree) != 0)
    ;

  fprintf(file, "\\begin{equation}\n");
  
  write_vertex_tex(mytree.root, mytree.size_tree, file, NO_POW_PARENT);
  fprintf(file, "\n\\end{equation}\n");

  fprintf(file, "\nВсем большое спасибо за внимание!\n\\end{center}\n");
  
  fprintf(file, "\nСписок литературы:\n"
    "\\newline1. \\href{https://matan.math.msu.su/media/uploads/2020/03/V.A.Zorich-Kniga-I-10-izdanie-Corr.pdf}{В.А. Зорич Математический Анализ Том I} \\newline $<<https://matan.math.msu.su/media/uploads/2020/03/V.A.Zorich-Kniga-I-10-izdanie-Corr.pdf>>$ \n"
    "\\newline2. \\href{https://vk.com/gushchin_dima}{Дмитрий Гущин} \\newline $<<https://vk.com/gushchin_dima>>$ \n" 
    "\\newline3. \\href{http://www.school30.spb.ru}{родная сошка} \\newline $<<http://www.school30.spb.ru>>$ \n"
    "\\newline4. \\href{https://github.com/kreininmv}{Эта я} \\newline $<<https://github.com/kreininmv>>$ \n");
  
  myparser.myvar.add_value();
  fprintf(file, "\\newpage\n");
  //Делаю график обычной функции
  make_graphic(file, num_var_diff);

  //Удаляю дерево с произвоодным
  mytree.delete_tree(mytree.root);
  //Возвращаюсь к изначальной фукнции
  mytree.root = old_root;

  
  
  vertex* root = make_taylor(6, num_var_diff);
  mytree.delete_tree(mytree.root);
  
  mytree.root = root;
  while (simplify_vertex(mytree.root, mytree.size_tree) != 0)
    ;
  fprintf(file, "\n\nРазложение в ряд Тейлора в окрестности точки %s = %lf:\n", myparser.myvar.table_var[num_var_diff].name, myparser.myvar.table_var[num_var_diff].value);
  fprintf(file, "\\begin{equation}\n");
  write_vertex_tex(mytree.root, mytree.size_tree, file, NO_POW_PARENT);
  fprintf(file, "\n\\end{equation}\n");
  
  make_graphic(file, num_var_diff);
  
  fprintf(file, "\\end{document}\n");
  
  
  fclose(file);

  char command[COMMAND_LEN] = "";

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\iconv\\iconv.exe -t UTF-8 -f CP1251 < C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, file_name);

  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "pdflatex ");
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, file_name);

  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T06DIFF\\T06DIFF\\temp_");
  strcat(command, mytree.dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);

  return 1;
}

int mk5::differenciator::convert_debug(char* expr) {

  strcpy(mytree.dump_name, "DEBUG");
  mytree.root = myparser.GetG(expr);
  
  dump_tex();

  int num_tree = 0;
  mk5::line static_var = {2, new char[2]};
  strcpy(static_var.str, "x");
  int num_static_var = myparser.myvar.find_var(static_var);
  vertex* node = diff(mytree.root, num_tree, num_static_var);
  mytree.delete_tree(mytree.root);

  mytree.size_tree = num_tree;
  mytree.root = node;

  dump_tex();

  delete[] static_var.str;

  return 0;
}

int mk5::differenciator::get_name_diff(double value, type_vertex type, char* str) {

  switch (type) {
  case NUM:
    sprintf(str, "%lf", value);
    return 1;
  case VAR:
    sprintf(str, "%s", myparser.myvar.get_var_name((int)value));
    return 1;
  case OPER:
    switch ((int)value) {
#define OPERATOR(name, num, diff, eval, tex, diff_tex) \
    case num:                                          \
      sprintf(str, "%c", num);                         \
                                                       \
      return 1;                       
#include "operations.h"
#undef OPERATOR
    default:
      fprintf(stderr, "Unknown operation (%lf)", value);
    }
  case FUNC:
    switch ((int)value) {
#define FUNCTION(name, num, diff, eval, tex, diff_tex) \
    case num:                                          \
      sprintf(str, "%s", #name);                       \
                                                       \
      return 1;                      
#include "functions.h"
#undef FUNCTION
    default:
      fprintf(stderr, "Unknown function (%lf)", value);
      return 1;
    }
  case nill:
    sprintf(str, "%s", "nill");
    return 1;
  default:
    fprintf(stderr, "Unknown command (%lf)", value);
    return 1;
  }
}