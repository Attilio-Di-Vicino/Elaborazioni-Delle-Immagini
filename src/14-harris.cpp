/**
 * STEP ALGORITMO DI HARRIS
 * 
 * 1. Calcolare le derivate parziali rispetto ad x e y (Dx, Dy)
 * 2. Calcolare Dx^2, Dy^2 e Dx*Dy
 * 3. Applicare un filtro Gaussiano a Dx2, Dy2 e Dx*Dy
 * 4. Si ottengono C00 , C11 , C01 (e quindi anche C10 )
 * 5. Calcolare l'indice R
 * 6. Normalizzare l'indice R in [0, 255]
 * 7. Sogliare R
*/

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src, src_gray, src_gray_myH;
int thresh = 200;
int max_thresh = 255;

const char* source_window = "Source image";
const char* corners_window = "Corners detected";

const char* my_source_window = "MySource image";
const char* my_corners_window = "MyCorners detected";

void cornerHarris_demo( int, void* );

void myCornerHarris( const Mat& src, Mat& dst, int blockSize, int ksize, double k, int borderType = 4 ) {

    Mat gaussianBlur;
    GaussianBlur( src, gaussianBlur, Size( 5, 5 ), 0, 0 );

    // 1. Calcolare le derivate parziali rispetto ad x e y (Dx, Dy)
    Mat dx, dy;
    Sobel( gaussianBlur, dx, CV_32FC1, 1, 0, ksize );
    Sobel( gaussianBlur, dy, CV_32FC1, 0, 1, ksize );

    // 2. Calcolare Dx^2, Dy^2 e Dx*Dy
    Mat dx2, dy2, dxy;
    // dx2 = dx * dx;
    // dy2 = dy * dy;
    // dxy = dx * dy;
    multiply( dx, dy, dxy );
    pow( dx, 2, dx2 );
	pow( dy, 2, dy2 );

    // 3. Applicare un filtro Gaussiano a Dx2, Dy2 e Dx*Dy
    Mat blurredDx2, blurredDy2, blurredDxy;
    GaussianBlur( dx2, blurredDx2, Size( 5, 5 ), 0, 0 );
    GaussianBlur( dy2, blurredDy2, Size( 5, 5 ), 0, 0 );
    GaussianBlur( dxy, blurredDxy, Size( 5, 5 ), 0, 0 );

    // 4. Si ottengono C00 , C11 , C01 (e quindi anche C10 )
    // dx2 -> C00
    // dy2 -> C11
    // dxy -> C10

    // 5. Calcolare l'indice R
    Mat result1, result2, determinant, track, R;
    multiply( blurredDx2, blurredDy2, result1 );
	multiply( blurredDxy, blurredDxy, result2 );

	determinant = result1 - result2;
	track = blurredDx2 + blurredDy2;

	pow( track, 2, track );
	track *= k;

	R = determinant - track;

    // 6. Normalizzare l'indice R in [0, 255]
    normalize( R, dst, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );

    // 7. Sogliare R
    // threshold( dst, dst, thresh, 255, THRESH_BINARY );
}

int main( int argc, char** argv )
{
    src = imread( argv[1] );
    if ( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    // Dimensioni desiderate per l'immagine ridotta
    int newWidth = 450;  // Larghezza desiderata in pixel
    int newHeight = 350; // Altezza desiderata in pixel

    // Ridimensiona l'immagine utilizzando l'interpolazione bilineare
    Mat resizedImage;
    resize( src, src, Size(newWidth, newHeight), 0, 0, INTER_LINEAR );

    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    cvtColor( src, src_gray_myH, COLOR_BGR2GRAY );

    namedWindow( source_window );
    // namedWindow( my_source_window );

    createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
    imshow( source_window, src );
    cornerHarris_demo( 0, 0 );

    waitKey(0);
    return 0;
}
void cornerHarris_demo( int, void* )
{
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    Mat dst = Mat::zeros( src.size(), CV_32FC1 );
    cornerHarris( src_gray, dst, blockSize, apertureSize, k );
    myCornerHarris( src_gray_myH, dst, blockSize, apertureSize, k );
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
    namedWindow( corners_window );
    imshow( corners_window, dst_norm_scaled );

    namedWindow( my_corners_window );
    imshow( my_corners_window, dst_norm_scaled );
}