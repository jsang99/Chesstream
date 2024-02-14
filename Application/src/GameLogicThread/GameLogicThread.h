/*
 * GameLogicThread.h
 *
 * Created: 2023-12-15 8:38:04 PM
 *  Author: vicga
 */ 


#ifndef GAMELOGICTHREAD_H_
#define GAMELOGICTHREAD_H_

#include "ShiftRegDriver/ShiftReg.h"
#include "WS2812BDriver/custom_ws2812b_driver.h"
#include "GAME LOGIC/boardFunctions.h"
#include "GAME LOGIC/square.h"
//#include "OLED/SSD1306.h"
#include <stdbool.h>



#define GL_TASK_SIZE 800  //<Size of stack to assign to the GL thread. In words
#define GL_TASK_PRIORITY (configMAX_PRIORITIES - 2)
#define MAGIC_CONVERSION 97

typedef enum gamelogicStateMachine_state {
	GL_STATE_READ_ARRAY = 0,	  ///< State used to read reed switches
	GL_STATE_PIECE_LIFT,		  ///< State used for when a piece has been lifted off the board
	GL_STATE_PIECE_PLACE,		  ///< State used for when a piece that has been lifted off is placed down
	GL_STATE_PIECE_CAPTURE,		  ///< State for when a capture is in progress
	GL_STATE_SHOW_B_MOVE,         ///< State to show best move
	GL_STATE_MAX_STATES           ///< Max number of states

} gamelogicStateMachine_state;

typedef enum piece_detect_state {
	PIECE_DETECT_NULL = 0,
	PIECE_DETECT_LIFT,
	PIECE_DETECT_PLACE,
	PIECE_DETECT_MAX_STATES
} piece_detect_state;

/******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

struct s_Square board[8][8];
struct s_Square *board_ptr[8][8];


/******************************************************************************
 * Global Function Declaration
 ******************************************************************************/
void vGameLogicThreadTask(void *pvParameters);
piece_detect_state checkDifferenceReed(void);
bool helperCheckPossibleMoves(unsigned char pieceType, uint8_t coordinate, unsigned char player, struct s_Square *board_ptr[8][8]);
void printMoveOLED();
void printWaitOLED();
bool checkCaptureValid();
void sendMQTT_GS_FEN();





#endif /* GAMELOGICTHREAD_H_ */