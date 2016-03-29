#include<opencv2/opencv.hpp>
#include<iostream>
#include<sstream>

using namespace cv;
using namespace std;


Mat& scanC(Mat& img, const uchar* const table)
{
    CV_Assert(img.depth() == CV_8U);

    int channels = img.channels();
    int columns = img.cols*channels;
    int rows = img.rows;

    if(img.isContinuous())
    {
        columns *= rows;
        rows = 1;
    }

    int i,j;
    uchar *p;
    for(i = 0; i<rows; ++i)
    {
        p = img.ptr<uchar>(i);
        for(j=0;j<columns;++j)
        {
            p[j] = table[p[j]];
        }
    }

    imshow("Final",img);
    waitKey(0);
    return img;
}

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        cout<<"argc = "<< argc << "!!" << " Aborting.. "<<endl;
        cout << "argv[1] = " <<argv[1] << endl;
        return -1;
    }

    int divideWidth = 0;
    stringstream s;
    s << argv[1];
    s >> divideWidth;

    uchar table[256];
    for(int i=0;i<256;i++)
        table[i] = (uchar)(divideWidth*(i/divideWidth));

    Mat img;
    img = imread("test.jpg");
    scanC(img,table);
}
