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
Mat myBlur( Mat, Mat );
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

    if( image.empty() )                      // Check for invalid input
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

    Mat paddedTestForBlur;
    MyPadding<Vec2b>::padding( image, paddedTestForBlur, 1, MyBorderType::BORDER_WRAP );
    Mat blurredImage = myBlur( image, paddedTestForBlur );

    imshow( "blurredImage", blurredImage );
    waitKey( 0 );

    Mat blurDefault;
    blur( image, blurDefault, Size( 3, 3 ) );

    imshow( "blurDefault", blurDefault );
    waitKey(0);
}

Mat myBlur( Mat image, Mat paddedImage ) { 

    for ( int i = 0; i < image.rows; i++ )
        for( int j = 0; j < image.cols; j++ ) {
            image.at<Vec2b>(i, j)[0] = sum8( paddedImage , i + 1, j + 1, 0 );
            image.at<Vec2b>(i, j)[1] = sum8( paddedImage , i + 1, j + 1, 1 );
        }
    return image;
}

uchar sum8( Mat image, int i, int j, int p ) {

    return  ( image.at<Vec2b>( i - 0, j - 0 )[p] +
              image.at<Vec2b>( i - 1, j - 1 )[p] +
              image.at<Vec2b>( i - 1, j - 0 )[p] +
              image.at<Vec2b>( i - 1, j + 1 )[p] +
              image.at<Vec2b>( i - 0, j - 1 )[p] +
              image.at<Vec2b>( i - 0, j + 1 )[p] +
              image.at<Vec2b>( i + 1, j - 1 )[p] +
              image.at<Vec2b>( i + 1, j - 0 )[p] +
              image.at<Vec2b>( i + 1, j + 1 )[p] ) / 9;
}

// Mat padding( Mat image, int top, int bottom, int left, int right ) { 

//     Mat paddedImage( image.rows + top + bottom, image.cols + left + right, image.type(), Scalar( 0, 0 ) );
    
//     for ( int i = 0; i < image.rows; i++ ) {
//         for ( int j = 0; j < image.cols; j++ ) {
//             paddedImage.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
//         }
//     }

//     int index = 0;
//     for ( int i = 0; i < top; i++  ) {
//         for ( int j = 0; j < paddedImage.cols; j++ ) {
//             if ( j < image.cols )
//                 index = j;
//             else
//                 index = image.cols - 1;
//             paddedImage.at<Vec2b>( i, j ) = image.at<Vec2b>( i, index );
//         }
//     }

//     index = 0;
//     for ( int i = 0; i < bottom; i++  ) {
//         for ( int j = 0; j < paddedImage.cols; j++ ) {
//             if ( j < image.cols )
//                 index = j;
//             else
//                 index = image.cols - 1;
//             paddedImage.at<Vec2b>( paddedImage.rows - i - 2, j ) = image.at<Vec2b>( image.rows - i - 2, index ); // da capire il - 2
//         }
//     }

//     index = 0;
//     for ( int j = 0; j < left; j++  ) {
//         for ( int i = 0; i < paddedImage.rows; i++ ) {
//             if ( i < image.rows )
//                 index = i;
//             else 
//                 index = image.rows - 1;
//             paddedImage.at<Vec2b>( i, j ) = image.at<Vec2b>( index, j );
//         }
//     }

//     index = 0;
//     for ( int j = 0; j < right; j++  ) {
//         for ( int i = 0; i < paddedImage.rows; i++ ) {
//             if ( i < image.rows )
//                 index = i;
//             else 
//                 index = image.rows - 10; // l' ultimo pixel è nero quindi prendo l'ultimo pixel - 10
//             paddedImage.at<Vec2b>( i, paddedImage.cols - j - 1 ) = image.at<Vec2b>( index, image.cols - j - 1 );
//         }
//     }

//     return paddedImage;
// }