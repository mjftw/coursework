#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

vector<Point> get_desc_points(Mat& src);
void call_back(int event, int x, int y, int flags, void* userdata);

vector<Point> desc_pts;

vector<Point> get_desc_points(Mat& src)
{
    Mat temp = src.clone();
    vector<Point> pts;

    namedWindow("Data Labelling", WINDOW_NORMAL);

    setMouseCallback("Data Labelling", call_back, NULL);

    imshow("Data Labelling", temp);

    cout << endl << "Data labelling function started." << endl << "Left click to label point, right click to remove last point." << endl << "Press any key to finish collecting points." << endl << endl;
    waitKey(0);

    if(desc_pts.size() < 11)
    {
        cout << "Insufficient number of points collected. Removing collected points." << endl;
        desc_pts.clear();
    }

    return desc_pts;
}

void call_back(int event, int x, int y, int flags, void* userdata)
{
    string point_name;
    if(event == EVENT_LBUTTONDOWN)
    {
        if(desc_pts.size() < 11)
        {
            switch(desc_pts.size())
            {
                case 0:
                    point_name = "Head";
                break;
                case 1:
                    point_name = "Left hand";
                break;
                case 2:
                    point_name = "Left elbow";
                break;
                case 3:
                    point_name = "Neck";
                break;
                case 4:
                    point_name = "Waist";
                break;
                case 5:
                    point_name = "Left knee";
                break;
                case 6:
                    point_name = "Left foot";
                break;
                case 7:
                    point_name = "Right foot";
                break;
                case 8:
                    point_name = "Right knee";
                break;
                case 9:
                    point_name = "Right elbow";
                break;
                case 10:
                    point_name = "Right hand";
                break;
            }

            cout << '\t' <<point_name << " point (" << desc_pts.size() << ") set to (" << x << ", " << y << ")." << endl;
            desc_pts.push_back(Point(x,y));
            if(desc_pts.size() == 11)
                cout << "All 11 points collected, press any key to finish." << endl;
        }
        else
        {
            cout << '\t' << "All 11 points already collected, right click to remove last point." << endl;
        }

    }
    else if(event == EVENT_RBUTTONDOWN)
    {
        cout << '\t' << "Previous point (" << (desc_pts.size() - 1) << ") removed." << endl;
        desc_pts.pop_back();
    }

}
