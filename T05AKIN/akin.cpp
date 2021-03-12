/* FILE NAME   : akin.cpp
 * PURPOSE     : Main start module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#include "akin.h"

 /*! Game for findins answers, adding new answers and questions
 * \param[in]  none
 * \param[out] none
 */
int mk5::akinator::start(void) {

  if (mytree.root == nullptr)
    akinator_init();

  menu();

  return 1;
}/* End of 'start' function */

/*! akinator_initialize a vertex
 * \param[in]  point to the vertex - vertex* cur_vertex
 * \param[out] none
 */
void mk5::akinator::akinator_init(void) {

  char str[MAX_STR] = "";
  mk5::line *lines = nullptr;
  mk5::line buf = {};

  int num_lines = ReadStrBuf(&lines, NAME_BASES, &buf);

  //txSpeak("\vIt's a game AKINATOR that made by Matvei Kreinin\n\n");
  //txSpeak("\vHe writed his mind into pc and now he can guess everything you think of\n\n");
  //txSpeak("\vBefore game you need to choose which version(base) of Matvey you will play with or reload your variant\n\n");
  //txSpeak("\vWould you like to read base or create new?\n\n");

  int button = 0;
  while (1) {
    printf("Type 'n' for New game\n"
           "Type 'l' for Load your base\n"
           "\nChoose from ready-made bases:\n\n");
    for (int i_name = 0; i_name < num_lines; i_name++)
      printf("%d. %s\n", 1 + i_name, lines[i_name].str);
    
    printf("Choose:");
    button = getchar();
    getchar();

    switch (button) {
    case 'n':
      
      printf("\nWrite name of base: ");
      getstr(str);
      strcat(mytree.dump_name, str);
      memcpy(str, &ZERO, MAX_STR);
      
      printf("\nAdd new object for it: ");
      getstr(str);

      mytree.init(str);
      
      if (buf.str != nullptr) {
        free(buf.str);
        free(lines);
        buf.len = num_lines = 0;
        buf.str = nullptr;
        lines = nullptr;
      }

      return;
    case 'l':

      printf("\nWrite name of base: ");
      getstr(str);
      
      if (!mytree.read(str)) {
        memcpy(str, &ZERO, MAX_STR);
        break;
      }
      
      if (buf.str != nullptr) {
        free(buf.str);
        free(lines);
        buf.len = num_lines = 0;
        buf.str = nullptr;
        lines = nullptr;
      }

      return;
    default:
      if ('0' <= button && button <= '0' + num_lines) {
        
        if (!mytree.read(lines[button - '0' - 1].str))
          break;
        
        if (buf.str != nullptr) {
          free(buf.str);
          free(lines);
          buf.len = num_lines = 0;
          buf.str = nullptr;
          lines = nullptr;
        }
        return;
      }
      else
       printf("Try to concetrate on your fingers\n");
      break;
    }
  }
  
}/* End of 'akinator_init' function */

/*! Shows the differences between two vertexes
 * \param[in]  first data  - data_type* data1
 * \param[in]  second_data - data_type* data2
 * \param[out] none
 */
int mk5::akinator::show_difference(data_type* data1, data_type* data2, vertex* root) {

  mk5::line* path1 = mytree.find_node(data1), *path2 = mytree.find_node(data2);

  if (path1->len == 0) {
    txSpeak("\vI can't find \"%s\".\n", data1);
    
    if (path2->len == 0)
      txSpeak("\vI can't find \"%s\".\n", data2);
    
    return 0;
  }
  else if (path2->len == 0) {
    txSpeak("\vI can't find \"%s\".\n", data2);
    
    return 0;
  }
  else if (path1->len == path2->len && !strncmp(data1, data2, path1->len)) {
    txSpeak("\vThey are the same.\n");
    
    return 0;
  }

  int i_cur = 0;
  for (; i_cur < MIN(path1->len - 1, path2->len - 1) && path1->str[i_cur] == path2->str[i_cur]; i_cur++)
    (path1->str[i_cur] == 1) ? root = root->right : root = root->left;

  vertex* cur_root = root;
  
  //First node
  int num_vertex1 = i_cur;

  while (num_vertex1 != path1->len) {
  
    txSpeak("\v%s differs from %s in that %s %s\n", data1, data2, (path1->str[num_vertex1] == 1) ? "it is" : "it is not", cur_root->data);
    
    if (path1->str[num_vertex1] == 1)
      cur_root = cur_root->right;
    else if (path1->str[num_vertex1] == 0)
      cur_root = cur_root->left;
    
    num_vertex1++;
  }
  
  cur_root = root;
  printf("\n");

  int num_vertex2 = i_cur;
  while (num_vertex2 != path2->len) {

    txSpeak("\v%s differs from %s in that %s %s\n", data2, data1, (path2->str[num_vertex2] == 1) ? "it is" : "it is not", cur_root->data);
    
    if (path2->str[num_vertex2] == 1)
      cur_root = cur_root->right;
    else if (path2->str[num_vertex2] == 0)
      cur_root = cur_root->left;

    num_vertex2++;
  }
  printf("\n");

  return 1;
}/* End of 'show_difference' function */

