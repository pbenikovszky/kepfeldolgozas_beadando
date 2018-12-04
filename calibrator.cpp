#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "calibrator.h"

using namespace cv;
using namespace std;

const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
const int max_value_H = 360 / 2;
const int max_value = 255;
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

Calibrator::Calibrator(Scalar lowHSV, Scalar highHSV) {
	_lowH = lowHSV.val[0];
	_lowS = lowHSV.val[1];
	_lowV = lowHSV.val[2];
	_highH = highHSV.val[0];
	_highS = highHSV.val[1];
	_highV = highHSV.val[2];

	// setting up the values for the trackerbar
	low_H = _lowH;
	low_S = _lowS;
	low_V = _lowV;

	high_H = _highH;
	high_S = _highS;
	high_V = _highV;
}

Calibrator::~Calibrator() {

}

Scalar Calibrator::getLowHSV() {
	return Scalar(_lowH, _lowS, _lowV);
}

Scalar Calibrator::getHighHSV() {
	return Scalar(_highH, _highS, _highV);
}

void Calibrator::setLowHSV(Scalar lowHSV) {
	_lowH = lowHSV.val[0];
	_lowS = lowHSV.val[1];
	_lowV = lowHSV.val[2];
}

void Calibrator::setHighHSV(Scalar highHSV) {
	_highH = highHSV.val[0];
	_highS = highHSV.val[1];
	_highV = highHSV.val[2];
}

#pragma region trackbar_handlers

static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
}

#pragma endregion

void Calibrator::Calibrate() {

	// declare the VideoCapture object to capture webcam pic
	// 0 means that app is using 1st webcam to capture (if there is more then 1 webcam)
	VideoCapture capWebcam(0);		

	// check if VideoCapture object was associated to webcam successfully
	if (capWebcam.isOpened() == false) {				
		cout << "error: capWebcam not accessed successfully\n\n";	// if not, print error message to std out
		return;														// and exit program
	}

	Mat imgOriginal; // input image
	Mat imgHSV; // input image converted to HSV
	Mat imgThresh; // image after using inRange with the low and high HSV treshold values

	Point objectCenter;
	int maxRadius;

	// this is for HoughCircler()
	vector<Vec3f> v3fCircles;	

	char charCheckForEscKey = 0;

	// declare windows
	namedWindow(window_detection_name);
	namedWindow(window_capture_name);

	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_capture_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_capture_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_capture_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_capture_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_capture_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_capture_name, &high_V, max_value, on_high_V_thresh_trackbar);

	// running the calibration until the Esc key is pressed or webcam connection is lost
	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {
		
		
		// get the next frame and check if frame read successfully or not
		if (!capWebcam.read(imgOriginal) || imgOriginal.empty()) {
			cout << "error: frame not read from webcam\n";		
			break;												
		}

		// convert the camera frame to HSV
		cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);

		// apply inRAnge with the treshold values
		inRange(imgHSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), imgThresh);

		// same processing as in the tracker class
		GaussianBlur(imgThresh, imgThresh, Size(3, 3), 0);

		Mat structuringElement = getStructuringElement(MORPH_RECT, Size(3, 3));

		dilate(imgThresh, imgThresh, structuringElement);
		erode(imgThresh, imgThresh, structuringElement);

		// fill circles vector with all circles in processed image
		HoughCircles(imgThresh,					
			v3fCircles,							
			CV_HOUGH_GRADIENT,					
			2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
			imgThresh.rows / 4,					// min distance in pixels between the centers of the detected circles
			100,								// high threshold of edge detector
			50,									// low threshold of edge detector
			40,									// min circle radius
			400);								// max circle radius

		// Check if any circle was detected or not
		// v3fCircles is an array of the found circles, each circle is an array itself
		// v3fCircles[][0] - x coord, v3fCircles[][1] - y coord, v3fCircles[][2] - radius
		if (v3fCircles.size() > 0) {
			objectCenter = Point((int)v3fCircles[0][0], (int)v3fCircles[0][1]);
			maxRadius = (int)v3fCircles[0][2];

			for (int i = 1; i < v3fCircles.size(); i++) {		
				// for each circle check the radius
				// and find the max radius
				if ((int)v3fCircles[i][2] > maxRadius) {
					maxRadius = (int)v3fCircles[i][2];
					objectCenter = Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]);
				}


			} // for

			// draw small green circle at center of detected object
			circle(imgOriginal, objectCenter, 3, Scalar(0, 255, 0), CV_FILLED);

			// draw red circle around the detected object
			circle(imgOriginal, objectCenter, maxRadius, Scalar(0, 0, 255), 3);

		} // if (v3fCircles.size() > 0)

		// show windows
		imshow(window_detection_name, imgOriginal);			
		imshow(window_capture_name, imgThresh);

		charCheckForEscKey = waitKey(1);			
	}	// end while

	// closing the calibration windows
	destroyWindow(window_detection_name);
	destroyWindow(window_capture_name);

	// setting the new treshold values for the calibrator object
	// using the trackbar values
	setLowHSV(Scalar(low_H, low_S, low_V));
	setHighHSV(Scalar(high_H, high_S, high_V));

}