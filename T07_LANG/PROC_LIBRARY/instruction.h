//DSL 
#define DO_PUSH(element)               StackPush(&stack, element);
#define DO_POP                         StackPop(&stack);
#define GET_ELEMENT                    *(code + cur_instr);
#define OPERTOR_IF(a, if_operator, b)  if (a if_operator b)

//Как называется виды копирования (когда в тупую, и когда нормально)
DEF_INSTR(push, 1,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);
    DO_PUSH(el);
  })

DEF_INSTR(pop , 2,
  {
    DO_POP;
  })

DEF_INSTR(add, 3,
  {
    double a = DO_POP;
    double b = DO_POP;
    DO_PUSH(a + b);
  })

DEF_INSTR(ax, 4,
  {
  })

DEF_INSTR(sub, 5,
  {
    double a = DO_POP;
    double b = DO_POP;
    DO_PUSH(b - a);
  })

DEF_INSTR(mul, 6,
  {
    double a = DO_POP;
    double b = DO_POP;
    DO_PUSH(a * b);
  })

DEF_INSTR(div, 7,
  {
    el = DO_POP;
    if (el == 0)
      return 0;
    double x = DO_POP;
    DO_PUSH(x / el);
  })

DEF_INSTR(bx, 8,
  {
  })

DEF_INSTR(sqrt, 9,
  {
    el = DO_POP;
    DO_PUSH(sqrt(el));
  })

DEF_INSTR(sin, 10,
  {
    el = DO_POP;
    DO_PUSH(sin(el));
  })

DEF_INSTR(cos, 11,
  {
    el = DO_POP;
    DO_PUSH(cos(el));
  })

DEF_INSTR(in, 12,
  {
    printf("My stupid slave, give me a number:");
    scanf("%lf", &el);
    DO_PUSH(el);
  })

DEF_INSTR(dump, 13,
  {
    Dump(stack);
  })

DEF_INSTR(hack, 14,
  {
    return 1;
  })

DEF_INSTR(hlt, 15,
  {
    return 0;
  })

DEF_INSTR(cx, 16,
  {
  })

DEF_INSTR(jmp, 17,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr = (size_t)el;

  })

DEF_INSTR(ja, 18,
  {
    double b = DO_POP;
    double a = DO_POP;

    OPERTOR_IF(a, > , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
      cur_instr += 8;

  })

DEF_INSTR(jae, 19,
  {
    double b = DO_POP;
    double a = DO_POP;
    
    OPERTOR_IF(a, >= , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
      cur_instr += 8;

  })

DEF_INSTR(jb, 20,
  {
    double b = DO_POP;
    double a = DO_POP;
    
    OPERTOR_IF(a, < , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
      cur_instr += 8;

  })

DEF_INSTR(jbe, 21,
  {
    double b = DO_POP;
    double a = DO_POP;
    
    OPERTOR_IF(a, <= , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
    cur_instr += 8;

  })

DEF_INSTR(je, 22,
  {
    double b = DO_POP;
    double a = DO_POP;
    
    OPERTOR_IF(a, == , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
      cur_instr += 8;

  })

DEF_INSTR(jne, 23,
  {
    double b = DO_POP;
    double a = DO_POP;
    
    OPERTOR_IF(a, != , b)
    {
      memcpy(&el, code + cur_instr, sizeof(double));
      cur_instr = (size_t)el;
    }
    else
      cur_instr += 8;

  })

DEF_INSTR(call, 24,
  {
    //Add current retreat from begin to calls stack
    StackPush(&calls, cur_instr + 8);
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr = (size_t)el;

  })

DEF_INSTR(rtrn, 25,
  {
    el = StackPop(&calls);
    cur_instr = (size_t)el;

  })

DEF_INSTR(putpixel, 26,
  {
    int color = (int)DO_POP;
    int y = (int)DO_POP;
    int x = (int)DO_POP;
    if (x > 0 && y > 0 && x <= 256 && y <= 256)
      RAM[x + y * 256] = color;
  })

DEF_INSTR(dx, 32,
  {
  })

DEF_INSTR(pow, 33,
  {
    double a = DO_POP;
    double b = DO_POP;
    el = pow(b, a); 
    DO_PUSH(el);
  })

DEF_INSTR(ln, 34, 
  {
    el = DO_POP;
    el = log(el);
    DO_PUSH(el);
  })
DEF_INSTR(exp, 35,
  {
    el = DO_POP;
    el = exp(el);
    DO_PUSH(el);
  })
DEF_INSTRRAM(push, 65,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    DO_PUSH(RAM[(int)el]);
  })

DEF_INSTRRAM(pop, 67,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    RAM[(int)el] = DO_POP;
  })

DEF_INSTR(sleap, 95,
  {
    getchar();
    getchar();
  })

DEF_INSTR(neg , 98, 
  {
    double a = DO_POP;
    a = -a;
    DO_PUSH(a);
  })

DEF_INSTR(out , 99, 
  {
    el = DO_POP;
    printf("%lf\n", el);
  })

DEF_INSTRARG(push, ax, 132, 
  {
    DO_PUSH(ax);
  })

DEF_INSTRARG(pop, ax, 134,
  {
    ax = DO_POP;
  })

DEF_INSTRARG(push, bx, 136, 
  {
    DO_PUSH(bx);
  })

DEF_INSTRARG(pop, bx, 138,
  {
    bx = DO_POP;
  })

DEF_INSTRARG(push, cx, 144, 
  {
    DO_PUSH(cx);
  })

DEF_INSTRARG(pop, cx, 146,
  {
    cx = DO_POP;
  })

DEF_INSTRARG(push, dx, 160, 
  {
    DO_PUSH(dx);
    
  })

DEF_INSTRARG(pop, dx , 162, 
  {
    dx = DO_POP;
  })

DEF_INSTRARGRAM(push, ax, 197,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    DO_PUSH(RAM[(int)(ax + el)]);
  })

DEF_INSTRARGRAM(pop, ax, 199,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    RAM[(int)(ax + el)] = DO_POP;
  })

DEF_INSTRARGRAM(push, bx, 201,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    DO_PUSH(RAM[(int)(bx + el)]);
  })

DEF_INSTRARGRAM(pop, bx, 203,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    RAM[(int)(bx + el)] = DO_POP;
  })

DEF_INSTRARGRAM(push, cx, 209,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    DO_PUSH(RAM[(int)(cx + el)]);
  })

DEF_INSTRARGRAM(pop, cx, 211,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    RAM[(int)(cx + el)] = DO_POP;
  })

DEF_INSTRARGRAM(push, dx, 225,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    
    cur_instr += sizeof(double);

    DO_PUSH(RAM[(int)(dx + el)]);
  })

DEF_INSTRARGRAM(pop, dx, 227,
  {
    memcpy(&el, code + cur_instr, sizeof(double));
    cur_instr += sizeof(double);

    RAM[(int)(dx + el)] = DO_POP;
  })

DEF_LABEL(label, 244,
  {
  })

DEF_INSTR(error, 255, 
  {
    return 0;
  })