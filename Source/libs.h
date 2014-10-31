using namespace std;
using namespace cv;

#define WHITE 255
#define BLACK 0

void findSkeleton(Mat& src, Mat& dst) //From summer work, should re-implement better
{
    // Using morphological method
    Mat skel(src.size(), CV_8UC1, Scalar(BLACK));
    Mat temp(src.size(), CV_8UC1);
    Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3));
    double maximum = BLACK;

    do
    {
        morphologyEx(src, temp, cv::MORPH_OPEN, element);
        bitwise_not(temp, temp);
        bitwise_and(temp, src, temp);
        bitwise_or(temp, skel, skel);
        erode(src, src, element);       //src(erode)

        minMaxLoc(src, BLACK, &maximum);
    }while (maximum != BLACK);

    morphologyEx(src, src, cv::MORPH_CLOSE, element);

    dst = skel.clone();
    return;
}

void findConnectedSkeleton(Mat& src, Mat& dst) //From summer work, should re-implement better
{
    /*  Medial Axis Transformation skeletonisation method, taken from Digital Image Processing book, pg650-653
        Algorithm steps:
        For each pixel:
            1. test pixels neighbours
            p8 p1 p2  |
            p7 p0 p3  | +y
            p6 p5 p4  \/
            ---------->
                +x

            2. Count Np0, number of non-zero neighbours of p0
            3. Count Tp0, number of 0-1 transitions in sequence p2, p3 .... p7, p8

            4. Check first conditions, and mark for deletion any point that meets all conditions
                cA:     2 <= Np0 <= 6
                cB:     Tp1 = 1
                cC:     p1 . p3 . p5 = 0
                cD:     p3 . p5 . p7 = 0
        5. Delete any points marked for deletion

        For each remaining pixel:
            6. test pixels neighbours
            p8 p1 p2
            p7 p0 p3
            p6 p5 p4

            7. Check second conditions, and mark for deletion any point that meets all conditions
                cA:     2 <= Np0 <= 6
                cB:     Tp1 = 1
                cC_:    p1 . p3 . p7 = 0
                cD_:    p1 . p5 . p7 = 0
        8. Delete any points marked for deletion

        repeat until no points are deleted in an iteration
    */

    Mat skel = src.clone(); //Src MUST be a binary image
    Mat skel_prev(src.size(), CV_8UC1, Scalar(BLACK)), skel_diff(src.size(), CV_8UC1, Scalar(BLACK));
    Mat to_delete(src.size(), CV_8UC1, Scalar(WHITE)); //matrix or points to be deleted
    double maximum = BLACK;

    bool p[9];  // flags showing if 8-neighbour points of p[0] are 0 or 1
    bool cA, cB, cC, cD, cC_, cD_; //flags showing if conditions are met
    int Tp0, Np0;
    do
    {
        skel_prev = skel.clone();

        //cannot test edge points, hence the -2
        for(int i=1; i<(src.rows-2); i++)    //x values
            for(int j=1; j<(src.cols-2); j++) //y values
            {
                if(src.at<uchar>(i,j) == WHITE)
                {
                    Np0 = 0;
                    Tp0 = 0;

                    p[0] = skel.at<uchar>(i  , j  );    //*TODO* Try switching i & j
                    p[1] = skel.at<uchar>(i-1, j  );
                    p[2] = skel.at<uchar>(i-1, j+1);
                    p[3] = skel.at<uchar>(i  , j+1);
                    p[4] = skel.at<uchar>(i+1, j+1);
                    p[5] = skel.at<uchar>(i+1, j  );
                    p[6] = skel.at<uchar>(i+1, j-1);
                    p[7] = skel.at<uchar>(i  , j-1);
                    p[8] = skel.at<uchar>(i-1, j-1);

                    for(int k=1; k<9; k++)
                    {
                        if(p[k])
                            Np0++;
                        if((k!=1) && (p[k] == 1) && (p[k-1] == 0))
                            Tp0++;
                    }

                    cA = ((2 <= Np0) && (Np0 <= 6));
                    cB = (Tp0 == 1);
                    cC = ((p[1] & p[3] & p[5]) == 0);
                    cD = ((p[3] & p[5] & p[7]) == 0);

                    if(cA & cB & cC & cD)
                        to_delete.at<uchar>(i,j) = BLACK;
                }
            }

        bitwise_and(skel, to_delete, skel);

        for(int i=1; i<(src.rows-2); i++)    //y values
            for(int j=1; j<(src.cols-2); j++) //x values
            {
                if(src.at<uchar>(i,j) == WHITE)
                {
                    Np0 = 0;
                    Tp0 = 0;

                    p[0] = skel.at<uchar>(i  , j  );
                    p[1] = skel.at<uchar>(i-1, j  );
                    p[2] = skel.at<uchar>(i-1, j+1);
                    p[3] = skel.at<uchar>(i  , j+1);
                    p[4] = skel.at<uchar>(i+1, j+1);
                    p[5] = skel.at<uchar>(i+1, j  );
                    p[6] = skel.at<uchar>(i+1, j-1);
                    p[7] = skel.at<uchar>(i  , j-1);
                    p[8] = skel.at<uchar>(i-1, j-1);

                    for(int k=1; k<9; k++)
                    {
                        if(p[k])
                            Np0++;
                        if((k!=1) && (p[k] == 1) && (p[k-1] == 0))
                            Tp0++;
                    }

                    cA = ((2 <= Np0) && (Np0 <= 6));
                    cB = (Tp0 == 1);
                    cC_ = ((p[1] & p[3] & p[7]) == 0);
                    cD_ = ((p[1] & p[5] & p[7]) == 0);

                    if(cA & cB & cC_ & cD_)
                        to_delete.at<uchar>(i,j) = BLACK;
                }
            }

        bitwise_and(skel, to_delete, skel);

        absdiff(skel, skel_prev, skel_diff);
        minMaxLoc(skel_diff, BLACK, &maximum);
    }while(maximum != BLACK);

    dst = skel.clone();
    return;
}
