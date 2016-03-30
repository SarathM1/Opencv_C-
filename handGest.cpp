#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
using namespace cv;
using namespace std;

#define distance 7
#define CONTOUR_LENGTH 250
 
Mat image, img_thresh,image_gray; 
CvMemStorage* storage = cvCreateMemStorage(0); 
RotatedRect contourcenter;
vector<vector<Point> > contours;         
vector<Point> cnt;
vector<Vec4i> hierarchy;
CvPoint armcenter;
CvMemStorage* palmstorage = cvCreateMemStorage(0);
CvMemStorage* fingerstorage = cvCreateMemStorage(0);
CvSeq* palm = cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),palmstorage);
CvSeq* fingerseq = cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),fingerstorage);
 


int main()
{
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout<<"Error reading frame, Aborting"<<endl;
        return -1;
    }
    
    while(1)
    {
        cap >> image ;              // Get a new frame from camera
        //output.create(image.size(),image.type());
        
        cvtColor(image,image_gray,CV_BGR2GRAY);
        if(!image.data){
            printf("Error: Couldn't open the image file.\n");
        }
         
        imshow("Image",image);
        
        threshold(image_gray, img_thresh, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
        imshow("threshold",img_thresh);
        
        findContours( img_thresh, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        int largest_contour_i = 0;
        int largest_area = 0;
        unsigned int i; 
        for(i=0;i<contours.size();i++)
        {
            double a = contourArea(contours[i]);
            if(a>largest_area)
            {
                largest_area = a;
                largest_contour_i = i;
            }
        }
        cnt = contours[largest_contour_i];
        
        vector<vector<Point> >hull( contours.size() );
        vector<vector<int> >hullsI( contours.size() );
        vector<vector<Vec4i> >defects( contours.size() );
        for(i=0; i<contours.size(); i++)
        {
            convexHull( Mat(contours[i]), hull[i], false);
            convexHull( Mat(contours[i]), hullsI[i], false);
            if(contours[i].size() > 3)
                convexityDefects( contours[i], hullsI[i], defects[i]);
        }
        Scalar color (255,255,255);
        //drawContours( image, contours, largest_contour_i, color, 2, 8, hierarchy, 0);
        
        /*color = (0, 0, 0);
        for(i = 0; i< contours.size(); i++)
            drawContours( image, hull, largest_contour_i, color, 2, 8, vector<Vec4i>(), 0);
        */

        /// Draw convexityDefects
        int diff1, diff2;
        for (i = 0; i < contours.size(); ++i)
        {
            for(const Vec4i& v : defects[i])
            {
                float depth = v[3] / 256;
                if (depth > 10) //  filter defects by depth, e.g more than 10
                {
                    int startidx = v[0]; Point ptStart(contours[i][startidx]);
                    int endidx = v[1]; Point ptEnd(contours[i][endidx]);
                    int faridx = v[2]; Point ptFar(contours[i][faridx]);
                    
                    diff1 = ptFar.y - ptEnd.y;
                    diff2 = ptFar.x - ptEnd.x;
                    if (diff1 < 20 || diff2 <20 )
                    {
                        continue;
                    }
                    putText(image, to_string(diff1) + ", " + to_string(diff2), ptEnd, FONT_HERSHEY_SIMPLEX, 0.5f, color);
                    //cout<< " diff1 = " << diff1 << " diff2 = " << diff2 << endl;
                    //line(image, ptStart, ptEnd, Scalar(0, 255, 0), 1);
                    //line(image, ptStart, ptFar, Scalar(0, 255, 0), 1);
                    line(image, ptEnd, ptFar, Scalar(0, 255, 0), 1);
                    circle(image, ptFar, 4, Scalar(0, 0, 255), 2);
                    circle(image, ptEnd, 4, Scalar(255, 0, 0), 2);
                }
            }
        }
        imshow( "Result window", image );
        
        if( waitKey(20)>=0 )
            break;
    } 
    return EXIT_SUCCESS;
}
 

