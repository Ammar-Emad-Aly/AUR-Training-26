import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
from std_srvs.srv import SetBool


class GoToGoalNode(Node):

    def __init__(self):
        super().__init__('go_to_goal')

        self.declare_parameter('target_x', 10.0)
        self.declare_parameter('target_y', 10.0)
        self.declare_parameter('linear_gain', 1.5)
        self.declare_parameter('angular_gain', 6.0)
        self.declare_parameter('distance_tolerance', 0.1)
        self.declare_parameter('angle_tolerance', 0.05)
        self.declare_parameter('loop_rate_hz', 20.0)

        self.target_x = self.get_parameter('target_x').value
        self.target_y = self.get_parameter('target_y').value
        self.linear_gain = self.get_parameter('linear_gain').value
        self.angular_gain = self.get_parameter('angular_gain').value
        self.distance_tolerance = self.get_parameter('distance_tolerance').value
        self.angle_tolerance = self.get_parameter('angle_tolerance').value
        self.loop_rate_hz = self.get_parameter('loop_rate_hz').value

        self.current_pose = None
        self.goal_reached = False
        self.active = False

        self.publisher = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        self.subscriber = self.create_subscription(Pose, '/turtle1/pose', self.pose_callback, 10)
        self.service = self.create_service(SetBool, '/start_movement', self.start_movement_callback)

        timer_period = 1.0 / self.loop_rate_hz
        self.timer = self.create_timer(timer_period, self.control_loop)

    def pose_callback(self, msg):
        self.current_pose = msg

    def start_movement_callback(self, request, response):
        if request.data:
            self.active = True
            self.goal_reached = False
            response.success = True
            response.message = 'Movement started'
        else:
            self.active = False
            cmd = Twist()
            cmd.linear.x = 0.0
            cmd.angular.z = 0.0
            self.publisher.publish(cmd)
            response.success = True
            response.message = 'Movement stopped'

        return response

    def normalize_angle(self, angle):
        while angle > math.pi:
            angle -= 2 * math.pi
        while angle < -math.pi:
            angle += 2 * math.pi
        return angle

    def control_loop(self):
        if not self.active or self.current_pose is None or self.goal_reached:
            return

        dx = self.target_x - self.current_pose.x
        dy = self.target_y - self.current_pose.y

        distance_error = math.sqrt(dx ** 2 + dy ** 2)

        target_angle = math.atan2(dy, dx)

        heading_error = self.normalize_angle(target_angle - self.current_pose.theta)

        if distance_error < self.distance_tolerance:
            cmd = Twist()
            cmd.linear.x = 0.0
            cmd.angular.z = 0.0
            self.publisher.publish(cmd)
            self.goal_reached = True
            self.active = False
            self.get_logger().info('Goal reached!')
            return

        if abs(heading_error) > self.angle_tolerance:
            linear_x = 0.0
            angular_z = self.angular_gain * heading_error
        else:
            linear_x = min(self.linear_gain * distance_error, 2.0)
            angular_z = self.angular_gain * heading_error

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


if __name__ == '__main__':
    main()