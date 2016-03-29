#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    Mat img;
    img = imread("test.jpg");
    imshow("img",img);
    waitKey(0);
}
