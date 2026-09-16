import math

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist
from turtlesim.msg import Pose


TARGET_X = 10.0
TARGET_Y = 10.0


class GoToGoalNode(Node):

    def __init__(self):
        super().__init__('go_to_goal_node')

       
        self.kp_linear = 1.5
        self.kp_angular = 6.0

        
        self.distance_tolerance = 0.1
        self.angle_tolerance = 0.05

        
        self.current_pose = None

        
        self.goal_reached = False

       
        self.publisher = self.create_publisher(Twist,'/turtle1/cmd_vel', 10 )
           
 
        self.subscriber = self.create_subscription(Pose,'/turtle1/pose', self.pose_callback,10 )


       
        self.timer = self.create_timer( 0.05,self.control_loop )


    def pose_callback(self, msg):

        self.current_pose = msg

    def normalize_angle(self, angle):

        while angle > math.pi:
            angle -= 2 * math.pi

        while angle < -math.pi:
            angle += 2 * math.pi

        return angle

    def control_loop(self):

        if self.current_pose is None or self.goal_reached:
            return

     
        dx = TARGET_X - self.current_pose.x
        dy = TARGET_Y - self.current_pose.y

        distance_error = math.sqrt(dx ** 2 + dy ** 2)

        target_angle = math.atan2(dy, dx)

        heading_error = self.normalize_angle(
            target_angle - self.current_pose.theta
        )


        if distance_error < self.distance_tolerance:

            cmd = Twist()

            cmd.linear.x = 0.0
            cmd.angular.z = 0.0

            self.publisher.publish(cmd)

            self.goal_reached = True

            self.get_logger().info('Goal reached!')

            return

       

        if abs(heading_error) > self.angle_tolerance:

           

            linear_x = 0.0

            angular_z = self.kp_angular * heading_error

        else:

            

            linear_x = min(
                self.kp_linear * distance_error,
                2.0
            )

            angular_z = self.kp_angular * heading_error

        

        cmd = Twist()

        cmd.linear.x = linear_x
        cmd.angular.z = angular_z

        self.publisher.publish(cmd)


def main():

    rclpy.init()

    node = GoToGoalNode()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


