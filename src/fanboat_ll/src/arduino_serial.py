#!/usr/bin/env python
import struct
import serial
import sys
import rospy
from std_msgs.msg import UInt8
from fanboat_ll.msg import fanboatLL
from fanboat_ll.msg import fanboatMotors

class TheNode(object):
  '''Class to communicate with the fanboat. Carrick Detweiler 2015'''

  #Reads one byte and updates the checksum
  #def readOneByteAndChecksum(self):
     #c = ord(self.port.read())
     #self.checksum = (self.checksum + c) & 0xff
     #return c

  def __init__(self):

    rospy.init_node('arduino_serial')

    # Make sure to do chmod 777 /dev/ttyUSB0 first to make it r/w
    port_file = '/dev/ttyACM0'
    baud = 9600

    self.port = serial.Serial(port=port_file, baudrate=baud)
    self.publisher = rospy.Publisher('fanboatLL', fanboatLL, queue_size=10)
    rospy.Subscriber("servoAngle",)

	
  def handleFanboatMotors(self,motors):
    #bound check
    left = min(max(motors.left,0),360)

    #Convert to ints
    left = int(left)

    print(left)

    #Start byte
    self.port.write(int(left))
    self.port.write('\n')

  def main_loop(self):
    '''main loop get a packet from the port and parse it and publish it'''
    r = rospy.Rate(400)

    # Publish each byte as it comes in
    while not rospy.is_shutdown():
      # Wait for the start bytes 'CD'
      c = self.port.read()
      while c != 'C':
        c = self.port.read()
        print("resync got"),
        print(ord(c)),
        print(c)
      c = self.port.read()
      if c != 'D':
        print("resync2 got"),
        print(ord(c)),
        print(c)
        continue

      #Init the checksum
      self.checksum = 0xCD

      #msg.leftMotorSetting = self.readOneByteAndChecksum()
      #msg.rightMotorSetting = self.readOneByteAndChecksum()

      #verify the checksum
      c = ord(self.port.read())
      if c != self.checksum:
        print "Error, invalid checksum"
        continue

      #Publish
      self.publisher.publish(msg)
      
      r.sleep()
      

if __name__ == '__main__':
  a = TheNode()
  a.main_loop()


