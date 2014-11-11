#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "data_label.h"

using namespace std;
using namespace cv;

int main()
{
    int nImgs = 10;
    for(int i=0; i<nImgs; i++)
    {
        stringstream src_path;
        src_path << "../Data/Pictures/bp" << i+1 << ".jpg";

        Mat src;

        cout << "Image: " << src_path.str() << endl;

        src = imread(src_path.str(), CV_LOAD_IMAGE_GRAYSCALE);
        if(src.empty())
        {
            cout << "Error loading source image" << endl;
            return -1;
        }

        float x_size = 600;    //scale factor
        resize(src, src, Size(x_size, src.cols*(float)(x_size/ src.rows)),0 , 0);
/*
        threshold(src, src, 1, WHITE, THRESH_BINARY);
        Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3)); //only required because image compression creates noisy edges & algorithm is very susceptible to noise
        morphologyEx(src, src, cv::MORPH_OPEN, element);
*/
        get_desc_points(src);

        stringstream op_path;
        op_path << "data_pts_" << src.rows << '_' << src.cols << ".csv" ;
        write_points(op_path.str(), true);

    }

    return 0;
}
