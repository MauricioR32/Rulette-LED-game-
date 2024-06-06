//LED Rullete Game!

//The purpose of this game is to stop the spnning LED 
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Wire.h>
#include <SPI.h>

//variables
bool gameOn = false;
bool spinning = false;
int currentLED = 0;
int winningLED = 0;
int score = 0;
unsigned long lastSpinTime = 0; //unssigned variable from "https://stackoverflow.com/questions/22587451/c-c-use-of-int-or-unsigned-int"
unsigned int spinInterval = 100;


AsyncDelay spinDelay;


void setup();
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
    CircuitPlayground.begin();
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
    if (CircuitPlayground.slideSwitch()) {
        if (!gameOn) {
            startGame();
        }
    } else {
        if (gameOn) {
            stopGame();
        }
    }

    if (gameOn) {
        if (CircuitPlayground.rightButton()) {
            spinning = true;
            spinDelay.start(spinInterval, AsyncDelay::MILLIS);
        }

        if (spinning) {
            spinLEDs();
        }

        if (CircuitPlayground.leftButton()) {
            spinning = false;
            checkWin();
        }
    }
}

void startGame() {
    gameOn = true;
    winningLED = random(0, 10);
    CircuitPlayground.setPixelColor(winningLED, 128, 0, 128);//
    delay(1000);
    CircuitPlayground.clearPixels();
}

void stopGame() {
    gameOn = false;
    CircuitPlayground.clearPixels();
    score = 0;
    Serial.println("Game reset");
}

void spinLEDs() {
    if (spinDelay.isExpired()) {
        CircuitPlayground.clearPixels();
        currentLED = (currentLED + 1) % 10;
        CircuitPlayground.setPixelColor(currentLED, 0, 0, 255); // Blue color for spinning
        spinDelay.restart();
    }
}

void checkWin() {
    if (currentLED == winningLED) {
        score++;
        Serial.print("You win! Score: ");
        Serial.println(score);
        playSound(true);
        sayResult(true);
        // Increase difficulty
        spinInterval = max(spinInterval - 10, 50); // Faster spinning
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

void sayResult(bool win) {
    if (win) {
        CircuitPlayground.speaker.say(spSAFE);
    } else {

        CircuitPlayground.speaker.say(spFAIL);
    }
}
