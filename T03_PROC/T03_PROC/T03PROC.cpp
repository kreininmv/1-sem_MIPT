/* FILE NAME   : T03PROC.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

/* Plan
$* 1. �������� ������� ���������� ������� ����� ����: push, pop, add, sub, mul, div, neg, 
$*                                                    out, sin, cos, sqrt, in, dump, hack (���� ��� ������)
$* 2. �������� ������� ��� ���������� ���� ������� ����� ����.
$* 3. �������� ��� ��� ��������������� ����� ����� � ��������� "��������" ���.
$* 4. �������� �������, ������� ����������� �������� ��� � "����������" (listing).
$* 5. ����� ������� ��� ������� �� � ��������� ����, � ���������� ������� 
$*      (�.�. ��� ����� ����� ����� ����� ���������� ���� � ����� ������� codes).
$* 6. ����� ������� ������� � ������������� ������� 
$*      (��� ����� ����� ���������, ��� � ������� ����� 1 ����, � � value 4 �����).
$* 7. ������� ��������� CPU, � �������� codes ������� codes, 
$*      ������� ����������, ������ ������ � �������� ���������� ax, bx, cx, dx.
$* �X. ������� ������� ��� �����. 
 */ 

/* Plan �� ��������:
++* 1. �������� ����� � ��������� �������, � ��� ��������� �������, � ���������, � ����������� �������.
++* 2. ������� ����� �� ��� ������, �.�. ���������� �� �����, ��� � ������ �� ��� ������ � ������� �� ���������.
++* 3. ������� ������. �� ��������� � ��� ������.
+-* 4. ������� �������, ��������� ����� ��������� � ������ ������ ������. 
++* 5. ������� ����. ��.
--* 6. ��������� ���� ���������� � ���������� ����� �� ����������. 
+-* 7. ��������� ������ � �����������.
-+* 8. ��������!!!
*/

#include "PROC.h"

void Render(void)
{
  mk5::proc myproc;

  myproc.RInstFileAsme("videoram.mk5asme");
  myproc.DoInstruction();
  Copy(myproc.RAM);
}

/*! Main function.
 * \param[in]  none.
 * \param[out] none.
 */
int main(void)
{
  SetDbgMemHooks();
  
  Render();
  

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  
  glutInitWindowSize(FRAME_W, FRAME_H);
  glutInitWindowPosition(0, 0);
  handle = glutCreateWindow("PML #30");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutMainLoop();

  printf("Hello from Spb\n");
   return 0;
}/* End of 'main' function */