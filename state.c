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
  // TODO: Implement this function.
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  return true;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  return true;
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  return '?';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum) {
  // TODO: Implement this function.
  return '?';
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}

void allocation_failed() {
  perror("malloc failed");
  exit(1);
}