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

using namespace cv;

/// Global variables
Mat src, src_gray;
Mat dst, detected_edges, detected_edgesMyCanny;

int hightThreshold = 190;
int const max_hightThreshold = 255;

int lowThreshold = 40;
int const max_lowThreshold = 255;

void nonMaxSuppression(const Mat& magnitude, const Mat& phase, Mat& edges) {
    edges = Mat::zeros(magnitude.size(), CV_8UC1);

    for (int i = 1; i < magnitude.rows - 1; i++) {
        for (int j = 1; j < magnitude.cols - 1; j++) {
            float angle = phase.at<float>(i, j);
            float mag = magnitude.at<float>(i, j);

            float q, r;

            // Calcolo delle coordinate dei pixel vicini
            if (angle < 0)
                angle += 180;

            if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180)) {
                q = magnitude.at<float>(i, j + 1);
                r = magnitude.at<float>(i, j - 1);
            }
            else if (angle >= 22.5 && angle < 67.5) {
                q = magnitude.at<float>(i - 1, j + 1);
                r = magnitude.at<float>(i + 1, j - 1);
            }
            else if (angle >= 67.5 && angle < 112.5) {
                q = magnitude.at<float>(i - 1, j);
                r = magnitude.at<float>(i + 1, j);
            }
            else {
                q = magnitude.at<float>(i - 1, j - 1);
                r = magnitude.at<float>(i + 1, j + 1);
            }

            // Suppressione dei non massimi
            if (mag >= q && mag >= r) {
                edges.at<uchar>(i, j) = static_cast<uchar>(mag);
            }
        }
    }
}


