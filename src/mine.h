#ifndef MINE_H
#define MINE_H

#include <ncurses.h>
typedef struct {
  char *name;
  void (*exec) (void);
  int is_submenu;
  int (*draw_submenu) (WINDOW *win);
  int toggled;
} Choice;

int maxChoiceLen(Choice choice[], int choice_amount);

#endif // MINE_H
