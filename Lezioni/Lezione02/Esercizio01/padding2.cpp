#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat image = imread("path_to_image", IMREAD_GRAYSCALE); // leggo l'immagine in scala di grigi
    Mat paddedImage;

    copyMakeBorder(image, paddedImage, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0)); // effettuo il padding

    //! [window]
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    Mat blurredImage = Mat::zeros(image.size(), CV_8UC1); // creo un'immagine vuota per la sfocatura

    int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}; // kernel 3x3 per la sfocatura
    int kernelSum = 0;
    for (int i = 0; i < 3; i++) // calcolo la somma degli elementi del kernel
    {
        for (int j = 0; j < 3; j++)
        {
            kernelSum += kernel[i][j];
        }
    }

    for (int i = 1; i < paddedImage.rows - 1; i++) // scorro l'immagine
    {
        for (int j = 1; j < paddedImage.cols - 1; j++)
        {
            int sum = 0;
            for (int k = -1; k <= 1; k++) // calcolo la somma degli elementi nella finestra 3x3
            {
                for (int l = -1; l <= 1; l++)
                {
                    sum += paddedImage.at<uchar>(i + k, j + l) * kernel[k + 1][l + 1];
                }
            }
            blurredImage.at<uchar>(i - 1, j - 1) = sum / kernelSum; // calcolo il valore sfocato dell'immagine
        }
    }
    imshow("Display window", image);
    waitKey(0);
    imshow("Display window", blurredImage);
    waitKey(0);

    return 0;
}
