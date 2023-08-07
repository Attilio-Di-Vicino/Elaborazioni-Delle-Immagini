/**
 * OTSU
 * 
 * 1. Calcolare l'istogramma normalizzato dell'immagine
 * 2. Calcolare le somme cumulative 𝑃1(𝑘) per k=0, 1,..., L-1
 * 3. Calcolare le medie cumulative 𝑚(𝑘) per k=0, 1,..., L-1
 * 4. Calcolare l'intensità globale media mg
 * 5. Calcolare la varianza interclasse 𝜎^2_b(𝑘) per k=0, 1,..., L-1
 * 6. Calcolare la soglia k*, ovvero il valore k per cui 𝜎^2_b(𝑘) è massimo
 * 7. ( Opzionale ) Calcolare il valore di separabilità 𝜂(𝑘 ∗ )
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>

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

int otsu( const Mat& src ) {
    
    Mat gauss;
    GaussianBlur( src, gauss, Size( 3, 3 ), 0, 0 );

    vector<double> normalizedHistogram; // Istogramma normalizzato
    vector<double> cumulativeSums( 256, 0.0f ); // Somme cumulative
    vector<double> cumulativeAverages( 256, 0.0f ); // Medie cumulative
    double mg = 0.0f; // Intensità globale media
    double interclassVariance = 0.0f; // Varianza interclasse
    int T = 0; // Soglia ottimale

    // 1. Calcolare l'istogramma normalizzato dell'immagine
    normalizedHistogram = createNormalizedHistogram( gauss );

    // 2, 3, 4.
    cumulativeSums.at(0) = normalizedHistogram.at(0);
    cumulativeAverages.at(0) = normalizedHistogram.at(0);
    mg = normalizedHistogram.at(0);
    for ( auto i = 1; i < 256; i++ ) {

        // 2, 3, 4.
        cumulativeSums.at(i) = cumulativeSums.at( i - 1 ) + normalizedHistogram.at(i);
        cumulativeAverages.at(i) = cumulativeAverages.at( i - 1 ) + i * normalizedHistogram.at(i);
        mg += i * normalizedHistogram.at(i);
    }

    // 5, 6.
    T = 0;
    for ( auto i = 1; i < 256; i++ ) {
        float numerator = pow( mg * cumulativeSums.at(i) - cumulativeAverages.at(i), 2 );
        float denominator = cumulativeSums.at(i) * ( 1 - cumulativeSums.at(i) );
        if ( denominator != 0 ) {
            double iv = numerator / denominator;
            if ( iv > interclassVariance ) {
                interclassVariance = iv;
                T = i;
            }
        }
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
    threshold( src, myOtsu, otsu( src ), 255, THRESH_BINARY );

    imshow( "src", src );
    imshow( "myOtsu", myOtsu );
    waitKey();
    return 0;
}