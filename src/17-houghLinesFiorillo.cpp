//https://github.com/eToTheEcs/hough-transform/blob/master/hough.cpp

#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

void polarToCartesian(double rho, int theta, Point& p1, Point& p2){
    int alpha = 1000;

    int x0 = cvRound(rho*cos(theta));
	int y0 = cvRound(rho*sin(theta));

	p1.x = cvRound(x0 + alpha*(-sin(theta)));
	p1.y = cvRound(y0 + alpha*(cos(theta)));

	p2.x = cvRound(x0 - alpha*(-sin(theta)));
	p2.y = cvRound(y0 - alpha*(cos(theta)));
}

void houghLines(Mat& src, Mat& dst, int cannyLTH, int cannyHTH, int HoughTH){

    //1
    int maxDist = hypot(src.rows, src.cols);
    vector<vector<int>> votes(maxDist*2, vector<int>(180, 0));

    //2
    Mat gsrc, edges;
    GaussianBlur(src,gsrc,Size(3,3),0,0);
    Canny(gsrc,edges,cannyLTH,cannyHTH);

    //3
    double rho;
    int theta;
    for(int x=0; x<edges.rows; x++)
        for(int y=0; y<edges.cols; y++)
            if(edges.at<uchar>(x,y) == 255)
                for(theta = 0; theta <= 180; theta++){
                    rho = round(y*cos(theta-90) + x*sin(theta-90)) + maxDist;
                    votes[rho][theta]++;
                }

    //4 - drawing
    dst=src.clone();
    Point p1, p2;
    for(size_t i=0; i<votes.size(); i++)
        for(size_t j=0; j<votes[i].size(); j++)
            if(votes[i][j] >= HoughTH){
                rho = i-maxDist;
                theta = j-90;
                polarToCartesian(rho,theta,p1,p2);
                line(dst,p1,p2,Scalar(255),2,LINE_AA);
            }
}

int main(int argc, char** argv){

    Mat src = imread(argv[1],IMREAD_GRAYSCALE);
    if(src.empty()) return -1;

    Mat dst;
    int cannyLTH = 120;
    int cannyHTH = 140;
    int HoughTH = 180;
    houghLines(src,dst,cannyLTH,cannyHTH,HoughTH);


    imshow("src",src);
    imshow("dst",dst);
    waitKey(0);
    return 0;
}