#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

class MyBlur {
    private:
        template<typename T> static Mat myBlurSumOneChannel( Mat, Mat, int, int );
        template<typename T> static Mat myBlurSumMoreChannels( Mat, Mat, int, int );
    public:
        template<typename T> static Mat myBlur( Mat, int );
};

template<typename T> Mat MyBlur::myBlur( Mat imageA, int kernelSize ) {
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

template<typename T> Mat MyBlur::myBlurSumOneChannel( Mat image, Mat paddedImage, int kernelSize, int channels ) { 

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

template<typename T> Mat MyBlur::myBlurSumMoreChannels( Mat image, Mat paddedImage, int kernelSize, int channels ) { 

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