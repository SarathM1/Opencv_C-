#include<opencv2/opencv.hpp>
#include<iostream>
#include<sstream>

using namespace cv;
using namespace std;


void scanC(Mat& img, const uchar* const table)
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

    
}

void scanIterator(Mat& img,const uchar* const table)
{
    int channels = img.channels();
    switch(channels)
    {
        case 1:
            {
                MatIterator_<uchar> it,end;
                for(it = img.begin<uchar>(),end=img.end<uchar>();it!=end;it++)
                    *it = table[*it];
                break;
            }
        case 3:
            {
                MatIterator_<Vec3b> it,end;
                for(it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; ++it)
                {
                    (*it)[0] = table[(*it)[0]];
                    (*it)[1] = table[(*it)[1]];
                    (*it)[2] = table[(*it)[2]];
                }
                break;
            }
    }
    
}

Mat scanLUT(Mat &img,const uchar* const table)
{
    Mat lookUpTable(1,256,CV_8U);

    uchar *p = lookUpTable.data;

    for(int i=0; i < 256; ++i)
        p[i] = table[i];
    
    Mat img_new;
    LUT(img,lookUpTable,img_new);
    return img_new;
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

    Mat img,img_1,img_2,img_3;
    img = imread("test.jpg");
    
    img_1 = img.clone();
    img_2 = img.clone();
    img_3 = img.clone();

    scanIterator(img_1,table);
    scanC(img_2,table);
    img_3 = scanLUT(img_3,table);

    imshow("ScanC",img_2);
    imshow("ScanIterator",img_1);
    imshow("ScanLUT",img_3);

    imshow("Image",img);
    waitKey(0);
}
