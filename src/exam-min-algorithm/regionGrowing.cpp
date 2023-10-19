#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stack>

using namespace cv;
using namespace std;

const int maxRegionNumber = 100;
const float minRegionAreaFactor = 0.01f;
const int th = 204;
const Point pointShift2D[8] = {
	Point(-1,-1),
	Point(-1, 0),
	Point(-1, 1),
	Point( 0,-1),
	Point( 0, 1),
	Point( 1,-1),
	Point( 1, 0),
	Point( 1, 1),
};

void grow(const Mat src, const Mat dst, Mat& mask, Point seed) {
	stack<Point> front;
	front.push(seed);
	while (!front.empty()) {
		Point center = front.top();
		mask.at<uchar>(center) = 1;
		front.pop();
		for (int i=0; i<8; i++) {
			Point neigh = center + pointShift2D[i];
			if ( neigh.x < 0 || neigh.x > src.cols || neigh.y < 0 || neigh.y > src.rows )
				continue;
			else {
				int delta = cvRound( pow( src.at<Vec3b>(center)[0] - src.at<Vec3b>(neigh)[0], 2 ) +
								     pow( src.at<Vec3b>(center)[1] - src.at<Vec3b>(neigh)[1], 2 ) +
								     pow( src.at<Vec3b>(center)[2] - src.at<Vec3b>(neigh)[2], 2 ) );
				if (delta < th && !dst.at<uchar>(neigh) && !mask.at<uchar>(neigh))
					front.push(neigh);
			}
		}
	}
}

void regionGrowing(const Mat src, Mat& dst) {
	dst = Mat::zeros(src.rows, src.cols, CV_8UC1);
	Mat mask = Mat::zeros(src.rows, src.cols, CV_8UC1);
	const int minRegionArea = int(src.rows * src.cols * minRegionAreaFactor);
	int label = 0;
	for (int i=0; i<src.rows; i++)
		for (int j=0; j<src.cols; j++)
			if (dst.at<uchar>(i,j) == 0) {
				grow(src, dst, mask, Point(j,i));
				if (sum(mask).val[0] > minRegionArea) {
					imshow("mask", mask * 255);
					waitKey(0);
					dst += mask * label;
					if (++label > maxRegionNumber) {
						cout << "Run out of the region number..." << endl;
						exit(EXIT_FAILURE);
					}
				} else 
					dst += mask * 255;
				mask -= mask;
			}
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1] );
	if(src.empty()) return -1;
	Mat dst;
	regionGrowing(src, dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
