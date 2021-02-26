#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <windows.h>

#define PRECISION           5e-4
#define SOLVE_ANY_SOLUTION  0x1E
#define TRUE                1

/* Check inaccuracy of 2 numbers.
 * \param[in] first number  - double a;
 * \param[in] second number - double b;
 * \param[out] true or false.
 */
int Compare(double a, double b);

/* Check inaccuracy of numbers.
 * \param[in]  first group of number  - double a0, double b0;
 * \param[in]  second group of number - double a1, double b1;
 * \param[out] true or false.
 */
int Compare2(double a0, double b0, double a1, double b1);

/* Solve linear equation function.
 * \param[in]  coefficient b       - double b;
 * \param[in]  coefficient c       - double c;
 * \param[in]  pointer to solution - double *sol;
 * \param[out] number of desicions.
 */
int SolveLin(double b, double c, double *sol);

/* Solve quadratic ecuation function.
 * \param[in]  coefficient a              - double a;
 * \param[in]  coefficient b              - double b;
 * \param[in]  coefficient c              - double c;
 * \param[in]  pointer to first solution  - double *sol1;
 * \param[in]  pointer to second solution - double *sol2;
 * \param[out] number of desicions.
 */
int SolveQuad(double a, double b, double c, double *sol1, double *sol2);

/* Solve equation function.
 * \param[in]  coefficient a              - double a;
 * \param[in]  coefficient b              - double b;
 * \param[in]  coefficient c              - double c;
 * \param[in]  pointer to first solution  - double *sol1;
 * \param[in]  pointer to second solution - double *sol2;
 * \param[out] number of desicions.
 */
int SolveEq(double a, double b, double c, double *sol1, double *sol2);

/* Print solution function.
 * \param[in] num of roots                     - int numofroot;
 * \param[in] point to first root of equation  - double *sol1;
 * \param[in] point to second root of equation - double *sol2;
 * \param[out] None.
 */
void PrintSolution(int numofroot, double *sol1, double *sol2);

/* Input function.
 * \param[in] coefficient a - double a;
 * \param[in] coefficient b - double b;
 * \param[in] coefficient c - double c;
 * \param[out] None.
 */
void Input(double *a, double *b, double *c);

/* Test function.
 * \param[in] coefficient a           - double a;
 * \param[in] coefficient b           - double b;
 * \param[in] coefficient c           - double c;
 * \param[in] first root of equation  - double sol1;
 * \param[in] second root of equation - double sol2;
 * \param[in] num of roots            - int numroots;
 * \param[out] true or false.
 */
int Test(double a, double b, double c, double sol1, double sol2, int numroots);

/* Unit test function.
 * \param[in]   None.
 * \param[out]  None.
 */
void UTest(void);


/*! Main function.
 * \param[in]  None.
 * \param[out] None.
 */
int main(void)
{
  double a = 0xEF, b = 0xEF, c = 0xEF, sol1 = 0xEF, sol2 = 0xEF;

  Input(&a, &b, &c);
  PrintSolution(SolveEq(a, b, c, &sol1, &sol2), &sol1, &sol2);
  UTest();
  getchar();
  getchar();

  return 0;
} /* End of 'main' function */

/*! Check inaccuracy of 2 numbers.
 * \param[in] first number  - double a;
 * \param[in] second number - double b;
 * \param[out] true or false.
 */
int Compare(double a, double b)
{
  return (fabs(a - b) <= PRECISION);
} /* End of 'Compare' function */

/*! Check inaccuracy of numbers.
 * \param[in]  first group of number  - double a0, double b0;
 * \param[in]  second group of number - double a1, double b1;
 * \param[out] true or false.
 */
int Compare2(double a0, double b0, double a1, double b1)
{
  if ((Compare(a0, a1) && Compare(b0, b1)))
    return 1;

  return 0;
} /* End of 'Compare' function */

/*! Solve linear equation function.
 * \param[in]  coefficient b       - double b;
 * \param[in]  coefficient c       - double c;
 * \param[in]  pointer to solution - double *sol;
 * \param[out] number of desicions.
 */
int SolveLin(double b, double c, double *sol)
{
  if (Compare(b, 0) && Compare(c, 0))
    return SOLVE_ANY_SOLUTION;

  if (Compare(b, 0))
    return 0;

  *sol = -c / b;

  return 1;
} /* End of 'SolveLin' function */

/*! Solve quadratic ecuation function.
 * \param[in]  coefficient a              - double a;
 * \param[in]  coefficient b              - double b;
 * \param[in]  coefficient c              - double c;
 * \param[in]  pointer to first solution  - double *sol1;
 * \param[in]  pointer to second solution - double *sol2;
 * \param[out] number of desicions.
 */
int SolveQuad(double a, double b, double c, double *sol1, double *sol2)
{
  if (Compare(c, 0))
  {
    *sol1 = 0;
    SolveLin(a, b, sol2);
    return (Compare(*sol1, *sol2)) ? 1 : 2;
  }

  double dis = b * b - 4 * a * c;
  if (dis < 0 || (Compare(dis, 0) != false))
    return 0;

  dis = sqrt(dis);
  *sol1 = (-b + dis) / (2 * a);
  *sol2 = (-b - dis) / (2 * a);

  return (Compare(dis, 0)) ? 1 : 2;
} /* Enf of 'SolveQuad' function */

