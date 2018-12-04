#ifndef BALL_H
#define BALL_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "player.h"

using namespace cv;
using namespace std;

class Ball {

public:

	Ball(unsigned int radius, Scalar color, unsigned int speed, int windowWidth, int windowHeight);
	~Ball();
	Point getPosition();
	Point getDirection();
	int getRadius();
	int getCollisionNumber();
	bool checkCollision(Player p);
	void move();
	void reset();
	void render(Mat img);

private:

	Point _position;
	Point _direction;
	int _radius;
	int _numOfCollisions;
	int _windowWidth;
	int _windowHeight;
	unsigned int _speed;
	unsigned int _initialSpeed;
	Scalar _color;

};

#endif // BALL_H
