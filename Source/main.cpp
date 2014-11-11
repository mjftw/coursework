#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "libs.h"
#include "data_labeling.h"

using namespace std;
using namespace cv;

int main()
{

    string src_path = "Data/Pictures/bp9_mask.tif";
    Mat src, dst;

    src = imread(src_path, CV_LOAD_IMAGE_GRAYSCALE);
    if(src.empty())
    {
        cout << "Error loading source image" << endl;
        return -1;
    }

    float sf = 0.25;    //scale factor
    resize(src, src, Size(0,0), sf, sf);

    threshold(src, src, 1, WHITE, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3)); //only required because image compression creates noisy edges & algorithm is very susceptible to noise
    morphologyEx(src, src, cv::MORPH_OPEN, element);

    namedWindow("src", WINDOW_NORMAL);
    imshow("src", src);

    find_skeleton_connected(src, dst);
    //find_skeleton(src, dst);


    float epsilon = 30, sizeTol = 0.3;
    reduce_points(dst, dst, epsilon, sizeTol);

    namedWindow("Final", WINDOW_NORMAL);
    imshow("Final", dst);

    waitKey(0);
    return 0;
}