/*! Shows the similliar between two vertexes
 * \param[in]  first data  - data_type* data1
 * \param[in]  second_data - data_type* data2
 * \param[out] none
 */
int mk5::akinator::show_similliar(data_type* data1, data_type* data2, vertex* root) {
  
  mk5::line* path1 = mytree.find_node(data1), *path2 = mytree.find_node(data2);

  if (path1->len == 0) {
    txSpeak("\v\nI can't find \"%s\".\n", data1);
    
    if (path2->len == 0)
      txSpeak("\v\nI can't find \"%s\".\n", data2);
    
    return 0;
  }
  else if (path2->len == 0) {
    txSpeak("\v\nI can't find \"%s\".\n", data2);
    
    return 0;
  }
  else if (path1->len == path2->len && !strncmp(data1, data2, path1->len)) {
    txSpeak("\v\nThey are the same.\n");
    
    return 0;
  }
  
  for (int i_cur = 0; i_cur < MIN(path1->len, path2->len) && path1->str[i_cur] == path2->str[i_cur]; i_cur++) {
    txSpeak("\v\n%s and %s %s similliar %s\n", data1, data2, (path1->str[i_cur] == 1) ? "are" : "are not", root->data);
    (path1->str[i_cur] == 1) ? root = root->right : root = root->left;
  }

  return 1;
}/* End of 'show_similliar' function */

/*! Loop for game.
 * \param[in]  point to current root - vertex* cur_root
 * \param[out] none
 */
int mk5::akinator::game_loop_safe(vertex* cur_root) {

  char ans[MAX_STR] = "", qst[MAX_STR] = "";
  int button = 0;
  while (1) {
    

    if (cur_root->left == nullptr && cur_root->right == nullptr) {
      txSpeak("\v\n%s\n", cur_root->data);
  
      return 1;
      }
    else {
      printf("\nQUESTION:");
      txSpeak("\v %s? \n", cur_root->data);
    }

    printf("\n0. For exit to menu\n"
      "1. For answer NO\n"
      "2. For answer YES\n"
      "3. For return to previous node\n"
      "Choose:");
    button = getchar();
    getchar();

    switch (button) {
    case EXIT_GAME:
      return 1;
    case ANSWER_YES:
      if (cur_root->right != nullptr)
        game_loop_safe(cur_root->right);
      else {
        printf("I win...\n");
      }
      return 1;
    case ANSWER_NO:
      if (cur_root->left != nullptr)
        game_loop_safe(cur_root->left);

      return 1;
    case RETURN:
      if (cur_root->parent != nullptr)
        game_loop_safe(cur_root->parent);
      else {
        printf("\nIt's a root of akinator. Stop to calling this option!!!\n");
        break;
      }
      return 1;
    default:
      printf("\nPlease be more carefull, when you put your fingers on keyboard!\n");

      break;
    }
  }

  return 1;
}/* End of 'game_loop_sage' function */

/*! Loop for game.
 * \param[in]  point to current root - vertex* cur_root
 * \param[out] none
 */
