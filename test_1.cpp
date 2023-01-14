#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

bool sortcol(const vector<float>& vec1, const vector<float>& vec2)
 {
   return vec1[0] < vec2[0];
}

int main(int argc, char *argv[]){

        char dirname[256];
        char buffer[256];
        FILE *fp;
        DIR *dirp;

        struct dirent *dp;
        int a=0;
        Mat target;
        Mat base_nine;
        Mat local;
        Mat close1;
        Mat close2;
        Mat close3;


        // check for sufficient arguments
        if( argc < 2) {
        printf("usage: %s <directory path>\n", argv[0]);
        exit(-1);

        }

        // get the directory path
        strcpy(dirname, argv[1]);
        // printf("Processing directory %s\n", dirname );

        // open the directory
        dirp = opendir( dirname );
        if( dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);

        }
        string path[1106];
        

        // loop over all the files in the image file listing
        while( (dp = readdir(dirp)) != NULL ) {

                // check if the file is an image
                if( strstr(dp->d_name, ".jpg") ||
                    strstr(dp->d_name, ".png") ||
                    strstr(dp->d_name, ".ppm") ||
                    strstr(dp->d_name, ".tif") ) {



                // build the overall filename
                strcpy(buffer, dirname);
                strcat(buffer, "/");
                path[a] = strcat(buffer, dp->d_name);
                a++;
                }
        }
                int n = sizeof(path)/sizeof(path[0]);
                sort(path, path + n);

                int b;

                //! [imread]
                target= imread("/home/samruddhi/CV/Project_2/olympus/pic.1016.jpg",IMREAD_COLOR);
                            imshow("target_image",target);
            

                vector<vector<int>> v( 1 , vector<int> (243, 0));

                base_nine.create(9,9,target.type());
                local.create(9,9,target.type());

                for(int i = 0 ; i < 9; i++ ){
                    for(int j = 0; j < 9; j++ ) {
                        for(int c=0; c<3 ; c++ ){
                            v[b][i * 9 + j + c * 9 * 9] = target.at<Vec3b>(target.rows/2 -4 + i,target.cols/2-4+j)[c];
                        }
                    }
                }
                 ///data base//

                vector<vector<float>> value( 1106 , vector<float> (2));
                vector<string> p;

                for(int b = 0 ; b < 1106; b++ ){
                    Mat data_base = imread(path[b], IMREAD_COLOR);
                    vector<vector<int>> vi( 1106 , vector<int> (243, 0));

                    for(int i = 0 ; i < 9; i++ ){
                        for(int j = 0; j < 9; j++ ) {
                            for(int c = 0; c < 3 ; c++ ){
                                vi[b][i * 9 + j + c * 9 *9] = data_base.at<cv::Vec3b>(data_base.rows/2 - 4 + i, data_base.cols/2-4+j)[c];
                            }
                        }
                    }

                    for(int i = 0 ; i < 243; i++ ){
                        value[b][0] += (v[0][i] - vi[b][i]) * (v[0][i] - vi[b][i]);

                        }

                        value[b][1] = b;

                }

                sort(value.begin(), value.end(), sortcol);

                cout<< " top closest images : " << endl;

                for(int b = 0 ; b < 1106 ; b++){
                cout << "Distance : " << value[b][0] << "path :" << value[b][1] << endl;
                }

                close1 = imread(path[int(value[1][1])], IMREAD_COLOR);
                close2 = imread(path[int(value[2][1])], IMREAD_COLOR);
                close3 = imread(path[int(value[3][1])], IMREAD_COLOR);

                imshow("First", close1);
                imshow("Second",close2);
                imshow("Third", close3);

                while (true) {
                        b = waitKey(0);

                        if (b == 113) {
                            destroyAllWindows();
                            break;
                        }
                }
    return 0;
}