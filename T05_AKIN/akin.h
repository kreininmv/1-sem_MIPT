#ifndef AKINH
#define AKINH
/* FILE NAME   : akin.h
 * PURPOSE     : Header module.
 * PROGRAMMER  : Matvei Kreinin,
 * LAST UPDATE : 19.11.2020.
 * NOTE        : None.
 */

#define TX_USE_SPEAK 
#include "TXLib.h"

#include "tree.h"

enum switches {
  PLAY         = '1',
  DUMP         = '2',
  F_SIMILLIAR  = '3',
  F_DIFFERENCE = '4',
  SAVE         = '5',
  PLAY_SAFE    = '6',
  EXIT_GAME    = '0',
  EXIT         = '7',
  ANSWER_NO    = '1',
  ANSWER_YES   = '2',
  RETURN       = '3'
};

const char NAME_BASES[] = "name_bases.txt";

namespace mk5 {

  /* For the game AKINATOR */
  class akinator{
  private:
    tree mytree;

    akinator operator= (const akinator& copy) = delete;
    akinator(const akinator& copy) = delete;

  public:
    ~akinator() {
    };
    akinator() = default;
   
    int start(void);

  private:

    int show_difference(data_type* data1, data_type* data2, vertex* root);

    int show_similliar(data_type* data1, data_type* data2, vertex* root);

    void akinator_init( void );

    int game_loop(vertex* cur_root);

    int game_loop_safe(vertex* cur_root);

    int menu(void);
  };
}
#endif