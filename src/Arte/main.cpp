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

    
    // Crea un'immagine vuota di dimensioni 300x300 pixel
    cv::Mat imageC(300, 300, CV_8UC3, cv::Scalar(255, 255, 255));

    // for ( int i = 0; i < imageC.rows; i++ ) {
    //     for ( int j = 0; j < imageC.cols; j++ ) {
    //         if ( i >= 75 &&  ) {
                
    //         }
    //     }
    // }

    // Disegna il contorno del cuore
    cv::line(imageC, cv::Point(150, 50), cv::Point(50, 150), cv::Scalar(0, 0, 255), 2);
    cv::line(imageC, cv::Point(50, 150), cv::Point(150, 250), cv::Scalar(0, 0, 255), 2);
    cv::line(imageC, cv::Point(150, 250), cv::Point(250, 150), cv::Scalar(0, 0, 255), 2);
    cv::line(imageC, cv::Point(250, 150), cv::Point(150, 50), cv::Scalar(0, 0, 255), 2);

    // // Riempie il cuore con pixel rossi
    // cv::Rect heart_rect(50, 50, 200, 200);
    // cv::Mat heart_roi = imageC(heart_rect);
    // heart_roi.setTo(cv::Scalar(0, 0, 255));

    // Visualizza l'immagine finale
    cv::imshow("Cuore Rosso Pixelato", imageC);
    cv::waitKey(0);

     // Crea un'immagine vuota di dimensioni 300x300 pixel
    cv::Mat imageF(300, 300, CV_8UC3, cv::Scalar(255, 255, 255));

    // Disegna il contorno del fiore
    cv::line(imageF, cv::Point(150, 50), cv::Point(50, 150), cv::Scalar(0, 0, 255), 2);
    cv::line(imageF, cv::Point(50, 150), cv::Point(150, 250), cv::Scalar(0, 0, 255), 2);
    cv::line(imageF, cv::Point(150, 250), cv::Point(250, 150), cv::Scalar(0, 0, 255), 2);
    cv::line(imageF, cv::Point(250, 150), cv::Point(150, 50), cv::Scalar(0, 0, 255), 2);

    cv::line(imageF, cv::Point(150, 75), cv::Point(75, 150), cv::Scalar(0, 255, 0), 2);
    cv::line(imageF, cv::Point(75, 150), cv::Point(150, 225), cv::Scalar(0, 255, 0), 2);
    cv::line(imageF, cv::Point(150, 225), cv::Point(225, 150), cv::Scalar(0, 255, 0), 2);
    cv::line(imageF, cv::Point(225, 150), cv::Point(150, 75), cv::Scalar(0, 255, 0), 2);

    // Riempie il centro del fiore con pixel gialli
    cv::Rect flower_rect(100, 100, 100, 100);
    cv::Mat flower_roi = imageF(flower_rect);
    flower_roi.setTo(cv::Scalar(0, 255, 255));

    // Visualizza l'immagine finale
    cv::imshow("Fiore Pixelato", imageF);
    cv::waitKey(0);

    // // open the first webcam plugged in the computer
    // cv::VideoCapture camera(0);
    // if (!camera.isOpened()) {
    //     std::cerr << "ERROR: Could not open camera" << std::endl;
    //     return 1;
    // }
    // // create a window to display the images from the webcam
    // cv::namedWindow("Webcam", CV_WINDOW_AUTOSIZE);
    // // this will contain the image from the webcam
    // cv::Mat frame;
    // // capture the next frame from the webcam
    // camera >> frame;
    // // display the frame until you press a key
    // while (1) {
    //     // show the image on the window
    //     cv::imshow("Webcam", frame);
    //     // wait (10ms) for a key to be pressed
    //     if (cv::waitKey(10) >= 0)
    //     break;
    // }

    std::stringstream file; // to write the file name
    cv::VideoCapture cap(0); // create a capture object
    int counter = 0; // Create counter

    cv::Mat frame; // Create a object
    cap.read(frame); // read the frame
    file << "/home/attilio/Scrivania/Elaborazioni-Delle-Immagini/src/Arte" << counter << ".jpg"; // file name
    cv::imwrite(file.str(), frame);
    counter++; // increment the counter

    return 0;
}