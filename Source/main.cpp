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

    string src_path = "Data/Pictures/bp9_mask.tif";
    Mat src, dst;

    src = imread(src_path, CV_LOAD_IMAGE_GRAYSCALE);
    float sf = 0.25;    //scale factor
    resize(src, src, Size(0,0), sf, sf);

    threshold(src, src, 1, WHITE, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3)); //only required because image compression creates noisy edges & algorithm is very susceptible to noise
    morphologyEx(src, src, cv::MORPH_OPEN, element);

    namedWindow("src", WINDOW_NORMAL);
    imshow("src", src);

    edge_detect(src, dst);

    findConnectedSkeleton(src, dst);
    //findSkeleton(src, dst);


    namedWindow("skele", WINDOW_NORMAL);
    imshow("skele", dst);
    imwrite("test_op.bmp", dst);

    waitKey(0);
    return 0;
}
