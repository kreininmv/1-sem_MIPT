/* FILE NAME   : tree.cpp
 * PURPOSE     : Module of tree class.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 04.12.2020.
 * NOTE        : None.
 */

#include "tree.h"

int print_vertex(mk5::vertex* node, FILE *file) {
  
  if (node->type == VAR || node->type == POL_OP || node->type == FUNC || node->type == STD_FUNC 
   || node->type == CMP || node->type == FUNCTION_DECLARATION         || node->type == CONCATENATION) {
    char str[MAX_STR] = "";
    node->name.print_str(str);
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0>  LEFT |<f1> '(%s)' | <f2> RIGHT \"];\n", (int)node, str);
  }
  else if (node->type == NUM)
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0>  LEFT |<f1> (%lf) | <f2> RIGHT \"];\n", (int)node, node->value);
  else if (node->type == OPER)
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0>  LEFT |<f1> (%c) | <f2> RIGHT \"];\n", (int)node, node->num);

  return 1;
}

 /*! Print information of cur node.
  * \param[in]  point to node   - vetex* node
  * \param[in]  num of vertexes - int num_vertex
  * \param[out] none.
  */
int mk5::tree::print_vertex_dump(vertex* node, const int num_vertex, FILE *file) {

  if (node->right != nullptr) {

    print_vertex(node, file);

    fprintf(file, "struct%i:f2 -> struct%i:f1;\n", (int)node, (int)(node->right));

    print_vertex_dump(node->right, num_vertex - 1, file);
  }

  if (node->left != nullptr) {
    print_vertex(node, file);

    fprintf(file, "struct%i:f0 -> struct%i:f1;\n", (int)node, (int)(node->left));

    print_vertex_dump(node->left, num_vertex - 1, file);
  }

  print_vertex(node, file);

  return 1;
}/* End of 'print' fucntion */

/*! Delete all vertexes in the tree
 * \param[in]  point to the root - vertex* node
 * \param[out] none
 */
void mk5::tree::delete_tree(vertex* node) {

  if (node == nullptr)
    return;

  if (node->left != nullptr)
    delete_tree(node->left);
  if (node->right != nullptr)
    delete_tree(node->right);

  delete node;
  node = nullptr;
}/* End of 'delete_tree' function */

/*! Make picture with a graphviz.
 * \param[in]  none.
 * \param[out] none.
 */
int mk5::tree::dump(void) {

  char file_name[MAX_STR] = "", command[COMMAND_LEN] = "";

  strcat(file_name, dump_name);
  strcat(file_name, ".dot");

  FILE* file = fopen(file_name, "wb");

  if (file == nullptr)
    return 0;

  fprintf(file, "digraph G{\n");

  fprintf(file, "node [color=\"#000000\", shape=record];\n");
  
  print_vertex(root, file);
    

  fprintf(file, "edge[color = \"black\"];\n");

  if (root->right != nullptr) {
    fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", (int)root, (int)(root->right));

    print_vertex_dump(root->right, size_tree - 1, file);
  }

  if (root->left != nullptr) {
    fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", (int)root, (int)(root->left));

    print_vertex_dump(root->left, size_tree - 1, file);
  }


  fprintf(file, "}");
  fclose(file);

  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\Graphviz\\bin\\dot.exe -Tpdf C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T07LANG\\T07LANG\\");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T07LANG\\T07LANG\\TREE\\");
  strcat(command, dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T07LANG\\T07LANG\\TREE\\");
  strcat(command, dump_name);
  strcat(command, ".pdf");

  if (system(command))
    assert(0);

  return 1;
}/* End of 'dump' function */

/*! Write tree in the file.
 * \param[in]  name of file        - const char* FileName
 * \param[in]  description of tree - const char* Description
 * \param[out] none
 */
int mk5::tree::write(const char* FileName) {

  FILE *file = fopen(FileName, "wb");

  if (file == nullptr) {
    fprintf(stderr, "I can't open file for writing...\n");

    return 0;
  }

  write_vertex(root, file);

  fclose(file);

  return 1;
}/* End of 'write' function */

/*! Write cur node in the file
 * \param[in]  point to the node - vertex* node
 * \param[in]  num of ' '        - int indent_size
 * \param[in]  file for writing  - FILE* file
 * \param[out] none
 */
int mk5::tree::write_vertex(vertex* node, FILE* file) {

  fprintf(file, "{");
  if (node == nullptr)
    fprintf(file, "nil");
  else {
    if (node->type == NUM)
      fprintf(file, "%lf", node->value);
    else if (node->type == FUNC || node->type == STD_FUNC) {
      fprintf(file, "$");
      node->name.print_file(file);
    }
    else if (node->type == VAR          || node->type == POL_OP || node->type == CMP 
          || node->type == CONCATENATION || node->type == FUNCTION_DECLARATION)
      node->name.print_file(file);
    else if (node->type == OPER)
      fprintf(file, "%c", node->num);

    write_vertex(node->left, file);
    write_vertex(node->right, file);
  }
  fprintf(file, "}");

  return 1;
}/*End of 'write_vertex' function */