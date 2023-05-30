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
#include <iostream>
#include "Algorithm/MyCanny.h"

using namespace cv;

/// Global variables
Mat src, src_gray;
Mat detected_edges, detected_edgesMyCanny;

int hightThreshold = 105;
int const max_hightThreshold = 255;

int lowThreshold = 25;
int const max_lowThreshold = 255;

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold( int, void* ) {

  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size( 3, 3 ) );
  blur( src_gray, detected_edgesMyCanny, Size( 3, 3 ) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, hightThreshold );
  MyCanny::myCanny( detected_edgesMyCanny, detected_edgesMyCanny, lowThreshold, hightThreshold );

  imshow( "Src", src_gray );
  imshow( "Canny", detected_edges );
  imshow( "MyCanny", detected_edgesMyCanny );
}

/** @function main */
int main( int argc, char** argv ) {

  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  /// Convert the image to grayscale
  // l'algoritmo si basa su immagini in scala di grigi da vedere
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window
  namedWindow( "Src", WINDOW_AUTOSIZE );
  namedWindow( "Canny", WINDOW_AUTOSIZE );
  namedWindow( "MyCanny", WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Max Threshold:", "Src", &hightThreshold, max_hightThreshold, CannyThreshold );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", "Src", &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
}