#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

class MyCanny {
    private:
        static void nonMaxSuppression( const Mat&, const Mat&, Mat& );
    public:
        static void myCanny( const Mat&, Mat&, double, double, int, bool );
};

void MyCanny::nonMaxSuppression( const Mat& magnitude, const Mat& phase, Mat& edges ) {

    // Inizializzazione
    edges = Mat::zeros( magnitude.size(), CV_8UC1 );
    magnitude.copyTo( edges );

    // for ( int i = 1; i < magnitude.rows - 1; i++ ) {
    //     for ( int j = 1; j < magnitude.cols - 1; j++ ) {
    //         float angle = phase.at<float>( i, j );
    //         float mag = magnitude.at<float>( i, j );

    //         float q, r;

    //         // Calcolo delle coordinate dei pixel vicini
    //         if ( angle < 0 )
    //             angle += 180;

    //         if ( ( angle >= 0 && angle < 22.5 ) || ( angle >= 157.5 && angle <= 180 ) ) {
    //             q = magnitude.at<float>( i, j + 1 );
    //             r = magnitude.at<float>( i, j - 1 );
    //         }
    //         else if ( angle >= 22.5 && angle < 67.5 ) {
    //             q = magnitude.at<float>( i - 1, j + 1 );
    //             r = magnitude.at<float>( i + 1, j - 1 );
    //         }
    //         else if ( angle >= 67.5 && angle < 112.5 ) {
    //             q = magnitude.at<float>( i - 1, j );
    //             r = magnitude.at<float>( i + 1, j );
    //         }
    //         else {
    //             q = magnitude.at<float>( i - 1, j - 1 );
    //             r = magnitude.at<float>( i + 1, j + 1 );
    //         }

    //         // Suppressione dei non massimi
    //         if ( mag >= q && mag >= r ) {
    //             edges.at<uchar>( i, j ) = static_cast<uchar>( mag );
    //         }
    //     }
    // }

    // float q,r;
    // for ( int i = 0; i < magnitude.rows; i++ ) {
    //     for ( int j = 0; j < magnitude.cols; j++ ) {
    //         float angle = phase.at<float>( i, j );
    //         float mag = magnitude.at<float>( i, j );

    //         if ( ( angle <= -157.5 && angle > 157.5 ) || ( angle > -22.5 && angle <= 22.5 ) ) {
	// 			if ( mag < magnitude.at<float>( i, j-1 ) || mag < magnitude.at<float>( i, j+1 ) )
	// 				edges.at<float>( i, j ) = 0;

    //         } else if ( ( angle <= -112.5 && angle > -157.5 ) || ( angle > 22.5 && angle <= 67.5 ) ) {
	// 			if ( mag < magnitude.at<float>( i-1 , j-1 ) || mag < magnitude.at<float>( i+1, j+1 ) )
	// 				edges.at<float>( i, j ) = 0;

    //         } else if ( ( angle <= -67.5 && angle > -112.5 ) || ( angle > 67.5 && angle <= 112.5 ) ) {
	// 			if ( mag < magnitude.at<float>( i+1, j ) || mag < magnitude.at<float>( i-1, j ) )
	// 				edges.at<float>( i, j ) = 0;

    //         } else if ( ( angle <= -22.5 && angle > -67.5 ) || ( angle > 112.5 && angle <= 157.5 ) ) {
	// 			if ( mag < magnitude.at<float>( i-1, j+1 ) || mag < magnitude.at<float>( i+1, j-1 ) )
	// 				edges.at<float>( i, j ) = 0;
	// 		}
    //     }
    // }

    float q,r;
    for ( int i = 0; i < magnitude.rows; i++ ) {
        for ( int j = 0; j < magnitude.cols; j++ ) {

            float angle = phase.at<float>( i, j );
            float mag = magnitude.at<float>( i, j );

            if ( ( angle <= -157.5 && angle > 157.5 ) || ( angle > -22.5 && angle <= 22.5 ) ) { 
				q = magnitude.at<float>( i, j - 1 );
                r = magnitude.at<float>( i, j + 1 );
			
            } else if ( ( angle <= -112.5 && angle > -157.5 ) || ( angle > 22.5 && angle <= 67.5 ) ) {
				if ( mag < magnitude.at<float>( i-1 , j-1 ) || mag < magnitude.at<float>( i+1, j+1 ) )
					edges.at<float>( i, j ) = 0;
			
            } else if ( ( angle <= -67.5 && angle > -112.5 ) || ( angle > 67.5 && angle <= 112.5 ) ) {
				if ( mag < magnitude.at<float>( i+1, j ) || mag < magnitude.at<float>( i-1, j ) )
					edges.at<float>( i, j ) = 0; 	
			
            } else if ( ( angle <= -22.5 && angle > -67.5 ) || ( angle > 112.5 && angle <= 157.5 ) ) {
				if ( mag < magnitude.at<float>( i-1, j+1 ) || mag < magnitude.at<float>( i+1, j-1 ) )
					edges.at<float>( i, j ) = 0;	
			}
        }
    }

    // int neighbor1X, neighbor1Y, neighbor2X, neighbor2Y;
    // float gradientAngle;
    // for (int x = 0; x < magnitude.rows; x++) {
    // for (int y = 0; y < magnitude.cols; y++) {
    //     gradientAngle = phase.at<float>(y, x);

    //     if (abs(gradientAngle) > 180)
    //         gradientAngle = abs(gradientAngle-180);
    //     else
    //         gradientAngle = abs(gradientAngle);

    //     if (gradientAngle <= 22.5) {
    //     neighbor1X = x-1; neighbor1Y = y;
    //     neighbor2X = x+1; neighbor2Y = y;
    //     } else if (22.5 < gradientAngle <= 67.5) {
    //     neighbor1X = x-1; neighbor1Y = y-1;
    //     neighbor2X = x+1; neighbor2Y = y+1;
    //     } else if (67.5 < gradientAngle <= 112.5) {
    //     neighbor1X = x; neighbor1Y = y-1;
    //     neighbor2X = x; neighbor2Y = y+1;
    //     } else if (112.5 < gradientAngle <= 157.5) {
    //     neighbor1X = x-1; neighbor1Y = y+1;
    //     neighbor2X = x+1; neighbor2Y = y-1;
    //     } else if (157.5 < gradientAngle <= 202.5) {
    //     neighbor1X = x-1; neighbor1Y = y;
    //     neighbor2X = x+1; neighbor2Y = y;
    //     }
    //     if ((0 <= neighbor1X < magnitude.rows)
    //     && (0 <= neighbor1Y < magnitude.cols)) {
    //     if (magnitude.at<float>(y, x) < magnitude.at<float>             (neighbor1Y, neighbor1X)) {
    //         edges.at<float>(y, x) = 0;
    //         continue;
    //     }
    //     }
    //     if ((0 <= neighbor2X < magnitude.rows)
    //     && (0 <= neighbor2Y < magnitude.cols)) {
    //     if (magnitude.at<float>(y, x) < magnitude.at<float>          (neighbor2Y, neighbor2X)) {
    //         edges.at<float>(y, x) = 0;
    //         continue;
    //     }
    //     }
    // }
    // }
}

