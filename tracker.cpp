#include "tracker.h"
using namespace std;

Tracker::Tracker(Scalar lowHSV, Scalar highHSV) {
	_lowHSV = lowHSV;
	_highHSV = highHSV;
}

Tracker::~Tracker() {

}


Mat Tracker::getBinFrame() {
	return _binFrame;
}

Mat Tracker::getHsvFrame() {
	return _hsvFrame;
}

void Tracker::setFrame(Mat _cameraFrame) {

	_bgrFrame = _cameraFrame;

	// Turning the camera BGR frame into a HSV image
	cvtColor(_bgrFrame, _hsvFrame, COLOR_BGR2HSV);

	// Binary image with the thresholded values is defined
	inRange(_hsvFrame, _lowHSV, _highHSV, _binFrame);

}

void Tracker::binFrameProcessing() {

	// process the frame

	// declaring the structuring element, 3x3 rectangle
	Mat structuringElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(_binFrame, _binFrame, structuringElement);
	erode(_binFrame, _binFrame, structuringElement);

	// not sure if needed, but it never hurts :D
	GaussianBlur(_binFrame, _binFrame, Size(3, 3), 0);

}

Point Tracker::trackObject() {

	// track the control object
	// see calibrator.cpp for comments
	// returns the center of the tracked object as Point
	// returns (-1, -1) if no object was detected

	// center of the object, 
	Point objectCenter = Point(-1, -1);
	int maxRadius;

	vector<Vec3f> v3fCircles;

	HoughCircles(_binFrame,
		v3fCircles,
		CV_HOUGH_GRADIENT,
		2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
		_binFrame.rows / 4,					// min distance in pixels between the centers of the detected circles
		100,								// high threshold of edge detector
		50,									// low threshold of edge detector
		40,									// min circle radius
		400);								// max circle radius)

	if (v3fCircles.size() > 0) {

		objectCenter = Point((int)v3fCircles[0][0], (int)v3fCircles[0][1]);
		maxRadius = (int)v3fCircles[0][2];

		for (int i = 1; i < v3fCircles.size(); i++) {

			if ((int)v3fCircles[i][2] > maxRadius) {
				maxRadius = (int)v3fCircles[i][2];
				objectCenter = Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]);
			}

		} // for

	} // if (v3fCircles.size() > 0)

	return objectCenter;

}