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

#define NUMBER(a) new vertex(a)
#define ONE       new vertex(1)
#define ZERO      new vertex(0.0)



//sin
FUNCTION(0,
  {
    return D_MUL(D_COS(cL), dL);
  })
//cos
FUNCTION(1,
  {
    return D_MUL(D_MUL(NUMBER(-1),(D_SIN(cL))), dL);
  })
//sqrt
FUNCTION(2,
  {
    return D_MUL(D_DIV(ONE, D_MUL(NUMBER(2), D_SQRT(cL))), dL);
  })
//ln
FUNCTION(3,
  {
    return D_MUL(D_DIV(ONE, cL), dL);
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
#undef R 
#undef L
#undef D_SIN
#undef D_COS
#undef D_SQRT
#undef D_LN
#undef NUMBER
#undef ONE   
#undef ZERO  