#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using namespace cv;

const int k = 6; // cluster
const double th = 0.05;

/**
 * Calcola in maniera randomatica i centrodi
 * 
 * @param img immagine
 * @param center centroidi
 * @param cluster gruppi
*/
void computeRandomCenter( const Mat& img, vector<Scalar>& center, vector<vector<Point>>& cluster ) {
    RNG randomNumberGenerator( getTickCount() );
    for ( auto label = 0; label < k; label++ ) {
        Point px;
        px.y = randomNumberGenerator.uniform( 0, img.rows );
        px.x = randomNumberGenerator.uniform( 0, img.cols );
        center.push_back( img.at<Vec3b>( px ) );
        cluster.push_back( vector<Point>() );
    }
}

/**
 * Calcola la distanza di un immagine a tre canali
 * 
 * @param px pixel ispezionato
 * @param center centroide di turno
 * @return distanza
*/
double computeColorDistance( const Scalar px, const Scalar center ) {
    double blueDiff  = abs( px[0] - center[0] );
    double greenDiff = abs( px[1] - center[1] );
    double redDiff   = abs( px[2] - center[2] );
    return blueDiff + greenDiff + redDiff;
}

/**
 * Popolamento cluster
 * 
 * @param img immagine
 * @param center centroidi
 * @param cluster gruppi
*/
void populateCluster( const Mat& img,vector<Scalar> center,vector<vector<Point>>& cluster ) {
    for ( auto y = 0; y < img.rows; y++ )
        for ( auto x = 0; x < img.cols; x++ ) {
            double dist = INFINITY;
            int labelID = 0;
            Scalar pixelVal = img.at<Vec3b>(y,x);
            for ( auto label = 0; label < k; label++ ) {
                double pxDist = computeColorDistance( pixelVal, center.at(label) );
                if ( pxDist < dist ) {
                    dist = pxDist;
                    labelID = label;
                }
            }
            cluster.at( labelID ).push_back( Point(x,y) );
        }
}

/**
 * Ricalcola i centroidi
 * 
 * @param img immagine
 * @param center centroidi
 * @param cluster gruppi
 * @param oldVal vecchio valore
 * @param newVal nuovo valore
 * @return distanza
*/
double adjustCenter( const Mat& img, vector<Scalar>& center, vector<vector<Point>> cluster, 
                    double& oldVal, double newVal ) {

    for ( auto label = 0; label < k; label++ ) {
        vector<Point> clusterIGroup = cluster.at(label);
        double newBlue = 0.0f;
        double newGreen = 0.0f;
        double newRed = 0.0f;
        for( size_t pixel = 0; pixel < clusterIGroup.size(); pixel++ ) {
            Scalar pixelVal = img.at<Vec3b>( clusterIGroup.at(pixel) );
            newBlue  += pixelVal[0];
            newGreen += pixelVal[1];
            newRed   += pixelVal[2];
        }
        newBlue /= clusterIGroup.size();
        newGreen /= clusterIGroup.size();
        newRed /= clusterIGroup.size();
        Scalar newCenter = Scalar( cvRound(newBlue), cvRound(newGreen), cvRound(newRed) );
        newVal += computeColorDistance( newCenter, center.at(label) );
        center.at(label) = newCenter;
    }
    newVal /= k;
    double diffChange = abs( newVal - oldVal );
    oldVal = newVal;
    return diffChange;
}

/**
 * Segmenta l'immagine in base ai centroidi
 * 
 * @param img immagine
 * @param center centroidi
 * @param cluster gruppi
*/
void segment(Mat& out, const vector<Scalar>& center, const vector<vector<Point>>& cluster ){
    for (auto label = 0; label < k; label++ )
        for ( auto point: cluster.at(label) )
            for ( auto i=0; i<3; i++ )
                out.at<Vec3b>( point )[i] = center.at(label)[i];
}

void myKmeans( const Mat src, Mat& dest ) {
    dest = Mat::zeros( src.size(), CV_8UC3 );
    vector<Scalar> center;
    vector<vector<Point>> cluster;
    computeRandomCenter( src, center, cluster );
    double newVal = 0.0f;
    double oldVal = INFINITY;
    double dist = abs( oldVal - newVal );

    while ( dist > th ) {
        
        newVal = 0.0f;
        for( auto label = 0; label < k; label++ )
            cluster.at( label ).clear();

        populateCluster( src, center, cluster );
        dist = adjustCenter( src, center, cluster, oldVal, newVal );
    }
    segment( dest, center, cluster );
}

int main( int argc, char const* argv[] ) {
    if ( argc < 2 ) {
        cout << "usage: " << argv[0] << " image_name" << endl;
        exit(0);
    }

    Mat src = imread( argv[1], IMREAD_COLOR );
    Mat dest;

    imshow( "src", src );
    myKmeans( src, dest );
    imshow( "dest", dest );
    waitKey();

    return 0;
}