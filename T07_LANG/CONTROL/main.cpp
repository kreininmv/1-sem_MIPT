#include <string.h>
#include <stdio.h>
#include <windows.h>

#define MAX_STR 200
const char ZERO = 0;

int main(int argc, char *argv[]) {

  char file_name[MAX_STR] = "";
  char command[MAX_STR] = "";
  char path[] = {"C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T07LANG\\DEBUG\\"};
  char myname[MAX_STR] = "";
  printf("WRITE NAME OF YOUR SUPER-PUPER PROGRAM))\n");
  scanf("%s", myname);
  
  strcat(command, path);
  strcat(command, "T07LANG.EXE ");
  strcat(command, myname);
  system(command);
  memcpy(command, &ZERO, MAX_STR);

  strcat(command, path);
  strcat(command, "MIDDLEEND.EXE ");
  strncat(command, myname, strlen(myname) - strlen(".txt"));
  strcat(command, ".tree");
  system(command);
  memcpy(command, &ZERO, MAX_STR);

  printf("\nDo you want to wiev your code??)\n");
  strcat(command, path);
  strcat(command, "FRONTEND-1.EXE ");
  strncat(command, myname, strlen(myname) - strlen(".txt"));
  strcat(command, ".tree");
  system(command);
  memcpy(command, &ZERO, MAX_STR);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T07LANG\\PROGRAMS\\");
  strcat(command, "new_");
  strcat(command, myname);
  system(command);
  memcpy(command, &ZERO, MAX_STR);
  printf("Lol, you think that you have choice)))\n");

  strcat(command, path);
  strcat(command, "BACKEND.EXE ");
  strncat(command, myname, strlen(myname) - strlen(".txt"));
  strcat(command, ".tree");
  system(command);
  memcpy(command, &ZERO, MAX_STR);

  strcat(command, path);
  strcat(command, "ASM.EXE ");  
  strncat(command, myname, strlen(myname) - strlen(".txt"));
  strcat(command, ".mk5");
  system(command);
  memcpy(command, &ZERO, MAX_STR);

  printf("\n\nPROGRAMS IS STARTED!!!\n"
             "======================\n");
  strcat(command, path);
  strcat(command, "PROC.EXE ");
  strncat(command, myname, strlen(myname) - strlen(".txt"));
  strcat(command, ".mk5asme");
  system(command);
  getchar();
  getchar();
  return 0;
}