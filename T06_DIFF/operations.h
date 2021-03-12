//name, num, diff, eval

#define D_MUL(a, b) new vertex(a, b, '*', OPER)
#define D_ADD(a, b) new vertex(a, b, '+', OPER)
#define D_SUB(a, b) new vertex(a, b, '-', OPER)
#define D_DIV(a, b) new vertex(a, b, '/', OPER)
#define D_POW(a, b) new vertex(a, b, '^', OPER)

#define cR new vertex(node->right)
#define cL new vertex(node->left)
#define dR diff(node->right, num_node, var_diff)
#define dL diff(node->left, num_node, var_diff)
#define L  node->left
#define R  node->right
#define CUR node
#define L_TYPE node->left->type
#define R_TYPE node->right->type
#define L_VALUE node->left->value
#define R_VALUE node->right->value

#define VALUE_BRACKET_TYPE (int)node->value
#define ARTICLE(x, bracket) write_article(file, x, num_node, var_diff, bracket)
#define WRITE_VERTEX(x, bracket) write_vertex_tex(x, num_node, file, bracket)

#define D_SIN(x)  new vertex(nullptr, x, my_sin, FUNC)
#define D_COS(x)  new vertex(nullptr, x, my_cos, FUNC)
#define D_TG(x)   new vertex(nullptr, x, my_tg, FUNC)
#define D_CTG(x)  new vertex(nullptr, x, my_ctg, FUNC)
#define D_EXP(x)  new vertex(nullptr, x, my_exp, FUNC)
#define D_SQRT(x) new vertex(nullptr, x, my_sqrt, FUNC)
#define D_LN(x)   new vertex(nullptr, x, my_ln, FUNC)
#define D_NEG(x)  new vertex(nullptr, x, '-', FUNC)

#define ZERO      new vertex(nullptr, nullptr, 0, NUM)
#define NUMBER(a) new vertex(nullptr, nullptr, a, NUM)
#define ONE       new vertex(nullptr, nullptr, 1, NUM)


OPERATOR(ADD, '+', 
  {
    num_node += 3;
    return D_ADD(dL, dR);
  }, 
  {
    return eval(L) + eval(R);
  },
  {
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, " + ");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, "\\bigg)' + \\bigg(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");
    
    return D_ADD(ARTICLE(L, NO_POW_PARENT),
                 ARTICLE(R, NO_POW_PARENT));
  })
OPERATOR(SUB, '-', 
  {
    num_node += 3;
    return D_SUB(dL, dR);
  },
  {
    return eval(L) - eval(R);
  },
  {
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, " - ");
    
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
  },
  {
    
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");
    
    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, "\\bigg)' - \\bigg(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    return D_SUB(ARTICLE(L, NO_POW_PARENT),
                 ARTICLE(R, NO_POW_PARENT));
      
  })
OPERATOR(DIV, '/', 
  {
    num_node += 12;
    return D_SUB(D_DIV(dL, cR), D_MUL(cL, D_MUL(dR, D_DIV(ONE, D_MUL(cR, cR)))));
  }, 
  {
    return eval(L) / eval(R);
  },
  {
    fprintf(file, " \\dfrac{ ");
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, "}{");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, "}");
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    
    WRITE_VERTEX(CUR, type_bracket);

    fprintf(file, "\\bigg)'\n\\end{equation}\n");
    
    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n");
    fprintf(file, " \\dfrac{");
    fprintf(file, "\\bigg(");
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    fprintf(file, "\\bigg)' \\cdot ");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    
    fprintf(file, " - ");
    WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, " \\cdot \\bigg(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, "\\bigg)'");
    fprintf(file, "}{");
    vertex* lower = D_POW(cR, NUMBER(2));
    WRITE_VERTEX(lower, NO_POW_PARENT);
    fprintf(file, "}\n\\end{equation}\n");

    return D_DIV(D_SUB(D_MUL(ARTICLE(L, NO_POW_PARENT), cR), D_MUL(cL, ARTICLE(R, NO_POW_PARENT))), lower);
  })
