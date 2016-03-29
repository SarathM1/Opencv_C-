#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

#define distance 7
#define CONTOUR_LENGTH 250
 
Mat image, img_thresh,image_gray; 
CvMemStorage* storage = cvCreateMemStorage(0); 
RotatedRect contourcenter;
vector<vector<Point> > contours;                                                                                                 
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
        int i; 
        for(i=0;i<contours.size();i++)
        {
            double a = contourArea(contours[i]);
            if(a>largest_area)
            {
                largest_area = a;
                largest_contour_i = i;
            }
        }
        
        Scalar color (255,255,255);
        drawContours( image, contours, largest_contour_i, color, 2, 8, hierarchy, 0);
        imshow( "Result window", image );
        
        if( waitKey(20)>=0 )
            break;
    } 
    return EXIT_SUCCESS;
}
 

