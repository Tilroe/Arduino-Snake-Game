#include "serial_handling.h"
#include "EEPROM.h"

extern shared_vars shared;

const uint16_t buf_size = 256;
char* buffer = (char *) malloc(buf_size); // buffer for reading characters

// Waits for information from desktop game logic program about
// what next to perform
void waitforsigns(){
    int idx = 0;
	char in_char = NULL;

	while (Serial.available()<0){/* wait for characters on serial port */}

    // Read into buffer until a new line is read
	while (in_char != '\n') {
	    while (Serial.available()>0){
	      in_char = Serial.read();
	      buffer[idx] = in_char;
	      idx++;
	    }
    }
    //Space in the end of the buffer to parse the line easily
    buffer[idx-1] = ' ';
    

     // Convert buffer to string
  	String buffer_str;
    buffer_str = String(buffer);
  	String string_coord;

    //Starting point of reading coords from the buffer
    int index = 2;
    
    //Get either G or F and the coordinates, or E
    char character_read = buffer[0]; // General instruction (G, F or E)

    if (((character_read) == 'G')||(character_read == 'F')){

    	for (int i=0;i<4;i++) { // Four coordinates to be read

            // Determines to indeces of the start and end of the coordinate being read
            // (could be anywhere from 1 to 3 characters long)
            int start = index;
    		while (buffer[index]!= ' '){
    			index++;
    		}
    		int end = index;

            // Get coordinate as string	
    		string_coord = buffer_str.substring(start,end);

            // Convert coord string to integer and store it
    		int coord = (string_coord).toInt();
    		shared.mycoord[i] = coord;
            index = end+1 ;
    	}

        // Draw depending on instruction
        if ((character_read) == 'G'){
            drawsnake();
        }
        else{ // character_read == 'F'
            draweatfruit();
        }   
    }

    // If neither G nor F were recieved, an E was recieved - reset game
    else{
        shared.direction = "AU"; // default direction
        EEPROM[0] = shared.score; // put score in first index of EEPROM to be sorted
        shared.score = 0; // Reset current score
    	drawendgame(); // Draw game over then highscores
    }
}