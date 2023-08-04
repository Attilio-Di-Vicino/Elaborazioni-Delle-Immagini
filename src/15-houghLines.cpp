/**
 * STEP ALGORITMO DI HOUGH LINES
 * 
 *  1. Inizializzare l'accumulatore H
 *  2. Applicare l'algoritmo di Canny per individuare i punti di edge
 *  3. Per ogni punto (𝑥, 𝑦) di edge
 *  4. Per ogni angolo 𝜃 = 0: 180 calcolare 𝜌 = 𝑥 cos 𝜃 + 𝑦 sin 𝜃
 *  5. Incrementare H(𝜌, 𝜃) = H(𝜌, 𝜃) + 1
 *  6. Le celle H 𝜌, 𝜃 con un valore maggiore di una soglia th corrispondono alle rette nell'immagine
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Utilizzata per la converione da gradi in radianti e viceversa
#define DEG2RAD CV_PI / 180
#define ALPHA 1000

int hypotenuse;

using namespace cv;

/**
 * Crea lo spazio per gli accumulatori
 * 
 * @param a rows dell'imaggine
 * @param b clos dell'immagine
 * @return spazio accumulatori
*/
Mat creatVotesSpacing( int a, int b ) {
    hypotenuse = hypot( a, b );
    // 2 * hypotenuse: rows
    // 180 + 1: cols
    return Mat::zeros( 2 * hypotenuse, 180 + 1, CV_32F );
}

/**
 * Votazione per le possibili linee rilevate nell'immagine
 * 
 * @param edge edge dell'immagine ( output di canny )
 * @param voteSpacing matrice per accumulatori ( votazioni )
*/
void votation( const Mat& edge, Mat& voteSpacing ) {
    
    float theta;
    int rho;

    for ( int y = 0; y < edge.rows; y++ ) {
        for ( int x = 0; x < edge.cols; x++ ) {
            // Se è un edge
            if ( edge.at<uchar>( y, x ) == 255 ) {
                // Angoli possibili -90 a 90
                for ( int thetaIndex = 0; thetaIndex <= 180; thetaIndex++ ) {
                    // Conversione in radianti dell'angolo corrente
                    theta = ( thetaIndex - 90 ) * DEG2RAD;
                    rho = cvRound( x * cos( theta ) + y * sin( theta ) ) + hypotenuse;
                    voteSpacing.at<float>( rho, thetaIndex )++;
                }
            }
        }
    }
}

void polarToCartesian( double rho, int theta, Point& begin, Point& end ) {

    int x0 = cvRound( rho * cos( theta ) );
	int y0 = cvRound( rho * sin( theta ) );

    begin.x = cvRound( x0 - ALPHA * ( -sin( theta )));
	begin.y = cvRound( y0 - ALPHA * ( cos( theta )));

	end.x = cvRound( x0 + ALPHA * ( -sin( theta )));
	end.y = cvRound( y0 + ALPHA * ( cos( theta )));
}

void detectLines( Mat& lines, const Mat& voteSpacing, int threshold ) {

    Point begin, end;
    
    for ( int rhoIndex = 0; rhoIndex < voteSpacing.rows; rhoIndex++ ) {
        for ( int thetaIndex = 0; thetaIndex <= 180; thetaIndex++ ) {
            
            if( voteSpacing.at<float>( rhoIndex, thetaIndex ) > threshold ) {
                int rho = rhoIndex - hypotenuse;
                float theta = ( thetaIndex - 90 ) * DEG2RAD;
                int x0 = cvRound( rho * cos( theta ) );
                int y0 = cvRound( rho * sin( theta ) );

                begin.x = cvRound( x0 - ALPHA * ( -sin( theta )));
                begin.y = cvRound( y0 - ALPHA * ( cos( theta )));

                end.x = cvRound( x0 + ALPHA * ( -sin( theta )));
                end.y = cvRound( y0 + ALPHA * ( cos( theta )));
                line( lines, begin, end, Scalar( 255 ), 2, LINE_AA );
            }
        }
    }
}

/**
 * Hough Lines
 * 
 * @param image Input single channel image
 * @param lines N-by-1 two-channel array
 * @param rho Resolution ( pixels )
 * @param theta Resolution ( radians )
 * @param thresold Unnormalized accumulator threasold
 * @param srn rho refinemet ( for MHT )
 * @param stn theta refinemet ( for MHT )
*/
void houghLines( Mat image, Mat& lines, double rho,
                double theta, int threshold, double srn = 0, double stn = 0 ) {
    
    image.copyTo( lines );
    
    // Convert to single channel and smoothing
    GaussianBlur( image, image, Size( 5, 5 ), 0, 0 );
    cvtColor( image, image, COLOR_RGB2GRAY );

    // 1. Inizializzare l'accumulatore H
    // In questo caso viene crato uno spazio ( Matrice )
    // Per contere i voti che vengono chiamati accumulatori
    // Creating votes
    Mat voteSpacing = creatVotesSpacing( image.rows, image.cols );

    // 2. Applicare l'algoritmo di Canny per individuare i punti di edge
    Mat edge;
    Canny( image, edge, 120, 140 );
    imshow( "edge", edge );

    // 3. Per ogni punto (𝑥, 𝑦) di edge
    // 4. Per ogni angolo 𝜃 = 0: 180 calcolare 𝜌 = 𝑥 cos 𝜃 + 𝑦 sin 𝜃
    // 5. Incrementare H(𝜌, 𝜃) = H(𝜌, 𝜃) + 1
    votation( edge, voteSpacing );

    // 6. Le celle H 𝜌, 𝜃 con un valore maggiore di una soglia th corrispondono alle rette nell'immagine
    detectLines( lines, voteSpacing, threshold );
}

int main( int argc, char* argv[] ) {

    if ( argc != 2 ) {
        std::cout  << "usage: " << argv[0] << " <image>" << std::endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    Mat myHoughLines;
    houghLines( src, myHoughLines, 0.0, 0.0, 180 );
    
    imshow( "src", src );
    imshow( "myHoughLines", myHoughLines );
    waitKey();

    return 0;
}