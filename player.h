#ifndef PLAYER_H
#define PLAYER_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Player {

public:

	Player(Point initialPosition, unsigned int width, unsigned int height, Scalar color, string name, unsigned int speed, int side, int windowWidth, int windowHeight);
	~Player();
	Point getPosition();
	Rect getRect();
	void moveUp();
	void moveDown();
	void moveCPU(unsigned int ballPos);
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getScore();
	int getSide();
	void score();
	string getName();
	Scalar getColor();
	void reset();
	void render(Mat img);

private:

	Point _position;
	Point _initialPosition;
	unsigned int _width;
	unsigned int _height;
	unsigned int _speed;
	unsigned int _score;
	int _windowWidth;
	int _windowHeight;
	// stores the side of the player
	// if the side value is negative, the player is on the left side
	// if non-negative, player is on the right side
	int _side;
	string _name;
	Scalar _color;


};

#endif // PLAYER_H
