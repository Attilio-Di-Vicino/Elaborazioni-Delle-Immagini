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
void houghCircles( InputArray image, OutputArray circles, int method, double dp, double minDist,
                double param1 = 100, double param2 = 100, int minRadius = 0, int maxRadius = 0 ) {

}