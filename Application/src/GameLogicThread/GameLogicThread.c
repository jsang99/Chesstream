/*
 * GameLogicThread.c
 *
 * Created: 2023-12-15 8:37:52 PM
 *  Author: vicga
 */ 


/******************************************************************************
 * Includes
 ******************************************************************************/

#include "GameLogicThread/GameLogicThread.h"
#include <errno.h>

#include "OLED/SSD1306.h"
#include "SerialConsole.h"
#include "WifiHandlerThread/WifiHandler.h"
#include "asf.h"
#include "main.h"

/******************************************************************************
 * Defines
 ******************************************************************************/


/******************************************************************************
 * Variables
 ******************************************************************************/
gamelogicStateMachine_state gameState;         ///< Holds the current state of the game
// struct GameDataPacket gamePacketIn;   ///< Holds the game packet to show
// struct GameDataPacket gamePacketOut;  ///< Holds the game packet to send back
// volatile uint8_t red = 0;             ///< Holds the color of the red LEDs. Can be set by MQTT
// volatile uint8_t green = 100;         ///< Holds the color of the green LEDs. Can be set by MQTT
// volatile uint8_t blue = 50;           ///< Holds the color of the blue LEDs. Can be set by MQTT

// uint8_t pressedKeys = 0;              ///< Variable to count how many presses the player has done
// uint8_t keysToPress = 0;              ///< Variable that holds the number of new keypresses the user should do
// bool playIsDone = false;              ///< Boolean flag to indicate if the player has finished moving.
//                                       ///< Useful for COntrol to determine when to send back a play.
unsigned char lift_piece_type;
unsigned char lift_piece_player;

uint8_t primary_piece_lift; ///< Coordinate of the piece being lifted (add second one for capture)
uint8_t primary_piece_place; ///< Coordinate of the piece being placed
uint8_t secondary_piece_lift;

int moveNumber = 0;

bool reed_state_temp [NUM_REEDS]; ///< Temp array use for filtering

bool board_state[NUM_REEDS];  ///< Array to hold current state of the board, in terms of which squareS are occupied
char bufferPrint[64];  ///< Buffer for debug
char fenString[90];
char updateGUIString[25];
/******************************************************************************
 * Forward Declarations
 ******************************************************************************/

/******************************************************************************
 * Callback Functions
 ******************************************************************************/

/******************************************************************************
 * Task Function
 ******************************************************************************/

