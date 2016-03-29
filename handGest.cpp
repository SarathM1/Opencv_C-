#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

#define distance 7
#define CONTOUR_LENGTH 250
 
Mat image, output; 
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
    image = imread("hand2.jpg",0); 
    output.create(image.size(),image.type());
     
    if(!image.data){
        printf("Error: Couldn't open the image file.\n");
    }
     
    threshold(image, image, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    erode(image,image,0,Point(-1,-1),1);
    dilate(image,image,0,Point(-1,-1),1); 
    //cvShowImage("HelloWorld",image);
    findContours( image, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0) ); 
    /*while(contours && contours->total <= CONTOUR_LENGTH){  
      contours = contours->h_next;                                                                
    } */
//    drawContours( output, contours, -1,  2, 8, 1,Point(0,0) ); 
    drawContours( output, contours, -1, CV_RGB(0,255,0), 2, 8, hierarchy, 0, Point(0,0) );
      contourcenter =  minAreaRect(contours);
      armcenter.x = cvRound(contourcenter.center.x);
      armcenter.y = cvRound(contourcenter.center.y);
      circle(output,armcenter,10,CV_RGB(255,255,255),-1,8,0);
     
    imshow("HelloWorld",output); 
    cvWaitKey(0); 
    cvDestroyWindow("HelloWorld"); 
    return EXIT_SUCCESS;
}
 

