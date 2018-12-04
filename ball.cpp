#include "ball.h"
#include "defines.h"
#include <time.h>

Ball::Ball(unsigned int radius, Scalar color, unsigned int speed, int windowWidth, int windowHeight) {
	srand(time(NULL));
	int x = DEFAULT_X_SPEED;
	int y;
	do {
		y = rand() % 6 - 3;
	} while (y == 0);
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_position = Point(_windowWidth / 2, (_windowHeight + TOPROW_HEIGHT) / 2);
	_direction = Point(x, y);
	_radius = radius;
	_color = color;
	_speed = speed;
	_initialSpeed = speed;
	_numOfCollisions = 0;
}

Ball::~Ball() {
}

Point Ball::getPosition() {
	return _position;
}

Point Ball::getDirection() {
	return _direction;
}

int Ball::getRadius() {
	return _radius;
}

bool Ball::checkCollision(Player p) {

	// return value, false by default
	bool isCollision = false;
	int collision = 0;
	// calculate top limit
	int top = p.getPosition().y - _radius + COLLISION_TRESHOLD;

	// calculate top limit
	int bottom = p.getPosition().y + p.getHeight() + _radius - COLLISION_TRESHOLD;

	// check vertical collision
	if (_position.y >= top && _position.y <= bottom) {
		//check horizontal collision
		if (p.getSide() < 0) {
			// Player is on the left side
			if (_position.x < p.getPosition().x + p.getWidth()) {
				isCollision = true;
			}
		}
		else {
			// Player is on the right side
			if (_position.x > p.getPosition().x) {
				isCollision = true;
			}
		}
	}

	if (isCollision == true) {
		_numOfCollisions++;
		if (_numOfCollisions % INCREASE_SPEED_TRESHOLD == 0) {
			_speed++;
		}
		_direction.x *= -1;
		// set the vertical angle

	}
	return isCollision;

}

void Ball::move() {
	_position.x += _speed * _direction.x;
	_position.y += _speed * _direction.y;

	if (_position.y > _windowHeight - _radius || _position.y < TOPROW_HEIGHT + _radius) {
		_direction.y *= -1;
	}

}

void Ball::reset() {
	srand(time(NULL));
	int xDirection = rand() % 2;
	int x = (xDirection == 0) ? -DEFAULT_X_SPEED : DEFAULT_X_SPEED;
	_numOfCollisions = 0;
	srand(time(NULL));
	int y;
	do {
		y = rand() % 6 - 3;
	} while (y == 0);
	_position = Point(_windowWidth / 2, (_windowHeight + TOPROW_HEIGHT) / 2);
	_speed = _initialSpeed;
	_direction = Point(x, y);
}

void Ball::render(Mat img) {
	circle(img, _position, _radius, _color, CV_FILLED);
}