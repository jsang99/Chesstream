#ifndef SQUARE_H_
#define SQUARE_H_

#include <stdint.h>

// TODO: use enum for some attributes

struct s_Square {
  // Coordinate
  uint8_t row; // numbers from 1 to 8 inclusive
  uint8_t col; // 1->a
                // 2->b
                // 3->c
                // 4->d
                // 5->e
                // 6->f
                // 7->g
                // 8->h

  // Piece Type
  unsigned char piece; // p -> pawn
                       // R -> rook
                       // N -> knight
                       // B -> bishop
                       // Q -> queen
                       // K -> king
                       // ' ' -> empty square

  // Player
  unsigned char player; // w -> white
                        // e -> empty
                        // b -> black

  // LED Number
  uint8_t LEDNum; // from 0 to 63 inclusive

  // Shift register bit number
  uint8_t shiftRegNum; // from 0 to 63 inclusive

  // LED status
  unsigned char LEDStatus; // r -> can take this piece
                           // g -> can move to this square
                           // b -> special moves (castling, en passant)
                           // ' ' -> LED off
};

#endif // SQUARE_H_
