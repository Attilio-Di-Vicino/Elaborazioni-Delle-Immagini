#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

class MyFilter2D {
    private:
        static void correlazioneC1( Mat src, Mat& dst, Mat paddedImage, Mat filter );
        template<typename T> static void correlazioneC( Mat src, Mat& dst, Mat paddedImage, Mat filter, int channels );
    public:
        template<typename T> static void myFilter2D( Mat src, Mat& dst, Mat filter );
};

template<typename T> void MyFilter2D::myFilter2D( Mat src, Mat& dst, Mat filter ) {

    int xPad = filter.rows;
    int yPad = filter.cols;

    Mat paddedImage;
    copyMakeBorder( src, paddedImage, xPad / 2, xPad / 2, yPad / 2, yPad / 2, BORDER_WRAP  );

    int channels = src.channels();

    if ( channels == 1 ) {
        correlazioneC1( src, dst, paddedImage, filter );
    } else if ( channels == 2 ) {
        correlazioneC<Vec2b>( src, dst, paddedImage, filter, channels );
    } else if ( channels == 3 ) {
        correlazioneC<Vec3b>( src, dst, paddedImage, filter, channels );
    }
}

void MyFilter2D::correlazioneC1( Mat src, Mat& dst, Mat paddedImage, Mat filter ) {

    for ( int i = 0; i < src.rows; i++ )
        for( int j = 0; j < src.cols; j++ ) {

            // dichiaro una variabile la somma
            // inizializzo ogni volta la somma a 0
            float sum = 0.0;

            for( int r = 0; r < filter.rows; r++ ) // kernel
                for( int c = 0; c < filter.cols; c++ ) {
                    
                    // somma nell'intorno
                    sum += paddedImage.at<uchar>( i + r, j + c ) * filter.at<float>( r, c );
                }
            
            //assegnazione all'immagine di ritorno
            dst.at<uchar>( i, j ) = floor( sum );
        }
}

template<typename T> void MyFilter2D::correlazioneC( Mat src, Mat& dst, Mat paddedImage, Mat filter, int channels ) {

    for ( int i = 0; i < src.rows; i++ )
        for( int j = 0; j < src.cols; j++ ) {

            // dichiaro un array per le somme con size uguale ai rispettivi canali dell'immagine
            // inizializzo ogni volta le somme
            float sum[ channels ];
            for ( int cii = 0; cii < channels; cii++ )
                sum[ cii ] = 0.0;

            for( int r = 0; r < filter.rows; r++ ) // kernel
                for( int c = 0; c < filter.cols; c++ ) {
                    
                    // somma nell'intorno
                    for ( int cii = 0; cii < channels; cii++ ) {
                        sum[ cii ] += paddedImage.at<T>( i + r, j + c )[ cii ] * filter.at<float>( r, c );
                    }
                }
            
            // assegnazione all'immagine di ritorno
            for ( int cii = 0; cii < channels; cii++ ) {
                dst.at<T>( i, j )[ cii ] = floor( sum[ cii ] );          
            }
        }
}
