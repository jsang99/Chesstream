/**************************************************************************/ /**
 * @file      boardFunctions.c
 * @brief     Template for ESE516 with Doxygen-style comments
 * @author    Kevin Sang
 * @date      Nov. 8, 2023

 ******************************************************************************/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "boardFunctions.h"
#include "square.h"
#include "LED.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/******************************************************************************
 * Defines
 ******************************************************************************/
#define TAKE true
#define MOVE false
/******************************************************************************
 * Variables
 ******************************************************************************/

/******************************************************************************
 * Forward Declarations
 ******************************************************************************/

/******************************************************************************
 * Callback Functions
 ******************************************************************************/

// TODO: discovered attack

/**
 * @fn		void vInitBoard(struct s_Square board[8][8], struct s_Square *
 * board_ptr[8][8])
 * @brief   The function initializes the board.
 *
 * @param board
 * @param board_ptr
 */
void vInitBoard() {
	
  // initialize each instances
  u_int8_t i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      // define column and row number
      board[i][j].row = i + 1;
      board[i][j].col = j + 1;

      // TODO: FIX initialize LEDstatus
      board[i][j].LEDStatus = ' ';

      // TODO: initialize sensor number

      if (i == 0 || i == 7) {
        // initialize player as white if at row 1, as black if at row 8
        board[i][j].player = (i == 0) ? 'w' : 'b';

        // initialize non-pawn pieces for white
        if ((j == 0) || (j == 7)) {
          board[i][j].piece = 'R'; // rook
        } else if ((j == 1) || (j == 6)) {
          board[i][j].piece = 'N'; // knight
        } else if ((j == 2) || (j == 5)) {
          board[i][j].piece = 'B'; // bishop
        } else if (j == 3) {
          board[i][j].piece = 'Q'; // queen
        } else {
          board[i][j].piece = 'K'; // king
        }

      } else if (i == 1 || i == 6) {
        // initialize player as white if at row 1, as black if at row 8
        board[i][j].player = (i == 1) ? 'w' : 'b';
        board[i][j].piece = 'p';

      } else {
        // initialize empty sqaures
        board[i][j].player = 'e';
        board[i][j].piece = ' ';
      }

      // populate board_ptr
      board_ptr[i][j] = &board[i][j];
      // printf("%d \n", board_ptr[i][j]);
    }
  }
  
	board_ptr[0][0]->LEDNum = 32;
	board_ptr[0][1]->LEDNum = 33;
	board_ptr[0][2]->LEDNum = 34;
	board_ptr[0][3]->LEDNum = 35;
	board_ptr[0][4]->LEDNum = 16;
	board_ptr[0][5]->LEDNum = 17;
	board_ptr[0][6]->LEDNum = 18;
	board_ptr[0][7]->LEDNum = 19;
	board_ptr[1][0]->LEDNum = 39;
	board_ptr[1][1]->LEDNum = 38;
	board_ptr[1][2]->LEDNum = 37;
	board_ptr[1][3]->LEDNum = 36;
	board_ptr[1][4]->LEDNum = 23;
	board_ptr[1][5]->LEDNum = 22;
	board_ptr[1][6]->LEDNum = 21;
	board_ptr[1][7]->LEDNum = 20;
	board_ptr[2][0]->LEDNum = 40;
	board_ptr[2][1]->LEDNum = 41;
	board_ptr[2][2]->LEDNum = 42;
	board_ptr[2][3]->LEDNum = 43;
	board_ptr[2][4]->LEDNum = 24;
	board_ptr[2][5]->LEDNum = 25;
	board_ptr[2][6]->LEDNum = 26;
	board_ptr[2][7]->LEDNum = 27;
	board_ptr[3][0]->LEDNum = 47;
	board_ptr[3][1]->LEDNum = 46;
	board_ptr[3][2]->LEDNum = 45;
	board_ptr[3][3]->LEDNum = 44;
	board_ptr[3][4]->LEDNum = 31;
	board_ptr[3][5]->LEDNum = 30;
	board_ptr[3][6]->LEDNum = 29;
	board_ptr[3][7]->LEDNum = 28;
	board_ptr[4][0]->LEDNum = 48;
	board_ptr[4][1]->LEDNum = 49;
	board_ptr[4][2]->LEDNum = 50;
	board_ptr[4][3]->LEDNum = 51;
	board_ptr[4][4]->LEDNum = 0;
	board_ptr[4][5]->LEDNum = 1;
	board_ptr[4][6]->LEDNum = 2;
	board_ptr[4][7]->LEDNum = 3;
	board_ptr[5][0]->LEDNum = 55;
	board_ptr[5][1]->LEDNum = 54;
	board_ptr[5][2]->LEDNum = 53;
	board_ptr[5][3]->LEDNum = 52;
	board_ptr[5][4]->LEDNum = 7;
	board_ptr[5][5]->LEDNum = 6;
	board_ptr[5][6]->LEDNum = 5;
	board_ptr[5][7]->LEDNum = 4;
	board_ptr[6][0]->LEDNum = 56;
	board_ptr[6][1]->LEDNum = 57;
	board_ptr[6][2]->LEDNum = 58;
	board_ptr[6][3]->LEDNum = 59;
	board_ptr[6][4]->LEDNum = 8;
	board_ptr[6][5]->LEDNum = 9;
	board_ptr[6][6]->LEDNum = 10;
	board_ptr[6][7]->LEDNum = 11;
	board_ptr[7][0]->LEDNum = 63;
	board_ptr[7][1]->LEDNum = 62;
	board_ptr[7][2]->LEDNum = 61;
	board_ptr[7][3]->LEDNum = 60;
	board_ptr[7][4]->LEDNum = 15;
	board_ptr[7][5]->LEDNum = 14;
	board_ptr[7][6]->LEDNum = 13;
	board_ptr[7][7]->LEDNum = 12;
}



