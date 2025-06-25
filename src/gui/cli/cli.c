#include "cli.h"

void showField(WINDOW *field, int **_field) {
  werase(field);
  box(field, 0, 0);
  if (_field) {
    for (size_t i = 0; i < FIELD_Y; i++) {
      for (size_t j = 0; j < FIELD_X; j++) {
        int cell_value = _field[i][j];
        if (cell_value) {
          wattron(field, COLOR_PAIR(1));
          mvwaddch(field, i + 1, j * 2 + 1, ' ');
          mvwaddch(field, i + 1, j * 2 + 2, ' ');
          wattroff(field, COLOR_PAIR(1));
        }
      }
    }
  }
  wrefresh(field);
}

void showInfo(WINDOW *info, GameInfo_t game) {
  werase(info);
  box(info, 0, 0);
  mvwprintw(info, 1, 1, "NEXT");
  if (game.next) {
    for (size_t i = 0; i < 4; i++) {
      for (size_t j = 0; j < 4; j++) {
        int cell_value = game.next[i][j];
        if (cell_value) {
          wattron(info, COLOR_PAIR(1));
          mvwaddch(info, i + 1, j * 2 + 1, ' ');
          mvwaddch(info, i + 1, j * 2 + 2, ' ');
          wattroff(info, COLOR_PAIR(1));
        }
      }
    }
  }
  mvwprintw(info, 4, 1, "HIGH SCORE %d", game.high_score);
  mvwprintw(info, 5, 1, "SCORE %d", game.score);
  mvwprintw(info, 6, 1, "LEVEL %d", game.level);
  if (game.pause == 1) mvwprintw(info, 7, 1, "PAUSE");
  if (game.pause == 2) mvwprintw(info, 7, 1, "GAME OVER!");
  wrefresh(info);
}

void print() {
  WINDOW *field = newwin(22, 22, 0, 0);
  WINDOW *info = newwin(12, 18, 0, 24);
  while (true) {
    userInput(get_input(), false);
    GameInfo_t current_state = updateCurrentState();
    if (current_state.pause == 3) break;
    showField(field, current_state.field);
    showInfo(info, current_state);
    wnoutrefresh(field);
    wnoutrefresh(info);
    doupdate();
    napms(10);
  }
}

void setup() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_WHITE);
}

UserAction_t get_input() {
  UserAction_t action = (UserAction_t)-1;
  int ch;
  while ((ch = getch()) != ERR) {
    switch (ch) {
      case KEY_RIGHT: {
        action = Right;
        break;
      }
      case KEY_LEFT: {
        action = Left;
        break;
      }
      case KEY_UP: {
        action = Up;
        break;
      }
      case KEY_DOWN: {
        action = Down;
        break;
      }
      case 'q': {
        action = Terminate;
        break;
      }
      case ' ': {
        action = Pause;
        break;
      }
      case 'a': {
        action = Action;
        break;
      }
      case 's': {
        action = Start;
        break;
      }
      default:
        action = (UserAction_t)-1;
        break;
    }
  }
  return action;
}

