#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

const int k = 6;
const double th = 0.05f;

void computeRandomCenter(const Mat src, vector<Scalar>& center, vector<vector<Point>>& cluster) {
	RNG randomNumberGenerator( getTickCount() );
	for (int label=0; label<k; label++) {
		Point px;
		px.x = randomNumberGenerator.uniform(0, src.cols);
		px.y = randomNumberGenerator.uniform(0, src.rows);
		center.push_back( src.at<Vec3b>(px) );
		cluster.push_back( vector<Point>() );
	}
}

double computeDistance(Scalar px, Scalar center) {
	double blue = abs( px[0] - center[0] );
	double green = abs( px[1] - center[1] );
	double red = abs( px[2] - center[2] );
	return (double) blue + green + red;
}

void populateCluster(const Mat src, vector<Scalar> center, vector<vector<Point>>& cluster) {
	for (int i=0; i<src.rows; i++)
		for (int j=0; j<src.cols; j++) {
			int labelID = 0;
			double dist = INFINITY;
			for (int label=0; label<k; label++) {
				double pxDist = computeDistance( src.at<Vec3b>(i,j), center.at(label) );
				if (pxDist < dist) {
					dist = pxDist;
					labelID = label;
				}
			}
			cluster.at(labelID).push_back(Point(j,i));
		}
}

double adjustCenter(const Mat src, vector<Scalar>& center, vector<vector<Point>> cluster, double& oldValue, double newValue) {
	for (int label=0; label<k; label++) {
		double blue = 0.0f, green = 0.0f, red = 0.0f;
		for (auto point: cluster.at(label)) {
			blue += src.at<Vec3b>( point )[0];
			green += src.at<Vec3b>( point )[1];
			red += src.at<Vec3b>( point )[2];
		}
		blue /= cluster.at(label).size(); green /= cluster.at(label).size(); red /= cluster.at(label).size();
		Scalar newCenter( cvRound(blue), cvRound(green), cvRound(red) );
		newValue += computeDistance( newCenter, center.at(label) );
		center.at(label) = newCenter;
	}
	newValue /= k;
	double change = abs( oldValue - newValue );
	oldValue = newValue;
	return change;
}

void segment(Mat& dst, vector<Scalar> center, vector<vector<Point>> cluster) {
	for (int label=0; label<k; label++)
		for (auto point: cluster.at(label))
			for (int i=0; i<3; i++)
				dst.at<Vec3b>(point)[i] = center.at(label)[i]; 
}

void myKmeans(const Mat src, Mat& dst) {
	src.copyTo(dst);
	vector<Scalar> center;
	vector<vector<Point>> cluster;
	computeRandomCenter(src, center, cluster);
	double oldValue = INFINITY;
	double newValue = 0.0f;
	double dist = abs( oldValue - newValue );
	while (dist > th) {
		newValue = 0.0f;
		for (int label=0; label<k; label++)
			cluster.at(label).clear();
		populateCluster(src, center, cluster);
		dist = adjustCenter( src, center, cluster, oldValue, newValue );
	}
	segment(dst, center, cluster);
}

int main( int argc, char** argv ) {
	Mat src = imread( argv[1] );
	if(src.empty()) return -1;
	Mat dst;
	myKmeans(src, dst);
	imshow("src", src);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}
