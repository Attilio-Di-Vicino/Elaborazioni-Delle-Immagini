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
        static void thresholdingIsteresi( const Mat&, Mat&, bool, bool );
    public:
        static void myCanny( const Mat&, Mat&, double, double, int, bool );
};

void MyCanny::nonMaxSuppression( const Mat& magnitude, const Mat& phase, Mat& edges ) {

    // Inizializzazione
    edges = Mat::zeros( magnitude.size(), CV_8UC1 );
    magnitude.copyTo( edges );

    uchar q,r;
    for ( int i = 0; i < magnitude.rows; i++ ) {
        for ( int j = 0; j < magnitude.cols; j++ ) {

            uchar angle = phase.at<uchar>( i, j );
            uchar mag = magnitude.at<uchar>( i, j );

            // Horizontal edge 0 gradi
            if ( ( angle <= -157.5 && angle > 157.5 ) || ( angle > -22.5 && angle <= 22.5 ) ) { 
				q = magnitude.at<uchar>( i, j-1 );
                r = magnitude.at<uchar>( i, j+1 );
			// -45 gradi
            } else if ( ( angle <= -112.5 && angle > -157.5 ) || ( angle > 22.5 && angle <= 67.5 ) ) {
                q = magnitude.at<uchar>( i-1, j-1 );
                r = magnitude.at<uchar>( i+1, j+1 );
			// Vertical edge 90 gradi
            } else if ( ( angle <= -67.5 && angle > -112.5 ) || ( angle > 67.5 && angle <= 112.5 ) ) {
                q = magnitude.at<uchar>( i+1, j );
                r = magnitude.at<uchar>( i-1, j );	
            // +45 gradi
            } else if ( ( angle <= -22.5 && angle > -67.5 ) || ( angle > 112.5 && angle <= 157.5 ) ) {
                q = magnitude.at<uchar>( i-1, j+1 );
                r = magnitude.at<uchar>( i+1, j-1 );
			}

            if ( mag < q || mag < r )
                edges.at<uchar>( i, j ) = 0;
        }
    }
}

void MyCanny::thresholdingIsteresi( const Mat& edges, Mat& dst, bool threshold1, bool threshold2 ) {

    edges.copyTo( dst );
    for ( int i = 0; i < edges.rows - 1; i++ ) {
        for ( int j = 0; j < edges.cols - 1; j++ ) {

            uchar pixel = edges.at<uchar>( i, j );

            if ( pixel >= threshold2 )
                pixel = 255;
            else if ( pixel <= threshold1 )
                pixel = 0;
            else {
                bool strongNeighbor = false;
                for ( int x = -1; x <= 1 && !strongNeighbor; x++ ) {
                    for ( int y = -1; y <= 1 && !strongNeighbor; y++ ) {
                        if ( edges.at<uchar>( i + x, j + y ) > threshold2 )
                            strongNeighbor = true;
                    }
                }
                if ( strongNeighbor )
                    pixel = 255;
                else
                    pixel = 0;
            }
            dst.at<uchar>( i, j ) = pixel;
        }
    }
}

void MyCanny::myCanny( const Mat& image, Mat& edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false ) {

    // 1. Convolvere l'immagine di input con un filtro Gaussiano
    Mat paddedImage;
    Mat gaussianBlur;
    copyMakeBorder( image, paddedImage, 2, 2, 2, 2, BORDER_REFLECT );
    GaussianBlur( paddedImage, gaussianBlur, Size( 5, 5 ), 0, 0 );

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
    // magnitude = abs( dx ) + abs( dy );
    cv::magnitude( dx, dy, magnitude );
    magnitude.convertTo( magnitude, CV_8UC1 );

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

    // 4. Applicare il thresholding con isteresi
    // Mat dst;
    // thresholdingIsteresi( edges, dst, threshold1, threshold2 );
    // dst.copyTo( edges );
    for ( int i = 0; i < edges.rows - 1; i++ ) {
        for ( int j = 0; j < edges.cols - 1; j++ ) {

            uchar pixel = edges.at<uchar>( i, j );

            if ( pixel >= threshold2 )
                pixel = 255;
            else if ( pixel <= threshold1 )
                pixel = 0;
            else {
                bool strongNeighbor = false;
                for ( int x = -1; x <= 1 && !strongNeighbor; x++ ) {
                    for ( int y = -1; y <= 1 && !strongNeighbor; y++ ) {
                        if ( edges.at<uchar>( i + x, j + y ) > threshold2 )
                            strongNeighbor = true;
                    }
                }
                if ( strongNeighbor )
                    pixel = 255;
                else
                    pixel = 0;
            }
            edges.at<uchar>( i, j ) = pixel;
        }
    }
}