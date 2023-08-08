#ifndef MYTHRESHOLD_H
#define MYTHRESHOLD_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

enum class MyType {
    THRESH_BINARY, THRESH_BINARY_INV, THRESH_TRUNC,
    THRESH_TOZERO, THRESH_TOZERO_INV, THRESH_OTSU, THRESH_OTSU2K 
};

class MyThreshold {
    private:
        
    public:
        static void threshold( Mat src, Mat& dst, double thresh, double maxval, int type );
        static void threshold( Mat src, Mat& dst, double thresh1, double thresh2, double maxval, int type );
        static void threshold( Mat src, Mat& dst, double thresh1, double thresh2, 
                               double minval, double midval, double maxval, int type );
};

void MyThreshold::threshold( Mat src, Mat& dst, double thresh, double maxval, int type ) {

}

void MyThreshold::threshold( Mat src, Mat& dst, double thresh1, double thresh2, double maxval, int type ) {
    threshold( src, dst, thresh1, thresh2, 0.0, 127.0, maxval, ( int ) MyType::THRESH_OTSU2K );
}

void MyThreshold::threshold( Mat src, Mat& dst, double thresh1, double thresh2, 
                               double minval, double midval, double maxval, int type ) {
    src.copyTo( dst );

    for ( auto i = 0; i < dst.rows; i++ )
        for ( auto j = 0; j < dst.cols; j++ ) {
            if ( dst.at<uchar>( i, j ) <= thresh1 )
                dst.at<uchar>( i, j ) = minval;
            else if ( dst.at<uchar>( i, j ) < thresh2 )
                dst.at<uchar>( i, j ) = midval;
            else 
                dst.at<uchar>( i, j ) = maxval;
        }
}

#endif // MYTHRESHOLD_H