#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define DEG2RAD CV_PI / 180
const int minRadius = 22;
const int maxRadius = 25;
const int th1 = 100;
const int th2 = 112;
const int hth = 123;

void houghCircles(const Mat src, Mat& dst) {
	src.copyTo(dst);
	Mat gauss, edges, src_gray;
	GaussianBlur(src, gauss, Size(5,5), 0, 0);
	const int sz[] = {gauss.rows, gauss.cols, maxRadius - minRadius};
	Mat votes(3, sz, CV_32F, Scalar(0));
	cvtColor(gauss, src_gray, CV_RGB2GRAY);
	Canny(src_gray, edges, th1, th2);
	imshow("edges", edges);
	for (int y=0; y<edges.rows; y++)
		for (int x=0; x<edges.cols; x++)
			if (edges.at<uchar>(y,x) == 255)
				for (int radius=minRadius; radius<maxRadius; radius++)
					for (int theta=0; theta<360; theta++) {
						int a = x - radius * cos(theta * DEG2RAD);
						int b = y - radius * sin(theta * DEG2RAD);
						if (a>=0 && b>=0 && a<src.cols && b<src.rows)
							votes.at<float>(b,a,radius-minRadius)++;
					}
	for (int radius=minRadius; radius<maxRadius; radius++)
		for (int i=0; i<src_gray.rows; i++)
			for (int j=0; j<src_gray.cols; j++)
				if (votes.at<float>(i,j,radius-minRadius) >= hth) {
					circle(dst, Point(j,i), 1, Scalar(255,0,0), 2, LINE_AA);
					circle(dst, Point(j,i), radius, Scalar(255,0,0), 2, LINE_AA);
				}
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1] );
	if(src.empty()) return -1;
	Mat dst;
	houghCircles(src,dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
