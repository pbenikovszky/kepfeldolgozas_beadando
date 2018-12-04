#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <cmath>    
#include <windows.h>

#include "defines.h"
#include "player.h"
#include "ball.h"
#include "tracker.h"
#include "calibrator.h"


using namespace cv;
using namespace std;


int mouseY = 0;
int WINDOW_WIDTH;
int WINDOW_HEIGHT;

void drawField(Mat dest, Player p1, Player p2) {

	line(dest, Point(WINDOW_WIDTH / 2, TOPROW_HEIGHT), Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT), Scalar(255, 255, 255));
	line(dest, Point(0, TOPROW_HEIGHT), Point(WINDOW_WIDTH, TOPROW_HEIGHT), Scalar(255, 255, 255));
	circle(dest, Point(WINDOW_WIDTH / 2, (WINDOW_HEIGHT + TOPROW_HEIGHT) / 2), 75, Scalar(255, 255, 255));
	circle(dest, Point(WINDOW_WIDTH / 2, (WINDOW_HEIGHT + TOPROW_HEIGHT) / 2), 15, Scalar(255, 255, 255), CV_FILLED);


	// set the Title text
	Size titleTextSize = getTextSize(TITLE_TEXT, FONT_HERSHEY_DUPLEX, 1, 1, 0);
	int titlePosX = (WINDOW_WIDTH - titleTextSize.width) / 2;
	int titlePosY = TOPROW_HEIGHT / 2 + 10;

	putText(dest, TITLE_TEXT, Point(titlePosX, titlePosY), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1);

	// calculate the player text positions
	Size txtP1NameSize = getTextSize(p1.getName(), FONT_HERSHEY_DUPLEX, FONT_SIZE, 1, 0);
	Size txtP2NameSize = getTextSize(p2.getName(), FONT_HERSHEY_DUPLEX, FONT_SIZE, 1, 0);

	int txtPosY = (TOPROW_HEIGHT + txtP1NameSize.height) / 2;
	int txtP1NamePos = 10;
	int txtP2NamePos = WINDOW_WIDTH - txtP2NameSize.width - 10;

	string txtP1Score = "Score: " + to_string(p1.getScore());
	string txtP2Score = "Score: " + to_string(p2.getScore());
	Size txtP2ScoreSize = getTextSize(txtP2Score, FONT_HERSHEY_DUPLEX, FONT_SIZE, 1, 0);

	int txtP1ScorePos = txtP1NamePos + txtP1NameSize.width + 10;
	int txtP2ScorePos = txtP2NamePos - txtP2ScoreSize.width - 10;

	putText(dest, p1.getName(), Point(txtP1NamePos, txtPosY), FONT_HERSHEY_DUPLEX, FONT_SIZE, Scalar(255, 255, 255), 1);
	putText(dest, p2.getName(), Point(txtP2NamePos, txtPosY), FONT_HERSHEY_DUPLEX, FONT_SIZE, Scalar(255, 255, 255), 1);
	putText(dest, txtP1Score, Point(txtP1ScorePos, txtPosY), FONT_HERSHEY_DUPLEX, FONT_SIZE, Scalar(255, 255, 255), 1);
	putText(dest, txtP2Score, Point(txtP2ScorePos, txtPosY), FONT_HERSHEY_DUPLEX, FONT_SIZE, Scalar(255, 255, 255), 1);

} // drawField


void showTracker(Mat dest, int pos, Player p) {
	line(dest, Point(p.getPosition().x, pos - 5), Point(p.getPosition().x + 10, pos + 5), Scalar(0, 0, 255));
	line(dest, Point(p.getPosition().x + 10, pos - 5), Point(p.getPosition().x, pos + 5), Scalar(0, 0, 255));
}

void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
	if (event == EVENT_MOUSEMOVE) {
		mouseY = y;
	}
}

