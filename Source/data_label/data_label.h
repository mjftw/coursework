#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void get_desc_points(Mat& src);
string get_point_name(int pt);
void call_back(int event, int x, int y, int flags, void* userdata);
bool write_points(string path, bool amend);

vector<Point> desc_pts;

#define WHITE 255
#define BLACK 0

void get_desc_points(Mat& src)
{
    Mat temp = src.clone();
    desc_pts.clear();

    namedWindow("Data Labelling", WINDOW_NORMAL);

    setMouseCallback("Data Labelling", call_back, NULL);

    imshow("Data Labelling", temp);

    cout << "Data labelling function started." << endl;
    cout << "Left click to label point, right click to remove last point." << endl;
    cout << "Press any key to finish collecting points." << endl;
    cout << "First point: " << get_point_name(0) << endl;
    waitKey(0);

    if(desc_pts.size() < 11)
    {
        cout << "Insufficient number of points collected. Continuing..." << endl;
        waitKey(0);
    }

    cout << endl;
    return;
}

void call_back(int event, int x, int y, int flags, void* userdata)
{
    if(event == EVENT_LBUTTONDOWN)
    {
        if(desc_pts.size() < 11)
        {
            cout << '\t' << get_point_name(desc_pts.size()) << " (" << desc_pts.size() << ") : (" << x << ", " << y << "). " << get_point_name(desc_pts.size() + 1) << " point next." << endl;
            desc_pts.push_back(Point(x,y));
            if(desc_pts.size() == 11)
                cout << "All 11 points collected, press any key to finish." << endl;
        }
        else
        {
            cout << '\t' << "All 11 points collected, right click to remove last point." << endl;
        }

    }
    else if(event == EVENT_RBUTTONDOWN)
    {
        if(desc_pts.size() > 0)
        {
        cout << '\t' << get_point_name(desc_pts.size() - 1) << " point (" << (desc_pts.size() - 1) << ") removed." << endl;
        desc_pts.pop_back();
        }
    }

    return;
}

string get_point_name(int pt)
{
switch(pt)
    {
    case 0:
        return "Waist";
    break;
    case 1:
        return "Neck";
    break;
    case 2:
        return "Head";
    break;
    case 3:
        return "Left hand";
    break;
    case 4:
        return "Left elbow";
    break;
    case 5:
        return "Left knee";
    break;
    case 6:
        return "Left foot";
    break;
    case 7:
        return "Right foot";
    break;
    case 8:
        return "Right knee";
    break;
    case 9:
        return "Right elbow";
    break;
    case 10:
        return "Right hand";
    break;
    default:
        return "No";
    break;
    }
}

bool write_points(string path, bool amend)
{
    ofstream dataFile;
    dataFile.open(path.c_str(), ios::out | (amend? ios::app : ios::trunc));

    if(!dataFile.is_open())
    {
        cout << "Could not open output file for writing.";
        return 1;
    }

    for(int i=0; i<(desc_pts.size() - 1); i++)
    {
        dataFile << desc_pts[i].x << ',' << desc_pts[i].y;
        if(i < (desc_pts.size()-2))
            dataFile << ',';
        if(i == (desc_pts.size() - 2))
            dataFile << endl;
    }

    dataFile.close();
    return 0;
}
