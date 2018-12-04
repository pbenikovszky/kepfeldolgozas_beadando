#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "calibrator.h"

using namespace cv;

class Tracker {

public:

	Tracker(Scalar lowHSV, Scalar highHSV);
	~Tracker();
	Mat getHsvFrame();
	Mat getBinFrame();
	void setFrame(Mat _cameraFrame);
	void binFrameProcessing();
	Point trackObject();


private:

	Mat _bgrFrame; // this is the camera frame
	Mat _hsvFrame; // bgrFrame converted to HSV
	Mat _binFrame; // inRange function applied to hsvFrame
	Scalar _lowHSV; // low HSV treshold values for inRange
	Scalar _highHSV; // high HSV treshold values for inRange

};


#endif // TRACKER_H