void myCanny2(Mat image, Mat& edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false) {

  // 1. Convolvere l'immagine di input con un filtro Gaussiano
  Mat paddedImage;
  Mat gaussianBlur;
  copyMakeBorder(image, paddedImage, 1, 1, 1, 1, BORDER_REFLECT);
  GaussianBlur(paddedImage, gaussianBlur, Size(3, 3), 0, 0);

  // 2. Calcolare la magnitudo e l'orientazione del gradiente
  Mat dx, dy;
  Sobel(gaussianBlur, dx, CV_32FC1, 1, 0, apertureSize);
  Sobel(gaussianBlur, dy, CV_32FC1, 0, 1, apertureSize);

  /**
   * La magnitudo rappresenta la forza del gradiente in ciascun punto dell'immagine.
   * I valori della magnitudo saranno più alti nelle regioni dell'immagine con cambiamenti
   * di intensità più significativi, ad esempio i bordi dell'oggetto. I valori bassi
   * corrisponderanno alle regioni dell'immagine con cambiamenti
   * di intensità meno marcati o a zone piatte.
  */
  Mat mag;
  magnitude(dx, dy, mag);

  /**
   * La fase fornisce informazioni sull'orientamento del gradiente in ciascun punto dell'immagine.
   * Viene solitamente misurata in gradi o radianti.
   * Ad esempio, una fase di 0 gradi potrebbe corrispondere a un gradiente orizzontale,
   * mentre una fase di 90 gradi potrebbe corrispondere a un gradiente verticale.
   * 
   * Nel contesto dell'algoritmo di Canny, la fase è utilizzata durante la
   * non-maximum suppression per determinare la direzione del gradiente in ciascun punto
   * e sopprimere i pixel non massimi lungo la direzione del gradiente.
   * La fase viene anche utilizzata durante il calcolo dei bordi deboli nel thresholding con isteresi,
   * per decidere se un pixel debolmente connesso a un pixel forte è accettabile come bordo.
  */
  Mat ang;
  phase(dx, dy, ang, true);

  // Ci assicuriamo che i valori siano compresi nel range corretto, in questo caso da 0 a 255.
  normalize(mag, mag, 0, 255, NORM_MINMAX);
  mag.convertTo(mag, CV_8UC1);

  /**
   * 3. Applicare la non-maximum suppression
   * 
   * La non-maximum suppression è una fase dell'algoritmo di Canny che serve a sottoporre
   * i pixel dell'immagine di magnitudo a un processo di soppressione,
   * mantenendo solo i valori massimi lungo la direzione del gradiente.
   * 
   * L'obiettivo della non-maximum suppression è quello di affinare i bordi dell'immagine,
   * mantenendo solo i punti che rappresentano i bordi veri e propri.
   * Questo viene fatto considerando l'orientamento del gradiente (fase) in ogni punto
   * e confrontando la magnitudo con i suoi vicini lungo questa direzione.
  */
  // for (int i = 1; i < mag.rows - 1; i++) {
  //   for (int j = 1; j < mag.cols - 1; j++) {
  //     float ang_val = ang.at<float>(i, j);
  //     uchar pixel = mag.at<uchar>(i, j);

  //     if ((ang_val <= -157.5 && ang_val > 157.5) || (ang_val > -22.5 && ang_val <= 22.5)) {
  //       if (pixel < mag.at<uchar>(i, j - 1) || pixel < mag.at<uchar>(i, j + 1))
  //         pixel = 0;
  //     } else if ((ang_val <= -112.5 && ang_val > -157.5) || (ang_val > 22.5 && ang_val <= 67.5)) {
  //       if (pixel < mag.at<uchar>(i - 1, j - 1) || pixel < mag.at<uchar>(i + 1, j + 1))
  //         pixel = 0;
  //     } else if ((ang_val <= -67.5 && ang_val > -112.5) || (ang_val > 67.5 && ang_val <= 112.5)) {
  //       if (pixel < mag.at<uchar>(i + 1, j) || pixel < mag.at<uchar>(i - 1, j))
  //         pixel = 0;
  //     } else if ((ang_val <= -22.5 && ang_val > -67.5) || (ang_val > 112.5 && ang_val <= 157.5)) {
  //       if (pixel < mag.at<uchar>(i - 1, j + 1) || pixel < mag.at<uchar>(i + 1, j - 1))
  //         pixel = 0;
  //     }

  //     mag.at<uchar>(i, j) = pixel;
  //   }
  // }
  nonMaxSuppression( mag, ang, edges );

  // 4. Applicare il thresholding con isteresi
  for (int i = 1; i < mag.rows - 1; i++) {
    for (int j = 1; j < mag.cols - 1; j++) {
      uchar pixel = mag.at<uchar>(i, j);

      if (pixel > threshold2)
        pixel = 255;
      else if (pixel < threshold1)
        pixel = 0;
      else if (pixel >= threshold1 && pixel <= threshold2) {
        bool strong_neighbor = false;

        for (int x = -1; x <= 1 && !strong_neighbor; x++) {
          for (int y = -1; y <= 1 && !strong_neighbor; y++) {
            if (mag.at<uchar>(i + x, j + y) > threshold2)
              strong_neighbor = true;
          }
        }

        if (strong_neighbor)
          pixel = 255;
        else
          pixel = 0;
      }

      mag.at<uchar>(i, j) = pixel;
    }
  }

  edges = mag(Rect(1, 1, mag.cols - 2, mag.rows - 2));
}


