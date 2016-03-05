#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int16.h>
#include <sstream>
#include<iostream>
#include<vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<stack>
#include<utility>
#define F first
#define S second

using namespace std;
using namespace cv;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int16>("chatter", 1000);
  ros::Rate loop_rate(10);

    Mat src1 = imread("ps1.png");
    int rows=src1.rows;
    int cols=src1.cols;
    int thr=40;
    int rminx=rows,rminy=cols,rmaxx=0,rmaxy=0,gminx=rows,gminy=cols,gmaxx=0,gmaxy=0;
    vector<vector<int> > area(rows,vector<int>(cols,1));
    vector<vector<int> > mark(rows,vector<int>(cols,0));
    vector<pair<pair<int,int>,int> > log;
    pair<pair<int,int>,int> pt;


    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            if(src1.at<Vec3b>(i,j)[0]<thr && src1.at<Vec3b>(i,j)[1]<thr && src1.at<Vec3b>(i,j)[2]>255-thr)
            {
                if(i<rminx)
                    rminx=i;
                if(i>rmaxx)
                    rmaxx=i;
                if(j<rminy)
                    rminy=j;
                if(j>rmaxy)
                    rmaxy=j;
            }

            if(src1.at<Vec3b>(i,j)[0]<thr && src1.at<Vec3b>(i,j)[2]<thr && src1.at<Vec3b>(i,j)[1]>255-thr)
            {
                if(i<gminx)
                    gminx=i;
                if(i>gmaxx)
                    gmaxx=i;
                if(j<gminy)
                    gminy=j;
                if(j>gmaxy)
                    gmaxy=j;
            }

            if(src1.at<Vec3b>(i,j)[0]>255-thr && src1.at<Vec3b>(i,j)[1]>255-thr && src1.at<Vec3b>(i,j)[1]>255-thr)
                area[i][j]=0;

        }

    }
    cout<<"red x "<<(rmaxx+rminx)/2<<endl;
    cout<<"red y "<<(rmaxy+rminy)/2<<endl;
    cout<<"green x "<<(gmaxx+gminx)/2<<endl;
    cout<<"green y "<<(gmaxy+gminy)/2<<endl;

    //DFS
    stack<pair<pair<int,int>,int> > s;
    int hr=0;
    pt.F.F=(rmaxx+rminx)/2;
    pt.F.S=(rmaxy+rminy)/2;
    pt.S=hr;
    mark[pt.F.F][pt.F.S]=1;
    s.push(pt);
    while(1)
    {

        hr++;
        if(s.size()==0)
        {
            cout<<"Empty"<<endl;
            break;
        }
        pair<pair<int,int>,int> top_pt=s.top();
        s.pop();
        if(log.size()>0)
        {
            if(top_pt.S==log.back().S)
            {
                log.pop_back();
            }
        }
        log.push_back(top_pt);
        cout<<top_pt.F.F<<' '<<top_pt.F.S<<endl;
        if(top_pt.F.F==(gmaxx+gminx)/2 && top_pt.F.S==(gmaxy+gminy)/2)
        {
            cout<<"Found"<<endl;
            break;
        }
        int available=0;

        if(top_pt.F.S>=(gmaxy+gminy)/2)
        {
            for(int i=top_pt.F.F-1;i<top_pt.F.F+2;i++)
            {
                for(int j=top_pt.F.S+1;j>top_pt.F.S-2;j--)
                {
                    if(i>=0 && j>=0 && i<rows && j<cols)
                    {
                        if(area[i][j]==1 && mark[i][j]==0)
                        {
                            pt.F.F=i;
                            pt.F.S=j;
                            pt.S=hr;
                            s.push(pt);
                            mark[i][j]=1;
                            available++;
                        }
                    }

                }

            }

        }
        else
        {
            for(int i=top_pt.F.F-1;i<top_pt.F.F+2;i++)
            {
                for(int j=top_pt.F.S-1;j<top_pt.F.S+2;j++)
                {
                    if(i>=0 && j>=0 && i<rows && j<cols)
                    {
                        if(area[i][j]==1 && mark[i][j]==0)
                        {
                            pt.F.F=i;
                            pt.F.S=j;
                            pt.S=hr;
                            s.push(pt);
                            mark[i][j]=1;
                            available++;
                        }
                    }

                }

            }
        }


        if(available==0)
        {
            mark[top_pt.F.F][top_pt.F.S]=0;
            hr--;
            log.pop_back();
        }



    }

    cout<<"out"<<endl;
    /*char waste;
    cin>>waste;

    Mat src2 = imread("ps1.png");
    for(int i=0;i<log.size();i++)
    {
        src2.at<Vec3b>(log[i].F.F,log[i].F.S)[0]=255;
    }
    cvNamedWindow("path",CV_WINDOW_AUTOSIZE);
    imshow("path",src2);
    cvWaitKey();
    cout<<"Fineshed log printing"<<endl;*/




  long int counti = 0;
  while (ros::ok() && counti<(log.size())*2)
  {
    /*std_msgs::String msg;

    std::stringstream ss;
    ss << log[count].F.F << ' '<<log[count].F.S<<endl;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(msg);*/


    std_msgs::Int16 msg;
    if(counti%2==0)
        msg.data = log[counti/2].F.F;
    else
        msg.data = log[counti/2].F.S;
    ROS_INFO("%d", msg.data);
    chatter_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
    ++counti;
  }


  return 0;
}
