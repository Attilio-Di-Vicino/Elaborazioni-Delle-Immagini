/**
 * Implementare l'algoritmo di correlazione/convoluzione e confrontare
 * l'output con quello prodotto dalla funzione filter2D().
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../Algorithm/MyFilter2D.h"


// g++ main.cpp $(pkg-config --cflags --libs opencv)
// ./a.out image.png

#include <iostream>
#define DIM 11

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    String imageName( "HappyFish.jpg" ); // by default
    if( argc > 1 )
    {
        imageName = argv[1];
    }

    Mat image;
    image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // oggetto mat utilizzato come filtro
    Mat myFilter = ( Mat_<float>( 3, 3 ) << 0,  1,  0,
                                            1, -4,  1,
                                            0,  1,  0);

    // Allocazione filtro correlazione e convoluzione
    // in questo caso si utilizza un filtro composto da 1
    // di fatti avremo lo stesso risultato con correlazione e convoluzione
    Mat average_filter = Mat::ones( DIM, DIM, CV_32F ) / ( float ) ( DIM * DIM );

    // blur
    Mat blurDefault;
    blur( image, blurDefault, Size( 3, 3 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);

    // Per la convoluzione ruotare il filtro di 180°
    // Mat avarage_filter_180;
    // rotate( average_filter, avarage_filter_180, ROTATE_180 );

    // correlazione
    Mat correlazione;
    filter2D( blurDefault, correlazione, image.type(), myFilter );

    imshow( "correlazione", correlazione );
    waitKey(0);

    // // convoluzione
    // Mat convoluzione;
    // filter2D( image, convoluzione, image.type(), avarage_filter_180 );

    // imshow( "convoluzione", convoluzione );
    // waitKey(0);

    // correlazione a colori
    Mat myCorrelazione;
    MyFilter2D::myFilter2D( blurDefault, myCorrelazione, myFilter );

    imshow( "myCorrelazione", myCorrelazione );
    waitKey(0);

    // differenza 
    Mat diff = abs(myCorrelazione - correlazione);

    imshow( "diff", diff );
    waitKey(0);

    return 0;
}