int mk5::akinator::game_loop(vertex* cur_root) {
  
  //cur_sub_root
  int button = 0;
  while (1) {
    
    printf("\nQUESTION:");
    txSpeak("\v %s? \n", cur_root->data);
    
    printf("\n0. For exit to menu\n"
      "1. For answer NO\n"
      "2. For answer YES\n"
      "3. For return to previous node\n"
      "Choose:");
    button = getchar();
    getchar();

    switch (button) {
    case EXIT_GAME:
      return 1;
    case ANSWER_YES:
      if (cur_root->right != nullptr)
        game_loop(cur_root->right);
      else {
        printf("I win...\n");
      }
      return 1;
    case ANSWER_NO:
      if (cur_root->left != nullptr)
        game_loop(cur_root->left);
      else {
        char ans[MAX_STR] = "", qst[MAX_STR] = "";
        printf("Input what you wish for: ");
        getstr(ans);
        printf("What differents between %s and %s?\n: ", cur_root->data, ans);
        getstr(qst);

        cur_root->push_right(ans, no_need_free_stock);
        cur_root->push_left(cur_root->data, cur_root->status);
        cur_root->push(qst, no_need_free_stock);
        printf("I am so sorry...\n");
      }

      return 1;
    case RETURN:
      if (cur_root->parent != nullptr)
        game_loop(cur_root->parent);
      else {
        printf("\nIt's a root of akinator. Stop to calling this option!!!\n");
        break;
      }
        return 1;
    default:
      printf("\nPlease be more carefull, when you put your fingers on keyboard!\n");
      
      break;
    }
  }

  return 1;
}/* End of 'game_loop' function */

/*! Mode for editing and others.
 * \param[in]  none
 * \param[out] none
 */
int mk5::akinator::menu(void) {
  
  char ans[MAX_STR] = "", qst[MAX_STR] = "";
  FILE* file = nullptr;
  int button = 0;

  while (1)
  {
    printf("\nMENU:"
             "Choose what you want to do:\n"
             "1. Play\n"
             "2. Show all vertexes\n"
             "3. Find similarities between two nodes\n"
             "4. Find difference between two nodes\n"
             "5. Save your base\n"
             "6. Play in safely mode\n"
             "7. EXIT\n"
             "Choose:");

    button = getchar();
    getchar();

    switch (button) {
    case PLAY: 
      
      txSpeak("\v\nGAME STARTED!\nNOW You need to answer YES or NO on my questions\nAnd I will guess what you are thinking...\n");
      
      game_loop(mytree.root);

      break;
    case DUMP:
      mytree.dump();
  
      break;
    case F_SIMILLIAR:
      printf("\nWrite name of the first node: ");
      getstr(qst);
      printf("\nWrite name of the second node: ");
      getstr(ans);
      
      show_similliar(qst, ans, mytree.root);
      
      break;
    case F_DIFFERENCE:
      printf("\nWrite name of the first node: ");
      getstr(qst);
      printf("\nWrite name of the second node: ");
      getstr(ans);
      
      show_difference(qst, ans, mytree.root);
      
      break;
    case SAVE:

      txSpeak("\v\nWrite name of your base: ");
      getstr(ans);
      strcat(ans, ".txt");
      mytree.write(ans);

      file = fopen(NAME_BASES, "a+");
      if (file == nullptr) {
        printf("\nSomethng going wrong...\n");
        break;
      }
      fprintf(file, "\n%s", ans);
      fclose(file);

      printf("\nIt's saved!\n");
      
      break;
    case PLAY_SAFE:
      txSpeak("\v\nGAME STARTED!\nNOW You need to answer YES or NO on my questions\nAnd I will guess what you are thinking...\n");
      
      game_loop_safe(mytree.root);
      
      break;
    case EXIT:
      printf("\nWould you save your base?\n"
             "1. YES\n"
             "2. NO\n");

      button = getchar();
      getchar();
      
      if (button == '2')
        return 1;

      txSpeak("\v\nWrite name of your base: ");
      getstr(ans);
      strcat(ans, ".txt");
      mytree.write(ans);

      file = fopen(NAME_BASES, "a+");
      if (file == nullptr) {
        printf("\nSomethng going wrong...\n");
        break;
      }
      fprintf(file, "\n%s", ans);
      fclose(file);

      printf("\nIt's saved!\n");

      return 1;
    default:
      printf("\n\n\nPlease be more carefull, when you put your fingers on keyboard!\n\n\n");

      break;
    }
  }
}/* End of 'menu' function */