void vGameLogicThreadTask(void *pvParameters){
	
	SerialConsoleWriteString("Game Logic Task Started!");
	
	vInitBoard(board, board_ptr);
	
	gameState = GL_STATE_READ_ARRAY;
	primary_piece_lift = -1;
	globalTrackPlayer = 'w';
	
	
	
	//Initialize board depending on initial positions of the pieces on the board
	read_array();
	for(int i = 0; i<NUM_REEDS; i++){
		board_state[convert_to_chess_tile(i)] = reed_state[i];
	}
	
	loop:
	while(1){
		switch(gameState){
			case(GL_STATE_READ_ARRAY): {
				//SerialConsoleWriteString("IN READ ARRAY STATE\r\n");

				if(checkDifferenceReed() == PIECE_DETECT_LIFT){
					//SerialConsoleWriteString("DIFFERENCE DETECTED! \r\n");
					gameState = GL_STATE_PIECE_LIFT;
				}
				break;
			}
			case(GL_STATE_PIECE_LIFT): {
				SerialConsoleWriteString("IN PIECE LIFT STATE\r\n");
// 				snprintf(bufferPrint, 64, "Piece lifted at square %d, %d!\r\n", primary_piece_change/8 + 1, (primary_piece_change % 8) + 1);
// 				SerialConsoleWriteString(bufferPrint);
				//Show possible moves
				
				lift_piece_type =  board_ptr[primary_piece_lift/8][primary_piece_lift%8]->piece;
				lift_piece_player = board_ptr[primary_piece_lift/8][primary_piece_lift%8]->player; 
				
				printWaitOLED();
				reset_LED_board_with_struct();
				
				if(helperCheckPossibleMoves(lift_piece_type, primary_piece_lift, lift_piece_player, board_ptr)){
					set_LED_board_with_struct(); //Set LED
					SerialConsoleWriteString("Setting LED\r\n");
					
					//Call MQTT if best possible moves mode is ON
					
					while(checkDifferenceReed() != PIECE_DETECT_PLACE){ //Wait for piece to be placed
						snprintf(bufferPrint, 64, "Waiting for piece at %d, %d to be placed\r\n", primary_piece_lift/8+1, primary_piece_lift%8+1);
						SerialConsoleWriteString(bufferPrint);
 						if(checkCaptureValid()){
							SerialConsoleWriteString("Second lift detected and valid - entering capture mode!\r\n");
							gameState = GL_STATE_PIECE_CAPTURE;
							goto loop;
						}

					}	
					reset_LED_board_with_struct();
					gameState = GL_STATE_PIECE_PLACE;
					break;
				} else { //If blank tile was triggered, set state back to initial read
					gameState = GL_STATE_READ_ARRAY;
					break;
				}	
			}
			case(GL_STATE_PIECE_PLACE): {
				reset_board_LED();
				
				//Update game board struct with position of new piece, using primary_piece_lift and primary_piece_place
				
				board_ptr[primary_piece_lift/8][primary_piece_lift%8]->piece = ' '; //Set tile the piece was lifted from to be empty
				board_ptr[primary_piece_lift/8][primary_piece_lift%8]->player = 'e';
				board_ptr[primary_piece_place/8][primary_piece_place%8]->piece = lift_piece_type; //Set tile the piece was placed on to have the appropriate type
				board_ptr[primary_piece_place/8][primary_piece_place%8]->player = lift_piece_player;
				
				
				
				
				SerialConsoleWriteString("PIECE PLACED! GOING BACK TO READ\r\n");
				
				if(primary_piece_lift != primary_piece_place){
					sendMQTT_GS_FEN();
					reset_LED_board_with_struct();
					printMoveOLED();
				}
				
				
				primary_piece_lift = -1;
				primary_piece_place = -1;
				
				gameState = GL_STATE_READ_ARRAY;
				break;
			}
			case(GL_STATE_PIECE_CAPTURE): {
				
				while(checkDifferenceReed() != PIECE_DETECT_PLACE){
					SerialConsoleWriteString("Waiting for valid capture move\r\n");
				}

				if(primary_piece_place == secondary_piece_lift){ //Only way to progress from here is to move the primary lifted piece into the square of the secondary lifted piece
					
					board_ptr[primary_piece_lift/8][primary_piece_lift%8]->piece = ' '; //Set tile the piece was lifted from to be empty
					board_ptr[primary_piece_lift/8][primary_piece_lift%8]->player = 'e';
					board_ptr[primary_piece_place/8][primary_piece_place%8]->piece = lift_piece_type; //Set tile the piece was placed on to have the appropriate type
					board_ptr[primary_piece_place/8][primary_piece_place%8]->player = lift_piece_player;
					
					SerialConsoleWriteString("CAPTURE COMPLETE!\r\n");
					
					if(primary_piece_lift != primary_piece_place){
						sendMQTT_GS_FEN();
						reset_board_LED();
						reset_LED_board_with_struct();
						printMoveOLED();
					}
					
					primary_piece_lift = -1;
					primary_piece_place = -1;
					secondary_piece_lift = -1;
					
					gameState = GL_STATE_READ_ARRAY;
				}
				break;
				
			}
			default: 
				gameState = GL_STATE_READ_ARRAY;
		}
	}
}

