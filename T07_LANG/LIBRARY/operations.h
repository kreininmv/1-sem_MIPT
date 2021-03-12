//name, num, diff, eval

#define D_MUL(a, b) new vertex(OPER, '*', a, b)
#define D_ADD(a, b) new vertex(OPER, '+', a, b)
#define D_SUB(a, b) new vertex(OPER, '-', a, b)
#define D_DIV(a, b) new vertex(OPER, '/', a, b)
#define D_POW(a, b) new vertex(OPER, '^'. a, b)

#define cR new vertex(node->right)
#define cL new vertex(node->left)
#define dR diff(node->right, var_diff)
#define dL diff(node->left, var_diff)
#define L  node->left
#define R  node->right


#define D_SIN(x)  new vertex(FUNC, "sin" , 3, 0, x, nullptr)
#define D_COS(x)  new vertex(FUNC, "cos" , 3, 0, x, nullptr)
#define D_SQRT(x) new vertex(FUNC, "sqrt", 4, 0, x, nullptr)
#define D_LN(x)   new vertex(FUNC, "ln",  2, 0,  x, nullptr)
#define D_EXP(x)  new vertex(FUNC, "exp", 3, 0,  x, nullptr)

#define ZERO      new vertex(0.0)
#define NUMBER(a) new vertex(a)
#define ONE       new vertex(1)


OPERATOR('+',
  {
    return D_ADD(dL, dR);
  })
OPERATOR('-',
  {
    return D_SUB(dL, dR);
  })
OPERATOR('/',
  {
    return D_SUB(D_DIV(dL, cR), D_MUL(cL, D_MUL(dR, D_DIV(ONE, D_MUL(cR, cR)))));
  })
OPERATOR('*',
  {
    return D_ADD(D_MUL(dL, cR), D_MUL(cL, dR));
  })
OPERATOR('^',
  {
    return nullptr;
    //int is_var_left  = find_var_vertex(L, var_diff);
    //int is_var_right = find_var_vertex(R, var_diff);
    //
    //if (is_var_left == 0 && is_var_right == 0)
    //  return ZERO;
    //else if (is_var_left == 1 && is_var_right == 0)
    //  return D_MUL(D_MUL(cR, D_POW(cL, D_SUB(cR, ONE))), dL);
    //else if (is_var_left == 0 && is_var_right == 1)
    //  return D_MUL(D_MUL(D_LN(cL), D_POW(cL, cR)), dR);
    //else
    //  return D_MUL( D_POW(cL, cR), D_ADD( D_MUL(dR, D_LN(cL) ),  D_MUL(cR,  diff(D_LN(cL), num_node, var_diff))));
  })
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
#undef D_SIN
#undef D_COS
#undef D_SQRT
#undef D_LN
#undef D_EXP
#undef ZERO  
#undef NUMBER
#undef ONE