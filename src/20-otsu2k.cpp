/**
 * OTSU SOGLIE MULTIPLE
 * 
 * 1. Calcolare l'istogramma normalizzato dell'immagine
 * 2. Calcolare le somme cumulative 
 * 3. Calcolare le medie cumulative
 * 4. Calcolare l'intensità globale media mg
 * 5. Calcolare la varianza interclassE
 * 6. Calcolare le sogliE k
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Algorithm/MyThreshold.h"

using namespace std;
using namespace cv;

vector<double> createNormalizedHistogram( const Mat& image ) {
    
    // Dim: 256, Value: 0.0
    vector<double> normalizedHistogram( 256, 0.0f );
    
    // Occorrenze ( istogramma )
    for ( int y = 0; y < image.rows; y++ )
        for ( int x = 0; x < image.cols; x++ )
            normalizedHistogram.at( image.at<uchar>( y, x ) )++;

    // Normalizzazione: ogni valore rappresenta la probabilità
    // di occorrenza del corrispondente livello di grigio nell'immagine.
    // Espressa tra 0 e 1.
    for ( int i = 0; i < 256; i++ )
        normalizedHistogram.at(i) /= image.rows * image.cols;
    
    return normalizedHistogram;
}

vector<int> otsu2k( const Mat& src ) {
    
    Mat gauss;
    GaussianBlur( src, gauss, Size( 3, 3 ), 0, 0 );

    vector<double> normalizedHistogram; // Istogramma normalizzato
    vector<double> cumulativeSums( 3, 0.0f ); // Somme cumulative
    vector<double> cumulativeAverages( 3, 0.0f ); // Medie cumulative
    double mg = 0.0f; // Intensità globale media
    double interclassVariance = 0.0f; // Varianza interclasse
    double iv = 0.0f;
    vector<int> T( 2, 0 ); // Soglia ottimale

    // 1. Calcolare l'istogramma normalizzato dell'immagine
    normalizedHistogram = createNormalizedHistogram( gauss );

    // Init
    cumulativeSums.at(0) = normalizedHistogram.at(0);
    cumulativeAverages.at(0) = normalizedHistogram.at(0);
    mg = normalizedHistogram.at(0);

    // 4. Calcolare l'intensità globale media mg
    for ( auto i = 1; i < 256; i++ )
        mg += i * normalizedHistogram.at(i);

    // 2, 3, 5, 6.
    for ( auto i = 1; i < 256 - 2; i++ ) {
        cumulativeSums.at(0) += normalizedHistogram.at(i);
        cumulativeAverages.at(0) += i * normalizedHistogram.at(i);

        for ( int j = i + 1; j < 256; j++ ) {
            cumulativeSums.at(1) += normalizedHistogram.at(j);
            cumulativeAverages.at(1) += j * normalizedHistogram.at(j);

            for ( int k = j + 1; k < 256; k++ ) {
                cumulativeSums.at(2) += normalizedHistogram.at(k);
                cumulativeAverages.at(2) += k * normalizedHistogram.at(k);

                for ( int z = 0; z < 3; z++ )
                    iv += cumulativeSums.at(z) * 
                            pow( ( cumulativeAverages.at(z) / cumulativeSums.at(z) - mg ), 2 ); 

                if ( iv > interclassVariance ) {
                    interclassVariance = iv;
                    T.at(0) = i; // Low
                    T.at(1) = j; // Hight
                }
                iv = 0.0f;
            }
            cumulativeSums.at(2) = cumulativeAverages.at(2) = 0.0f;
        }
        cumulativeSums.at(1) = cumulativeAverages.at(1) = 0.0f;
    }

    return T;
}

int main( int argc, char* argv[] ) {

    if ( argc != 2 ) {
        cout << "usage: " << argv[0] << " <image> " << endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_GRAYSCALE );
    
    Mat myOtsu;
    src.copyTo( myOtsu );

    vector<int> T = otsu2k( src );
    cout << "T: " << T.at(0) << " T: " << T.at(1) << endl;
    
    // Applica la soglia alle diverse regioni dell'immagine
    MyThreshold::threshold( src, myOtsu, T.at(0), T.at(1), 255, ( int ) MyType::THRESH_OTSU2K );

    imshow( "src", src );
    imshow( "myOtsu", myOtsu );
    waitKey();
    return 0;
}