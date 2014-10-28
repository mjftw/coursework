using namespace std;
using namespace cv;

#define WHITE 255
#define BLACK 0

void findSkeleton(Mat& src, Mat& dst) //From summer work, should re-implement better
{
    Mat skel(src.size(), CV_8UC1, Scalar(BLACK));
    Mat temp(src.size(), CV_8UC1);
    Mat element = getStructuringElement(MORPH_CROSS, cv::Size(3, 3));
    bool done;

    do
    {
        morphologyEx(src, temp, cv::MORPH_OPEN, element);
        bitwise_not(temp, temp);
        bitwise_and(src, temp, temp);
        bitwise_or(skel, temp, skel);
        erode(src, src, element);

        double max;
        minMaxLoc(src, 0, &max);
        done = (max == 0);
    }while (!done);

    dst = skel.clone();
    return;
}

void divideIntoSubimgs(Mat& src, vector<Mat>* op, int n, int m) //From summer work, written by Merlin Webster
{
    op->clear();

    int w = src.cols / n;
    int h = src.rows / m;
    Mat temp;

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
        {
            temp = src(Rect(j*w, i*h, w, h));
            op->push_back(src(Rect(j*w, i*h, w, h)));
        }
}

void combineSubimgs(vector<Mat>* src, Mat& op, int n, int m) //From summer work, written by Merlin Webster
{
    if(src->empty())
        return;

    int w = src->at(0).cols;
    int h = src->at(0).rows;

    op = Mat(w*m, h*n, CV_8UC1, Scalar::all(BLACK));
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
            op(Rect(j*w, i*h, h, w)) += src->at(i*n + j);

    return;
}

void optimalThreshSubimgs(Mat& src, Mat& dst, int min_fg_bg_diff, int n, int m) //From summer work, written by Merlin Webster
{
    vector<Mat> subimgs;

    divideIntoSubimgs(src, &subimgs, n, m);
    combineSubimgs(&subimgs, dst, n, m);
    return;
}
