#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "libs.h"
#include "C:/opencv/build/include/opencv2/opencv.hpp"
#include "C:/opencv/build/include/opencv2/opencv_modules.hpp"

using namespace std;
using namespace cv;

int main()
{

    string src_path = "Data/Pictures/bp9_mask.jpg";
    Mat src, dst;

    src = imread(src_path, CV_LOAD_IMAGE_GRAYSCALE);
    float sf = 0.5;    //scale factor
    resize(src, src, Size(0,0), sf, sf);

    namedWindow("src", WINDOW_NORMAL);
    imshow("src", src);

    findSkeleton(src, dst);

    namedWindow("skele", WINDOW_NORMAL);
    imshow("skele", dst);

    waitKey(0);
    return 0;
}