/*! Solve equation function.
 * \param[in]  coefficient a              - double a;
 * \param[in]  coefficient b              - double b;
 * \param[in]  coefficient c              - double c;
 * \param[in]  pointer to first solution  - double *sol1;
 * \param[in]  pointer to second solution - double *sol2;
 * \param[out] number of desicions.
 */
int SolveEq(double a, double b, double c, double *sol1, double *sol2)
{
  assert(isfinite(a));
  assert(isfinite(b));
  assert(isfinite(c));

  assert(sol1 != NULL);
  assert(sol2 != NULL);
  assert(sol1 != sol2);

  if (Compare(a, 0))
    return SolveLin(b, c, sol1);

  return SolveQuad(a, b, c, sol1, sol2);
} /* End of 'SolveEq' function */

/*! Menu function.
 * \param[in] num of roots                     - int numofroot;
 * \param[in] point to first root of equation  - double *sol1;
 * \param[in] point to second root of equation - double *sol2;
 * \param[out] None.
 */
void PrintSolution(int numofroot, double *sol1, double *sol2)
{
  #ifdef DEBUG
  printf("Hello. You are in the Menu.\n\n");
  printf("In usual Menu, you can select any options you want.\n\n");
  printf("In This Menu, you can NOT select anything.\n\n");
  printf("Instead, I will select anything for you.\n\n");
  printf("You can anly sit still, wait and Look.\n\n");
  printf("(Note that this is not a glLookAt().)\n\n");
  printf("Have a nice OpenGL.\n\n");
  #endif


  switch (numofroot)
  {
  case SOLVE_ANY_SOLUTION:
    printf("Any number is solution!\n");
    break;

  case 0:
    printf("Equation has no solutions!\n");
    break;

  case 1:
    printf("Equation has 1 solution, x = %lg\n", *sol1);
    break;

  case 2:
    printf("Equation has 2 solutions, x1 = %lg and x2 = %lg\n", *sol1, *sol2);
    break;

  default:
    printf("Something going wrong...\n");
    break;
  }
}
/* End of 'Menu' function */

/*! Input function.
 * \param[in] coefficient a - double a;
 * \param[in] coefficient b - double b;
 * \param[in] coefficient c - double c;
 * \param[out] None.
 */
void Input(double *a, double *b, double *c)
{
  int check = 0;

  printf("Progrram of solving equation, enter coefficients, a b c:");
  check = scanf("%lg %lg %lg", a, b, c);

  while (check != 3)
  {
    printf("\nSomething going wrong, please enter numbers!!!\n");
    printf("Prrrrogrram offf sssolving equation!!!, enter!!!! coefficients!!!!! I SAY!!!, a b c:");

    while (getchar() != '\n')
      ;

    check = scanf("%lg %lg %lg", a, b, c);
  }
  printf("Oh God. You finally did it.\n");
}
/* Enf of 'Input' function */

/*! Test function.
 * \param[in] coefficient a           - double a;
 * \param[in] coefficient b           - double b;
 * \param[in] coefficient c           - double c;
 * \param[in] first root of equation  - double sol1;
 * \param[in] second root of equation - double sol2;
 * \param[in] num of roots            - int numroots;
 * \param[out] true or false.
 */
int Test(double a, double b, double c, double sol1, double sol2, int numroots)
{
  double localsol1 = 0, localsol2 = 0;

  /* Count of roots are the same and one of three conditions are true:
   * 1) count of roots - zero
   * 2) count of roots - one and root is similar
   * 3) count of roots - two and roots are similar
   * 4) count of roots - SOLVE_ANY_SOLUTION (0x1E)
   */
  if ((numroots == SolveEq(a, b, c, &localsol1, &localsol2))
    && ((numroots == 0)
      || (numroots == 1 && Compare(localsol1, sol1))
      || (numroots == 2 && Compare2(sol1, sol2, localsol1, localsol2))
      || (numroots == SOLVE_ANY_SOLUTION)))
    return 1;

  return 0;
}/* End of 'Test' function */

/*! Unit test function.
 * \param[in]   None.
 * \param[out]  None.
 */
void UTest(void)
{
  double a = 0, b = 0, c = 0, sol1 = 0, sol2 = 0;
  int i = 1, numr = 0;

  FILE* FIn = fopen("tests.dat", "r");
  FILE* FOut = fopen("results.dat", "w");

  if (FIn == NULL)
  {
    printf("We can't find the file - tests.dat\n");
    return;
  }
  if (FOut == NULL)
  {
    printf("We can't create this file - results.dat\n");
    return;
  }

  while (fscanf(FIn, "%lg %lg %lg %lg %lg %d", &a, &b, &c, &sol1, &sol2, &numr) == 6)
  {
    if (Test(a, b, c, sol1, sol2, numr) != TRUE)
      fprintf(FOut, "TEST N - %i ERROR!!\n", i);
    else
      fprintf(FOut, "TEST N - %i GOOD!!!\n", i);

    i++;
  }

  fclose(FIn);
  fclose(FOut);
} /* End of 'UTest' function */
