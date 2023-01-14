#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
// #include "filters.h"
#include <opencv2/imgcodecs.hpp>


using namespace std;
using namespace cv;


// Negative image
int Neg( cv::Mat &src, cv::Mat &dst ){
    int c,i,j;
    for(i=0;i<src.rows;i++){
        for(j=0;j<src.cols;j++){
            for(c=0;c<3;c++) {
                dst.at<Vec3b>(i,j)[c]= (255 - src.at<Vec3b>(i,j)[c]);
            }
        }
    } 
    return(0);
}

// Alternative gray scale live video

int greyscale( Mat &src, Mat &dst ){

    int c,i,j;
    for(i=0;i<src.rows;i++){
        for(j=0;j<src.cols;j++){
                dst.at<uchar>(i,j)= src.at<Vec3b>(i,j)[0]*0.114+  // 0.299⋅R+0.587⋅G+0.114⋅B
                                       src.at<Vec3b>(i,j)[1]*0.587+
                                       src.at<Vec3b>(i,j)[2]*0.299;
        }
    }
    return (0);
}


// // a 5x5 Gaussian filter as separable 1x5 filters

int blur5x5( Mat &src, Mat &dst ){
    
    Mat local;
    int c,i,j;
    src.copyTo(local);

    // [1 2 4 2 1] horizontal, divide the result by 10

    for(i=2;i<src.rows-2;i++){
            for(j=2;j<src.cols-2;j++){
                for(c=0;c<3;c++) {
                    local.at<Vec3b>(i,j)[c]=src.at<Vec3b>(i,j-2)[c]*0.1+ 
                                            src.at<Vec3b>(i,j-1)[c]*0.2+
                                            src.at<Vec3b>(i,j)[c]*0.4+
                                            src.at<Vec3b>(i,j+1)[c]*0.2+
                                            src.at<Vec3b>(i,j+2)[c]*0.1;
                }                           
            }  
        }

                
    // [1 2 4 2 1] vertical,divide the result by 10

    for(i=2;i<src.rows-2;i++){
        for(j=2;j<src.cols-2;j++){
            for(c=0;c<3;c++) {
                dst.at<Vec3b>(i,j)[c]=local.at<Vec3b>(i-2,j)[c]*0.1+ 
                                        local.at<Vec3b>(i-1,j)[c]*0.2+
                                        local.at<Vec3b>(i,j)[c]*0.4+
                                        local.at<Vec3b>(i+1,j)[c]*0.2+
                                        local.at<Vec3b>(i+2,j)[c]*0.1;
            }                           
        }  
    }

    return (0);
}

// // a 5x5 laws filter as separable 1x5 filters

int laws5x5( Mat &src, Mat &dst ){
    
    Mat local;
    int c,i,j;
    src.copyTo(local);

    // [1 4 6 4 1] L5, divide the result by 16

    for(i=2;i<src.rows-2;i++){
        for(j=2;j<src.cols-2;j++){
            for(c=0;c<3;c++) {
                local.at<Vec3b>(i,j)[c]=src.at<Vec3b>(i-2,j)[c]*0.0625+ 
                                            src.at<Vec3b>(i-1,j)[c]*0.25+
                                            src.at<Vec3b>(i,j)[c]*0.375+
                                            src.at<Vec3b>(i+1,j)[c]*0.25+
                                            src.at<Vec3b>(i+2,j)[c]*0.0625;
            }                           
        }  
    }

                
    // [-1 -2 0 2 1] E5 ,divide the result by 1

    for(i=2;i<src.rows-2;i++){
        for(j=2;j<src.cols-2;j++){
            for(c=0;c<3;c++) {
                dst.at<Vec3s>(i,j)[c]=local.at<Vec3b>(i,j-2)[c]*(-1)+ 
                                        local.at<Vec3b>(i,j-1)[c]*(-2)+
                                        local.at<Vec3b>(i,j)[c]*0+
                                        local.at<Vec3b>(i,j+1)[c]*2+
                                        local.at<Vec3b>(i,j+2)[c]*1;
            }                           
        }  
    }
    convertScaleAbs(dst,dst);
    

    return (0);
}


// // a 3x3 Sobel X and 3x3 Sobel Y filter as separable 1x3 filters

// // sobelx

