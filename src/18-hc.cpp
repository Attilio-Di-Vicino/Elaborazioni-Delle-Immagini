#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void houghCircles(Mat& src, Mat& dst, int cannyLTH, int cannyHTH, int HoughTH, int Rmin, int Rmax){

    //1
    vector<vector<vector<int>>>
        votes(src.rows, vector<vector<int>>(src.cols, vector<int>(Rmax-Rmin+1,0)));

    //2
    Mat gsrc, edges;
    GaussianBlur(src,gsrc,Size(7,7),0,0);
    Canny(gsrc,edges,cannyLTH,cannyHTH);

    //3
    for(int x=0; x<edges.rows; x++)
        for(int y=0; y<edges.cols; y++)
            if(edges.at<uchar>(x,y) == 255)
                for(int r=Rmin; r<=Rmax; r++)
                    for(int theta=0; theta<360; theta++){
                        int a = y - r*cos(theta*CV_PI/180);
                        int b = x - r*sin(theta*CV_PI/180);
                        if(a>=0 && a<edges.cols && b>=0 && b<edges.rows)
                            votes[b][a][r-Rmin]++;
                    }

    //4
    dst=src.clone();
    for(int r=Rmin; r<Rmax; r++)
        for(int b=0; b<edges.rows; b++)
            for(int a=0; a<edges.cols; a++)
                if(votes[b][a][r-Rmin] > HoughTH){
                    circle(dst, Point(a,b), 3, Scalar(0), 2, 8, 0);
                    circle(dst, Point(a,b), r, Scalar(0), 2, 8, 0);
                }
}

int main(int argc, char** argv){

    Mat src = imread(argv[1],IMREAD_GRAYSCALE);
    if(src.empty()) return -1;

    Mat dst;
    int cannyLTH = 150;
    int cannyHTH = 230;
    int HoughTH = 130;
    int Rmin = 40, Rmax = 130;
    houghCircles(src,dst,cannyLTH,cannyHTH,HoughTH,Rmin,Rmax);

    imshow("src",src);
    imshow("dst",dst);
    waitKey(0);
    return 0;
}