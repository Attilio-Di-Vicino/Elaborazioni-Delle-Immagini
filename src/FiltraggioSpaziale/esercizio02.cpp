/**
 * Implementare l'algoritmo di correlazione/convoluzione e confrontare
 * l'output con quello prodotto dalla funzione filter2D() usando un filtro media.
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../Algorithm/MyPadding.h"


// g++ esercizio02.cpp $(pkg-config --cflags --libs opencv)
// ./a.out image.png

#include <iostream>
#define DIM 11

using namespace cv;
using namespace std;

void myFilter2D( Mat, Mat&, Mat, MyBorderType );
void myFilter2DG( Mat, Mat&, Mat, MyBorderType );
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

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    waitKey(0);

    // oggetto mat utilizzato come filtro
    Mat myFilter = ( Mat_<uchar>( 3, 3 ) << 1, 1, 1,
                                            1, 1, 1,
                                            1, 1, 1);

    // Allocazione filtro correlazione e convoluzione
    // in questo caso si utilizza un filtro composto da 1
    // di fatti avremo lo stesso risultato con correlazione e convoluzione
    Mat average_filter = Mat::ones( DIM, DIM, CV_32F ) / ( float ) ( DIM * DIM );

    //Per la convoluzione ruotare il filtro di 180°
    Mat avarage_filter_180;
    rotate( average_filter, avarage_filter_180, ROTATE_180 );

    // correlazione
    Mat correlazione;
    filter2D( image, correlazione, image.type(), average_filter );

    imshow( "correlazione", correlazione );
    waitKey(0);

    // convoluzione
    Mat convoluzione;
    filter2D( image, convoluzione, image.type(), avarage_filter_180 );

    imshow( "convoluzione", convoluzione );
    waitKey(0);

    // correlazione
    Mat myCorrelazione;
    image.copyTo( myCorrelazione );
    myFilter2DG( image, myCorrelazione, average_filter, MyBorderType::BORDER_WRAP );

    imshow( "myCorrelazione", myCorrelazione );
    waitKey(0);

    // differenza 
    Mat diff = correlazione - myCorrelazione;

    imshow( "diff", diff );
    waitKey(0);

    return 0;
}

void myFilter2D( Mat src, Mat& dst, Mat kernel, MyBorderType borderType ) {

    int ksize = kernel.rows;

    Mat paddedImage;
    //MyPadding<Vec3b>::autoPadding( src, paddedImage, ksize, borderType );
    copyMakeBorder( src, paddedImage, ksize / 2, ksize / 2, ksize / 2, ksize / 2, BORDER_REFLECT  );

    int channels = src.channels();

    for ( int i = 0; i < src.rows; i++ )
        for( int j = 0; j < src.cols; j++ ) {

            // dichiaro un array per le somme con size uguale ai rispettivi canali dell'immagine
            // inizializzo ogni volta le somme
            int sum[ channels ];
            for ( int cii = 0; cii < channels; cii++ )
                sum[ cii ] = 0;

            for( int r = 0; r < ksize; r++ ) // kernel
                for( int c = 0; c < ksize; c++ ) {
                    
                    // somma nell'intorno
                    for ( int cii = 0; cii < channels; cii++ ) {
                        sum[ cii ] += paddedImage.at<Vec3b>( i + r, j + c )[ cii ] * kernel.at<float>( r, c );
                    }
                }
            
            // media della somma e assegnazione all'immagine di ritorno
            for ( int cii = 0; cii < channels; cii++ ) {
                dst.at<Vec3b>( i, j )[ cii ] = sum[ cii ];          
            }
        }
        // normalize( dst, dst, 0, 255, NORM_MINMAX, CV_8U );
}

void myFilter2DG( Mat src, Mat& dst, Mat kernel, MyBorderType borderType ) {

    int ksize = kernel.rows;

    Mat paddedImage;
    MyPadding<uchar>::autoPadding( src, paddedImage, ksize, borderType );

    int channels = src.channels();

    for ( int i = 0; i < src.rows; i++ )
        for( int j = 0; j < src.cols; j++ ) {

            // dichiaro un array per le somme con size uguale ai rispettivi canali dell'immagine
            // inizializzo ogni volta le somme
            int sum[ channels ];
            for ( int cii = 0; cii < channels; cii++ )
                sum[ cii ] = 0;

            for( int r = 0; r < ksize; r++ ) // kernel
                for( int c = 0; c < ksize; c++ ) {
                    
                    // somma nell'intorno
                    for ( int cii = 0; cii < channels; cii++ ) {
                        sum[ cii ] += paddedImage.at<uchar>( i + r, j + c ) * kernel.at<float>( r, c );
                    }
                }
            
            // media della somma e assegnazione all'immagine di ritorno
            for ( int cii = 0; cii < channels; cii++ ) {
                dst.at<uchar>( i, j ) = sum[ cii ];          
            }
        }
        // normalize( dst, dst, 0, 255, NORM_MINMAX, CV_8U );
}