/**
 * @fn struct s_CheckSquareResult sCheckSquare(u_int8_t row, u_int8_t col, struct s_Square * board_ptr[8][8], unsigned char movedSquarePlayer)
 * @brief Checks if a s_Square is eligible for a move or a take by another piece.
 *
 * @param row
 * @param col
 * @param board_ptr
 * @param movedSquarePlayer
 * @return struct s_CheckSquareResult
 */
struct s_CheckSquareResult sCheckSquare(u_int8_t row, u_int8_t col,
                                        struct s_Square *board_ptr[8][8],
                                        unsigned char movedSquarePlayer) {

  // TODO: think about how the canMove and canTake attributes can be utilized
  // better
  struct s_CheckSquareResult result;
  if (board_ptr[row - 1][col - 1]->piece == ' ') { // not encountering a piece
    // can move here
    result.canMove = true;
    result.stayInLoop = true;

    // update LED status
    board_ptr[row - 1][col - 1]->LEDStatus = 'g';

    return result;

  } else { // encountering a piece
    if (board_ptr[row - 1][col - 1]->player ==
        movedSquarePlayer) { // same player's piece
      // cannot move here; cannot take here
      // break from loop
      result.stayInLoop = false;

      return result;
    } else { // opponent's piece
      // can take here
      result.canTake = true;
      result.stayInLoop = false;

      // update LED status
      board_ptr[row - 1][col - 1]->LEDStatus = 'r';

      return result;
    }
  }
}
/**
 * @fn struct s_CheckSquareResult sCheckSquarePawn(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer)
 * @brief Checks if a s_Square is eligible for a move or a take by a pawn
 * 
 * @param row 
 * @param col 
 * @param board_ptr 
 * @param movedSquarePlayer 
 * @param canTake
 * @return struct s_CheckSquareResult 
 */
