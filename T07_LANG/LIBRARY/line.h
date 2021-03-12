#ifndef LINEH
#define LINEH

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
/* Debug memory allocation support */
#ifndef NDEBUG 
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h> 

#define SetDbgMemHooks()                                           \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))      

static class __Dummy
{
public:
  /* Class constructor */
  __Dummy(VOID)
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __ooppss;

#endif /* _DEBUG */ 

#ifndef NDEBUG
#  ifdef _CRTDBG_MAP_ALLOC 
#    define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#  endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

namespace mk5 {
  class line {
  public:

    char *str;
    int len = 0;

    line(void) { ; }
    explicit line(char *st, int ln) : str(st), len(ln) {
    }

    explicit line(const char *st, int ln) : len(ln) {
      str = (char *)st;
    }
    
    line & operator= (const line& copy) {
      str = copy.str;
      len = copy.len;

      return *this;
    }

    bool operator==(const line& copy) {
      if (strncmp(copy.str, str, max(copy.len, len)) == 0)
        return true;
      
      return false;
    }

    ~line(void) {
      str = nullptr;
      len = 0;
    }

    int print(void) {
      for (int i = 0; i < len; i++)
        printf("%c", str[i]);

      return 1;
    }

    int print_file(FILE *file) {

      assert(file != nullptr);

      for (int i = 0; i < len; i++)
        fprintf(file, "%c", str[i]);
      
      return 1;
    }

    int print_str(char *string) {
      for (int i = 0; i < len; i++)
        sprintf(string + i, "%c", str[i]);
      
      return 1;
    }
  };
}
#endif