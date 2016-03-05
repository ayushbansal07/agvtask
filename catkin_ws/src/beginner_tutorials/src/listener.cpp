#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int16.h>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat src1= imread("ps1.png");
long int counti=0;
int a,b;
void chatterCallback(const std_msgs::Int16 msg)
{
  ROS_INFO("I heard: [%d]", msg.data);
  if(counti%2==0)
  {
    a=msg.data;
  }
  else
  {
    b=msg.data;
    src1.at<Vec3b>(b,a)[0]=255;
    imwrite("ps1_output.png",src1);
  }
  counti++;

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::spin();

  return 0;
}
