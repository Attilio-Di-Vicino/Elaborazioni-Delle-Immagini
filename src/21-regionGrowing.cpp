/**
 * SEGMENTAZIONE: REGION GROWING
 * 
 * 1. Formare l'immagine fQ che nel punto (x,y) contiene il valore 1 se Q(f(x,y))
 *    è vero altrimenti contiene il valore 0
 * 
 * 2. Aggiungere ad ogni seed i pixel impostati ad 1 in fQ che risultano [4,8] connessi al seed stesso
 * 
 * 3. Marcare ogni componente connessa con un'etichetta diversa
*/

#include <iostream>
#include <stack>
#include <opencv2/opencv.hpp>

using std::cout;
using std::endl;
using std::stack;
using namespace cv;

// parameters
const int thresh = 200;
const uchar maxRegionNum = 100;
const double minRegionAreaFactor = 0.01;
const cv::Point PointShift2D[8] = {
    cv::Point(  1,  0 ),
    cv::Point(  1, -1 ),
    cv::Point(  0, -1 ),
    cv::Point( -1, -1 ),
    cv::Point( -1,  0 ),
    cv::Point( -1,  1 ),
    cv::Point(  0,  1 ),
    cv::Point(  1,  1 )
};

void regionGrowing( const cv::Mat src, cv::Mat& dest ) {

    dest = Mat::zeros( src.rows, src.cols, CV_8UC1 );
    Mat mask = Mat::zeros( src.rows, src.cols, CV_8UC1 );
    
    int minRegionArea = int( src.rows * src.cols * minRegionAreaFactor ); // ( minRegionAreaFactor * 100 )
    uchar padding = 1;  // use which number to pad in "dest"

    for ( auto y = 0; y < src.rows; y++ )
        for ( auto x = 0; x < src.cols; x++ )
            if ( dest.at<uchar>( Point( x, y ) ) == 0 ) {
                grow( src, dest, mask, Point( x, y ) );

                if ( sum( mask ).val[0] > minRegionArea ) {
                    imshow( "region", mask * 255 );
                    waitKey();
                    
                    dest = dest + mask * padding;
                    
                    if ( ++padding > maxRegionNum ) {
                        cout << "run out of max_region_num..." << endl;
                        exit( EXIT_FAILURE );
                    }
                } else
                    dest = dest + mask * 255;

                mask = mask - mask;
            }
}

void grow( const Mat& src, const Mat& dest, Mat& mask, Point seed ) {
    
    stack<Point> front;
    front.push( seed );

    while( !front.empty() ) {

        Point center = front.top(); // Head
        mask.at<uchar>( center ) = 1; // seme visitato
        front.pop();

        for( auto i = 0; i < 8; i++ ) { // 8 intorno
            
            Point neigh = center + PointShift2D[i]; // Mi basta un addizione per l'intorno in questione
            if ( neigh.x < 0 || neigh.x >= src.cols || neigh.y < 0 || neigh.y >= src.rows )
                continue;
            else {
                // Viene calcolata la differenza dei quadrati tra i valori dei canali ( Predicato )
                int delta = cvRound( pow( src.at<Vec3b>( center )[0] - src.at<Vec3b>( neigh )[0], 2 ) +
                                     pow( src.at<Vec3b>( center )[1] - src.at<Vec3b>( neigh )[1], 2 ) +
                                     pow( src.at<Vec3b>( center )[2] - src.at<Vec3b>( neigh )[2], 2 ) );

                if ( delta < thresh && !dest.at<uchar>( neigh ) && !mask.at<uchar>( neigh ) )
                    front.push( neigh );
            }  
        }
    }
}

int main( int argc, char** argv ) {

    if ( argc != 2 ) {
        cout << "\nusage: " << argv[0] << " <name_image> " << endl;
        exit( EXIT_FAILURE );
    }
    
    // 1. read source image
    cv::Mat src = cv::imread( argv[1], cv::IMREAD_COLOR );

    cv::Mat myRegionGrowing;
    regionGrowing( src, myRegionGrowing );

    cv::namedWindow( "src", cv::WINDOW_AUTOSIZE );
    cv::imshow( "src", src );

    cv::namedWindow( "myRegionGrowing", cv::WINDOW_AUTOSIZE );
    cv::imshow( "myRegionGrowing", myRegionGrowing );
    cv::waitKey();
    return 0;
}