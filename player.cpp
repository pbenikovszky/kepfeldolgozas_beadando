#include "player.h"
#include "ball.h"
#include "defines.h"

Player::Player(Point initialPosition, unsigned int width, unsigned int height, Scalar color, string name, unsigned int speed, int side, int windowWidth, int windowHeight) {
	_position = initialPosition;
	_initialPosition = initialPosition;
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_width = width;
	_height = height;
	_color = color;
	_name = name;
	_speed = speed;
	_score = 0;
	_side = side;
}

Player::~Player() {

}

unsigned int Player::getWidth() {
	return _width;
}

unsigned int Player::getHeight() {
	return _height;
}

unsigned int Player::getScore() {
	return _score;
}

Point Player::getPosition() {
	return _position;
}

Scalar Player::getColor() {
	return _color;
}

string Player::getName() {
	return _name;
}

Rect Player::getRect() {
	return Rect(_position.x, _position.y, _width, _height);
}

int Player::getSide() {
	return _side;
}

void Player::moveUp() {
	_position.y = _position.y - _speed;
	if (_position.y < TOPROW_HEIGHT) {
		_position.y = TOPROW_HEIGHT;
	}
}

void Player::moveDown() {
	_position.y = _position.y + _speed;
	if (_position.y > _windowHeight - _height) {
		_position.y = _windowHeight - _height;
	}
}

void Player::moveCPU(unsigned int ballPos) {
	if (ballPos > _position.y + _height + 10) {
		moveDown();
	}
	else if (ballPos < _position.y - 10) {
		moveUp();
	}
}

void Player::score() {
	_score++;
}

void Player::reset() {
	_position = _initialPosition;
}

void Player::render(Mat img) {
	rectangle(img, getRect(), _color, -1, 8);
}