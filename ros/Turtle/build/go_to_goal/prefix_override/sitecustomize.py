import sys
if sys.prefix == 'D:\\ros\\Turtle\\.pixi\\envs\\default':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = 'D:\\ros\\Turtle\\install\\go_to_goal'
