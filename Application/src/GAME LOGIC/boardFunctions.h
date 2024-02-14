/**************************************************************************//**
* @file      boardFunctions.h
* @brief     Headerfile for boardFunctions.c
* @author    Kevin Sang
* @date      Nov. 8, 2023

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "square.h"
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "GameLogicThread/GameLogicThread.h"

/******************************************************************************
* Defines
******************************************************************************/
#ifndef BOARDFUNCTIONS_H_
#define BOARDFUNCTIONS_H_

/******************************************************************************
* Structures and Enumerations
******************************************************************************/
struct s_CheckSquareResult {
    bool stayInLoop;
    bool canMove;
    bool canTake;
};

/******************************************************************************
* Global Function Declaration
******************************************************************************/
void vInitBoard();
void vClearLEDs(struct s_Square *board_ptr[8][8]);
struct s_CheckSquareResult sCheckSquare(uint8_t row, uint8_t col, struct s_Square * board_ptr[8][8], unsigned char movedSquarePlayer);
void vCheckFullRowCol(uint8_t row, uint8_t col, struct s_Square * board_ptr[8][8], unsigned char movedSquarePlayer);
void vCheckFullDiagonals(uint8_t row, uint8_t col, struct s_Square * board_ptr[8][8], unsigned char movedSquarePlayer);
void vShowPossibleMoves(uint8_t row, uint8_t col, struct s_Square * board_ptr[8][8]);
void vCheckSurroundings(uint8_t row, uint8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer);
void vCheckKnight(uint8_t row, uint8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer);
void vCheckPawn(uint8_t row, uint8_t col, struct s_Square *board_ptr[8][8], unsigned char movedSquarePlayer);
void vGenerateFEN(struct s_Square *board_ptr[8][8], char * FEN_String[90], unsigned char playerType, int moveNumber);

#endif // BOARDFUNCTIONS_H_