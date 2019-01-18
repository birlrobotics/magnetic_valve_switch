#!/usr/bin/env python
import rospy
import sys
from std_msgs.msg import String
from ur_msgs.msg import IOStates
from magnetic_valve_switch.srv import switcher, switcherRequest,switcherResponse
import ipdb

_rate = 10

Received_Flag = None
Gripper_state = None

def callback(msg):
    global Received_Flag 
    Received_Flag = msg.digital_out_states[0].state

def service_cb(req):
    global Gripper_state
    res = switcherResponse()
    Gripper_state = req.gripper_state
    res.success = True
    return res

def main():
    rospy.init_node("io_swicth", anonymous=True)
    pub = rospy.Publisher('ur_driver/URScript', String, queue_size=1)
    rospy.wait_for_message('/ur_driver/io_states',IOStates,timeout=5)
    sub = rospy.Subscriber("/ur_driver/io_states", IOStates, callback)
    service = rospy.Service('gripper_switcher', switcher, service_cb)
    
    rate = rospy.Rate(_rate)
    while not rospy.is_shutdown():
        if Gripper_state == "open":
            while not rospy.is_shutdown():
                pub_msg = String()
                pub_msg.data = "set_digital_out(0,True)"
                pub.publish(pub_msg)
                rate.sleep()
                if Received_Flag == True:
                    break

        elif Gripper_state == "close":
            while not rospy.is_shutdown():
                pub_msg = String()
                pub_msg.data = "set_digital_out(0,False)"
                pub.publish(pub_msg)
                rate.sleep()
                if Received_Flag == False:
                    break


if __name__ == '__main__':
    try:
        sys.exit(main())   
    except rospy.ROSInterruptException:
        pass
        