# Omni Wheeled Robot Control

An omni wheeled robot with configuration as shown below is controlled. Three operations are performed namely
 
1. Find the angle of the robot with the wall 
2. Make the robot parallel to the wall
3. Translate it to a distance of 30cm from the wall

### Robot Configuration

This is the configuration of the wheels and form of the robot.


### Installation

Download the folder containing the input, output and source files. Alternately input files can be created using the same format as provided. Choose a local directory in your system and add its path.
```
$ cd /LocalDirectory 
$ git clone https://github.com/nilayoza97/omni_robot.git
```
### Compile and Run the program
```
$ make
$ ./omni_control
```
### Method Explanation

1. To find the angle, simply extend the sensor distance lines and form a rectancle with the inclined robot. The triangle formed with the wall gives you the angle Theta.

2. The robot is made parallel to the wall by connecting all the wheels which form the circle and then finding the length of the arc to be traced by all the wheels in order to cover an angle Theta with the wall.

3. Translating the robot to 30cm from the wall requires the current distance of the robot from the wall, which can be found out by using simple geometry as shown. Then give the pulse command to the side motors (as viewed fromm the top) so that they cover the required distance to bring the robot 30cm away from the wall. The front and rear motors will act as caster wheels in this operation as there is no need for rotation.
