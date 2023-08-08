#ifndef MYPADDING_H
#define MYPADDING_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

enum class MyBorderType {
    NO_BORDER, BORDER_CONSTANT, BORDER_WRAP, BORDER_REPLICATE, BORDER_REFLECT, BORDER_REFLECT_101
};

template <typename T> class MyPadding {
    private:
        static void noBorder(           Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );

        static void borderConstant(     Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );

        static void borderWrap(         
                                        Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );

        static void borderReplicate(    Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );

        static void borderReflect(      Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );

        static void borderReflect101(   Mat src, Mat& dst, 
                                        int top, int bottom, int left, int right, 
                                        const Scalar value );
    public:
        static void padding( Mat src, Mat& dst, int top, int bottom, int left, int right, MyBorderType borderType, const Scalar value );
        static void padding( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value );
        static void padding( Mat src, Mat& dst, int top, int bottom, int left, int right, MyBorderType borderType );
        static void padding( Mat src, Mat& dst, int top, int bottom, int left, int right );

        static void padding( Mat src, Mat& dst, int top, int right, MyBorderType borderType, const Scalar value );
        static void padding( Mat src, Mat& dst, int top, int right, const Scalar value );
        static void padding( Mat src, Mat& dst, int top, int right, MyBorderType borderType );
        static void padding( Mat src, Mat& dst, int top, int right );
        
        static void padding( Mat src, Mat& dst, int borderSize, MyBorderType borderType, const Scalar value );
        static void padding( Mat src, Mat& dst, int borderSize, const Scalar value );
        static void padding( Mat src, Mat& dst, int borderSize, MyBorderType borderType );
        static void padding( Mat src, Mat& dst, int borderSize );
        
        static void autoPadding( Mat src, Mat& dst, int ksize, MyBorderType borderType );
        static void autoPadding( Mat src, Mat& dst, int xPad, int yPad, MyBorderType borderType );
};

template <typename T> void MyPadding<T>::autoPadding( Mat src, Mat& dst, int ksize, MyBorderType borderType ) {
    int size = floor( ksize / 2 );
    padding( src, dst, size, size, size, size, borderType, Scalar( 0, 0 ) );
}

template <typename T> void MyPadding<T>::autoPadding( Mat src, Mat& dst, int xPad, int yPad, MyBorderType borderType ) {
    int xSize = floor( xPad / 2 );
    int ySize = floor( yPad / 2 );
    padding( src, dst, xSize, xSize, ySize, ySize, borderType, Scalar( 0, 0 ) );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int bottom, int left, int right, MyBorderType borderType, const Scalar value ) {
    switch ( borderType )
    {
        case MyBorderType::NO_BORDER: noBorder( src, dst, top, bottom, left, right, value );
            break;
        case MyBorderType::BORDER_CONSTANT: borderConstant( src, dst, top, bottom, left, right, value );
            break;
        case MyBorderType::BORDER_WRAP: borderWrap( src, dst, top, bottom, left, right, value );
            break;
        case MyBorderType::BORDER_REPLICATE: borderReplicate( src, dst, top, bottom, left, right, value );
            break;
        case MyBorderType::BORDER_REFLECT: borderReflect( src, dst, top, bottom, left, right, value );
            break;
        case MyBorderType::BORDER_REFLECT_101: borderReflect101( src, dst, top, bottom, left, right, value );
            break;
        default: borderWrap( src, dst, top, bottom, left, right, value );
            break;
    }
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    padding( src, dst, top, bottom, left, right, MyBorderType::BORDER_REFLECT_101, value );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int bottom, int left, int right, MyBorderType borderType ) {
    padding( src, dst, top, bottom, left, right, borderType, Scalar( 0, 0 ) );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int bottom, int left, int right ) {
    padding( src, dst, top, bottom, left, right, MyBorderType::BORDER_REFLECT_101, Scalar( 0, 0 ) );
}

