#ifndef DEFINES_H
#define DEFINES_H

// Window size

//#define WINDOW_WIDTH 640
//#define WINDOW_HEIGHT 480
#define TOPROW_HEIGHT 50

// Player size

#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 100

// Text and font sizes

#define FONT_SIZE 0.6
#define TITLE_TEXT "PONG"

// Default ball speed
#define DEFAULT_X_SPEED 5

// Collision treshold (vertical)
#define COLLISION_TRESHOLD 2
#define INCREASE_SPEED_TRESHOLD 7 // number of collision after speed is increased
// Keys

#define KEY_UP 72 // Up Arrow
#define KEY_UP_DEFAULT 119 // W
#define KEY_DOWN 80 // Down Arrow
#define KEY_DOWN_DEFAULT 115 // S

#define WAIT_TIMEFRAME 1

#endif // DEFINES_H
