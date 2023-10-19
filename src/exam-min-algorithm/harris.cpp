#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define KERNEL Size(7,7)
const int ksize = 11;
const int th = 100;
const float k = 0.04f;

void harris(const Mat src, Mat& dst) {
	Mat gauss, dx, dy, dx2, dy2, dxy, PPD, PSD, det, trace, R, dstNorm;
	GaussianBlur(src, gauss, KERNEL, 0, 0);
	Sobel(gauss, dx, CV_32FC1, 1, 0, ksize);
	Sobel(gauss, dy, CV_32FC1, 0, 1, ksize);
	multiply(dx, dy, dxy);
	pow(dx, 2, dx2);
	pow(dy, 2, dy2);
	GaussianBlur(dx2, dx2, KERNEL, 0, 0);
	GaussianBlur(dy2, dy2, KERNEL, 0, 0);
	GaussianBlur(dxy, dxy, KERNEL, 0, 0);
	multiply(dx2, dy2, PPD);
	multiply(dxy, dxy, PSD);
	det = PPD - PSD;
	trace = dx2 + dy2;
	pow(trace, 2, trace);
	trace *= k;
	R = det - trace;
	normalize(R, dstNorm, 0, 255, NORM_MINMAX);
	convertScaleAbs(dstNorm, dst);
	for (int i=0; i<dstNorm.rows; i++)
		for (int j=0; j<dstNorm.cols; j++)
			if (dstNorm.at<float>(i,j) >= th)
				circle(dst, Point(j,i), 5, Scalar(0), 2, LINE_AA);
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1], IMREAD_GRAYSCALE );
	if(src.empty()) return -1;
	Mat dst;
	harris(src, dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