int sobelX3x3( Mat &src, Mat &dst ){
    Mat local(src.rows, src.cols, CV_16SC3);
    int c,i,j;
    // src.copyTo(local);

    
    // [-1 0 1] horizontal, divide the result by 1

    for(i=1;i<src.rows-1;i++){
            for(j=1;j<src.cols-1;j++){
                for(c=0;c<3;c++) {
                    local.at<Vec3s>(i,j)[c]=src.at<Vec3b>(i,j-1)[c]*(-1)+
                                            src.at<Vec3b>(i,j)[c]*0+
                                            src.at<Vec3b>(i,j+1)[c]*1;
                }                           
            }  
        }

     
    // [1 2 1] vertical, divide the result by 4

    for(i=1;i<src.rows-1;i++){
            for(j=1;j<src.cols-1;j++){
                for(c=0;c<3;c++) {
                    dst.at<Vec3s>(i,j)[c]=local.at<Vec3s>(i-1,j)[c]*0.25+
                                          local.at<Vec3s>(i,j)[c]*0.50+
                                          local.at<Vec3s>(i+1,j)[c]*0.25;
                }                           
            }  
        }
    convertScaleAbs(dst,dst);
    

    return (0);
}

// sobely

int sobelY3x3( Mat &src, Mat &dst ){
    Mat local;
    int c,i,j;
    src.copyTo(local);

    // [1 2 1] horizontal, divide the result by 4
    
    for(i=1;i<src.rows-1;i++){
            for(j=1;j<src.cols-1;j++){
                for(c=0;c<3;c++) {
                    local.at<Vec3b>(i,j)[c]=src.at<Vec3b>(i,j-1)[c]*0.25+
                                            src.at<Vec3b>(i,j)[c]*0.5+
                                            src.at<Vec3b>(i,j+1)[c]*0.25;
                }                           
            }  
        }

     // [1 0 -1] vertical, divide the result by 1
    
    for(i=1;i<src.rows-1;i++){
            for(j=1;j<src.cols-1;j++){
                for(c=0;c<3;c++) {
                    dst.at<Vec3s>(i,j)[c]=local.at<Vec3b>(i-1,j)[c]*1+
                                          local.at<Vec3b>(i,j)[c]*0+
                                          local.at<Vec3b>(i+1,j)[c]*(-1);
                }                           
            }  
   
        }
    convertScaleAbs(dst,dst);

    return (0);
}

// //  a gradient magnitude image from the X and Y Sobel images

// // Euclidean distance for magnitude: I = sqrt( sx*sx + sy*sy )

int magnitude( Mat &sx, Mat &sy, Mat &dst ){
    int c,i,j;
    int sum_squareroot;
    for(i=0;i<sx.rows;i++){
            for(j=0;j<sx.cols;j++){
                for(c=0;c<3;c++){
                    dst.at<Vec3b>(i,j)[c]= sqrt(sx.at<Vec3b>(i,j)[c]*sx.at<Vec3b>(i,j)[c]+
                                                sy.at<Vec3b>(i,j)[c]*sy.at<Vec3b>(i,j)[c]);   
                }
            }    
        }
    return(0);
}
// //  a gradient orientation from the X and Y Sobel images

// int orientation( Mat &sx, Mat &sy, Mat &dst ){
//     int c,i,j;
//     int sum_squareroot;
//     for(i=0;i<sx.rows;i++){
//             for(j=0;j<sx.cols;j++){
//                 for(c=0;c<3;c++) {
//                     dst.at<Vec3s>(i,j)[c]= atan2(sy.at<Vec3s>(i,j)[c],sx.at<Vec3b>(i,j)[c];                                 
//                 }
//             }    
//         }
//     return(0);


// blurs and quantizes a color image

int blurQuantize( Mat &src, Mat &dst, int levels = 15 ){
    blur5x5(src,dst);
    int c,i,j;
    int b= 255/levels;
    for(i=0;i<dst.rows;i++){
            for(j=0;j<dst.cols;j++){
                for(c=0;c<3;c++) {
                    int xt = dst.at<Vec3b>(i,j)[c]/b;
                    int xf = xt*b;
                    dst.at<Vec3b>(i,j)[c]=xf;
                }
            }    
    }
    return (0);
}



// cartoonization function using the gradient magnitude and blur/quantize filters

// int cartoon( Mat &src, Mat&dst, int levels, int magThreshold){
//     Mat magn,blurq;
//     src.copyTo(magn);
//     src.copyTo(blurq);
//     Mat sx(src.rows, src.cols, CV_16SC3);
//     Mat sy(src.rows, src.cols, CV_16SC3);
//     int c,i,j;
//         sobelX3x3(src,sx);
//         sobelY3x3(src,sy);
//         magnitude(sx,sy,magn);
//         blurQuantize(src,blurq,levels);
//        for(i=0;i<magn.rows;i++){
//             for(j=0;j<magn.cols;j++){
//                 for(c=0;c<3;c++) { 
//                                     if (magn.at<Vec3b>(i,j)[c]>magThreshold) {
//                                         dst.at<Vec3b>(i,j)[c]=0;
//                                     }
//                                     else {
//                                         dst.at<Vec3b>(i,j)[c]= blurq.at<Vec3b>(i,j)[c];
//                                     }
//                 }
//             }    
//     }
//     return (0);
// }
        
















