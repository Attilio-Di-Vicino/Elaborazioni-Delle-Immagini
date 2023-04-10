#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

// g++ padding3.cpp -o padding3 $(pkg-config --cflags --libs opencv)
// ./padding3 giacinto2.jpg

Vec2b sum8( Mat , int, int );
void gaussianBlur(Mat& image, Mat& blurred_image, int kernel_size, double sigma);
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

    //! [window]
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    imshow( "Display window", image );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]


    /**
    * padding speculare 1px
    * per effettuare somma di 8px
    */

    cout << "Padding speculare 1px" << std::endl ;

    int top = 1;
    int bottom = 1;
    int left = 1;
    int right = 1;

    Mat paddedImage( image.rows + top + bottom, image.cols + left + right, image.type(), Scalar( 0, 0 ) );
    
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            paddedImage.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
        }
    }

    int index = 0;
    for ( int i = 0; i < top; i++  ) {
        for ( int j = 0; j < paddedImage.cols; j++ ) {
            if ( j < image.cols )
                index = j;
            else
                index = image.cols - 1;
            paddedImage.at<Vec2b>( i, j ) = image.at<Vec2b>( i, index );
        }
    }

    index = 0;
    for ( int i = 0; i < bottom; i++  ) {
        for ( int j = 0; j < paddedImage.cols; j++ ) {
            if ( j < image.cols )
                index = j;
            else
                index = image.cols - 1;
            paddedImage.at<Vec2b>( paddedImage.rows - i - 2, j ) = image.at<Vec2b>( image.rows - i - 2, index ); // da capire il - 2
        }
    }

    index = 0;
    for ( int j = 0; j < left; j++  ) {
        for ( int i = 0; i < paddedImage.rows; i++ ) {
            if ( i < image.rows )
                index = i;
            else 
                index = image.rows - 1;
            paddedImage.at<Vec2b>( i, j ) = image.at<Vec2b>( index, j );
        }
    }

    index = 0;
    for ( int j = 0; j < right; j++  ) {
        for ( int i = 0; i < paddedImage.rows; i++ ) {
            if ( i < image.rows )
                index = i;
            else 
                index = image.rows - 10; // l' ultimo pixel è nero quindi prendo l'ultimo pixel - 10
            paddedImage.at<Vec2b>( i, paddedImage.cols - j - 1 ) = image.at<Vec2b>( index, image.cols - j - 1 );
        }
    }

    //! [imshow]
    imshow( "Display window", paddedImage );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]

    cout << "somma di 8" << std::endl;

    Mat blurredImage = Mat::zeros( image.size(), CV_8UC1 ); // creo un'immagine vuota per la sfocatura

    for ( int i = 0; i < blurredImage.rows - 1; i++ ) {
        for ( int j = 0; j < blurredImage.cols - 1; j++ ) {
            // blurredImage.at<Vec2b>( i, j ) = Vec2b( 200, 200 );
            blurredImage.at<Vec2b>( i, j ) = sum8( paddedImage, i + 1, j + 1 );
        }
    }

    imshow( "Display window", blurredImage );
    waitKey(0);
}

Vec2b sum8( Mat image, int i, int j ) {
    Vec2b result = Vec2b( 0, 0 );

    int sum0 =  image.at<Vec2b>( i - 0, j - 0 )[0] +
                image.at<Vec2b>( i - 1, j - 1 )[0] +
                image.at<Vec2b>( i - 1, j - 0 )[0] +
                image.at<Vec2b>( i - 1, j + 1 )[0] +
                image.at<Vec2b>( i - 0, j - 1 )[0] +
                image.at<Vec2b>( i - 0, j + 1 )[0] +
                image.at<Vec2b>( i + 1, j - 1 )[0] +
                image.at<Vec2b>( i + 1, j - 0 )[0] +
                image.at<Vec2b>( i + 1, j + 1 )[0];
    
    int sum1 =  image.at<Vec2b>( i - 0, j - 0 )[1] +
                image.at<Vec2b>( i - 1, j - 1 )[1] +
                image.at<Vec2b>( i - 1, j - 0 )[1] +
                image.at<Vec2b>( i - 1, j + 1 )[1] +
                image.at<Vec2b>( i - 0, j - 1 )[1] +
                image.at<Vec2b>( i - 0, j + 1 )[1] +
                image.at<Vec2b>( i + 1, j - 1 )[1] +
                image.at<Vec2b>( i + 1, j - 0 )[1] +
                image.at<Vec2b>( i + 1, j + 1 )[1];

    if ( sum0 > 255 )
        sum0 = (sum0 % 255) + 1;
    if ( sum1 > 255 )
        sum1 = (sum1 % 255) + 1;

    return Vec2b( sum0, sum1 );
}

void gaussianBlur(Mat& image, Mat& blurred_image, int kernel_size, double sigma)
{
    Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32FC1);
    float sum = 0.0;

    int center = kernel_size / 2;

    // crea il kernel gaussiano
    for (int i = 0; i < kernel_size; i++)
    {
        for (int j = 0; j < kernel_size; j++)
        {
            float x = i - center;
            float y = j - center;
            kernel.at<float>(i, j) = exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel.at<float>(i, j);
        }
    }

    // normalizza il kernel
    kernel /= sum;

    // applica la convoluzione all'immagine di input
    blurred_image = Mat::zeros(image.rows, image.cols, image.type());

    for (int y = center; y < image.rows - center; y++)
    {
        for (int x = center; x < image.cols - center; x++)
        {
            Vec3f sum = Vec3f(0, 0, 0);

            for (int i = -center; i <= center; i++)
            {
                for (int j = -center; j <= center; j++)
                {
                    int pixel_x = x + j;
                    int pixel_y = y + i;

                    Vec3f pixel = image.at<Vec3b>(pixel_y, pixel_x);
                    float weight = kernel.at<float>(i + center, j + center);

                    sum += pixel * weight;
                }
            }

            Vec3b blurred_pixel = sum;
            blurred_image.at<Vec3b>(y, x) = blurred_pixel;
        }
    }
}

