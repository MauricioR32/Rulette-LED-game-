//LED Rullete Game!

//The purpose of this game is to stop the spnning LED 
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Wire.h>
#include <SPI.h>

//variables
bool gameOn = false; //Tracks the game if is active
bool spinning = false; //Tracks if the LED is spinning 
int currentLED = 0; //Tracks current LED let
int winningLED = 0; //Mark the LED Winning Spot 
int score = 0; // This will keep track of the score
unsigned long lastSpinTime = 0; //mlong variable from "https://www.geeksforgeeks.org/c-long/" Stores the last time LED Spined
unsigned int spinInterval = 100;// This wil interpret the time between the interval/// unssigned variable from "https://stackoverflow.com/questions/22587451/c-c-use-of-int-or-unsigned-int"


AsyncDelay spinDelay;

// Function declarations
void loop();
void startGame();
void stopGame();
void spinLEDs();
void checkWin();
void playSound(bool win);
void sayResult(bool win);


const uint8_t spSAFE[] PROGMEM = { 0x08,0xF8,0x39,0x4C,0x02,0x1A,0xD0,0x80,0x05,0x3C,0x60,0x81,0x95,0x0F,0x15,0xE2,0x6A,0xAB,0x4F,0xD1,0x43,0x8A,0x8A,0xBF,0xB9,0xD5,0xAD,0x57,0x3F,0xAA,0x23,0xBB,0x3F,0x9E,0xCB,0xDC,0xF3,0x99,0x9E,0x5E,0x19,0xCD,0xEB,0x8E,0x79,0x7A,0x43,0x13,0xED,0x39,0x0C,0x18,0x7E,0x5C,0x02,0x12,0x90,0x00,0x07,0x28,0x40,0x81,0xFF,0x07 };
const uint8_t spFAIL[] PROGMEM = { 0x04,0x98,0x3E,0x8D,0x03,0x1C,0xD0,0x80,0x07,0x4A,0xBF,0x54,0x9B,0x3A,0x79,0x9C,0xCD,0xAA,0x9B,0x0F,0x31,0x8F,0x37,0xB7,0xBE,0xCD,0x6A,0x47,0x2A,0x66,0xB3,0xB7,0xB3,0xDB,0x6B,0x5F,0xC7,0x56,0x44,0x58,0x8E,0x76,0xAA,0x7B,0xD8,0x33,0xB9,0x32,0xD7,0x3C,0xF9,0x0C,0x67,0xD4,0x13,0x9E,0x98,0xC7,0x5F,0xEE,0x49,0x7C,0xAA,0x8D,0xF3,0xF9,0xF7,0xFF,0x01 };

void setup() {
    CircuitPlayground.begin();//initialare circuit design
    Serial.begin(9600);

    // Begins game
    gameOn = false;
    spinning = false;
    currentLED = 0;
    score = 0;
// Randum generator
    randomSeed(analogRead(0));
}

void loop() {
    if (CircuitPlayground.slideSwitch()) { ///check if the switch is on
        if (!gameOn) {
            startGame();
        }
    } else {
        if (gameOn) {  // stops the game
            stopGame();
        }
    }

    if (gameOn) {
        if (CircuitPlayground.rightButton()) { /// starts spinning if right is pressed
            spinning = true;
            spinDelay.start(spinInterval, AsyncDelay::MILLIS);
        }

        if (spinning) {// just spins
            spinLEDs();
        }
        // Stop spinning and check if the player wins when the left button is pressed
        if (CircuitPlayground.leftButton()) {
            spinning = false;
            checkWin();
        }
    }
}

void startGame() {
    gameOn = true;  // Set game status to on
    winningLED = random(0, 10); // Randomly select a winning LED
    CircuitPlayground.setPixelColor(winningLED, 128, 0, 128);// Highlight the winning LED
    delay(1000); /// wait for 1 second
    CircuitPlayground.clearPixels(); // clears LEDs
}

void stopGame() {
    gameOn = false; // Set game status to inactive
    CircuitPlayground.clearPixels(); // Clear the LEDs
    score = 0;// Reset score
    Serial.println("Game reset");// Print reset message
}
// Check if the spin delay has expired
void spinLEDs() {
    if (spinDelay.isExpired()) {
        CircuitPlayground.clearPixels();// Clear the current LED
        currentLED = (currentLED + 1) % 10;//// Move to the next LED
        CircuitPlayground.setPixelColor(currentLED, 0, 0, 255); // Blue color for spinning
        spinDelay.restart();// restarts
    }
}
// This set of code will check if output is a winning LED spot 
void checkWin() {
    if (currentLED == winningLED) {
        score++; // increase score +1 if it's in the winning spot
        Serial.print("You win! Score: ");
        Serial.println(score); // Will print a winning messafe
        playSound(true); // Will play the winning sound
        sayResult(true); // WIl
        // Increase difficulty
        spinInterval = max(spinInterval - 10, 60); // Faster spinning
    } else {
        Serial.println("You lose!");
        playSound(false);
        sayResult(false);
        stopGame();
    }
}

void playSound(bool win) {
    if (win) {
        // Winning sound
        for (int i = 0; i < 3; i++) {
            CircuitPlayground.playTone(1000, 200);
            delay(200);
        }
    } else {
        // Losing sound
        CircuitPlayground.playTone(500, 500);
    }
}

// Added pre-recorded sound depending in the functions
void sayResult(bool win) {
    if (win) {
        CircuitPlayground.speaker.say(spSAFE); //Display sound when you win
    } else {

        CircuitPlayground.speaker.say(spFAIL); //Display sound when you lose
    }
}
