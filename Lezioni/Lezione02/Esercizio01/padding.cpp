/**
 * Realizzare una funzione che effettui il padding di un'immagine
 * 
 * Sostituire al valore di ogni pixel il valore medio dei livelli di grigio in un intorno 3x3
*/
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// g++ padding.cpp -o padding $(pkg-config --cflags --libs opencv)
// ./padding opencv-logo.png
// ./padding giacinto.jpg

#include <iostream>
//! [includes]

//! [namespace]
using namespace cv;
using namespace std;
//! [namespace]

Vec2b sum8( Mat image, int i, int j ) {
    Vec2b sum = 0;
    sum +=  image.at<Vec2b>( i - 0, j - 0 ) +
            image.at<Vec2b>( i - 1, j - 1 ) +
            image.at<Vec2b>( i - 1, j - 0 ) +
            image.at<Vec2b>( i - 1, j + 1 ) +
            image.at<Vec2b>( i - 0, j - 1 ) +
            image.at<Vec2b>( i - 0, j + 1 ) +
            image.at<Vec2b>( i + 1, j - 1 ) +
            image.at<Vec2b>( i + 1, j - 0 ) +
            image.at<Vec2b>( i + 1, j + 1 );
    
    // cout << " Sum[" << i << "]: " << sum << endl;
    //Vec2b result = Vec2b( sum[0]%255, sum[1]%255 );
    //return result;
    // return sum;
    return sum;
}

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

    //Utility
    Mat imageC;

    /**
     * 255 padding bianco
     * La funzione "create" della classe "Mat" di OpenCV viene utilizzata per creare la nuova immagine "imageC".
     * I parametri passati alla funzione sono le dimensioni dell'immagine, che sono le dimensioni dell'immagine
     * originale aumentate di due pixel su ogni lato e il tipo di immagine, che viene copiato dall'immagine originale.
     * 
     * Successivamente, la funzione "setTo" viene utilizzata per impostare tutti i pixel dell'immagine "imageC"
     * sul valore di colore bianco (255,255,255).
     * 
     * Infine, viene utilizzato un doppio ciclo "for" per copiare i pixel dell'immagine "image" nell'immagine "imageC"
     * spostando ciascun pixel dell'immagine originale di una posizione verso destra e verso il basso, in modo da centrare
     * l'immagine originale all'interno dell'immagine più grande. Il valore del pixel dell'immagine originale viene quindi
     * copiato nella posizione corrispondente nell'immagine "imageC".
    */
    int top = 1;
    int bottom = 1;
    int left = 1;
    int right = 1;

    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    imageC.setTo( cv::Scalar( 255, 255 ) );
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
        }
    }

    cout << "padding bianco 1px" << endl;
    imshow( "Display window", imageC);
    waitKey(0);

    /**
    * padding nero 50px
    */

    top = 50;
    bottom = 50;
    left = 50;
    right = 50;
    
    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    // imageC.setTo( cv::Scalar( 0, 0 ) );
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
        }
    }

    for ( int i = 0; i < top; i++  ) {
        for ( int j = 0; j < imageC.cols; j++ ) {
            imageC.at<Vec2b>( i, j ) = Vec2b( 0, 0);
            // imageC.at<Vec2b>( imageC.rows - i - 1, j ) = Vec2b( 0, 0 );
        }
    }


    for ( int j = 0; j < left; j++  ) {
        for ( int i = 0; i < imageC.rows; i++ ) {
            imageC.at<Vec2b>( i, j ) = Vec2b( 0, 0 );
            imageC.at<Vec2b>( i, imageC.cols - j - 1 ) = Vec2b( 0, 0 );
        }
    }

    cout << "padding nero 50px" << endl;
    imshow( "Display window", imageC);
    waitKey(0);

    /**
    * padding speculare 50px
    */

    top = 50;
    bottom = 50;
    left = 50;
    right = 50;
    
    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
        }

    }

    int index = 0;
    for ( int i = 0; i < top; i++  ) {
        for ( int j = 0; j < imageC.cols; j++ ) {
            if ( j < image.cols )
                index = j;
            else
                index = image.cols - 1;
            imageC.at<Vec2b>(i, j) = image.at<Vec2b>(i, index);
            //imageC.at<Vec2b>(imageC.rows - i - 1, j) = image.at<Vec2b>(image.rows - i - 1, index);
        }
    }

    index = 0;
    for ( int j = 0; j < left; j++  ) {
        for ( int i = 0; i < imageC.rows; i++ ) {
            if ( i < image.rows )
                index = i;
            else 
                index = image.rows - 1;
            imageC.at<Vec2b>( i, j ) = image.at<Vec2b>( index, j );
            imageC.at<Vec2b>( i, imageC.cols - j - 1 ) = image.at<Vec2b>( index, image.cols - j - 1 );
        }
    }

    cout << "padding speculare 50px" << endl;
    imshow( "Display window", imageC);
    waitKey(0);


    /**
    * padding speculare 1px
    * per effettuare somma di 8px
    */

    top = 1;
    bottom = 1;
    left = 1;
    right = 1;
    
    imageC.create( image.rows + top + bottom, image.cols + left + right, image.type() );
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            imageC.at<cv::Vec2b>( i + bottom, j + right ) = image.at<cv::Vec2b>( i, j );
        }

    }

    index = 0;
    for ( int i = 0; i < top; i++  ) {
        for ( int j = 0; j < imageC.cols; j++ ) {
            if ( j < image.cols )
                index = j;
            else
                index = image.cols - 1;
            imageC.at<Vec2b>(i, j) = image.at<Vec2b>(i, index);
            //imageC.at<Vec2b>(imageC.rows - i - 1, j) = image.at<Vec2b>(image.rows - i - 1, index);
        }
    }

    index = 0;
    for ( int j = 0; j < left; j++  ) {
        for ( int i = 0; i < imageC.rows; i++ ) {
            if ( i < image.rows )
                index = i;
            else 
                index = image.rows - 1;
            imageC.at<Vec2b>( i, j ) = image.at<Vec2b>( index, j );
            imageC.at<Vec2b>( i, imageC.cols - j - 1 ) = image.at<Vec2b>( index, image.cols - j - 1 );
        }
    }

    cout << "padding speculare 1px per effettuare somma di 8" << endl;
    imshow( "Display window", imageC);
    waitKey(0);
    
    Mat imageResult;
    imageResult.create( image.rows, image.cols, image.type() );
    //imageResult.setTo( cv::Scalar( 0, 0 ) );

    cout << "Image Result before" << endl;
    imshow( "Display window", imageResult);
    waitKey(0);

    for ( int i = 0; i < imageResult.rows - 1; i++ ) {
        for ( int j = 0; j < imageResult.cols - 1; j++ ) {
            // imageResult.at<Vec2b>( i, j ) = Vec2b( 255, 255 );
            imageResult.at<Vec2b>( i, j ) = sum8( imageC, i+1, j+1 );
        }
    }

    cout << "somma di 8" << endl;
    imshow( "Display window", imageResult);
    waitKey(0);

    return 0;
}