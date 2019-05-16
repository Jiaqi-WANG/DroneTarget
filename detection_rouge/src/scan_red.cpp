#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <detection_rouge/barycentre_data.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64.h>
 
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
 #include "cv.h"
 #include "math.h"
#include <stdio.h>

using namespace std;
using namespace cv; 

 char location[20];
 int h_min=0;
 int s_min=110;
 int v_min=60;
 
 int h_max=10;
 int s_max=255;
 int v_max=255;
 	int b=1;
Mat cameraFeed;
Mat HSV;
Mat threshold_ori;
Mat scanRed(Mat frame);
ros::Publisher barycentre_data;


class ImageConverter
{
  ros::NodeHandle it;
  image_transport::ImageTransport n;
  image_transport::Subscriber rgb_sub;

 
public:
  ImageConverter()
    : n(it)
  {
    // Subscrive to input video feed and publish output video feed
    rgb_sub = n.subscribe("/image_converter/output_video", 1, &ImageConverter::rgbCallback, this);
    barycentre_data = it.advertise<detection_rouge::barycentre_data>("/barycentre_data",1000);
  }
 


void rgbCallback(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImageConstPtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); // Caution the type here.
    }
    catch (cv_bridge::Exception& ex)
    {
        ROS_ERROR("cv_bridge exception in rgbcallback: %s", ex.what());
        return;
    }
 
 
    scanRed(cv_ptr->image);
    
    //delay 10ms so that screen can refresh.
    //image will not appear without this waitKey() command
    waitKey(1);
 
}
};
 
int main(int argc, char *argv[])
{


    ros::init(argc, argv, "scan");
    ImageConverter ic;
 
//	topic name of HornedSungem
    

    ros::spin();
    return 0;
}

Mat scanRed(Mat frame){ 
	Mat tempImage=frame.clone();
	float x=0,y=0,width=0,height=0;
	cvtColor(frame,frame,CV_BGR2HSV);
	Scalar hsv_min(h_min,s_min,v_min);
	Scalar hsv_max(h_max,s_max,v_max);
	Mat dstImage=Mat::zeros(frame.size(),CV_8U);
	inRange(frame,hsv_min,hsv_max,dstImage);
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5));
	morphologyEx(dstImage,dstImage,MORPH_ERODE,element,Point(-1,-1),1);
	morphologyEx(dstImage,dstImage,MORPH_DILATE,element,Point(-1,-1),4);
	imshow("dst",dstImage);
	vector<vector<Point> >contours;
	findContours(dstImage,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
	vector<Rect>rect(contours.size());
	for(int i=0;i<contours.size();i++)
	{

		rect[i]=boundingRect(contours[i]);
		x=rect[i].x;
		y=rect[i].y;
		width=rect[i].width;
		height=rect[i].height;
		sprintf(location,"X : %.1f, Y : %.1f",x+0.5*width,y+0.5*height);
		if(height*width>2000){
		rectangle(tempImage,Point(x,y),Point(x+width,y+height),Scalar(0,255,0),2);
		Point p(x+0.5*width,y+0.5*height);
		circle(tempImage,p,4,Scalar(0,255,0),-1);
		cv::putText(tempImage,location,Point(x+0.5*width+5,y+0.5*height+5), cv ::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2, 8, 0);
		detection_rouge::barycentre_data myMsg;
		myMsg.data.resize(3);
		myMsg.data[0] = x+0.5*width;
		myMsg.data[1] = y+0.5*height;
		myMsg.data[2] = height;
		int a= sizeof(myMsg.data)/sizeof(float);
		if(a == 6){
		b++;
		myMsg.No = b;}
		barycentre_data.publish(myMsg);}
	}
	imshow("result",tempImage);
	return tempImage;
}




