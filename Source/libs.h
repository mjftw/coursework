using namespace std;
using namespace cv;

#define WHITE 255
#define BLACK 0

void edge_detect(Mat& src, Mat& dst);
void find_skeleton(Mat& src, Mat& dst);
void find_skeleton_connected(Mat& src, Mat& dst);
void edge_detect(Mat& src, Mat& dst);
void reduce_points(Mat& src, Mat dst);
vector<Point> split_and_merge(vector<Point> src, float epsilon); //Split and merge algorithm on each contour (Ramer–Douglas–Peucker algorithm), pseudocode used from wiki page

void find_skeleton(Mat& src, Mat& dst)
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

void find_skeleton_connected(Mat& src, Mat& dst)
{
    /*  Medial Axis Transformation skeletonisation method, taken from Digital Image Processing book, pg650-653
        Algorithm steps:
        For each boundary pixel:
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
    Mat edge(src.size(), CV_8UC1, Scalar(BLACK));
    double maximum = BLACK;

    bool p[9];  // flags showing if 8-neighbour points of p[0] are 0 or 1
    bool cA, cB, cC, cD, cC_, cD_; //flags showing if conditions are met
    int Tp0, Np0;
    do
    {
        skel_prev = skel.clone();
        edge_detect(skel, edge);
        //cannot test edge points, hence the -2
        for(int i=1; i<(src.rows-2); i++)    //x values
            for(int j=1; j<(src.cols-2); j++) //y values
            {
                if(edge.at<uchar>(i,j) == WHITE)
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
                    if((p[1] == 1) && (p[8] == 0))
                        Tp0++;

                    cA = ((2 <= Np0) && (Np0 <= 6));
                    cB = (Tp0 == 1);
                    cC = ((p[1] & p[3] & p[5]) == 0);
                    cD = ((p[3] & p[5] & p[7]) == 0);

                    if(cA & cB & cC & cD)
                        to_delete.at<uchar>(i,j) = BLACK;
                }
            }

        bitwise_and(skel, to_delete, skel);
        edge_detect(skel, edge);

        for(int i=1; i<(src.rows-2); i++)    //y values
            for(int j=1; j<(src.cols-2); j++) //x values
            {
                if(edge.at<uchar>(i,j) == WHITE)
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
                    if((p[1] == 1) && (p[8] == 0))
                        Tp0++;

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

void edge_detect(Mat& src, Mat& dst)
{
    Mat edge(src.size(), CV_8UC1, Scalar(BLACK));
    Mat kernel = (Mat_<char>(3,3)  << -1, -1, -1,
                                      -1,  8, -1,
                                      -1, -1, -1); //laplacian edge detection
    filter2D(src, dst, -1, kernel);
    return;
}

void reduce_points(Mat& src, Mat dst)
{
    /* Algorithm plan:
        1. Find points where Tp0 <=3
        2. Mask these bifurcation points to create separate polys
        3. Find contours
        4. Split and merge algorithm on each contour (Ramer–Douglas–Peucker algorithm), pseudocode used from wiki page
        5. combine contours
    */
    Mat temp = src.clone();
    Mat temp2(src.size(), CV_8UC1, Scalar(BLACK));

    vector<Point> splitPt;
    vector<vector<Point> > lineSegs, lineSegsReduced;
    bool p[9];
    int Tp0;
    int avgSegSize = 0;

    for(int i=1; i<(src.rows-2); i++)    //x values
        for(int j=1; j<(src.cols-2); j++) //y values
        {
            if(src.at<uchar>(i,j) == WHITE)
            {
                Tp0 = 0;

                p[0] = src.at<uchar>(i  , j  );
                p[1] = src.at<uchar>(i-1, j  );
                p[2] = src.at<uchar>(i-1, j+1);
                p[3] = src.at<uchar>(i  , j+1);
                p[4] = src.at<uchar>(i+1, j+1);
                p[5] = src.at<uchar>(i+1, j  );
                p[6] = src.at<uchar>(i+1, j-1);
                p[7] = src.at<uchar>(i  , j-1);
                p[8] = src.at<uchar>(i-1, j-1);

                for(int k=1; k<9; k++)
                    if((k!=1) && (p[k] == 1) && (p[k-1] == 0))
                        Tp0++;
                if((p[1] == 1) && (p[8] == 0))
                    Tp0++;

                if(Tp0 > 2)
                {
                    splitPt.push_back(Point(i,j));

                    temp.at<uchar>(i  , j  ) = BLACK;
                    temp.at<uchar>(i-1, j  ) = BLACK;
                    temp.at<uchar>(i-1, j+1) = BLACK;
                    temp.at<uchar>(i  , j+1) = BLACK;
                    temp.at<uchar>(i+1, j+1) = BLACK;
                    temp.at<uchar>(i+1, j  ) = BLACK;
                    temp.at<uchar>(i+1, j-1) = BLACK;
                    temp.at<uchar>(i  , j-1) = BLACK;
                    temp.at<uchar>(i-1, j-1) = BLACK;
                }
            }
        }

    findContours(temp, lineSegs, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);

    for(int i=0; i<lineSegs.size(); i++)
        avgSegSize += arcLength(lineSegs[i], false);
    avgSegSize /= lineSegs.size();

    float sizeTolerance = 0.3;
    for(int i=0; i<lineSegs.size(); i++)
    {
        if(arcLength(lineSegs[i], false) >= (avgSegSize * sizeTolerance))
        {
            lineSegsReduced.push_back(lineSegs[i]);
            approxPolyDP(lineSegs[i], lineSegsReduced[lineSegsReduced.size()-1], 30, true);
        }
    }


    drawContours(temp2, lineSegsReduced, -1, Scalar(WHITE));
    namedWindow("temp2", WINDOW_NORMAL);
    imshow("temp2", temp2);

    dst = temp.clone();

    return;
}
void reduce_points(Mat& src, Mat dst);

//vector<Point> split_and_merge(vector<Point> pts, float epsilon)
//{// Find the point with the maximum distance
//   int dmax = 0;
//   float d = 0;
//   int index = 0;
//   int endL = pts.size();
//
//   vector<Point> recResults1, recResults2;
//   vector<Point> partPts1, partPts2;
//   vector<Point> resultList;
//
//   for(int i=1; i<(endL-1); i++)
//   {
//        d = sqrt(pow(pts[0].x - pts[endL].x, 2) + pow(pts[0].y - pts[endL].y, 2));
//        if(d > dmax)
//        {
//            index = i;
//            dmax = d;
//        }
//   }
//   if(dmax > epsilon)
//   {// Recursive call
//        for(int i=0; i<index; i++)
//            partPts1.push_back(pts[i]);
//        for(int i=index; i<endL; i++)
//            partPts2.push_back(pts[i]);
//
//        recResults1 = split_and_merge(partPts1, epsilon);
//        recResults2 = split_and_merge(partPts2, epsilon);
//
//    // Build the result list
//        for(int i=0; i<(endL-1); i++)
//            resultList.push_back(recResults1[i]);
//        for(int i=0; i<endL; i++)
//            resultList.push_back(recResults2[i]);
//
//   }
//   else
//   {
//       resultList.clear();
//       resultList.push_back(pts[0]);
//       resultList.push_back(pts[endL]);
//   }
//
//    // Return the result
//    return resultList;
//}
