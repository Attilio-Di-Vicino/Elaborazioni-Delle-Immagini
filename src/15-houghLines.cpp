/**
 * STEP ALGORITMO DI HOUGH LINES
 * 
 *  1. Inizializzare l'accumulatore H
 *  2. Applicare l'algoritmo di Canny per individuare i punti di edge
 *  3. Per ogni punto (𝑥, 𝑦) di edge
 *  4. Per ogni angolo 𝜃 = 0: 180 calcolare 𝜌 = 𝑥 cos 𝜃 + 𝑦 sin 𝜃
 *  5. Incrementare H(𝜌, 𝜃)=H(𝜌, 𝜃)+1
 *  6. Le celle H 𝜌, 𝜃 con un valore maggiore di una soglia th corrispondono alle rette nell'immagine
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;

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
void houghLines( InputArray image, OutputArray lines, double rho,
                double theta, int threshold, double srn = 0, double stn = 0 ) {
    


}

int main( int argc, char* argv[] ) {

    if ( argc != 2 ) {
        std::cout  << "usage: " << argv[0] << " <image>" << std::endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    
    imshow( "Src", src );
    waitKey();

    return 0;
}