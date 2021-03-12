/* FILE NAME   : compose.cpp
 * PURPOSE     : Module of or reading tree.
 * PROGRAMMER  : Matvei Kreinin.
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include "compose.h"

//mk5::vertex* mk5::compose::read_file(const char* FileName, int &size_tree) {
//
//  FILE *file = fopen(FileName, "rb");
//  if (file == nullptr) {
//    fprintf(stderr, "I can't read file (%s)\n", FileName);
//
//    return nullptr;
//  }
//
//  num_lines = ReadStrBuf(&lines, FileName, &buf);
//
//  int cur_line = FILE_START;
//
//  return read_vertex_file(cur_line, lines, size_tree);
//}/* End of 'read_file' function*/
//
//mk5::vertex* mk5::compose::read_tree_str(char* expr, int &size_tree) {
//
//  int len_expr = strlen(expr);
//  line new_expr = {0, new char[len_expr]};
//
//  for (int i_sym = 0; i_sym < len_expr; i_sym++)
//    if (expr[i_sym] != ' ')
//      new_expr.str[new_expr.len++] = expr[i_sym];
//
//  new_expr.str[new_expr.len] = 0;
//
//  int cur_symb = 0;
//
//  vertex* vertex_return = read_vertex_str(new_expr, cur_symb, size_tree);
//
//  delete[] new_expr.str;
//
//  return vertex_return;
//}
//
//mk5::vertex* mk5::compose::read_vertex_file(int &cur_line, mk5::line* line, int &size_tree) {
//
//  char cur_string = *line[cur_line].str, next_string = *line[cur_line + 1].str;
//
//  if (cur_string == '[') {
//
//    cur_line += 1;
//    type_vertex type = nill;
//    
//    line[cur_line].str++;
//    line[cur_line].len -= 3;
//
//    vertex *cur_vertex = new vertex(nullptr, nullptr, GetValue(&line[cur_line], type), type);
//
//    if (next_string == '?') {
//
//      cur_line += 1;
//      cur_vertex->right = read_vertex_file(cur_line, line, size_tree);
//
//      cur_line += 1;
//      cur_vertex->left = read_vertex_file(cur_line, line, size_tree);
//
//      cur_line += 1;
//
//      size_tree++;
//      return cur_vertex;
//    }
//    else {
//      
//      cur_line += 1;
//      size_tree++;
//      return cur_vertex;
//    }
//  }
//
//  return nullptr;
//}/* End of 'read_vertex' function */
//
//int add_vertex_sheet(mk5::line &expr, int &cur_symb, int &size_tree, mk5::vertex *cur_vertex) {
//  
//  int next_command = 0;
//
//  while (expr.str[cur_symb + next_command] != ')')
//    next_command++;
//
//  mk5::line cur_command = {next_command, expr.str + cur_symb};
//
//  cur_vertex->value = GetValue(&cur_command, cur_vertex->type);
//
//  cur_symb += next_command;
//  size_tree++;
//  
//  return 1;
//}
//
//int add_vertex_node(mk5::line &expr, int &cur_symb, int &size_tree, mk5::vertex *cur_vertex) {
//
//  int next_command = 0;
//  while (expr.str[cur_symb + next_command] != '(')
//    next_command++;
//
//  mk5::line cur_command = {next_command, expr.str + cur_symb};
//
//  cur_vertex->value = GetValue(&cur_command, cur_vertex->type);
//
//  cur_symb += next_command;
//  size_tree++;
//
//  return 1;
//}
//
//mk5::vertex* mk5::compose::read_vertex_str(mk5::line &expr, int &cur_symb, int &size_tree) {
//
//  mk5::vertex* cur_vertex = new mk5::vertex;
//
//  if (expr.str[cur_symb] == '(') {
//
//    if (expr.str[cur_symb + 1] == '(') {
//      //Reading left tree
//      cur_symb++;
//      cur_vertex->left = read_vertex_str(expr, cur_symb, size_tree);
//    }
//    else 
//    {
//      //It's a sheet 
//      cur_symb++;
//      
//      add_vertex_sheet(expr, cur_symb, size_tree, cur_vertex);
//      cur_symb++;
//      
//      if (cur_vertex->type == nill) {
//        delete cur_vertex;
//        return nullptr;
//      }
//        
//      return cur_vertex;
//    }
//    
//    //Reading cur vertex
//
//    add_vertex_node(expr, cur_symb, size_tree, cur_vertex);
//
//    //Reading right tree
//    cur_vertex->right = read_vertex_str(expr, cur_symb, size_tree);
//    cur_symb++;
//
//    size_tree++;
//
//    return cur_vertex;
//  }
//  else {
//    //It's a sheet 
//    add_vertex_sheet(expr, cur_symb, size_tree,  cur_vertex);
//    cur_symb++;
//
//    if (cur_vertex->type == nill) {
//      delete cur_vertex;
//      return nullptr;
//    }
//
//    return cur_vertex;
//  }
//
//  return nullptr;
//}