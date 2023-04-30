/**
 * Realizzare una funzione che effettui il padding di un'immagine
 * 
 * Sostituire al valore di ogni pixel il valore medio dei livelli di grigio in un intorno 3x3
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../Algorithm/MyPadding.h"
#include "../Algorithm/MyBlur.h"

// g++ blurAtColor.cpp -o padding $(pkg-config --cflags --libs opencv)
// ./padding giacinto.jpg

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

uchar sum8( Mat , int, int, int );
template<typename T> Mat myBlur( Mat, int );
template<typename T> Mat myBlurSumOneChannel( Mat, Mat, int, int );
template<typename T> Mat myBlurSumMoreChannels( Mat, Mat, int, int );
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

    Mat blurredImage = myBlur<Vec3b>( image, 11 );

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

/**
 * myBlur permette di effettuare il blur su un'immagine
 * utilizza un kernel il quale sarà indispensabile per effettuare 
 * la somma e la media nell'intorno preso in input per semplicità
 * se l'intorno è pari verrà trasformato in dispari per avere un ancor point centrale al kernel
 * 
 * @param imageA immagine di default al quale si vuole applicare il blur
 * @param kernelSize size del kernel
 * @return immagine con blur effettuato
*/
template<typename T> Mat myBlur( Mat imageA, int kernelSize ) { 

    Mat image( imageA );

    // se è pari trasformalo a dispari
    if ( ( kernelSize % 2 ) == 0 )
        kernelSize++;

    int paddingSize = floor( kernelSize / 2 );

    Mat paddedImage;
    MyPadding<T>::padding( image, paddedImage, paddingSize, MyBorderType::BORDER_WRAP );

    int channels = image.channels();

    if ( channels == 1 ) {
        return myBlurSumOneChannel<uchar>( image, paddedImage, kernelSize, channels );
    } else if ( channels == 2 ) {
        return myBlurSumMoreChannels<Vec2b>( image, paddedImage, kernelSize, channels );
    } else if ( channels == 3 ) {
        return myBlurSumMoreChannels<Vec3b>( image, paddedImage, kernelSize, channels );
    }
    
    return image;
}

template<typename T> Mat myBlurSumOneChannel( Mat image, Mat paddedImage, int kernelSize, int channels ) { 

    for ( int i = 0; i < image.rows; i++ )
        for( int j = 0; j < image.cols; j++ ) {
            int sum = 0;
            for( int r = 0; r < kernelSize; r++ ) // kernel
                for( int c = 0; c < kernelSize; c++ ) {
                    // somma nell'intorno
                    sum += paddedImage.at<T>( i + r, j + c );
                }
            // media della somma e assegnazione all'immagine di ritorno
            image.at<T>( i, j ) = sum / ( kernelSize * kernelSize );
        }
    return image;
}

template<typename T> Mat myBlurSumMoreChannels( Mat image, Mat paddedImage, int kernelSize, int channels ) { 

    for ( int i = 0; i < image.rows; i++ )
        for( int j = 0; j < image.cols; j++ ) {

            // dichiaro un array per le somme con size uguale ai rispettivi canali dell'immagine
            // inizializzo ogni volta le somme
            int sum[ channels ];
            for ( int cii = 0; cii < channels; cii++ )
                sum[ cii ] = 0;

            for( int r = 0; r < kernelSize; r++ ) // kernel
                for( int c = 0; c < kernelSize; c++ ) {
                    // somma nell'intorno
                    for ( int cii = 0; cii < channels; cii++ ) {
                        sum[ cii ] += paddedImage.at<T>( i + r, j + c )[ cii ];
                    }
                }
            
            // media della somma e assegnazione all'immagine di ritorno
            for ( int cii = 0; cii < channels; cii++ ) {
                image.at<T>( i, j )[ cii ] = sum[ cii ] / ( kernelSize * kernelSize );
            }
        }
    return image;
}