/**
 * STEP ALGORITMO DI HOUGH LINES
 * 
 *  1. Inizializzare l'accumulatore H
 *  2. Applicare l'algoritmo di Canny per individuare i punti di edge
 *  3. Per ogni punto (𝑥, 𝑦) di edge
 *  4. Per ogni angolo 𝜃 = 0: 360 e per ogni raggio r=Rmin : Rmax
 *  5. Calcolare 𝑎 = 𝑥 − 𝑟 ∗ cos( 𝜃 * π / 180 ) e 𝑏 = 𝑦 − 𝑟 ∗ sin( 𝜃 * π / 180 ) 
 *  6. Incrementare H(𝑎, 𝑏, 𝑟)=H(𝑎, 𝑏, 𝑟) + 1
 *  7. Le celle H 𝑎, 𝑏, 𝑟 con un valore maggiore di una soglia th corrispondono
 *     alle circonferenze nell'immagine
*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define KERNEL Size( 5, 5 )
#define DEG2RAD CV_PI / 180
#define BLUE Scalar( 255 )

using namespace std;
using namespace cv;

/**
 * Hough Circles
 * 
 * @param image input single channel image
 * @param circles N-by-1 3-channel or vector Vec3f
 * @param method Always cv::HOUGH_GRADIENT
 * @param dp Accumulator resolution ( ratio )
 * @param minDist Required separation ( between lines )
 * @param param1 Upper Canny threshold
 * @param param2 Unnormalized accumulator threasold
 * @param minRadius Smallest radius to consider
 * @param maxRadius Largest radius to consider
*/
void myHoughCircles( Mat image, Mat& circles, int method, double dp, double minDist,
                double param1 = 100, double param2 = 100, int minRadius = 0, int maxRadius = 0 ) {
    
    image.copyTo( circles );
    GaussianBlur( image, image, KERNEL, 0 );
    cvtColor( image, image, COLOR_RGB2GRAY );

    // 1. Inizializzare l'accumulatore H - Tridimensionale
    const int sz[] = { image.rows, image.cols, maxRadius - minRadius + 1 };
    Mat votes = Mat( 3, sz, CV_8U, Scalar(0) );

    // 2. Applicare l'algoritmo di Canny per individuare i punti di edge
    Mat edges;
    Canny( image, edges, 30, 170 );
    imshow( "edges", edges );

    // 3. Per ogni punto (𝑥, 𝑦) di edge
    // 4. Per ogni angolo 𝜃 = 0: 360 e per ogni raggio r=Rmin : Rmax
    // 5. Calcolare 𝑎 = 𝑥 − 𝑟 ∗ cos( 𝜃 * π / 180 ) e 𝑏 = 𝑦 − 𝑟 ∗ sin( 𝜃 * π / 180 ) 
    // 6. Incrementare H(𝑎, 𝑏, 𝑟)=H(𝑎, 𝑏, 𝑟) + 1
    for ( int y = 0; y < edges.rows; y++ )
        for ( int x = 0; x < edges.cols; x++ )
            if ( edges.at<uchar>( y, x ) == 255 )
                for ( int r = minRadius; r <= maxRadius; r++ )
                    for ( int theta = 0; theta <= 360; theta++ ) {
                        int a = x - r * cos( theta * DEG2RAD );
                        int b = y - r * sin( theta * DEG2RAD );
                        if ( a >= 0 && a < edges.cols && b >= 0 && b < edges.rows )
                            votes.at<uchar>( b, a, r - minRadius )++;
                    }

    // 7. Le celle H 𝑎, 𝑏, 𝑟 con un valore maggiore di una soglia th
    // corrispondono alle circonferenze nell'immagine
    for ( int r = minRadius; r <= maxRadius; r++ )
        for ( int b = 0; b < image.rows; b++ )
            for ( int a = 0; a < image.cols; a++ )
                if ( votes.at<uchar>( b, a, r - minRadius ) > param2 ) {
                    circle( circles, Point( a, b ), 1, Scalar( 255, 0, 255 ), 3, LINE_AA );
                    circle( circles, Point( a, b ), r, Scalar( 255, 0, 255 ), 3, LINE_AA );
                }
}

int main( int argc, char* argv[] ) {
    
    if ( argc != 2 ) {
        cout << "uasage: " << argv[0] << " <image>" << endl;
        exit( EXIT_FAILURE );
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    Mat houghCircles, myHoughCirclesImg;

    src.copyTo( houghCircles );

    Mat gray;
    cvtColor( src, gray, COLOR_BGR2GRAY );
    medianBlur( gray, gray, 5 );
    vector<Vec3f> circles;

    HoughCircles( gray, circles, HOUGH_GRADIENT, 1,
                gray.rows / 16, // change this value to detect circles with different distances to each other
                170, 17, 1, 30  // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles 
                                );
    
    for( size_t i = 0; i < circles.size(); i++ ) {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle( houghCircles, center, 1, Scalar(255,0,255), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle( houghCircles, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }

    myHoughCircles( src, myHoughCirclesImg, HOUGH_GRADIENT, 0, 0, 0, 160, 1, 30 );

    imshow( "src", src );
    imshow( "houghCircles", houghCircles );
    imshow( "myHoughCirclesImg", myHoughCirclesImg );
    waitKey();
    return 0;
}