void myCanny( Mat image, Mat& edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false ) {

  // 1. Convolvere l'immagini di input con un filtro Gaussiano
  Mat paddedImage;
  Mat gaussianBlur;
  copyMakeBorder( image, paddedImage, 1, 1, 1, 1, BORDER_REFLECT );
  GaussianBlur( paddedImage, gaussianBlur, Size( 3, 3 ), 0, 0 );

  // 2. Calcolare la magnitudo e l'orientazione del gradiente
  Mat dx, dy;
	Sobel( gaussianBlur, dx, CV_32FC1, 1, 0, apertureSize );
	Sobel( gaussianBlur, dy, CV_32FC1, 0, 1, apertureSize );

  // Magnitudo
  Mat mag;
  mag = abs( dx + dy );
  
  // Orientamento del gradiente
  Mat ang;
  float ang_val;
  phase( dx, dy, ang, true );

  // normalize( mag, mag, 0, 255, NORM_MINMAX );
	mag.convertTo( mag, CV_8UC1 );
	
	// normalize( ang, ang, -180, 180, NORM_MINMAX );

  // 3. Applicare la non maxima suppression
  for ( int i = 1; i < mag.rows - 1; i++ ) {

		for( int j = 1; j < mag.cols - 1; j++ ) {

			ang_val = ang.at<float>( i, j );
			
			if ( ( ang_val <= -157.5 && ang_val > 157.5 ) || ( ang_val > -22.5 && ang_val <= 22.5 ) ) { 
				if ( mag.at<uchar>( i, j ) < mag.at<uchar>( i, j-1 ) || mag.at<uchar>( i, j ) < mag.at<uchar>( i, j+1 ) )
					mag.at<uchar>( i, j ) = 0;
			
      } else if ( ( ang_val <= -112.5 && ang_val > -157.5 ) || ( ang_val > 22.5 && ang_val <= 67.5 ) ) {
				if ( mag.at<uchar>( i, j ) < mag.at<uchar>( i-1 , j-1 ) || mag.at<uchar>( i, j ) < mag.at<uchar>( i+1, j+1 ) )
					mag.at<uchar>( i, j ) = 0;
			
      } else if ( ( ang_val <= -67.5 && ang_val > -112.5 ) || ( ang_val > 67.5 && ang_val <= 112.5 ) ) {
				if ( mag.at<uchar>( i, j ) < mag.at<uchar>( i+1, j ) || mag.at<uchar>( i, j ) < mag.at<uchar>( i-1, j ) )
					mag.at<uchar>( i, j ) = 0; 	
			
      } else if ( ( ang_val <= -22.5 && ang_val > -67.5 ) || ( ang_val > 112.5 && ang_val <= 157.5 ) ) {
				if ( mag.at<uchar>( i, j ) < mag.at<uchar>( i-1, j+1 ) || mag.at<uchar>( i, j ) < mag.at<uchar>( i+1, j-1 ) )
					mag.at<uchar>( i, j ) = 0;	
			}
		}
	}

  // 4. Applicare il thresholding con isteresi
  for ( int i = 1; i < mag.rows-1; i++ ) {
		for ( int j = 1; j < mag.cols-1; j++ ) {

			if ( mag.at<uchar>( i, j ) > threshold2 ) 
        mag.at<uchar>( i, j ) = 255;
			
      else if ( mag.at<uchar>( i, j ) < threshold1 )
        mag.at<uchar>( i, j ) = 0;
			
      else if ( mag.at<uchar>( i, j ) <= threshold2 && mag.at<uchar>( i, j ) >= threshold1 ) {
				
        bool strong_n = false;
				for ( int x = -1; x <= 1 && !strong_n ; x++ ) {
					for ( int y = -1; y <= 1 && !strong_n ; y++ ) {
						if( mag.at<uchar>( i+x , j+y ) > threshold2 )
              strong_n = true;
					}
				}

				if ( strong_n )
          mag.at<uchar>( i,j ) = 255;
				else
          mag.at<uchar>( i, j ) = 0;
			}
		}
	}

	int x = 1;
	int y = 1;
	int height = mag.rows -2;
	int width = mag.cols -2;
	Rect roiRect( x, y, width, height );
	edges = mag( roiRect );
}

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
  myCanny2( detected_edgesMyCanny, detected_edgesMyCanny, lowThreshold, hightThreshold );

  /// Using Canny's output as a mask, we display our result
  // dst = Scalar::all(0);

  // src.copyTo( dst, detected_edges);
  imshow( "Canny", detected_edges );
  imshow( "My Canny", detected_edgesMyCanny );
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
  namedWindow( "Canny", WINDOW_AUTOSIZE );
  namedWindow( "My Canny", WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Max Threshold:", "Canny", &hightThreshold, max_hightThreshold, CannyThreshold );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", "Canny", &lowThreshold, max_lowThreshold, CannyThreshold );

    /// Create a Trackbar for user to enter threshold
  createTrackbar( "Max Threshold:", "My Canny", &hightThreshold, max_hightThreshold, CannyThreshold );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", "My Canny", &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
}