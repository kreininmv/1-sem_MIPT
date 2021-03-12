#pragma once
#ifndef PARSERH
#define PARSERH
/* FILE NAME   : tree.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include <iostream>

#include "tree.h"
#include "token.h"
#include "func_var.h"

namespace mk5 {

  class parser {

  public:
    token *ptr = nullptr;
    int   token_size = 0;
    char *buf = nullptr;
    int cur_func = 0;
    
    int count_if = 0;
    int count_while = 0;

    stock<function> functions;
    stock<variable> variables;
    
    vertex* GetG(token *arr, int size);

    vertex* read_vertex(void);

    vertex* read_tree(char *ptr) {
      buf = ptr;
      return read_vertex();
    }


    parser() = default;
    ~parser() {

    }

    parser operator= (const parser & copy) = delete;
    parser(const parser& copy) = delete;
    
  private:
    int add_var(line& var);


    vertex* GetStandartFunc(void);

    vertex* GetDec(void);

    vertex* CreateArgs(void);

    vertex* GetBody(void);

    vertex* GetFunc(void);
    
    vertex* GetCreateFunc(void);

    vertex* GetIf(void);

    vertex* GetCreateVar(void);

    vertex* GetVar(void);

    vertex* GetWord(void);

    vertex* GetWhile(void);

    vertex* GetReturn(void);

    vertex* GetPrint(void);

    vertex* GetCondition(void);
    
    vertex* GetEqual(void);

    vertex* GetE(void);

    vertex* GetT(void);

    vertex* GetW(void);

    vertex* GetP(void);

    vertex* GetN(void);

    vertex* GetId(void);

    int CountLeft(vertex* node);
  };
}
#endif