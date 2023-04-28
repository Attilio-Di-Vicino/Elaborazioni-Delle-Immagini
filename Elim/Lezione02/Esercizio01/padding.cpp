/**
 * Realizzare una funzione che effettui il padding di un'immagine
 * 
 * Sostituire al valore di ogni pixel il valore medio dei livelli di grigio in un intorno 3x3
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../../Algorithm/MyPadding.h"

// g++ padding.cpp -o padding $(pkg-config --cflags --libs opencv)
// ./padding giacinto.jpg

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

uchar sum8( Mat , int, int, int );
Mat myBlur( Mat, int );
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
    image = imread( samples::findFile( imageName ), IMREAD_GRAYSCALE ); // Read the file
    //! [imread]

    if( image.empty() ) // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey( 0 );

    Mat paddedTest;
    MyPadding<Vec2b>::padding( image, paddedTest, 20, MyBorderType::BORDER_CONSTANT, Scalar( 200, 200 ) );

    imshow( "paddedTest", paddedTest );
    waitKey( 0 );


    Mat paddedTest2;
    MyPadding<Vec2b>::padding( image, paddedTest2, 100, MyBorderType::BORDER_WRAP );

    imshow( "paddedTest2", paddedTest2 );
    waitKey( 0 );

    Mat paddedTest3;
    copyMakeBorder( image, paddedTest3, 100, 100, 100, 100 , BORDER_WRAP );

    imshow( "paddedTest3", paddedTest3 );
    waitKey( 0 );

    Mat blurredImage = myBlur( image, 57 );

    imshow( "blurredImage", blurredImage );
    waitKey( 0 );

    Mat blurDefault;
    blur( image, blurDefault, Size( 23, 23 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);
}

Mat myBlur( Mat imageA, int kernelSize ) { 

    Mat image( imageA );

    // se è pari trasformalo a dispari
    if ( ( kernelSize % 2 ) == 0 )
        kernelSize++;

    int paddingSize = kernelSize / 2;

    Mat paddedImage;
    MyPadding<uchar>::padding( image, paddedImage, paddingSize, MyBorderType::BORDER_WRAP );

    for ( int i = 0; i < image.rows; i++ )
        for( int j = 0; j < image.cols; j++ ) {

            int sum = 0;
            for( int r = 0; r < kernelSize; r++ )
                for( int c = 0; c < kernelSize; c++ ) {
                    sum += paddedImage.at<uchar>( i + r, j + c );
                }
            image.at<uchar>( i, j ) = sum / ( kernelSize * kernelSize );
            // image.at<uchar>(i, j) = sum8( paddedImage , i + 1, j + 1, 0 );
        }
    return image;
}

uchar sum8( Mat image, int i, int j, int p ) {

    return  ( image.at<uchar>( i - 0, j - 0 ) +
              image.at<uchar>( i - 1, j - 1 ) +
              image.at<uchar>( i - 1, j - 0 ) +
              image.at<uchar>( i - 1, j + 1 ) +
              image.at<uchar>( i - 0, j - 1 ) +
              image.at<uchar>( i - 0, j + 1 ) +
              image.at<uchar>( i + 1, j - 1 ) +
              image.at<uchar>( i + 1, j - 0 ) +
              image.at<uchar>( i + 1, j + 1 ) ) / 9;
}