#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

const int th1 = 20;
const int th2 = 50;
const int ksize = 3;

void myCanny(const Mat src, Mat& dst) {
	Mat gauss, dx, dy, magnitude, phase;
	GaussianBlur(src, gauss, Size(5,5), 0, 0);
	Sobel(gauss, dx, CV_32FC1, 1, 0, ksize);
	Sobel(gauss, dy, CV_32FC1, 0, 1, ksize);
	cv::magnitude(dx, dy, magnitude);
	normalize(magnitude, magnitude, 0, 255, NORM_MINMAX, CV_8UC1);
	cv::phase(dx, dy, phase, true);
	uchar q, r;
	for (int i=1; i<magnitude.rows-1; i++)
		for (int j=1; j<magnitude.cols-1; j++) {
			float angle = phase.at<float>(i,j) > 180 ? phase.at<float>(i,j) - 360: phase.at<float>(i,j);
			uchar mag = magnitude.at<uchar>(i,j);
			if ((angle < -157.5 || angle > 157.5) || (angle >= -22.5 && angle <= 22.5)) {
				q = magnitude.at<uchar>(i,j-1);
				r = magnitude.at<uchar>(i,j+1);
			} else if ((angle <= 157.5 && angle > 112.5) || (angle >= -67.5 && angle < -22.5)) {
				q = magnitude.at<uchar>(i+1,j-1);
				r = magnitude.at<uchar>(i-1,j+1);
			} else if ((angle <= 112.5 && angle > 67.5) || (angle >= -112.5 && angle < -67.5)) {
				q = magnitude.at<uchar>(i-1,j);
				r = magnitude.at<uchar>(i+1,j);
			} else if ((angle <= 67.6 && angle > 22.5) || (angle >= -157.5 && angle < -112.5)) {
				q = magnitude.at<uchar>(i-1,j-1);
				r = magnitude.at<uchar>(i+1,j+1);
			} 
			if (mag < r || mag < q)
				magnitude.at<uchar>(i,j) = 0;
		}
	for (int i=1; i<magnitude.rows-1; i++)
		for (int j=1; j<magnitude.cols-1; j++) {
			uchar px = magnitude.at<uchar>(i,j);
			if (px >= th2) px = 255;
			else if (px < th1) px = 0;
			else {
				bool sn = false;
				for (int x=-1; x<=1 && !sn; x++)
					for (int y=-1; y<=1 && !sn; y++)
						if (magnitude.at<uchar>(i+x,j+y) > th2)
							sn = true;
				if (sn) px = 255;
				else px = 0;
			}
			magnitude.at<uchar>(i,j) = px;
		}	
	magnitude.copyTo(dst);
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1], IMREAD_GRAYSCALE );
	if(src.empty()) return -1;
	Mat dst;
	myCanny(src,dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0); 
	return 0;
}
