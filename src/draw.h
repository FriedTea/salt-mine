#ifndef DRAW_H
#define DRAW_H

#include <ncurses.h>
#include "mine.h"

void drawMsg(int min_h, char *msg);
int drawTitle(int win_h);

int drawChoices(
    WINDOW *win,
    Choice *choice,
    int choice_amount,
    int *highlighted,
    int is_submenu
  );

int handleInput(
    WINDOW *win,
    Choice *choice,
    int choice_amount,
    int *highlighted
  );

int drawSalt(int amount);
void drawMiner(int win_h, int *counter);
int drawAll(
    WINDOW *win,
    Choice *choice,
    int choice_amount,
    int *highlighted,
    int *counter,
    int is_submenu);

#endif // DRAW_H
