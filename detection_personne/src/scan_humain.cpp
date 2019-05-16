#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64.h>
#include <detection_rouge/barycentre_data.h>
 
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
 	int b=1;
 
Mat cameraFeed;
Mat HSV;
Mat threshold_ori;
ros::Publisher barycentre_datap;
Mat scanhumain(Mat frame);
cv_bridge::CvImageConstPtr cv_ptr;
 
void rgbCallback(const sensor_msgs::ImageConstPtr& msg)
{

    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); // Caution the type here.
    }
    catch (cv_bridge::Exception& ex)
    {
        ROS_ERROR("cv_bridge exception in rgbcallback: %s", ex.what());
        exit(-1);
    }

    scanhumain(cv_ptr->image);
    //delay 10ms so that screen can refresh.
    //image will not appear without this waitKey() command
    waitKey(1);
 
}
 
 
int main(int argc, char *argv[])
{


    ros::init(argc, argv, "HornedSungemGrabber");
 
    ros::NodeHandle n,nh;
 
//	topic name of HornedSungem
    ros::Subscriber rgb_sub = n.subscribe("/image_converter/output_video", 1, rgbCallback);
    ROS_INFO("Subscribe to the HS color image topic.");
    barycentre_datap = nh.advertise<detection_rouge::barycentre_data>("/barycentre_data_personne",1000);
    ros::spin();
    return 0;
}

Mat scanhumain(Mat frame){
	cv::HOGDescriptor hog; 
 
    float x=0,y=0,width=0,height=0;

    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   
    vector<vector<Point> >contours;

    std::vector<cv::Rect> regions;
    hog.detectMultiScale(frame, regions, 0, cv::Size(8, 8), cv::Size(16, 16), 1.05, 1);
 

    for (size_t i = 0; i < regions.size(); i++)
    {
        cv::rectangle(frame, regions[i], cv::Scalar(0, 255, 0), 3);
		x=regions[i].x;
		y=regions[i].y;
		width=regions[i].width;
		height=regions[i].height;
		if(height*width>15000){
		sprintf(location,"X : %.1f, Y : %.1f",x+0.5*width,y+0.5*height);
		Point p(x+0.5*width,y+0.5*height);
		circle(frame,p,4,Scalar(0,255,0),-1);
		cv::putText(frame,location,Point(x+0.5*width+5,y+0.5*height+5), cv ::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2, 8, 0);
		detection_rouge::barycentre_data myMsg;
		myMsg.data.resize(3);
		myMsg.data[0] = x+0.5*width;
		myMsg.data[1] = y+0.5*height;
		myMsg.data[2] = height;
		int a= sizeof(myMsg.data)/sizeof(float);
		if(a == 6){
			b++;
			myMsg.No = b;}
		barycentre_datap.publish(myMsg);}
    }
 
    cv::imshow("hog", frame);
    return frame;
}

