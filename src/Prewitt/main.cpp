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
    image = imread( samples::findFile( imageName ), IMREAD_GRAYSCALE ); // Read the file

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    // Rimuovi il rumore sfocando con un filtro gaussiano (dimensione del kernel = 3)
    GaussianBlur( image, image, Size (3, 3), 0, 0, BORDER_DEFAULT );

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // filtro prewitt gradiente x
    Mat myFilterGX = ( Mat_<float>( 3, 3 ) << -1,  -1,  -1,
                                               0,   0,   0,
                                               1,   1,   1 );
    
    // filtro prewitt gradiente y
    Mat myFilterGY = ( Mat_<float>( 3, 3 ) << -1,  0,  1,
                                              -1,  0,  1,
                                              -1,  0,  1 );

    // my sobel 90 gradiente x
    Mat myPrewittGX;
    MyFilter2D::myFilter2D( image, myPrewittGX, myFilterGX );

    // my sobel 90 gradiente y
    Mat myPrewittGY;
    MyFilter2D::myFilter2D( image, myPrewittGY, myFilterGY );

    Mat myAbsPrewittX, myAbsPrewittY, myPrewitt;

    // riconversione in CV_8U
    convertScaleAbs( myPrewittGX, myAbsPrewittX );
    convertScaleAbs( myPrewittGY, myAbsPrewittY );

    // Cerchiamo di approssimare il gradiente sommando entrambi i gradienti direzionali 
    // (nota che questo non è affatto un calcolo esatto! ma va bene per i nostri scopi).
    addWeighted( myAbsPrewittX, 0.5, myAbsPrewittY, 0.5, 0, myPrewitt );

    imshow( "myPrewitt", myPrewitt );
    waitKey(0);

    return 0;
}