int main() {

	// Create a Calibrator object to calibrate the
	// HSV treshold values for object tracing
	Calibrator calibrator(Scalar(0, 100, 150), Scalar(70, 230, 240));
	calibrator.Calibrate();

	string gameName = "PONG - Game of the year edition";

	VideoCapture cap(0);

	Mat frame; // for capture
	//cap >> frame;
	cap.read(frame);
	WINDOW_WIDTH = frame.cols;
	WINDOW_HEIGHT = frame.rows;
	Mat img = Mat::zeros(WINDOW_HEIGHT, WINDOW_WIDTH, CV_64FC3);

	namedWindow(gameName, CV_WINDOW_AUTOSIZE);

	char charInput = 0;
	int initialSpeed = 8;
	int startPos = (WINDOW_HEIGHT + TOPROW_HEIGHT - PLAYER_HEIGHT) / 2;

	Player p1(Point(10, startPos), PLAYER_WIDTH, PLAYER_HEIGHT, Scalar(0, 255, 255), "Human", initialSpeed, -1, WINDOW_WIDTH, WINDOW_HEIGHT);
	Player p2(Point(WINDOW_WIDTH - 10 - PLAYER_WIDTH, startPos), PLAYER_WIDTH, PLAYER_HEIGHT, Scalar(255, 0, 255), "CPU", initialSpeed, 1, WINDOW_WIDTH, WINDOW_HEIGHT);

	Ball ball(15, Scalar(0, 255, 0), 2, WINDOW_WIDTH, WINDOW_HEIGHT);

	imshow(gameName, img);
	moveWindow(gameName, 100, 50);
	setMouseCallback(gameName, mouse_callback);

	int ballRadius = ball.getRadius();
	Tracker tracker(calibrator.getLowHSV(), calibrator.getHighHSV());
	
	// if reset is set to true then the game will pause for a second
	// i.e. after score
	bool reset = false;
	int playerPos = -1;

	// playing the game until the Esc key is pressed
	while (charInput != 27) {
		
		// moving the ball
		ball.move();

		// moving the player
		//cap.read(frame);
		if (cap.read(frame) && !frame.empty()) {
			tracker.setFrame(frame);
			tracker.binFrameProcessing();
			playerPos = tracker.trackObject().y;

			if (playerPos != -1) {
				if (playerPos < p1.getPosition().y + p1.getHeight() / 2 - 5) {
					p1.moveUp();
				}
				else if (playerPos > p1.getPosition().y + p1.getHeight() / 2 + 5) {
					p1.moveDown();
				}
			}
		}

		// controlling the player with the mouse
		// FOR TESTING ONLY

		//if (mouseY < p1.getPosition().y + p1.getHeight() / 2 - 5) {
		//	p1.moveUp();
		//} else if (mouseY > p1.getPosition().y + p1.getHeight() / 2 + 5) {
		//	p1.moveDown();
		//}

		// moving CPU player
		p2.moveCPU(ball.getPosition().y);

		// drawing the field, rendering the players and the ball
		img = Mat::zeros(WINDOW_HEIGHT, WINDOW_WIDTH, CV_64FC3);
		drawField(img, p1, p2);

		p1.render(img);
		p2.render(img);
		ball.render(img);

		//// show the position of the tracker ball, if tracked
		if (playerPos != -1) {
			showTracker(img, playerPos, p1);
		}

		if (reset) {
			Sleep(1000);
			reset = false;
		}

		// check if there is a goal or not
		if (ball.getPosition().x < ballRadius || ball.getPosition().x > WINDOW_WIDTH - ballRadius) {
			if (ball.getPosition().x < ballRadius) {
				p2.score();
			}
			else {
				p1.score();
			}
			ball.reset();
			p1.reset();
			p2.reset();
			reset = true;
		}
		else { // check collision
			ball.checkCollision(p1);
			ball.checkCollision(p2);
		}

		imshow(gameName, img);

		// reading a key
		charInput = waitKey(WAIT_TIMEFRAME);

	} // while

	return 0;

} // main