//check difference reed should return whether a piece has been lifted or if a piece has been placed
piece_detect_state checkDifferenceReed(void){
	
	read_array(); //Updates values in reed_state -IF FILTERING, FILTER HERE!
	
// 	for(int i = 0; i<NUM_REEDS; i++){
// 		reed_state_temp[i] = reed_state[i]; //Copy first read to temp array
// 	}
// 	
// 	read_array();
// 	
// 	for(int i = 0; i<NUM_REEDS; i++){
// 		if(reed_state_temp[i] != reed_state_temp[i]){
// 			return PIECE_DETECT_NULL;		//Only validate change if two consecutive reads produce the same result
// 		}
// 	}
	
	for(int i = 0; i<NUM_REEDS; i++){
		int coord = convert_to_chess_tile(i);
		if(board_state[coord] != reed_state[i]){
			
			if(reed_state[i]){ //Piece has been placed
				
				snprintf(bufferPrint, 64, "Piece place checked at %d, %d!\r\n", coord/8 + 1, (coord % 8) + 1);
				SerialConsoleWriteString(bufferPrint);
				
				if((board_ptr[coord/8][coord%8]->LEDStatus != ' ' || coord == primary_piece_lift) && (gameState == GL_STATE_PIECE_LIFT || gameState == GL_STATE_PIECE_CAPTURE)){ //Validating move
					
					snprintf(bufferPrint, 64, "piece placed at square %d, %d!\r\n", coord/8 + 1, (coord % 8) + 1);
					SerialConsoleWriteString(bufferPrint);
					board_state[coord] = reed_state[i]; //Update board_state	
					primary_piece_place = coord;	//Save what square is being changed
					return PIECE_DETECT_PLACE;
					
				}
				SerialConsoleWriteString("NOTHING CHANGED\r\n");
				return PIECE_DETECT_NULL;
			} else {
				
				if((board_ptr[coord/8][coord%8]->piece != ' ') && gameState == GL_STATE_READ_ARRAY){
					
					vTaskDelay(1000); //To avoid accidental trips, wait for a period of time before reading it a second time
					
					read_array();
					
					if(!reed_state[convert_to_shift_index(coord)] && board_ptr[coord/8][coord%8]->player == globalTrackPlayer){ //Convert coord to index, and check again
						primary_piece_lift = coord;		//Save what square is being changed
						board_state[coord] = reed_state[i]; //Update board_state
						snprintf(bufferPrint, 64, "piece lifted at square %d, %d!\r\n", coord/8 + 1, (coord % 8) + 1);
						SerialConsoleWriteString(bufferPrint);
						if(globalTrackPlayer == 'w'){
							globalTrackPlayer ='b';
						}
						else if(globalTrackPlayer == 'b'){
							globalTrackPlayer = 'w';
						}
						return PIECE_DETECT_LIFT;
					}
					
					
				}
				SerialConsoleWriteString("NOTHING CHANGED\r\n");
				return PIECE_DETECT_NULL;
			}
		}
	}
	
	return PIECE_DETECT_NULL;
}