/****************************************************/
template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int right, MyBorderType borderType, const Scalar value ) {
    padding( src, dst, top, top, right, right, borderType, value );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int right, const Scalar value ) {
    padding( src, dst, top, top, right, right, MyBorderType::BORDER_REFLECT_101, value );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int right, MyBorderType borderType ) {
    padding( src, dst, top, top, right, right, borderType, Scalar( 0, 0 ) );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int top, int right ) {
    padding( src, dst, top, top, right, right, MyBorderType::BORDER_REFLECT_101, Scalar( 0, 0 ) );
}

/****************************************************/
template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int borderSize, MyBorderType borderType, const Scalar value ) {
    padding( src, dst, borderSize, borderSize, borderSize, borderSize, borderType, value );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int borderSize, const Scalar value ) {
    padding( src, dst, borderSize, borderSize, borderSize, borderSize, MyBorderType::BORDER_REFLECT_101, value );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int borderSize, MyBorderType borderType ) {
    padding( src, dst, borderSize, borderSize, borderSize, borderSize, borderType, Scalar( 0, 0 ) );
}

template <typename T> void MyPadding<T>::padding( Mat src, Mat& dst, int borderSize ) {
    padding( src, dst, borderSize, borderSize, borderSize, borderSize, MyBorderType::BORDER_REFLECT_101, Scalar( 0, 0 ) );
}

/****************************************************/
template <typename T> void MyPadding<T>::noBorder( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    // DA IMPLEMENTARE
}

/**
 * Il padding constant semplicemente si limita a copiare l'immagine originale
 * in un'immagine dst la quale avrà un numero indefinito in più di pixel
 * posizionati rispettivamente top, bottom, right, left
 * il colore di questi pixel aggiuntivi sarà di default è impostato a nero ( 0 )
 * ma può anche essere customizatto passandoglielo in input
*/
template <typename T> void MyPadding<T>::borderConstant( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    dst.create( src.rows + top + bottom, src.cols + left + right, src.type() );
    dst.setTo( value );
    src.copyTo( dst( Rect( bottom, right, src.cols, src.rows ) ) );
}

/**
 * Il padding border wrap in sostanza vuole simulare una replica infinita dell'immagine
 * come se alla fine dell'immagine riparta di nuovo la rappresentazione della stessa immagine
 * quindi di conseguenza avremo sempre un numero indefinito di pixel aggiuntivi
 * ed avremo poi la replica dell'immagine
 * es. il top sarà uguale al bottom dell'immagine originale e così via 
 * 
 * @param src immagine al cui applicare il padding
 * @param dst immagine col padding
 * @param top padding superiore
 * @param bottom padding inferiore
 * @param left padding a sinistra
 * @param right padding a destra
 * @param value costante per il padding di default 0
*/
template <typename T> void MyPadding<T>::borderWrap( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    borderConstant( src, dst, top, bottom, left, right, value );

    for ( int i = 0; i < src.rows; i++ ) {
        for ( int j = 0; j < left; j++ ) {
            dst.at<T>( i + top, j ) = src.at<T>( i, src.cols - left + j );
        }
    }

    for ( int i = 0; i < src.rows; i++ ) {
        for ( int j = 0; j < right; j++ ) {
            dst.at<T>( i + top, dst.cols - right + j ) = src.at<T>( i , j );
        }
    }

    // per top e bottom lavoro direttamente sull'immagine dst essendo gia stata modificata
    for ( int i = 0; i < top; i++ ) {
        for ( int j = 0; j < dst.cols; j++ ) {
            dst.at<T>( i, j ) = dst.at<T>( dst.rows - bottom - top + i, j );
        }
    }

    for ( int i = 0; i < bottom; i++ ) {
        for ( int j = 0; j < dst.cols; j++ ) {
            dst.at<T>( dst.rows - bottom + i, j ) = dst.at<T>( i + top, j );
        }
    }
}

template <typename T> void MyPadding<T>::borderReplicate( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    // DA IMPLEMENTARE
}

template <typename T> void MyPadding<T>::borderReflect( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    // DA IMPLEMENTARE
}

template <typename T> void MyPadding<T>::borderReflect101( Mat src, Mat& dst, int top, int bottom, int left, int right, const Scalar value ) {
    // DA IMPLEMENTARE
}

#endif // MYPADDING_H