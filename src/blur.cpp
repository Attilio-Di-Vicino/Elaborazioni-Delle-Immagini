/**
 * Realizzare una funzione che effettui il padding di un'immagine
 * 
 * Sostituire al valore di ogni pixel il valore medio dei livelli di grigio in un intorno 3x3
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Algorithm/MyPadding.h"
#include "Algorithm/MyBlur.h"

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

int main( int argc, char** argv )
{
    //! [load]
    String imageName( "HappyFish.jpg" ); // by default
    if( argc > 1 )
    {
        imageName = argv[1];
    }
    //! [load]

    //! [mat]
    Mat image;
    //! [mat]

    //! [imread]
    image = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Read the file
    //! [imread]

    if( image.empty() ) // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey( 0 );

    Mat blurredImage = MyBlur::myBlur<Vec3b>( image, 11 );

    imshow( "blurredImage", blurredImage );
    waitKey( 0 );

    /**
     * Le operazioni di smoothind di openCV che ci propone sono:
     * blur() è la funzione di smoothing più “semplice”
     * boxFilter() è una generalizzazione di blur, in quanto presenta un parametro in più ( ddepth )
     * GaussianBlur() determina i pesi attraverso una funzione gaussiana ( una campana 3d )
     * 
     * In questo caso viene proposto un esempio applicativo di blur()
    */
    Mat blurDefault;
    blur( image, blurDefault, Size( 11, 11 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);
}