/* FILE NAME   : tree.cpp
 * PURPOSE     : Module of tree class.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "akin.h"

mk5::stock storage(1000, 50);

mk5::vertex::vertex(data_type* data_add, status_vertex type) : left(nullptr), right(nullptr), parent(nullptr), status(type) {
  if (type == no_need_free_buf)
    data = data_add;
  else if (type == no_need_free_stock)
    data = storage.add(data_add, strlen(data_add) + 1);
}

mk5::vertex::vertex(vertex* left_add, vertex* right_add, vertex* parent_add, data_type* data_add, status_vertex type)
  : left(left_add), right(right_add), parent(parent_add), status(type) {
  
  if (type == no_need_free_buf)
    data = data_add;
  else if (type == no_need_free_stock)
    data = storage.add(data_add, strlen(data_add) + 1);
}

/*! Create new vertex and adding to the left
 * \param[in]  point to the vertex of adding - vertex* cur_vertex
 * \param[in]  data for adding               - const data_type data
 * \param[out] none.
 */
int mk5::vertex::push_left(data_type* data, status_vertex type) {

  if (left != nullptr)
    return 0;

  left = new vertex(nullptr, nullptr, this, data, type);

  return 1;
}/* End of 'push_left' function */

/*! Create new vertex and adding to the right
 * \param[in]  point to the vertex of adding - vertex* cur_vertex
 * \param[in]  data for adding               - const data_type data
 * \param[out] none.
 */
int mk5::vertex::push_right(data_type* data, status_vertex type) {

  if (right != nullptr)
    return 0;

  right = new vertex(nullptr, nullptr, this, data, type);

  return 1;
}/* End of 'push_right' function */

/*! Push to data to vertex
 * \param[in]  data for adding            - data_type* new_data
 * \param[out] none.
 */
int mk5::vertex::push(data_type* new_data, status_vertex type) {

  if (type == no_need_free_buf)
    data = new_data;
  else if (type == no_need_free_stock)
    data = storage.add(new_data, strlen(new_data) + 1);

  return 1;
}/* End of 'push' function */

/*! Print information of cur node.
 * \param[in]  point to node   - vetex* node
 * \param[in]  num of vertexes - int num_vertex
 * \param[out] none.
 */
int mk5::tree::print_vertex_dump(vertex* node, const int num_vertex, FILE *file) {

  if (num_vertex < 1)
    return 0;

  if (node->right != nullptr) {
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0> мер | <f1> '%s?' | <f2> дю \"];\n", (int)node, node->data);
    fprintf(file, "struct%i:f2 -> struct%i:f1;\n  ", (int)node, (int)(node->right));

    print_vertex_dump(node->right, num_vertex - 1, file);
  }

  if (node->left != nullptr) {
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#9cfff4\", label=\"<f0> мер | <f1> '%s?' | <f2> дю \"];\n", (int)node, node->data);
    fprintf(file, "struct%i:f0 -> struct%i:f1;\n  ", (int)node, (int)(node->left));

    print_vertex_dump(node->left, num_vertex - 1, file);
  }

  if (node->right == nullptr && node->left == nullptr)
    fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#89F09B\", label=\"<f0> мер | <f1> '%s' | <f2> дю \"];\n", (int)node, node->data);

  return 1;
}/* End of 'print' fucntion */

/*! Delete all vertexes in the tree
 * \param[in]  point to the root - vertex* node
 * \param[out] none
 */