OPERATOR(MUL, '*', 
  {
    num_node += 7;
    return D_ADD(D_MUL(dL, cR), D_MUL(cL, dR));
  }, 
  {
    return eval(L) * eval(R);
  },
  {
    int left_priority = find_priority_lowest(L);
    
    if (left_priority == '+' || left_priority == '-') {
      fprintf(file, "(");
      WRITE_VERTEX(L, VALUE_BRACKET_TYPE);

      fprintf(file, ")");
    }
    else 
      WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
    
    fprintf(file, " \\cdot ");

    int right_priority = find_priority_lowest(R);
    
    if (right_priority == '+' || right_priority == '-') {
      fprintf(file, "( ");
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
      fprintf(file, " )");
    }
    else
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\\bigg(\n");
    WRITE_VERTEX(L, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");
    
    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n");
    fprintf(file, "\\bigg(");
    WRITE_VERTEX(L, NO_POW_PARENT);
    fprintf(file, "\\bigg)' \\cdot ");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, " + ");
    WRITE_VERTEX(L, NO_POW_PARENT);
    fprintf(file, " \\cdot \\bigg(");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    return D_ADD(D_MUL(ARTICLE(L, NO_POW_PARENT), cR), 
                 D_MUL(cL, ARTICLE(R, NO_POW_PARENT)));
  })
OPERATOR(POW, '^', 
  {
    int is_var_left  = find_var_vertex(L, var_diff);
    int is_var_right = find_var_vertex(R, var_diff);

    if (is_var_left == 0 && is_var_right == 0)
      return ZERO;
    else if (is_var_left == 1 && is_var_right == 0)
      return D_MUL(D_MUL(cR, D_POW(cL, D_SUB(cR, ONE))), dL);
    else if (is_var_left == 0 && is_var_right == 1)
      return D_MUL(D_MUL(D_LN(cL), D_POW(cL, cR)), dR);
    else
      return D_MUL( D_POW(cL, cR), D_ADD( D_MUL(dR, D_LN(cL) ),  D_MUL(cR,  diff(D_LN(cL), num_node, var_diff))));
  },
  {
    return pow(eval(L), eval(R));
  },
  {
    if (L_TYPE == FUNC) {
      WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
      fprintf(file, "{");
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
      fprintf(file, "}");
    }
    else if ((L_TYPE == VAR && R_TYPE == NUM)
           || (L_TYPE == NUM && R_TYPE == VAR))
    {
      WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
      fprintf(file, "^{");
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
      fprintf(file, "}");
    }
    else {
      fprintf(file, "(");
      WRITE_VERTEX(L, VALUE_BRACKET_TYPE);
      fprintf(file, ")^{");
      WRITE_VERTEX(R, NO_POW_PARENT);
      fprintf(file, "}");
    }
  },
  {
    int is_var_left = find_var_vertex(L, var_diff);
    int is_var_right = find_var_vertex(R, var_diff);

    if (is_var_left == 0 && is_var_right == 0) {
      fprintf(file, "%s\n", introduction[rand() % num_introduction]);
      fprintf(file, "\\begin{equation}\\bigg(\n");
      WRITE_VERTEX(CUR, type_bracket);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");
      
      fprintf(file, "%s\n", answer[rand() % num_answer]);
      fprintf(file, "\\begin{equation}\n 0 \n\\end{equation}\n");

      return ZERO;
    }
    else if (is_var_left == 1 && is_var_right == 0) {
      
      fprintf(file, "%s\n", introduction[rand() % num_introduction]);
      fprintf(file, "\\begin{equation}\\bigg(\n");
      WRITE_VERTEX(CUR, type_bracket);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");

      vertex* main_part = D_POW(cL, D_SUB(cR, ONE));

      fprintf(file, "%s\n", answer[rand() % num_answer]);
      fprintf(file, "\\begin{equation}\n");
      WRITE_VERTEX(R, NO_POW_PARENT);
      fprintf(file, " \\cdot ");
      WRITE_VERTEX(main_part, NO_POW_PARENT);
      fprintf(file, " \\cdot \\bigg(");
      WRITE_VERTEX(L, NO_POW_PARENT);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");

      return D_MUL(D_MUL(cR, main_part), ARTICLE(L, NO_POW_PARENT));
    }
    else if (is_var_left == 0 && is_var_right == 1) {

      fprintf(file, "%s\n", introduction[rand() % num_introduction]);
      fprintf(file, "\\begin{equation}\\bigg(\n");
      WRITE_VERTEX(CUR, NO_POW_PARENT);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");

      vertex* left_part = D_LN(cL);
      vertex* main_part = D_POW(cL, cR);

      fprintf(file, "%s\n", answer[rand() % num_answer]);
      fprintf(file, "\\begin{equation}\n");
      WRITE_VERTEX(left_part, NO_POW_PARENT);
      fprintf(file, " \\cdot ");
      WRITE_VERTEX(main_part, NO_POW_PARENT);
      fprintf(file, " \\cdot \\bigg(");
      WRITE_VERTEX(R, NO_POW_PARENT);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");

      return D_MUL(D_MUL(left_part, main_part), ARTICLE(R, NO_POW_PARENT));
    }
    else {
      fprintf(file, "%s\n", introduction[rand() % num_introduction]);
      fprintf(file, "\\begin{equation}\n\\bigg(\n");
      WRITE_VERTEX(CUR, type_bracket);
      fprintf(file, "\\bigg)'\n\\end{equation}\n");
      
      vertex* main_part = D_POW(cL, cR);
      vertex* second_left = D_LN(cL);
      vertex* second_right = D_LN(L);
      
      fprintf(file, "%s\n", answer[rand() % num_answer]);
      fprintf(file, "\\begin{equation}\n");
      WRITE_VERTEX(main_part, NO_POW_PARENT);
      fprintf(file, " \\cdot ( \\bigg( ");
      WRITE_VERTEX(R, NO_POW_PARENT);
      fprintf(file, "\\bigg)' \\cdot ");
      WRITE_VERTEX(second_left, NO_POW_PARENT);
      fprintf(file, " + "); 
      WRITE_VERTEX(R, NO_POW_PARENT);
      fprintf(file, " \\cdot \\bigg(");
      WRITE_VERTEX(second_right, NO_POW_PARENT);
      fprintf(file, "\\bigg)')\n\\end{equation}\n");

      return D_MUL(main_part, D_ADD( D_MUL(ARTICLE(R, NO_POW_PARENT), second_left),
                                     D_MUL(cR, ARTICLE(second_right, NO_POW_PARENT))));
    }
  }
)

#undef D_MUL
#undef D_ADD
#undef D_SUB
#undef D_DIV
#undef D_POW
#undef cR
#undef cL
#undef dR
#undef dL
#undef L
#undef R
#undef CUR
#undef L_TYPE
#undef R_TYPE
#undef L_VALUE
#undef R_VALUE
#undef VALUE
#undef ARTICLE
#undef WRITE_VERTEX
#undef D_SIN
#undef D_COS
#undef D_TG
#undef D_CTG
#undef D_EXP
#undef D_SQRT
#undef D_LN
#undef D_NEG
#undef NUMBER
#undef ONE