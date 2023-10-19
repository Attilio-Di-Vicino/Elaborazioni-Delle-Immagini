#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
	Mat src = imread( argv[1] );
	if(src.empty()) return -1;
	Mat dst;
	imshow("src", src);
	waitKey(0);
	return 0;
}
