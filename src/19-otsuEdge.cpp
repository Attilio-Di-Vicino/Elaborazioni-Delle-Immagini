/**
 * Quando la regione da segmentare è così piccola da rendere il suo contributo
 * all'istogramma trascurabile, lo smoothing non risolve il problema.
 * Possiamo pero migliorare l'istogramma considerando solo i pixel di edge
 * tra gli oggetti e il background. In questo modo la probabilità che un pixel
 * appartenga al foreground è pressoché uguale alla probabilità che appartenga al background,
 * per cui le mode saranno simili
 * 
 * OTSU EDGE
 * 
 * 1. Calcolare un immagine di edge ( Laplacian )
 * 2. Individuare un valore di soglia T
 * 3. Applicare la soglia all'immagine di edge, ottenendo un'immagine binaria
 * 4. Calcolare l'istogramma utilizzando solo i pixel dell'immagine di input che
 *    corrispondono alle posizioni dei pixel con valore 1 nell'immagine binaria
 * 5. Utilizzare il metodo Otsu con l'istogramma ottenuto
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

vector<double> createNormalizedEdgeHistogram( const Mat& image ) {
    
    Mat edge, blurredImg;
    // Dim: 256, Value: 0.0
    vector<double> normalizedEdgeHistogram( 256, 0.0f );

    int edgePixel = 0;
    medianBlur( image, blurredImg, 5 );
    Canny( blurredImg, edge, 15, 30 ); // Immagine Cellule
    imshow( "edge", edge );

    for ( int y = 0; y < edge.rows; y++ )
        for ( int x = 0; x < edge.cols; x++)
            if ( edge.at<uchar>( y, x ) == 255 ) {
                normalizedEdgeHistogram.at( image.at<uchar>( y, x ) )++;
                edgePixel++;
            }
    
    // Normalizzazione
    for( auto &x : normalizedEdgeHistogram )
        x /= edgePixel;
    
    return normalizedEdgeHistogram;
}

int otsu( const Mat& src ) {
    
    Mat gauss;
    GaussianBlur( src, gauss, Size( 3, 3 ), 0, 0 );

    vector<double> normalizedEdgeHistogram; // Istogramma normalizzato
    vector<double> cumulativeSums( 256, 0.0f ); // Somme cumulative
    vector<double> cumulativeAverages( 256, 0.0f ); // Medie cumulative
    double mg = 0.0f; // Intensità globale media
    double interclassVariance = 0.0f; // Varianza interclasse
    int T = 0; // Soglia ottimale

    // 1. Calcolare l'istogramma normalizzato dell'immagine
    normalizedEdgeHistogram = createNormalizedEdgeHistogram( gauss );

    // 2, 3, 4.
    cumulativeSums.at(0) = normalizedEdgeHistogram.at(0);
    cumulativeAverages.at(0) = normalizedEdgeHistogram.at(0);
    mg = normalizedEdgeHistogram.at(0);
    for ( auto i = 1; i < 256; i++ ) {
        cumulativeSums.at(i) = cumulativeSums.at( i - 1 ) + normalizedEdgeHistogram.at(i);
        cumulativeAverages.at(i) = cumulativeAverages.at( i - 1 ) + i * normalizedEdgeHistogram.at(i);
        mg += i * normalizedEdgeHistogram.at(i);
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
