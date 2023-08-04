#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#define EDGE 255
#define BLUE Scalar( 255 )
#define DEG2RAD CV_PI / 180
#define ALPHA 1000

using namespace std;
using namespace cv;

void polarToCartesian( int rho, float theta, Point& begin, Point& end ) {
    int x0 = cvRound( rho * cos( theta ));
	int y0 = cvRound( rho * sin( theta ));
	begin.x = cvRound( x0 - ALPHA * ( -sin( theta )));
    begin.y = cvRound( y0 - ALPHA * ( cos( theta )));
    end.x = cvRound( x0 + ALPHA * ( -sin( theta )));
    end.y = cvRound( y0 + ALPHA * ( cos( theta )));
}

void houghLines( Mat& src, Mat& dst, int cannyLTH, int cannyHTH, int HoughTH ) {

    src.copyTo( dst );
    Mat gsrc, edges;
    GaussianBlur( src, gsrc, Size( 5, 5), 0, 0 );
    cvtColor( src, src, CV_BGR2GRAY );

    // 1
    int maxDist = hypot( src.rows, src.cols );
    Mat votes = Mat::zeros( 2 * maxDist, 180 + 1, CV_32F );

    // 2
    Canny( gsrc, edges, cannyLTH, cannyHTH );
    imshow( "edges", edges );

    // 3, 4, 5
    int rho;
    int thetaIndex;
    for ( int x = 0; x < edges.rows; x++ )
        for ( int y  =0; y < edges.cols; y++ )
            if ( edges.at<uchar>( x, y ) == EDGE )
                for ( thetaIndex = 0; thetaIndex <= 180; thetaIndex++ ) {
                    float theta = ( thetaIndex - 90 ) * DEG2RAD;
                    rho = cvRound( y * cos( theta ) + x * sin( theta ) ) + maxDist;
                    votes.at<float>( rho, thetaIndex )++;
                }

    // 6
    Point begin, end;
    for ( int i = 0; i < votes.rows; i++ )
        for ( int j = 0; j <= 180; j++ )
            if ( votes.at<float>( i, j ) > HoughTH ) {
                rho = i - maxDist;
                float theta = ( j - 90 ) * DEG2RAD;
                polarToCartesian( rho, theta, begin, end );
                line( dst, begin, end, BLUE, 2, LINE_AA );
            }
}

int main( int argc, char** argv ) {

    if ( argc != 2 ) {
        std::cout  << "usage: " << argv[0] << " <image>" << std::endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    if ( src.empty() ) return -1;

    Mat dst;
    int cannyLTH = 120;
    int cannyHTH = 140;
    int HoughTH = 180;
    houghLines( src, dst, cannyLTH, cannyHTH, HoughTH );

    imshow( "src", src );
    imshow( "dst", dst );
    waitKey(0);
    return 0;
}