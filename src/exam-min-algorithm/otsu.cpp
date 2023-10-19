#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

vector<double> createNormalizedHistogram(const Mat src) {
	double nxm = src.rows * src.cols;
	vector<double> intensity(256, 0.0f);
	for (int i=0; i<src.rows; i++)
		for (int j=0; j<src.cols; j++)
			intensity.at(src.at<uchar>(i,j))++;
	for (auto& prob: intensity)
		prob /= nxm;
	return intensity;
}

uchar otsu(const Mat src) {
	Mat gauss;
	GaussianBlur(src, gauss, Size(5,5), 0, 0);
	vector<double> normalizedHistogram = createNormalizedHistogram(gauss);
	vector<double> cumulativeSums(256, 0.0f);
	vector<double> cumulativeAvarages(256, 0.0f);
	double mg = 0.0f;
	double interclassVariance = 0.0f;
	uchar T = 0;
	cumulativeSums.at(0) = normalizedHistogram.at(0);
	cumulativeAvarages.at(0) = normalizedHistogram.at(0);
	mg = normalizedHistogram.at(0);
	for (int i=1; i<256; i++) {
		cumulativeSums.at(i) = cumulativeSums.at(i-1) + normalizedHistogram.at(i);
		cumulativeAvarages.at(i) = cumulativeAvarages.at(i-1) + i * normalizedHistogram.at(i);
		mg += i * normalizedHistogram.at(i);
	}
	for (int i=1; i<256; i++) {
		float numerator = pow(mg * cumulativeSums.at(i) - cumulativeAvarages.at(i), 2);
		float denominator = cumulativeSums.at(i) * (1 - cumulativeSums.at(i));
		if (denominator != 0) {
			double iv = numerator / denominator;
			if (iv > interclassVariance) {
				interclassVariance = iv;
				T = i;
			}
		}
	}
	return T;
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1], IMREAD_GRAYSCALE );
	if(src.empty()) return -1;
	Mat dst;
	threshold(src, dst, otsu(src), 255, THRESH_BINARY);  
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
