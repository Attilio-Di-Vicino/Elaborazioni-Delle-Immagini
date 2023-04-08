// Realizzare una funzione che effettui il padding di un'immagine
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// g++ padding.cpp -o padding $(pkg-config --cflags --libs opencv)
// ./padding opencv-logo.png

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

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    //! [window]
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    imshow( "Display window", image );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]

    //Utility
    Mat imageC;

    /**
     * 255 padding bianco
     * La funzione "create" della classe "Mat" di OpenCV viene utilizzata per creare la nuova immagine "imageC".
     * I parametri passati alla funzione sono le dimensioni dell'immagine, che sono le dimensioni dell'immagine
     * originale aumentate di due pixel su ogni lato e il tipo di immagine, che viene copiato dall'immagine originale.
     * 
     * Successivamente, la funzione "setTo" viene utilizzata per impostare tutti i pixel dell'immagine "imageC"
     * sul valore di colore bianco (255,255,255).
     * 
     * Infine, viene utilizzato un doppio ciclo "for" per copiare i pixel dell'immagine "image" nell'immagine "imageC"
     * spostando ciascun pixel dell'immagine originale di una posizione verso destra e verso il basso, in modo da centrare
     * l'immagine originale all'interno dell'immagine più grande. Il valore del pixel dell'immagine originale viene quindi
     * copiato nella posizione corrispondente nell'immagine "imageC".
    */
    int top = 1;
    int bottom = 1;
    int left = 1;
    int right = 1;

    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    imageC.setTo( cv::Scalar( 255, 255, 255 ) );
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec3b>( i + bottom, j + right ) = image.at<cv::Vec3b>( i, j );
        }
    }

    imshow( "Display window", imageC);
    waitKey(0);

    /**
     * padding speculare
    */
    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    imageC.setTo( cv::Scalar( 255, 255, 255 ) );
    for ( int i = 0; i < image.rows; i++ ) {

        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec3b>( i + bottom, j + right ) = image.at<cv::Vec3b>( i, j );

            if ( i == 0 ) {
                // copio la prima riga di image nell'image di copia
                imageC.at<Vec3b>( 0, j ) = image.at<cv::Vec3b>( 0, j );
            } else if ( i == image.rows ) {
                // copio l'ultima riga di image nell'image di copia
                imageC.at<Vec3b>( imageC.rows, j ) = image.at<cv::Vec3b>( image.rows, j );
            }
        }

    }

    for ( int i = 0; i < image.rows; i++ ) {
        imageC.at<Vec3b>( i, 0 ) = image.at<cv::Vec3b>( i, 0 );
    }

    for ( int i = 0; i < image.rows; i++ ) {
        imageC.at<Vec3b>( i, imageC.cols ) = image.at<cv::Vec3b>( i, image.cols );
    }


    imshow( "Display window", imageC);
    waitKey(0);

    /**
    * padding speculare testing
    */

    top = 10;
    bottom = 10;
    left = 10;
    right = 10;
    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    imageC.setTo( cv::Scalar( 255, 255, 255 ) );
    for ( int i = 0; i < image.rows; i++ ) {

        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec3b>( i + bottom, j + right ) = image.at<cv::Vec3b>( i, j );

            if ( i == 0 ) {
                // copio la prima riga di image nell'image di copia
                imageC.at<Vec3b>( 0, j ) = 0;
            } else if ( i == image.rows ) {
                // copio l'ultima riga di image nell'image di copia
                imageC.at<Vec3b>( imageC.rows, j ) = 0;
            }
        }

    }

    for ( int j = 0; j < 10; j++  ) {

        for ( int i = 0; i < imageC.rows; i++ ) {

            if (j > 0 && j < imageC.cols) {
                // copio la prima riga di image nell'image di copia
                imageC.at<Vec3b>( i, j ) = 0;
                // copio l'ultima riga di image nell'image di copia
                imageC.at<Vec3b>( i, imageC.cols-j ) = 0;
            }
        }
    }

    for ( int i = 0; i < 10; i++  ) {
        for ( int j = 0; j < imageC.cols; j++ ) {

            if (i > 0 && i < imageC.rows) {
                imageC.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
                imageC.at<Vec3b>(imageC.rows - i, j) = Vec3b(0, 0, 0);
            }
        }
    }




    imshow( "Display window", imageC);
    waitKey(0);

    return 0;
}