//Returns false if an invalid piece change was triggered (i.e. when blank tile gets triggered or piece with no possible move gets triggered)
bool helperCheckPossibleMoves(unsigned char pieceType, uint8_t coordinate, unsigned char player, struct s_Square *board_ptr[8][8]){
	switch(pieceType){
		case('p'):{
			vCheckPawn(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case('R'):{
			vCheckFullRowCol(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case('N'):{
			vCheckKnight(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case('B'):{
			vCheckFullDiagonals(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case('Q'):{
			vCheckFullRowCol(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			vCheckFullDiagonals(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case('K'):{
			vCheckSurroundings(coordinate/8 + 1, coordinate%8 + 1, board_ptr, player);
			break;
		}
		case(' '): {
			SerialConsoleWriteString("BLANK TILE TRIPPED!\\r\n");
			return false;
		}
		default: return false;
		
	}
	for(int i = 0; i<NUM_LED; i++){
		if(board_ptr[i/8][i%8]->LEDStatus != ' '){
			return true;
			
		}
	}
	return false;
}

bool checkCaptureValid(){
	
	read_array();
	
	for(int i = 0; i<NUM_REEDS; i++){
		int coord = convert_to_chess_tile(i);
		//If tile that changed is red and switch is open
		if(board_state[coord] != reed_state[i] && board_ptr[coord/8][coord%8]->LEDStatus == 'r' && !reed_state[i]){
			board_state[coord] = reed_state[i];
			secondary_piece_lift = coord;
			return true;
		}
	}
	return false;
}

void printMoveOLED(){
	char playerString[20];
	char pieceString[20];
	
	if(gameState == GL_STATE_PIECE_CAPTURE){
		if(lift_piece_player == 'w'){
			snprintf(playerString, 20, "White captures with their");
			} else {
			snprintf(playerString, 20, "Black captures with their");
		}
	} else {
		if(lift_piece_player == 'w'){
			snprintf(playerString, 20, "White moves their");
			} else {
			snprintf(playerString, 20, "Black moves their");
		}
	}
	
	switch(lift_piece_type){
		case('p'):{
			snprintf(pieceString, 20, "pawn");
			break;
		}
		case('R'):{
			snprintf(pieceString, 20, "rook");
			break;
		}
		case('N'):{
			snprintf(pieceString, 20, "knight");
			break;
		}
		case('B'):{
			snprintf(pieceString, 20, "bishop");
			break;
		}
		case('Q'):{
			snprintf(pieceString, 20, "queen");
			return true;
		}
		case('K'):{
			snprintf(pieceString, 20, "king");
			break;
		}
		default: return;
	}
	
	int row = primary_piece_place / 8 + 1;
	char col = primary_piece_place % 8 + MAGIC_CONVERSION;
	
	i2c_clear();
	i2c_setCursor(0, 0);
	i2c_print(playerString);
	i2c_setCursor(0, 2);
	
	snprintf(bufferPrint, 64, "%s to %c%d!\n", pieceString, col, row);
	i2c_print(bufferPrint);
}

void printWaitOLED(){
	char playerString[20];
	char message[20];
	if(lift_piece_player == 'w'){
		snprintf(playerString, 20, "Waiting for white");
		} else {
		snprintf(playerString, 20, "Waiting for black");
	}
	snprintf(message, 64, "to move...");
	
	i2c_setCursor(0, 4);
	i2c_print(playerString);
	i2c_setCursor(0, 5);
	i2c_print(message);
}


void sendMQTT_GS_FEN(){
	
	uint8_t rowPrev = primary_piece_lift / 8 + 1;
	unsigned char colPrev = primary_piece_lift % 8 + MAGIC_CONVERSION;
	uint8_t rowAfter = primary_piece_place / 8 + 1;
	unsigned char colAfter = primary_piece_place % 8 + MAGIC_CONVERSION;
	
	
	//Call MQTT to update game board
	
	/**********************************MQTT*********************************************/
	memset(updateGUIString, 0, sizeof(updateGUIString));
	sprintf(updateGUIString, "%s%c%d-%c%d", updateGUIString, colPrev, rowPrev, colAfter, rowAfter);
	SerialConsoleWriteString("SENDING GUI STRING: ");
	SerialConsoleWriteString(updateGUIString);
	SerialConsoleWriteString("\r\n");
	
	int error = WifiAddChessInputToQueue(updateGUIString);
	
	if (pdTRUE == error)
	{
		SerialConsoleWriteString("Chess Input sent!/n");
		} else {
		SerialConsoleWriteString("Chess Input not sent!/n");

	}
	/**********************************MQTT*********************************************/
	
	
	/**********************************MQTT*********************************************/
	memset(fenString, 0, sizeof(fenString));
	vGenerateFEN(board_ptr, fenString, lift_piece_player, moveNumber/2); //Generates fen string and fills it to fenString array
	
	error = WifiAddFENStringToQueue(fenString);
	if (pdTRUE == error)
	{
		SerialConsoleWriteString("FEN sent!/n");
		} else {
		SerialConsoleWriteString("FEN Not sent!/n");
	}
	
	moveNumber++;
	
	vTaskDelay(2000);
}