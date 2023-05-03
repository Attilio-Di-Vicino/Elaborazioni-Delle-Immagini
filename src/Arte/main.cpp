#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>

// g++ main.cpp $(pkg-config --cflags --libs opencv)
// ./a.out image.png

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    String imageName("HappyFish.jpg"); // by default
    if (argc > 1)
    {
        imageName = argv[1];
    }

    Mat image;
    image = imread(samples::findFile(imageName), IMREAD_COLOR); // Read the file

    // image = Mat::zeros( 500, 700, CV_32S );

    if (image.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", image);
    waitKey(0);

    int min_value = 0;
    int max_value = 255;

    int random_number = min_value + (std::rand() % (max_value - min_value + 1));

    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            image.at<Vec3b>(i, j) = i * j / 28 * 24;
        }

    imshow("Display window", image);
    waitKey(0);

    return 0;
}