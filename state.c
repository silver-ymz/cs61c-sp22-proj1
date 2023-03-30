#include "state.h"
#include "snake_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);
void allocation_failed();

/* Helper function to get a character from the board (already implemented
 * for you). */
static char get_board_at(game_state_t *state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for
 * you). */
static void set_board_at(game_state_t *state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state() {
  char defalut_board[10][15] = {
      "##############", "#            #", "#        *   #", "#            #",
      "#   d>       #", "#            #", "#            #", "#            #",
      "#            #", "##############"};

  game_state_t *state = malloc(sizeof(game_state_t));
  if (state == NULL) {
    allocation_failed();
  }

  state->x_size = 14;
  state->y_size = 10;
  state->num_snakes = 1;

  char **board = malloc(sizeof(char *) * 10);
  if (board == NULL) {
    allocation_failed();
  }

  for (int i = 0; i < 10; i++) {
    char *row = malloc(sizeof(char) * 15);
    if (row == NULL) {
      allocation_failed();
    }

    strcpy(row, defalut_board[i]);
    board[i] = row;
  }

  state->board = board;

  snake_t *snakes = malloc(sizeof(snake_t));
  if (snakes == NULL) {
    allocation_failed();
  }

  snakes->head_x = 5;
  snakes->head_y = 4;
  snakes->tail_x = 4;
  snakes->tail_y = 4;
  snakes->live = true;
  state->snakes = snakes;

  return state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  free(state->snakes);

  const int board_arr_size = state->y_size;
  for (int i = 0; i < board_arr_size; i++) {
    free(state->board[i]);
  }

  free(state->board);

  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  int row = state->y_size;

  for (int i = 0; i < row; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }

  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) { return strchr("wasd", c) != NULL; }

static bool is_snake(char c) { return strchr("wasd^<>vx", c) != NULL; }

static char body_to_tail(char c) {
  switch (c) {
  case '^':
    return 'w';
  case '<':
    return 'a';
  case '>':
    return 'd';
  case 'v':
    return 's';
  default:
    return 0;
  }
}

static int incr_x(char c) {
  if (c == '>' || c == 'd') {
    return 1;
  }

  if (c == '<' || c == 'a') {
    return -1;
  }

  return 0;
}

static int incr_y(char c) {
  if (c == 'v' || c == 's') {
    return 1;
  }

  if (c == '^' || c == 'w') {
    return -1;
  }

  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum) {
  const int head_x = state->snakes[snum].head_x;
  const int head_y = state->snakes[snum].head_y;
  const char head = get_board_at(state, head_x, head_y);

  const int x = head_x + incr_x(head);
  const int y = head_y + incr_y(head);

  return get_board_at(state, x, y);
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum) {
  const int head_x = state->snakes[snum].head_x;
  const int head_y = state->snakes[snum].head_y;
  const char head = get_board_at(state, head_x, head_y);

  const int x = head_x + incr_x(head);
  const int y = head_y + incr_y(head);

  set_board_at(state, x, y, head);

  state->snakes[snum].head_x = x;
  state->snakes[snum].head_y = y;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum) {
  const int tail_x = state->snakes[snum].tail_x;
  const int tail_y = state->snakes[snum].tail_y;
  const char tail = get_board_at(state, tail_x, tail_y);

  const int x = tail_x + incr_x(tail);
  const int y = tail_y + incr_y(tail);

  set_board_at(state, tail_x, tail_y, ' ');
  set_board_at(state, x, y, body_to_tail(get_board_at(state, x, y)));

  state->snakes[snum].tail_x = x;
  state->snakes[snum].tail_y = y;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  const int snake_size = state->num_snakes;

  for (int i = 0; i < snake_size; i++) {
    if (!state->snakes[i].live) {
      continue;
    }

    const char next = next_square(state, i);

    if (next == ' ') {
      update_head(state, i);
      update_tail(state, i);

      continue;
    }

    if (next == '*') {
      update_head(state, i);

      add_food(state);

      continue;
    }

    state->snakes[i].live = false;

    const int head_x = state->snakes[i].head_x;
    const int head_y = state->snakes[i].head_y;

    set_board_at(state, head_x, head_y, 'x');
  }
}

/* Task 5 */
game_state_t *load_board(char *filename) {
  game_state_t *state = malloc(sizeof(game_state_t));
  if (state == NULL) {
    allocation_failed();
  }

  char buffer[10010], *(char_arr_ptr[10000]);

  FILE *file = fopen(filename, "r");

  if (fgets(buffer, 10000, file) == NULL) {
    state->x_size = 0;
    state->y_size = 0;

    return state;
  }

  const int x_size = strlen(buffer) - 1;
  int y_size = 0;

  do {
    char_arr_ptr[y_size] = malloc(sizeof(char) * (x_size + 1));
    strncpy(char_arr_ptr[y_size], buffer, x_size);
    char_arr_ptr[y_size][x_size] = '\0';
    y_size++;
  } while (fgets(buffer, 10000, file) != NULL);

  char **board = malloc(sizeof(char *) * y_size);
  if (board == NULL) {
    allocation_failed();
  }

  for (int i = 0; i < y_size; i++) {
    board[i] = char_arr_ptr[i];
  }

  state->board = board;
  state->x_size = x_size;
  state->y_size = y_size;

  return state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum) {
  int x = state->snakes[snum].tail_x;
  int y = state->snakes[snum].tail_y;

  while (true) {
    const char now_ch = get_board_at(state, x, y);
    const int x_next = x + incr_x(now_ch);
    const int y_next = y + incr_y(now_ch);

    if (!is_snake(get_board_at(state, x_next, y_next))) {
      break;
    }

    x = x_next;
    y = y_next;
  }

  state->snakes[snum].head_x = x;
  state->snakes[snum].head_y = y;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  const int x_size = state->x_size;
  const int y_size = state->y_size;
  int snake_size = 0;
  int snake_x[1000], snake_y[1000];

  for (int i = 0; i < x_size; i++) {
    for (int j = 0; j < y_size; j++) {
      if (is_tail(get_board_at(state, i, j))) {
        snake_x[snake_size] = i;
        snake_y[snake_size] = j;
        snake_size++;

        if (snake_size == 1000) {
          fprintf(stderr, "too many snakes");
          exit(1);
        }
      }
    }
  }

  state->num_snakes = snake_size;
  state->snakes = malloc(sizeof(snake_t) * snake_size);
  for (int i = 0; i < snake_size; i++) {
    state->snakes[i].tail_x = snake_x[i];
    state->snakes[i].tail_y = snake_y[i];
    state->snakes[i].live = true;
    find_head(state, i);
  }

  return state;
}

void allocation_failed() {
  perror("malloc failed");
  exit(1);
}