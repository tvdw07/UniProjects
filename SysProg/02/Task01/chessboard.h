#ifndef CHESSBOARD_H
#define CHESSBOARD_H


struct board_t {
    int **fields;   /**< Dynamically allocated 2D array of board fields. */
    int n;          /**< Size of the board (n x n). */
    int x;          /**< Current horizontal position of the knight. */
    int y;          /**< Current vertical position of the knight. */
    int count;      /**< Number of fields visited so far. */
};

/**
 * @brief Initializes a chessboard and places the knight at the starting position.
 *
 * Allocates memory for an n x n board and initializes all fields to 0.
 * The starting field is marked with 1 and the visited field counter is set to 1.
 *
 * @param b Pointer to the chessboard structure.
 * @param n Size of the chessboard (n x n).
 * @param x Starting horizontal position of the knight.
 * @param y Starting vertical position of the knight.
 * @return 0 on success, non-zero on failure.
 */
int init_board(struct board_t* b, int n, int x, int y);

/**
 * @brief Checks whether a knight can move to a given field.
 *
 * Calculates the target position relative to the knight's current position.
 * A move is valid if the target field is within the board boundaries
 * and has not been visited before.
 *
 * @param b Pointer to the chessboard structure.
 * @param x Horizontal movement relative to the current position.
 * @param y Vertical movement relative to the current position.
 * @return 1 if the target field is free, 0 otherwise.
 */
int isfree(struct board_t* b, int x, int y);

/**
 * @brief Moves the knight to a new field.
 *
 * Updates the knight's current position, increments the visited field
 * counter and marks the new field with the current step number.
 * The move is assumed to be valid.
 *
 * @param b Pointer to the chessboard structure.
 * @param x Horizontal movement relative to the current position.
 * @param y Vertical movement relative to the current position.
 */
void new_jump(struct board_t* b, int x, int y);

/**
 * @brief Undoes the last knight movement.
 *
 * Resets the currently visited field to 0, moves the knight back
 * to its previous position and decrements the visited field counter.
 *
 * @param b Pointer to the chessboard structure.
 * @param x Horizontal movement of the move to undo.
 * @param y Vertical movement of the move to undo.
 */
void remove_jump(struct board_t* b, int x, int y);

/**
 * @brief Returns the number of fields visited by the knight.
 *
 * @param b Pointer to the chessboard structure.
 * @return Number of visited fields.
 */
int visited_fields(struct board_t* b);

/**
 * @brief Prints the current state of the chessboard to stdout.
 *
 * Displays the board as a formatted grid, where each field contains
 * the step number in which it was visited.
 * Unvisited fields are represented by 0.
 *
 * @param b Pointer to the chessboard structure.
 */
void print_board(struct board_t* b);

/**
 * @brief Frees the dynamically allocated memory of the chessboard.
 *
 * Releases the memory allocated for each row of the board
 * and the array containing the row pointers.
 *
 * @param b Pointer to the chessboard structure.
 */
void free_board(struct board_t* b);

#endif // CHESSBOARD_H