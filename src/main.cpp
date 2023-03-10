#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

#include "ros/ros.h"
#include "std_msgs/Bool.h"

ros::Publisher pub_flag;
std_msgs::Bool flag_msg;
std::atomic<bool> flag(false);
std::mutex flag_mutex;

void function1()
{

    std::cout << "Hello function1" << std::endl;

    {
        std::lock_guard<std::mutex> lock(flag_mutex);
        flag = true;
    }

    flag_msg.data = flag;
    pub_flag.publish(flag_msg);
}

void function2()
{

    std::cout << "Hello function2" << std::endl;

    {
        std::lock_guard<std::mutex> lock(flag_mutex);
        flag = false;
    }

    flag_msg.data = flag;
    pub_flag.publish(flag_msg);
}

void threadLoop1()
{

    ros::Rate loop_rate(10);

    while (ros::ok())
    {

        function1();

        loop_rate.sleep();

        ros::spinOnce();
    }
}

void threadLoop2()
{

    ros::Rate loop_rate(10);

    while (ros::ok())
    {

        function2();

        loop_rate.sleep();

        ros::spinOnce();
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "thread_syncronization");
    ros::NodeHandle nh;
    pub_flag = nh.advertise<std_msgs::Bool>("/flag", 10000);

    std::thread thread1(threadLoop1);
    std::thread thread2(threadLoop2);

    thread1.detach();
    thread2.detach();

    ros::spin();

    return 0;
}