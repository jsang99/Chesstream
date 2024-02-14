# Chesstream

    * Team Number: 2
    * Team Name: Chesstream
    * Team Members: Victor Gao
    * Description of test hardware: 
       * Developed on Mac and Windows with Altium + Atmel Studio + VSCode
       * Tested on SAMW25
       * Deployed on SAMD21G18 + WINC1500

<img src="https://github.com/jsang99/Chesstream/assets/60195598/b5e20076-c7f9-47c5-ba45-d0afed46c3d1" alt="drawing" style="width:600px;"/>

# Video Presentation
You can find our video presentation linked [here](https://youtu.be/DB6ynQ7qNNc).

# Project Photos & Screenshots
You can find our project photos linked [here](https://drive.google.com/drive/folders/1FITRqvcP4e3Jklzj9Cre_g566sTN2i4j?usp=drive_link).
# Project Summary
## 1. Device Description
Chesstream aims to bring together the physical touch of classical chess as well as the connectivity of online chess. The internet interface allows us to stream a physical game anywhere in the world, play against a chess bot one a real board, and gain real time commentary on the state of a game. 
## 2. Inspiration
We are both casual chess player who want to bring modernity to the ancient game of chess, without sacrificing its traditional physical charm. 
## 3. Device Functionality

![Screenshot 2023-12-18 at 22 01 04](https://github.com/ese5160/a14-final-submission-group-chesstream/assets/60195598/9630a315-c1c5-417f-aad0-a048f60cf3a3)

### Sensors
#### Reed Switch
Our sensors consist of 64 reed switches. They are used to detect the presence of chess pieces glued to magnets in each tile. They have a binary state, which allow us to map their on/off status to whether there is/is not a piece in the tile. Every 8 of them are connected to 1 PISO shift registers. All shift registers are then dairy chained together to provide a serial input to a GPIO pin on our processor board. 

### Actuators
#### LED
We have in total 64 addressable RGB LEDs. Similar to the reed switches, they are assigned to each indivitual tile. They are connected in series and are individually addresseable with a RGB888 based custom scheme. Their behaviors are defined as below:
| LED Color | Purpose                             |
|-----------|-------------------------------------|
| Off       | Default                             |
| Green     | Legal move for lifted piece         |
| Red       | Legal capture for lifted piece      |
| Blue      | Origin tile of top engine move      |
| Purple    | Destination tile of top engine move |

#### OLED Display Screen
The OLED Display provides the player with a quick summary of what move has been made on the board. This I2C based screen is not our main display, but it is an easy way to provide a brief feedback to the user. 

## 4. Project Links
Provide a URL to your Node-RED instance for our review (make sure it’s running on your Azure instance!)
Provide the share link to your final PCBA on Altium 365.
* NodeRed Instance (Link no longer works due to server shutdown :/) [link](http://172.203.132.103:1880/#flow/6876b6f51a74c211)
* Main Board PCBA Alitum365 [link](https://365.altium.com/files/D4E06251-FFA3-47C6-89CF-CEFF8A098985)
* Quater Module PCBA Altium365 [link](https://365.altium.com/files/D596A9AF-ADFE-4079-915E-C864F213AB50)

## 5. Challenges
Where did you face difficulties? This could be in firmware, hardware, software, integration, etc.
How did you overcome these challenges?
### Software
#### Unique Identifiers for Chess Piece (or Lack Thereof)
We do not have unique identifiers for each of the pieces. Our magnetism based sensors can only tell whether there is or is not a piece on their respective tile. We resolved this by deploying a software solution that tracks each piece and passes the peice information from one tile to another tile when a movement takes place. Our solution requires a standard chess start position as an intial state, from which the tracking is implemented. Unit tests are conducted in laptop terminal first before deployed to the MCU. 

### Firmware
#### Integrated Code Overflows RAM
After testing each individual threads, the integrated code refuses to compile and encounters "region 'ram' overflowed by XX bytes" error. We resolved this by reducing the pre-assigned RAM for MQTT related tasks from 512 bytes to 256 bytes. 

### Hardware
#### Daisy Chain and Signal Integrity
We initially created 1 singular daisy chain encompassing all 4 modular sensor-actuator boards. However, we had repeated connectivity issues with the last board in the change no matter which module we choose. We suspect that the delays introduced by the signal passing through each components in the chain might have either reduced the signal strength or skewed the clock signal, rendering the synchronous communication completely useless. We solved this by using two shorter daisy chains encompassing two boards each. 

#### Reed Switch Sensitivity
The reed switches seem to have manufacture issues. Some of them are easily stuck at an open/close position after exposure to a magnetic field. Others simply react with extreme sensitivity and would flicker without apparent chess movement. We resolved this by replacing and resoldering obviously defective switches, and introducing delays and filtering functions to 'debounce' the switches. 

## 6. Prototype Learnings
What lessons did you learn by building and testing this prototype?
If you had to build this device again, what would you do differently?
### "If It's Not Tested, It Doesn't Work!"
Things can look on paper/IDE/schematic, but before we actually deploy it to our hardware, they could all fail in ways we could not possibly fathom. 

### Double Checking Custom 3D Component Models
Our LED models were flipped 180 degrees. Luckily we only had to invert our power rails and input/output lines to fix it, but this could have been much worse and derailed our entire project.

### The More Comments, The More Better
Codes written a couple days ago can be completely unreadable without proper variable naming schemes and comments. Same is true for pin layouts on PCBAs without proper silk screen labels. 

## 7. Next Steps
### Better Sensors 
The lackluster quality of the reed switches seems to be a major bottleneck for user experience. Software solutions can only do so much to patch up the problem. Either a new sensing scheme or more reliable sensors will provide a smoother user experience. 

### Firmware Reliability
Our firmware still crashes when too many moves are made in sequence. We believe better placed semaphores will prevent too many threads trying to address the LED at the same time, which seems to occasionally produce racing conditions. 

### Full Chess Game Experience
We have implemented all legal chess moves except for castling and en passant. These are curcial functions that can be implemented with more time and testing. Additionally, pawn-promotion is curently set to auto promotion to queen. An interface that allows for promotion to any other piece would be desirable. 

### Custom Chess Clock
An integrated chess clock would be ideal for a more realistic competitive experience. 

## 8. Takeaways from ESE5160
Electronic product design is truely an arduous and all-encompassing task. It is multi-disciplinary in nature and requires multiple iterations of SW/FW/HW development cycles to reach commercial marketablitiy. 


# Codebase
All code needed to compile this project has been uploaded to this repository. 
External code base used:
* [chessboard.js](https://chessboardjs.com/index.html)
* [stockfish.online](https://stockfish.online/)
* [OpenAI](https://platform.openai.com/docs/overview)
* [OLED Library](https://github.com/elektro155/OLED-display-SSD1306-library-converted-for-ARM-C-language)

