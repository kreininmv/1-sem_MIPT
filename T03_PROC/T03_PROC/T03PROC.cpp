/* FILE NAME   : T03PROC.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

/* Plan
$* 1. Написать функции считывания функций через файл: push, pop, add, sub, mul, div, neg, 
$*                                                    out, sin, cos, sqrt, in, dump, hack (коды для взлома)
$* 2. Написать функции для выполнения этих функций через стэк.
$* 3. Написать код для ассемблирования этого файла в текстовый "машинный" код.
$* 4. Написать функции, которые преобразует машинный код в "нормальный" (listing).
$* 5. Потом сделать уже перевод не в текстовый файл, а равномерно битовый 
$*      (т.е. еще нужен будет нужно знать количество байт и длину массива codes).
$* 6. Потом сделать перевод в неравмномерно битовый 
$*      (тут нужно будет учитывать, что у команды длина 1 байт, а у value 4 байта).
$* 7. Сделать структуру CPU, с массивом codes длинной codes, 
$*      версией процессора, стэком внутри и четырьмя регистрами ax, bx, cx, dx.
$* №X. Сделать листинг еще круче. 
 */ 

/* Plan на выходные:
++* 1. Написать прогу с условными метками, с без условными метками, с функциями, с оперативной памятью.
++* 2. Сделать матан за две недели, т.е. переписать ту фигню, что я сделал на эту неделю и сделать на следующую.
++* 3. Сделать аналит. на следующую и эту неделю.
+-* 4. Сделать Дискран, прочитать главу виленкина и решить оттуда задачи. 
++* 5. Сделать англ. яз.
--* 6. Прочитать свой английский и посмотреть серию на английском. 
+-* 7. Прочитать Зорича и Беклемишева.
-+* 8. ЧИЛЛЛИТЬ!!!
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