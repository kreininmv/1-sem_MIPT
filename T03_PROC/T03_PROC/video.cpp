/* FILE NAME   : T03PROC.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 23.10.2020.
 * NOTE        : None.
 */

#include "PROC.h"

//VIDEOCARD
static int IsPause = 0, IsExit = 1;
static double SyncTime = 0, FPS = 0, DeltaTime = 0;
static unsigned char Frame[FRAME_H][FRAME_W][4] = {};
int handle = 0;

void Timer(void)
{
  long t = clock();
  static long StartTime = -1, OldTime, PauseTime, FPSTime, FrameCount;

  if (StartTime == -1)
    StartTime = OldTime = FPSTime = t;
  if (IsPause)
  {
    DeltaTime = 0;
    PauseTime += t - OldTime;
  }
  else
  {
    SyncTime = (t - StartTime - PauseTime) / (double)CLOCKS_PER_SEC;
    DeltaTime = (t - OldTime) / (double)CLOCKS_PER_SEC;
  }
  FrameCount++;
  if (t - FPSTime > CLOCKS_PER_SEC)
  {
    static char Buf[100];
    FPS = FrameCount / ((t - FPSTime) / (double)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
    sprintf(Buf, "FPS: %.3f", FPS);
    glutSetWindowTitle(Buf);
  }
  OldTime = t;
}

void Keyboard(unsigned char Key, int x, int y)
{
  if (Key == 27)
    exit(0);
  else if (Key == 'f')
    glutFullScreen();
  else if (Key == 'p')
    IsPause = !IsPause;
}

void Display(void)
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawPixels(FRAME_W, FRAME_H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Frame);
  glutSwapBuffers();

  Timer();
  glutPostRedisplay();
  
}

void Copy(double* RAM)
{
  for (int y = 0; y < FRAME_H; y++)
    for (int x = 0; x < FRAME_W; x++)
    {
      Frame[y][x][0] = RAM[x + 256 * y];
      Frame[y][x][1] = RAM[x + 256 * y];
      Frame[y][x][2] = RAM[x + 256 * y];
      Frame[y][x][3] = 0;
    }
}