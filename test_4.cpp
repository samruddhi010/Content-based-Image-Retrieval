#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "filters.h"


/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.

 */
using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    Mat data_base;
    Mat target;
    Mat local1;
    Mat magn;
    Mat orienration;
    Mat local_magn;
    // Mat local_sx;
    // Mat local_sy;
    Mat local_orienration;
    
    char dirname[256];
    char buffer[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;

    // check for sufficient arguments
    if( argc < 2) {
        printf("usage: %s <directory name>\n", argv[0]);
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );

    // open the directory
    dirp = opendir( dirname );
    if( dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }
    
    target= imread("/home/samruddhi/CV/Project_2/olympus/pic.0535.jpg",IMREAD_COLOR);
    imshow("target_image",target);

    // using gradient magnitude to calculate the texture//
    Mat sx(target.rows, target.cols, CV_16SC3);
    Mat sy(target.rows, target.cols, CV_16SC3);
    sobelX3x3(target, sx);
    // cout << sx << endl;
    sobelY3x3(target, sy);
    // cout << sy << endl;
    target.copyTo(magn);
    magnitude(sx,sy,magn);
    
    // cout << magn << endl;
               
    const int Hsize = 8; /*no of bins equal to 8*/
    int c= 0;
    int i=0;
    int j=0; 
    int rix=0, bix=0,gix=0;
    int divisor = 256/Hsize;

//////////calculating features for target histogram ////////

    float *hist_3d =new float [Hsize*Hsize*Hsize];

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        hist_3d[i] = 0;
    }

    float *text =new float [Hsize*Hsize*Hsize];

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        text[i] = 0;
    }

    for (int i=0;i<target.rows;i++){
        for(int j=0;j<target.cols;j++){ 
            for(int c=0;c<3;c++){    
                bix = (target.at<Vec3b>(i,j)[0])/divisor;
                gix = (target.at<Vec3b>(i,j)[1])/divisor;
                rix = (target.at<Vec3b>(i,j)[2])/divisor;
                hist_3d[rix*Hsize*Hsize + gix*Hsize +bix]++;
            }         
        }
    }

    //////////calculating texture for target histogram ////////
    for (int i=0;i<magn.rows;i++){
        for(int j=0;j<magn.cols;j++){ 
            for(int c=0;c<3;c++){    
                bix = (magn.at<Vec3b>(i,j)[0])/divisor;
                gix = (magn.at<Vec3b>(i,j)[1])/divisor;
                rix = (magn.at<Vec3b>(i,j)[2])/divisor;
                text[rix*Hsize*Hsize + gix*Hsize +bix]++;
            }         
        }
    }
    
    //histogram normalization//
    float histogram_sum = 0.0; 
    float histogram_sum1 = 0.0; 

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        histogram_sum += hist_3d[i];
    }
    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        histogram_sum1 += text[i];
    }

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        hist_3d[i] = hist_3d[i]/histogram_sum;
    }

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        text[i] = text[i]/histogram_sum1;
    }
   

    vector<pair<float, string>> samruddhi;

    // loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ) {

            // check if the file is an image
            if( strstr(dp->d_name, ".jpg") ||
                strstr(dp->d_name, ".png") ||
                strstr(dp->d_name, ".ppm") ||
                strstr(dp->d_name, ".tif") ) {

                //printf("processing image file: %s\n", dp->d_name);

                // build the overall filename
                strcpy(buffer, dirname);
                strcat(buffer, "/");
                strcat(buffer, dp->d_name);

                //printf("full path name: %s\n", buffer);


/////calculations for data base images//////

            // using gradient magnitude to calculate the texture//
                data_base = imread( buffer, IMREAD_COLOR);
                Mat local_sx(data_base.rows, data_base.cols, CV_16SC3);
                Mat local_sy(data_base.rows, data_base.cols, CV_16SC3);
                data_base.copyTo(local_magn);
                sobelX3x3(data_base, local_sx);
                sobelY3x3(data_base, local_sy);
                magnitude(local_sx,local_sy,local_magn);


                int local_rix = 0, local_gix = 0, local_bix = 0; 

                float *local_hist_3d =new float [Hsize*Hsize*Hsize];

                for(int i=0; i<Hsize*Hsize*Hsize; i++){
                    local_hist_3d[i] = 0;
                }

                float *local_text =new float [Hsize*Hsize*Hsize];

                for(int i=0; i<Hsize*Hsize*Hsize; i++){
                    local_text[i] = 0;
                }
///////calculating features for data base histogram/////
                for (int i=0;i<data_base.rows;i++){
                    for(int j=0;j<data_base.cols;j++){ 
                        for(int c=0;c<3;c++){    
                                local_bix = (data_base.at<Vec3b>(i,j)[0])/divisor;
                                local_gix = (data_base.at<Vec3b>(i,j)[1])/divisor;
                                local_rix = (data_base.at<Vec3b>(i,j)[2])/divisor;
                                local_hist_3d[local_rix*Hsize*Hsize + local_gix*Hsize +local_bix]++;
                        }         
                    }
                 }
//calculating textures for data_base histogram///////
                for(int i=0;i<local_magn.rows;i++){
                    for(int j=0;j<local_magn.cols;j++){ 
                        for(int c=0;c<3;c++){    
                            local_bix = (local_magn.at<Vec3b>(i,j)[0])/divisor;
                            local_gix = (local_magn.at<Vec3b>(i,j)[1])/divisor;
                            local_rix = (local_magn.at<Vec3b>(i,j)[2])/divisor;
                            local_text[local_rix*Hsize*Hsize + local_gix*Hsize +local_bix]++;
                        }         
                    }
                }
                
                    //histogram normalization//
                    float local_histogram_sum = 0.0; 
                    float local_histogram_sum1 = 0.0; 

                    for(int i=0; i<Hsize*Hsize*Hsize; i++){
                        local_histogram_sum += local_hist_3d[i];
                    }
                    for(int i=0; i<Hsize*Hsize*Hsize; i++){
                        local_histogram_sum1 += local_text[i];
                    }

                    for(int i=0; i<Hsize*Hsize*Hsize; i++){
                        
                        local_hist_3d[i] = local_hist_3d[i]/local_histogram_sum;
                       // cout<<local_hist_3d[i]<<endl;
                    }

                    for(int i=0; i<Hsize*Hsize*Hsize; i++){
                        local_text[i] = local_text[i]/local_histogram_sum1;
                        
                    }
                                         
                    
                    //calculating histogram similiarity//
                    float sim_hist3d = 0.0;
                    float sim_text = 0.0;
                    

                    for(int i =0; i<Hsize*Hsize*Hsize; i++){
                        sim_hist3d += min(hist_3d[i],local_hist_3d[i]);
                        sim_text += min(text[i],local_text[i]);
                        
                    }
                    cout<<sim_hist3d<<endl;


                    //histogram intersection//

                    float difference = (0.5*(1-sim_hist3d)+0.5*(1-sim_text));

                    delete local_hist_3d;
                    delete local_text;
                    samruddhi.push_back({difference, buffer});

            }
    }
        ////sorting the image and arranging in ascending order.//////
        sort(samruddhi.begin(), samruddhi.end());
                    Mat dst_frames;
                    for( int i=1;i < 4; i++){
                        cout<< samruddhi[i].first << " "<< samruddhi[i].second <<endl;
                        dst_frames = imread(samruddhi[i].second,IMREAD_COLOR);
                        imshow("frame"+ to_string (i)+ "jpg",dst_frames);
                    }
            char key = waitKey();
            while (1)
            {
                if(key=='q'){
                destroyAllWindows();
                break;        
                }
            }
            
    
    printf("Terminating\n");

    return(0);

}


















