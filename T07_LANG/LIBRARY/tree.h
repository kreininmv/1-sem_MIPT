#pragma once
#ifndef TREEH
#define TREEH
/* FILE NAME   : tree.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin.
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "func_var.h"
#include "parser.h"

const int MAX_STR = 130;
const int COMMAND_LEN = 500;

const char ZERO = 0;


namespace mk5 {

  class tree {
  public:
    vertex* root = nullptr;
    int size_tree = 0;
    char dump_name[MAX_STR] = "";
    char *ptr = nullptr;
  private:
    tree operator= (const tree& copy) = delete;
    tree(const tree& copy) = delete;

  public:
    tree() = default;
    ~tree() {
      if (root != nullptr)
        delete_tree(root);

      root = nullptr;
      size_tree = 0;
    }

    int dump(void);

    int write(const char* FileName);

    void delete_tree(vertex* node);

  private:

    int print_vertex_dump(vertex* node, int num_vertex, FILE* file);

    int write_vertex(vertex* node, FILE* file);
  };
}
#endif