/**
 * STEP ALGORITMO DI CANNY
 * 
 *  1. Convolvere l'immagini di input con un filtro Gaussiano
 *  2. Calcolare la magnitudo e l'orientazione del gradiente
 *  3. Applicare la non maxima suppression
 *  4. Applicare il thresholding con isteresi
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables
Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;

int hightThreshold = 125;
int const max_hightThreshold = 255;

int lowThreshold = 35;
int const max_lowThreshold = 255;

int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

void myCanny( Mat image, Mat edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false ) {

    // Gaussian blur
    Mat gaussianBlurDefault;
    GaussianBlur( image, gaussianBlurDefault, Size( 3, 3 ), 0, 0 );

    Mat dx, dy;
	Sobel( gaussianBlurDefault, dx, CV_32FC1, 1, 0, kernel_size);
	Sobel( gaussianBlurDefault, dy, CV_32FC1, 0, 1, kernel_size);

    Mat mag;
    mag = abs( dx + dy );
    
    Mat ph;
    phase( dx, dy, ph, true );
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold( int, void* ) {

  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  // Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
  Canny( detected_edges, detected_edges, lowThreshold, hightThreshold, kernel_size );
  myCanny( detected_edges, detected_edges, lowThreshold, hightThreshold, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
}

/** @function main */
int main( int argc, char** argv ) {

  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window
  namedWindow( window_name, WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Max Threshold:", window_name, &hightThreshold, max_hightThreshold, CannyThreshold );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
  }