struct s_CheckSquareResult sCheckSquarePawn(u_int8_t row, u_int8_t col,
                                        struct s_Square *board_ptr[8][8],
                                        unsigned char movedSquarePlayer, bool wantTake) {

  // TODO: think about how the canMove and canTake attributes can be utilized
  // better
  
  struct s_CheckSquareResult result;

  if (row<1 || row>8 || col<1 || col>8) {
    return result;
  }

  if(wantTake == false) {
    if (board_ptr[row - 1][col - 1]->piece == ' ') { // not encountering a piece
        // can move here
        result.canMove = true;

        // update LED status
        board_ptr[row - 1][col - 1]->LEDStatus = 'g';
    } 

  } else {
    if (!(board_ptr[row - 1][col - 1]->piece == ' ')) { // encountering a piece
        if (board_ptr[row - 1][col - 1]->player != movedSquarePlayer) {
            result.canTake = true;
            // update LED status
            board_ptr[row - 1][col - 1]->LEDStatus = 'r';
        }
    }
  }
    
  return result;
}

/**
 * @fn void vCheckFullRowCol(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer)
 * @brief Checks the full row and full col for possible moves
 * 
 * @param row 
 * @param col 
 * @param board_ptr 
 * @param movedSquarePlayer 
 */
void vCheckFullRowCol(u_int8_t row, u_int8_t col,
                      struct s_Square *board_ptr[8][8],
                      unsigned char movedSquarePlayer) {
  u_int8_t i;
  bool stayInLoop;

  // searches upwards for possible moves
  stayInLoop = 1;
  for (i = 1; col + i <= 8 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row, col + i, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches downwards for possible moves
  stayInLoop = 1;
  for (i = 1; col - i > 0 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row, col - i, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches left for possible moves
  stayInLoop = 1;
  for (i = 1; row - i > 0 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row - i, col, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches right for possible moves
  stayInLoop = 1;
  for (i = 1; row + i <= 8 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row + i, col, board_ptr, movedSquarePlayer).stayInLoop;
  }
}

/**
 * @fn void vCheckFullDiagonals(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer)
 * @brief Checks full diagonals on a board for moves
 * 
 * @param row 
 * @param col 
 * @param board_ptr 
 * @param movedSquarePlayer 
 */
void vCheckFullDiagonals(u_int8_t row, u_int8_t col,
                         struct s_Square *board_ptr[8][8],
                         unsigned char movedSquarePlayer) {
  u_int8_t i;
  bool stayInLoop;

  // searches left-downwards for possible moves
  stayInLoop = 1;
  for (i = 1; col - i > 0 && row - i > 0 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row - i, col - i, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches left-upwards for possible moves
  stayInLoop = 1;
  for (i = 1; col + i <= 8 && row - i > 0 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row - i, col + i, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches right-downwards for possible moves
  stayInLoop = 1;
  for (i = 1; col - i > 0 && row + i <= 8 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row + i, col - i, board_ptr, movedSquarePlayer).stayInLoop;
  }

  // searches right-upwards for possible moves
  stayInLoop = 1;
  for (i = 1; col + i <= 8 && row + i <= 8 && stayInLoop; i++) {
    stayInLoop =
        sCheckSquare(row + i, col + i, board_ptr, movedSquarePlayer).stayInLoop;
  }
}

/**
 * @fn void vCheckSurroundings(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer)
 * @brief Checks the surrounding squares for moves
 * 
 * @param row 
 * @param col 
 * @param board_ptr 
 * @param movedSquarePlayer 
 */
void vCheckSurroundings(u_int8_t row, u_int8_t col,
                        struct s_Square *board_ptr[8][8],
                        unsigned char movedSquarePlayer) {

  // only used for King
  // | X | X | X | -> row3
  // | X | K | X | -> row2
  // | X | X | X | -> row1
  //   |   |   |
  //   v   v   v
  // col1 col2 col3

  for (int r = row - 1; r <= row + 1; r++) { // from row1 to row3
    if (r < 1 || r > 8) {
      continue; // Skip rows outside the board.
    }
    for (int c = col - 1; c <= col + 1; c++) {
      if (c < 1 || c > 8) {
        continue; // Skip columns outside the board.
      }
      if (r == row && c == col) {
        continue; // Skip the center square.
      }
      sCheckSquare(r, c, board_ptr, movedSquarePlayer);
    }
  }
}

void vCheckKnight(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8],
                  unsigned char movedSquarePlayer) {

  // only used for Knights
  // |   | X |   | X |   | -> row5
  // | X |   |   |   | X | -> row4
  // |   |   | N |   |   | -> row3
  // | X |   |   |   | X | -> row2
  // |   | X |   | X |   | -> row1
  //   |   |   |   |   |
  //   v   v   v   v   v
  // col1 col2 col3 col4 col5

  for (int r = row - 2; r <= row + 2; r++) { // from row1 to row5
    if (r < 1 || r > 8) {
      continue; // Skip rows outside the board.
    }
    for (int c = col - 2; c <= col + 2; c++) {
      if (c < 1 || c > 8) {
        continue; // Skip columns outside the board.
      }
      if (r == row || c == col) {
        continue; // Skip the center row and col.
      }
      if ((r == row - 2 || r == row + 2) && (c == col - 1 || c == col + 1)) {
        sCheckSquare(r, c, board_ptr, movedSquarePlayer);
      }
      if ((r == row - 1 || r == row + 1) && (c == col - 2 || c == col + 2)) {
        sCheckSquare(r, c, board_ptr, movedSquarePlayer);
      }
    }
  }
}

void vCheckPawn(u_int8_t row, u_int8_t col, 
                struct s_Square *board_ptr[8][8],
                unsigned char movedSquarePlayer) {
    
    // only used for Pawns

    // | X | X | X | -> row3
    // |   | P |   | -> row2
    // | X | X | X | -> row1
    //   |   |   | 
    //   v   v   v  
    // col1 col2 col3

    // if white moves (lower row to higher row)
    if (movedSquarePlayer == 'w') {
        // check to move in front 
        sCheckSquarePawn(row+1, col, board_ptr, movedSquarePlayer, MOVE);

        // check to take front-left and front-right
        sCheckSquarePawn(row+1, col-1, board_ptr, movedSquarePlayer, TAKE);
        sCheckSquarePawn(row+1, col+1, board_ptr, movedSquarePlayer, TAKE);

        if (row == 2) { // first move,
            sCheckSquarePawn(row+2, col, board_ptr, movedSquarePlayer, MOVE);
        }
    }

    // if black moves
    if (movedSquarePlayer == 'b') {
        // check to move in front 
        sCheckSquarePawn(row-1, col, board_ptr, movedSquarePlayer, MOVE);

        // check to take front-left and front-right
        sCheckSquarePawn(row-1, col+1, board_ptr, movedSquarePlayer, TAKE);
        sCheckSquarePawn(row-1, col-1, board_ptr, movedSquarePlayer, TAKE);

        if (row == 7) { // first move,
            sCheckSquarePawn(row-2, col, board_ptr, movedSquarePlayer, MOVE);
        }
    }


    // TODO: En Passant
  
}

void vShowPossibleMoves(u_int8_t row, u_int8_t col,
                        struct s_Square *board_ptr[8][8]) {

  unsigned char movedSquarePiece;
  movedSquarePiece = board_ptr[row - 1][col - 1]->piece;
  unsigned char movedSquarePlayer;
  movedSquarePlayer = board_ptr[row - 1][col - 1]->player;

  if (movedSquarePiece == 'p') { // p -> pawn
    // TODO: En Passant

  } else if (movedSquarePiece == 'R') { // R -> rook
    vCheckFullRowCol(row, col, board_ptr, movedSquarePlayer);

  } else if (movedSquarePiece == 'N') { // N -> knight
    vCheckKnight(row, col, board_ptr, movedSquarePlayer);

  } else if (movedSquarePiece == 'B') { // B -> bishop
    vCheckFullDiagonals(row, col, board_ptr, movedSquarePlayer);

  } else if (movedSquarePiece == 'Q') { // Q -> queen
    vCheckFullRowCol(row, col, board_ptr, movedSquarePlayer);
    vCheckFullDiagonals(row, col, board_ptr, movedSquarePlayer);

  } else if (movedSquarePiece == 'K') { // K -> king
    vCheckSurroundings(row, col, board_ptr, movedSquarePlayer);

    // TODO: Castling
  } else ;

}

/**
 * @fn void updateLED(u_int8_t row, u_int8_t col, struct s_Square *board_ptr[8][8], u_int8_t R, u_int8_t G, u_int8_t B, RgbColorPacket *all_LED_Signal[64])
 * 
 * @brief Updates the color of the LED associated with the specific board coordinate 
 * 
 * @param row 
 * @param col 
 * @param board_ptr 
 * @param R 
 * @param G 
 * @param B 
 * @param all_LED_Signal 
 */
void vUpdateLED(u_int8_t row, u_int8_t col, 
               struct s_Square *board_ptr[8][8], 
               u_int8_t R, u_int8_t G, u_int8_t B, 
               RgbColorPacket *all_LED_Signal[64]){

    all_LED_Signal[board_ptr[row-1][col-1]->LEDNum]->red = R;
    all_LED_Signal[board_ptr[row-1][col-1]->LEDNum]->green = G;
    all_LED_Signal[board_ptr[row-1][col-1]->LEDNum]->blue = B;

}

void vReadShiftReg(u_int8_t row, u_int8_t col, 
               struct s_Square *board_ptr[8][8], 
               u_int8_t R, u_int8_t G, u_int8_t B, 
               u_int16_t *value){
    // for (int8_t i; i<8; i++) {
    //     for (int8_t j; j<8; j++) {
    //         board_ptr[i][j]
    //     }
    // }//TODO: finish this 
}


/** 
 * @fn  void vGenerateFEN(struct s_Square *board_ptr[8][8], 
                  char * FEN_String[90])
 * @brief Generate FEN String from Board Status
 * 
 * @param board_ptr 
 * @param FEN_String 
 */
void vGenerateFEN(struct s_Square *board_ptr[8][8], 
                  char * FEN_String[90], unsigned char playerType, int moveNumber){
  uint8_t blankCounter = 0;
  for (int i=7; i>=0; i--) {
    for (int j=0; j<8; j++) {
      switch (board_ptr[i][j]->player) {
        case('b'):
          if (blankCounter!=0){
            sprintf(FEN_String, "%s%d", FEN_String, blankCounter);
            blankCounter = 0;
          }

          switch (board_ptr[i][j]->piece) {
            case('p'):
              strcat(FEN_String, "p");
              break;
            case('R'):
              strcat(FEN_String, "r");
              break;
            case('N'):
              strcat(FEN_String, "n");
              break;
            case('B'):
              strcat(FEN_String, "b");
              break;
            case('Q'):
              strcat(FEN_String, "q");
              break;
            case('K'):
              strcat(FEN_String, "k");
              break; 
            default:
              break;
          }
          break;
        case('w'):
          if (blankCounter!=0){
            sprintf(FEN_String, "%s%d", FEN_String, blankCounter);
            blankCounter = 0;
          }

          switch (board_ptr[i][j]->piece) {
            case('p'):
              strcat(FEN_String, "P");
              break;
            case('R'):
              strcat(FEN_String, "R");
              break;
            case('N'):
              strcat(FEN_String, "N");
              break;
            case('B'):
              strcat(FEN_String, "B");
              break;
            case('Q'):
              strcat(FEN_String, "Q");
              break;
            case('K'):
              strcat(FEN_String, "K");
              break; 
            default:
              break;
          }
          break;
        case('e'):
          blankCounter+=1;
          break;
        default:
          break;
      }
    }
    if (blankCounter!=0){
      sprintf(FEN_String, "%s%d", FEN_String, blankCounter);
      blankCounter = 0;
    }
    if (i!=0) {
      strcat(FEN_String, "/"); // Print the FEN_String
    }
  }
  
  unsigned char invertPlayer;
  if(playerType = 'w'){
	  invertPlayer = 'b';
  } else if (playerType = 'b'){
	  invertPlayer = 'w';
  }
  
  sprintf(FEN_String, "%s %c - - 0 %d", FEN_String, invertPlayer, moveNumber);
  
}