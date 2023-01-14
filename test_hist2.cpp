#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <map>

/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.

 */
using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  Mat data_base;
  Mat dst;
  Mat target;

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

    const int Hsize = 8; /*no of bins equal to 8*/

   
    target= imread("/home/samruddhi/CV/Project_2/olympus/pic.0164.jpg",IMREAD_COLOR);
    imshow("target_image",target);
    int c= 0, i=0, j=0; 
    int rix=0, bix=0,gix=0;
    int divisor = 256/Hsize;
    ///////calculating features for target histogram /////////

    float *hist_3d =new float [Hsize*Hsize*Hsize];

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        hist_3d[i] = 0;
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
 
    ////////////histogram normalization////////////////////
    float histogram_sum = 0.0; 

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        histogram_sum += hist_3d[i];
    }

    for(int i=0; i<Hsize*Hsize*Hsize; i++){
        hist_3d[i] = hist_3d[i]/histogram_sum;
    }

    vector<pair<float, string>> samruddhi;

  // loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ) {

          // check if the file is an image
        if( strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif") ) {

            // printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);
            
            data_base = imread( buffer, IMREAD_COLOR);
            int local_rix = 0, local_gix = 0, local_bix = 0;  

            ///Initializng data_base histogram //////

            float *local_hist_3d =new float [Hsize*Hsize*Hsize];
            for(int i=0; i<Hsize*Hsize*Hsize; i++){
                local_hist_3d[i] = 0;
            }
            ///Calculating features for data_base histogram //////

            for(int i=0;i<data_base.rows;i++){
                for(int j=0;j<data_base.cols;j++){ 
                    for(int c=0;c<3;c++){    
                        local_bix = (data_base.at<Vec3b>(i,j)[0])/divisor;
                        local_gix = (data_base.at<Vec3b>(i,j)[1])/divisor;
                        local_rix = (data_base.at<Vec3b>(i,j)[2])/divisor;
                        local_hist_3d[local_rix*Hsize*Hsize + local_gix*Hsize +local_bix]++;
                    }         
                }
            } 

            ////normalising the histograms//

                float local_histogram_sum = 0.0;

                for(int i=0; i<Hsize*Hsize*Hsize; i++){
                    local_histogram_sum += local_hist_3d[i];
                }
                for(int i=0; i<Hsize*Hsize*Hsize; i++){
                    local_hist_3d[i] = local_hist_3d[i]/local_histogram_sum;
                }
      
            //calculating histogram similiarity//
                float sim_hist_3d = 0.0;
                
                for(int i =0; i<Hsize*Hsize*Hsize; i++){
                    sim_hist_3d += min(hist_3d[i],local_hist_3d[i]);
                }
      

            //histogram intersection//

            float difference =(1-sim_hist_3d);


            delete local_hist_3d;        
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



      

















