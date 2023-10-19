#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define DEG2RAD CV_PI / 180
#define ALPHA 1000
const int th1 = 120;
const int th2 = 160;
const int hth = 140;

void polarToCartesian(const int rho, const float theta, Point& begin, Point& end) {
	const int x0 = cvRound( rho * cos(theta) );
	const int y0 = cvRound( rho * sin(theta) );
	begin.x = cvRound( x0 - ALPHA * (-sin(theta)) );
	begin.y = cvRound( y0 - ALPHA * ( cos(theta)) );
	end.x   = cvRound( x0 + ALPHA * (-sin(theta)) );
	end.y   = cvRound( y0 + ALPHA * ( cos(theta)) );
}

void houghLines(const Mat src, Mat& dst) {
	src.copyTo(dst);
	Mat gauss, edges, src_gray;
	GaussianBlur(src, gauss, Size(5,5), 0, 0);
	const int maxDist = hypot( src.rows, src.cols );
	Mat votes = Mat::zeros( 2*maxDist, 180+1, CV_32FC1 );
	cvtColor(gauss, src_gray, CV_RGB2GRAY);
	Canny(src_gray, edges, th1, th2);
	imshow("edges", edges);
	int rho, thetaIndex;
	for (int y=0; y<edges.rows; y++)
		for (int x=0; x<edges.cols; x++) 
			if (edges.at<uchar>(y,x) == 255) 
				for (thetaIndex=0; thetaIndex<=180; thetaIndex++) {
					float theta = (thetaIndex - 90) * DEG2RAD;
					rho = ( x*cos(theta) + y*sin(theta) ) + maxDist;
					votes.at<float>(rho, thetaIndex)++;
				}
	Point begin, end;
	for (int i=0; i<votes.rows; i++)
		for (int j=0; j<votes.cols; j++)
			if (votes.at<float>(i,j) >= th) {
				float theta = (j - 90) * DEG2RAD;
				rho = i - maxDist;
				polarToCartesian( rho, theta, begin, end );
				line( dst, begin, end, Scalar(255,0,0), 2, LINE_AA );
			}
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1] );
	if(src.empty()) return -1;
	Mat dst;
	houghLines(src, dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
