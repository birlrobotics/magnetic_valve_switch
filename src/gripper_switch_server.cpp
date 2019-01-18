#include<ros/ros.h>
#include<iostream>
#include<std_msgs/String.h>
#include<ur_msgs/IOStates.h>
#include<magnetic_valve_switch/switcher.h>
using namespace std;

bool IO_flag;
string Gripper_state_flag;

void io_callback(const ur_msgs::IOStates::ConstPtr& msg)
{   
    IO_flag = msg->digital_out_states[0].state;
}

bool switcher_cb(magnetic_valve_switch::switcher::Request  &req, magnetic_valve_switch::switcher::Response &res)
{
    Gripper_state_flag = req.gripper_state;
    cout<< "Gripper_state_flag : " << Gripper_state_flag << endl;
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "gripper_switch");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);
    ros::Publisher ur_script_pub;
    ros::Subscriber ur_state_sub;
    ros::ServiceServer gripper_state_service;

    ur_script_pub = nh.advertise<std_msgs::String>("ur_driver/URScript",1);
    ros::topic::waitForMessage<ur_msgs::IOStates>("/ur_driver/io_states", ros::Duration(10));
    ur_state_sub = nh.subscribe("/ur_driver/io_states", 100, io_callback);
    gripper_state_service = nh.advertiseService("gripper_switcher", switcher_cb);
    
    while(ros::ok())
    {
        if(Gripper_state_flag=="open")
        {   
            while(ros::ok())
            {  
                std_msgs::String cmd;
                cmd.data="set_digital_out(0,True)";
                ur_script_pub.publish(cmd);
                if(IO_flag == 1)
                {   
                    break;
                }
                ros::spinOnce();
                loop_rate.sleep();
            }
        }

        if(Gripper_state_flag=="close")
        {
            while(ros::ok())
            {   
                std_msgs::String cmd;
                cmd.data="set_digital_out(0,False)";
                ur_script_pub.publish(cmd);
                if(IO_flag == 0)
                {   
                    break;
                }
                ros::spinOnce();
                loop_rate.sleep();
            }
        }
    ros::spinOnce();
    loop_rate.sleep();
    }
    return 0;
}
