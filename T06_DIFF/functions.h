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

#define NUMBER(a) new vertex(nullptr, nullptr, a, NUM)
#define ONE       new vertex(nullptr, nullptr, 1, NUM)
#define ZERO      new vertex(nullptr, nullptr, 0, NUM)

#define BEATIFUL_BRACKETS_0 if(type_bracket != POW_PARENT) \
                              fprintf(file, "{}");

FUNCTION(sin,  1, 
  {
    return D_MUL(D_COS(cR), dR);
  }, 
  {
    return sin(eval(R));
  },
  {
    
    fprintf(file, " \\newsin{(");
    WRITE_VERTEX(R, type_bracket);
  
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n(");
    vertex *left = D_COS(cR);
    WRITE_VERTEX(left, NO_POW_PARENT);
    fprintf(file, ") \\cdot \\bigg(");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    return D_MUL(left, ARTICLE(R, NO_POW_PARENT));
  })
FUNCTION(cos,  2, 
  {
    return D_MUL(D_NEG(D_SIN(cR)), dR);
  }, 
  {
    return cos(eval(R));
  },
  {
    fprintf(file, " \\newcos{(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n(");
    vertex *left = D_NEG(D_SIN(cR));
    WRITE_VERTEX(left, NO_POW_PARENT);
    fprintf(file, ") \\cdot \\bigg(");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");
    
    return D_MUL(left, ARTICLE(R, NO_POW_PARENT));
  })
FUNCTION(tg,   3, 
  {
    return D_DIV(ONE, D_MUL(dR, D_MUL(D_COS(cR), D_COS(cR))));
  }, 
  {
    return tan(eval(R));
  },
  {
    fprintf(file, " \\newtg{(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);

    fprintf(file, "\\begin{equation}\n");
    vertex* denumerator = D_POW(D_COS(cR), NUMBER(2));
    fprintf(file, " \\dfrac{\\bigg( ");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'}{");
    WRITE_VERTEX(denumerator, NO_POW_PARENT);
    fprintf(file, "}\n\\end{equation}\n");

    return D_DIV(ARTICLE(R, NO_POW_PARENT), denumerator);
  })
FUNCTION(ctg,  4, 
  {
    return D_NEG(D_DIV(ONE, D_MUL(dR, D_MUL(D_SIN(cR), D_SIN(cR)))));
  }, 
  {
    return 1 / tan(eval(R));
  },
  {
    
    fprintf(file, " \\newctg{(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);

    fprintf(file, "\\begin{equation}\n");
    vertex* denumerator = D_NEG(D_POW(D_COS(cR), NUMBER(2)));
    fprintf(file, " \\dfrac{\\bigg( ");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'}{");
    WRITE_VERTEX(denumerator, NO_POW_PARENT);
    fprintf(file, "}\n\\end{equation}\n");

    return D_DIV(ARTICLE(R, NO_POW_PARENT), denumerator);
  })
FUNCTION(sqrt, 5, 
  {
    return D_MUL(D_DIV(ONE, D_MUL(NUMBER(2), D_SQRT(cR))), dR);
  }, 
  {
    return sqrt(eval(R));
  }, 
  {
    fprintf(file, " \\newsqrt{(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");

    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);
    fprintf(file, "\\begin{equation}\n");
    vertex* denumerator = D_MUL(NUMBER(2), D_SQRT(cR));
    fprintf(file, " \\dfrac{\\bigg(");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'}{");
    WRITE_VERTEX(denumerator, NO_POW_PARENT);
    fprintf(file, "}\n\\end{equation}\n");

    return D_DIV(ARTICLE(R, NO_POW_PARENT), denumerator);
    
  })
FUNCTION(exp, 6, 
  {
   return D_MUL(D_EXP(cR), dR);
  },
  {
    return exp(eval(R));
  },
  {
    fprintf(file, " \\newexp{(");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
 
    fprintf(file, ")}");
    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\\bigg(\n");
    WRITE_VERTEX(CUR, type_bracket);
    
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);
    
    fprintf(file, "\\begin{equation}\n");
    WRITE_VERTEX(CUR, NO_POW_PARENT);
    
    fprintf(file, "\\cdot \\bigg(");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    return D_MUL(D_EXP(cR), ARTICLE(R, NO_POW_PARENT));
  })
FUNCTION(ln, 7, 
  {
    return D_MUL(D_DIV(ONE, cR), dR);
  },
  {
    if (eval(R) <= 0)
      return -INFINITY;

    return log(eval(R));
  },
  {
    if (R_TYPE == VAR) {
      fprintf(file, "\\newln{");
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
      fprintf(file, "}");
    }
    else {
      fprintf(file, "\\newln{(");
      WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
      fprintf(file, ")}");
    }

    BEATIFUL_BRACKETS_0;
  },
  {
    fprintf(file, "%s\n", introduction[rand() % num_introduction]);
    fprintf(file, "\\begin{equation}\n\\bigg(");
    WRITE_VERTEX(CUR, type_bracket);
    fprintf(file, "\\bigg)'\n\\end{equation}\n");

    fprintf(file, "%s\n", answer[rand() % num_answer]);

    fprintf(file, "\\begin{equation}\n");
    fprintf(file, " \\dfrac{\\bigg( ");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "\\bigg)'}{");
    WRITE_VERTEX(R, NO_POW_PARENT);
    fprintf(file, "}\n\\end{equation}\n");

    return D_DIV(ARTICLE(R, NO_POW_PARENT), cR);
  }
  )
FUNCTION(neg, '-',
  {
    return D_NEG(dR);
  },
  {
    return -eval(R);
  },
  {
    fprintf(file, "(-");
    WRITE_VERTEX(R, VALUE_BRACKET_TYPE);
    fprintf(file, ")");
  },
  {
   return D_NEG(dR);
  })

#undef BEATIFUL_BRACKETS_0
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
#undef ZERO