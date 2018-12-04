#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Calibrator {

public:

	Calibrator(Scalar lowHSV, Scalar highHSV);
	~Calibrator();
	Scalar getLowHSV();
	Scalar getHighHSV();
	void setLowHSV(Scalar lowHSV);
	void setHighHSV(Scalar highHSV);
	void Calibrate();

private:

	int _lowH;
	int _lowS;
	int _lowV;
	int _highH;
	int _highS;
	int _highV;
};

#endif // CALIBRATOR_H