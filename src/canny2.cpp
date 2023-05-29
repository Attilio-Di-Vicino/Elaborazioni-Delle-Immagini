#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void nonMaxSuppression( const Mat& magnitude, const Mat& phase, Mat& edges ) {

    // Inizializzazione
    edges = Mat::zeros( magnitude.size(), CV_8UC1 );

    for ( int i = 1; i < magnitude.rows - 1; i++ ) {
        for ( int j = 1; j < magnitude.cols - 1; j++ ) {
            float angle = phase.at<float>( i, j );
            float mag = magnitude.at<float>( i, j );

            float q, r;

            // Calcolo delle coordinate dei pixel vicini
            if ( angle < 0 )
                angle += 180;

            if ( ( angle >= 0 && angle < 22.5 ) || ( angle >= 157.5 && angle <= 180 ) ) {
                q = magnitude.at<float>( i, j + 1 );
                r = magnitude.at<float>( i, j - 1 );
            }
            else if ( angle >= 22.5 && angle < 67.5 ) {
                q = magnitude.at<float>( i - 1, j + 1 );
                r = magnitude.at<float>( i + 1, j - 1 );
            }
            else if ( angle >= 67.5 && angle < 112.5 ) {
                q = magnitude.at<float>( i - 1, j );
                r = magnitude.at<float>( i + 1, j );
            }
            else {
                q = magnitude.at<float>( i - 1, j - 1 );
                r = magnitude.at<float>( i + 1, j + 1 );
            }

            // Suppressione dei non massimi
            if ( mag >= q && mag >= r ) {
                edges.at<uchar>( i, j ) = static_cast<uchar>( mag );
            }
        }
    }
}

void myCanny( const Mat& image, Mat& edges, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false ) {
    
    // 1. Convolvere l'immagine di input con un filtro Gaussiano
    Mat paddedImage;
    Mat gaussianBlur;
    copyMakeBorder( image, paddedImage, 1, 1, 1, 1, BORDER_REFLECT );
    GaussianBlur( paddedImage, gaussianBlur, Size( 3, 3 ), 0, 0 );

    // 2. Calcolare la magnitudo e l'orientazione del gradiente
    Mat dx, dy;
    Sobel( gaussianBlur, dx, CV_32FC1, 1, 0, apertureSize );
    Sobel( gaussianBlur, dy, CV_32FC1, 0, 1, apertureSize );

    // Magnitudo
    Mat magnitude;
    magnitude = abs( dx + dy );

    // Orientamento del gradiente
    Mat phase;
    cv::phase( dx, dy, phase, true );

    // 3. Applicare la non-maximum suppression
    nonMaxSuppression( magnitude, phase, edges );

    // 4. Applicare il thresholding con isteresi
    threshold( edges, edges, threshold1, threshold2, THRESH_BINARY );
}

int main( int argc, char** argv ) {
    // Caricamento dell'immagine di input
    Mat image = imread( argv[1] , IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Impossibile caricare l'immagine di input." << endl;
        return -1;
    }

    // Parametri dell'algoritmo di Canny
    double threshold1 = 40;
    double threshold2 = 160;
    int apertureSize = 3;
    bool L2gradient = false;

    // Applicazione dell'algoritmo di Canny
    Mat edges;
    myCanny(image, edges, threshold1, threshold2, apertureSize, L2gradient);

    // Applicazione dell'algoritmo di Canny
    Mat edgesCanny;
    myCanny(image, edgesCanny, threshold1, threshold2, apertureSize, L2gradient);

    // Visualizzazione dei risultati
    imshow("Input", image);
    imshow("Edges", edges);
    imshow("edgesCanny", edgesCanny);
    waitKey(0);

    return 0;
}