void mk5::tree::delete_tree(vertex* node) {

  if (node->left != nullptr)
    delete_tree(node->right);
  if (node->right != nullptr)
    delete_tree(node->left);

  delete node;
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
  fprintf(file, "struct%i [style=\"filled\", fillcolor=\"#FF019A\", label=\"<f0> мер  |<f1> '%s?' | <f2> дю\"];\n", (int)root, root->data);
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

  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\iconv\\iconv.exe -t UTF-8 -f CP1251 < C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\temp_");
  strcat(command, file_name);

  if(system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\Graphviz\\bin\\dot.exe -Tpdf C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\temp_");
  strcat(command, file_name);
  strcat(command, " > C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\TREE\\");
  strcat(command, dump_name);
  strcat(command, ".pdf");
  
  if (system(command))
    assert(0);

  memcpy(command, &ZERO, COMMAND_LEN);
  strcat(command, "C:\\Users\\MK5\\Desktop\\MIPT\\Programming\\T05AKIN\\T05AKIN\\TREE\\");
  strcat(command, dump_name);
  strcat(command, ".pdf");
  
  if (system(command))
    assert(0);

  return 1;
}/* End of 'dump' function */

/*! Build tree by the information in the file
 * \param[in]  name of file - const char* FileName
 * \param[out] none
 */
int mk5::tree::read(const char* FileName) {
  
  num_lines = ReadStrBuf(&lines, FileName, &buf);

  strncpy(dump_name, FileName, strlen(FileName) - strlen(".txt"));

  if (num_lines == 0)
    return 0;

  //if (strncmp(lines[2].str + 1, "ENG", 3)) {
  //  fprintf(stderr, "I can't read bases that didn't writen on english\n...");
  //  return 0;
  //}

  //Debug info
  printf("%s\n%s\n", lines[0].str, lines[1].str);
  int cur_line = 3;

  root = read_vertex(cur_line, lines, root);
  size_tree++;

  root->parent = nullptr;

  VERIFICATION;

  return 1;
}/* End of 'read' function */

/*! Reading iformation about vertex
 * \param[in]  number of cur line - int &cur_line
 * \param[in]  array of strings   - line* line
 * \param[in]  point to paren     - vertex* vertex
 * \param[out] point to the new vertex
 */
mk5::vertex* mk5::tree::read_vertex(int &cur_line, mk5::line* line, vertex* parent) {

  char cur_string = *line[cur_line].str, next_string = *line[cur_line + 1].str;

  if (cur_string == '[') {

    cur_line += 1;
    
    vertex *cur_vertex = new vertex(nullptr, nullptr, parent, line[cur_line].str + 1, no_need_free_buf);

    if (next_string == '?') {

      cur_line += 1;
      cur_vertex->right = read_vertex(cur_line, line, cur_vertex);

      cur_line += 1;
      cur_vertex->left = read_vertex(cur_line, line, cur_vertex);

      cur_line += 1;

      size_tree++;
      return cur_vertex;
    }
    else {
      cur_line += 1;
      size_tree++;
      return cur_vertex;
    }
  }

  return nullptr;
}/* End of 'read_vertex' function */

/*! Make parents for all vertexes
 * \param[in]  son    - vertex* son
 * \param[in]  father - vertex* father
 * \param[out] none
 */
int mk5::tree::make_parent(vertex* son, vertex* father) {

  if (son->left != nullptr)
    make_parent(son->left, son);

  if (son->right != nullptr)
    make_parent(son->right, son);

  son->parent = father;

  return 0;
}/* End of 'make_parent' function */

/*! Init root of tree.
 * \param[in]  data - data_type* data
 * \param[out] none
 */
int mk5::tree::init(data_type* data) {
  
  if (root != nullptr)
    return 0;

  root = new vertex(nullptr, nullptr, nullptr, data, no_need_free_stock);
  
  VERIFICATION;

  return 1;
}/* End of 'init' function */

/*! Verificate father of vertex.
 * \param[in]  son    - vertex* son
 * \param[in]  father - vertex* father
 * \param[out] 1 - all ok, 0 - shit happens
 */
int mk5::tree::verification_node(vertex* son, vertex* father) {

  if (son->parent != father) {
    fprintf(stderr, "ERROR, num(%i) data(%s) doesn't have a father!!!", (int)son, son->data);
    assert(0);
    return 0;
  }

  if (son->left != nullptr)
    if (!verification_node(son->left, son))
      return 0;

  if (son->right != nullptr)
    if (!verification_node(son->right, son))
      return 0;

  return 1;
}/* End of 'verification_node' function */

/*! Function to verificate tree
 * \param[in]  none
 * \param[out] none
 */
int mk5::tree::verification(void) {

  if (!verification_node(root->left, root)) {
    fprintf(stderr, "Left subtree of root is not good\n");
    
    return 0;
  }

  if (!verification_node(root->right, root)){
    fprintf(stderr, "Right subtree of root is not good\n");
    
    return 0;
  }

  dump();

  return 1;
}/* End of 'verification' function */

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

  //if (Description != nullptr)
  //  fprintf(file, "{%s}\n", Description);
  //else
  fprintf(file, "{It's not a Matvei}\n");
  fprintf(file, "{version 1.0}\n");
  fprintf(file, "{ENG}\n");
  
  VERIFICATION;
  write_vertex(root, 0, file);

  fclose(file);

  return 1;
}/* End of 'write' function */

/*! It's a auxiliart function for find node.
 * \param[in]  data for finding         - data_type* data
 * \param[in]  path for current node    - mk5::line *path
 * \param[out] moment when find vertex
 */
int mk5::tree::auxiliary_find_node(data_type* data, mk5::line *path, vertex* cur_root) {

  if (cur_root->left != nullptr) {
    path->str[path->len] = 0;
    path->len++;
    if (auxiliary_find_node(data, path, cur_root->left))
      return 1;
  }

  if (cur_root->right != nullptr) {
    path->str[path->len] = 1;
    path->len++;
    if (auxiliary_find_node(data, path, cur_root->right))
      return 1;
  }

  if (!strcmp(data, cur_root->data))
    return 1;

  path->str[path->len] = 2;
  path->len--;

  return 0;
}/* End of 'auxiliary_find_node' function */

/*! Find the way to vertex
 * \param[in]  data for find - data_type* data
 * \param[out] way
 */
mk5::line* mk5::tree::find_node(data_type* data) {
  
  VERIFICATION;

  mk5::line *path = new line;
  path->len = 0;
  path->str = new char[MAX_STR];

  auxiliary_find_node(data, path, root);

  //printf("(%d)\n", (int)path);

  return path;
}/* End of 'find_node' function */

/*! Write cur node in the file
 * \param[in]  point to the node - vertex* node
 * \param[in]  num of ' '        - int indent_size
 * \param[in]  file for writing  - FILE* file
 * \param[out] none
 */
int mk5::tree::write_vertex(vertex* node, int indent_size, FILE* file) {
  fprintf(file, "%*c\n", indent_size, '[');

  indent_size += 4;

  if (node->right != nullptr && node->left != nullptr) {
    fprintf(file, "%*c%s?\n", indent_size, '?', node->data);
    write_vertex(node->right, indent_size, file);

    write_vertex(node->left, indent_size, file);
  }
  else
    fprintf(file, "%*c%s`\n", indent_size, '`', node->data);

  indent_size -= 4;
  fprintf(file, "%*c\n", indent_size, ']');

  return 1;
}/*End of 'write_vertex' function */