void MyCanny::myCanny( const Mat& image, Mat& edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false ) {

    // 1. Convolvere l'immagine di input con un filtro Gaussiano
    Mat paddedImage;
    Mat gaussianBlur;
    copyMakeBorder( image, paddedImage, 1, 1, 1, 1, BORDER_REFLECT );
    GaussianBlur( paddedImage, gaussianBlur, Size( 3, 3 ), 0, 0 );

    // 2. Calcolare la magnitudo e l'orientazione del gradiente
    Mat dx, dy;
    Sobel( gaussianBlur, dx, CV_32FC1, 1, 0, apertureSize );
    Sobel( gaussianBlur, dy, CV_32FC1, 0, 1, apertureSize );

    /**
     * La Magnitudo rappresenta la forza del gradiente in ciascun punto dell'immagine.
     * I valori della magnitudo saranno più alti nelle regioni dell'immagine con cambiamenti
     * di intensità più significativi, ad esempio i bordi dell'oggetto. I valori bassi
     * corrisponderanno alle regioni dell'immagine con cambiamenti
     * di intensità meno marcati o a zone piatte.
     */
    Mat magnitude;
    //magnitude = abs( dx + dy );
    cv::magnitude( dx, dy, magnitude );

    normalize( magnitude, magnitude, 0, 1, NORM_MINMAX );

    /**
     * La Fase fornisce informazioni sull'orientamento del gradiente in ciascun punto dell'immagine.
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
    Mat phase;
    cv::phase( dx, dy, phase, true );

    imshow( "magnitude", magnitude );
    imshow( "phase", phase );
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
    nonMaxSuppression( magnitude, phase, edges );
    imshow( "edges", edges );

    // 4. Applicare il thresholding con isteresi
    // threshold( edges, edges, threshold1, threshold2, THRESH_BINARY );

    // 4. Applicare il thresholding con isteresi
    for ( int i = 1; i < magnitude.rows - 1; i++ ) {
        for ( int j = 1; j < magnitude.cols - 1; j++ ) {

            float pixel = magnitude.at<float>( i, j );

            if ( pixel > threshold2 )
                pixel = 255;
            else if ( pixel < threshold1 )
                pixel = 0;
            else if ( pixel >= threshold1 && pixel <= threshold2 ) {
                bool strong_neighbor = false;

                for ( int x = -1; x <= 1 && !strong_neighbor; x++ ) {
                    for ( int y = -1; y <= 1 && !strong_neighbor; y++ ) {
                        if ( magnitude.at<float>( i + x, j + y ) > threshold2 )
                            strong_neighbor = true;
                    }
                }

                if ( strong_neighbor )
                    pixel = 255;
                else
                    pixel = 0;
            }
            edges.at<float>( i, j ) = pixel;
        }
    }
}