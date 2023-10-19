#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

vector<double> createNormalaizedHistogram(const Mat src) {
	vector<double> normalaizedHistogram(256, 0.0f);
	double nxm = src.rows * src.cols;
	for (int i=0; i<src.rows; i++)
		for (int j=0; j<src.cols; j++)
			normalaizedHistogram.at( src.at<uchar>(i,j) )++;
	for (auto& prob: normalaizedHistogram)
		prob /= nxm;
	return normalaizedHistogram;
}

vector<int> otsu2k(const Mat src) {
	vector<int> T(3, 0);
	vector<double> normalaizedHistogram = createNormalaizedHistogram(src);
	vector<double> cumulativeSums(3, 0.0f);
	vector<double> cumulativeAverages(3, 0.0f);
	double mg = 0.0f, interclassVariance = 0.0f, iv = 0.0f;
	cumulativeSums.at(0) = normalaizedHistogram.at(0);
	cumulativeAverages.at(0) = normalaizedHistogram.at(0);
	mg = normalaizedHistogram.at(0);
	for (int i=1; i<256; i++)
		mg += i * normalaizedHistogram.at(i);
	for (int i=1; i<256-2; i++) {
		cumulativeSums.at(0) += normalaizedHistogram.at(i);
		cumulativeAverages.at(0) += i * normalaizedHistogram.at(i);
		for (int j=i+1; j<256; j++) {
			cumulativeSums.at(1) += normalaizedHistogram.at(j);
			cumulativeAverages.at(1) += j * normalaizedHistogram.at(j);
			for (int k=j+1; k<256; k++) {
				cumulativeSums.at(2) += normalaizedHistogram.at(k);
				cumulativeAverages.at(2) += k * normalaizedHistogram.at(k);
				for (int z=0; z<3; z++)
					iv += cumulativeSums.at(z) * pow( cumulativeAverages.at(z) / cumulativeSums.at(z) - mg, 2 );
				if (iv > interclassVariance) {
					interclassVariance = iv;
					T.at(0) = i;
					T.at(1) = j;
				}
				iv = 0.0f;
			}
			cumulativeSums.at(2) = cumulativeAverages.at(2) = 0.0f;
		}
		cumulativeSums.at(1) = cumulativeAverages.at(1) = 0.0f;
	}
	return T;
}

void myThreshold(const Mat src, Mat& dst, int th1, int th2) {
	src.copyTo(dst);
	for (int i=0; i<src.rows; i++)
		for (int j=0; j<src.cols; j++) 
			if (src.at<uchar>(i,j) >= th2)
				dst.at<uchar>(i,j) = 255;
			else if (src.at<uchar>(i,j) >= th1)
				dst.at<uchar>(i,j) = 127;
			else 
				dst.at<uchar>(i,j) = 0;
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1], IMREAD_GRAYSCALE );
	if(src.empty()) return -1;
	Mat dst;
	vector<int> T = otsu2k(src);
	myThreshold(src, dst, T.at(0), T.at(1));
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
