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
  def readOneByteAndChecksum(self):
     c = ord(self.port.read())
     self.checksum = (self.checksum + c) & 0xff
     return c

  #Read two bytes in LSB order and update the checksum
  def readTwoByteAndChecksum(self):
     c = ord(self.port.read())
     tmp = c
     self.checksum = (self.checksum + c) & 0xff
     c = ord(self.port.read())
     self.checksum = (self.checksum + c) & 0xff
     return tmp + (c << 8)

  def handleFanboatMotors(self,motors):
    #bound check
    left = min(max(motors.left,-1.0),1.0)
    right = min(max(motors.right,-1.0),1.0)
    #convert from [-1.0 to 1.0] to [0 to 180]
    left = ((left+1.0)/2.0) * 180
    right = ((right+1.0)/2.0) * 180

    #Convert to ints
    left = int(left)
    right = int(right)

    print(left),
    print(right)

    #Start byte
    self.port.write(chr(0xCD));
    self.port.write(chr(left))
    self.port.write(chr(right))
    #checksum
    self.port.write(chr((0x23 + left + right) & 0xff))

  def __init__(self):

    rospy.init_node('fanboat_serial')

    # Make sure to do chmod 777 /dev/ttyUSB0 first to make it r/w
    port_file = '/dev/ttyUSB0'
    baud = 38400

    self.port = serial.Serial(port=port_file, baudrate=baud)
    self.publisher = rospy.Publisher('fanboatLL', fanboatLL, queue_size=10)
    rospy.Subscriber("motors",fanboatMotors,self.handleFanboatMotors)


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

      msg = fanboatLL()
      msg.header.stamp = rospy.Time.now()
      msg.roll = self.readTwoByteAndChecksum()/100.0
      msg.pitch = self.readTwoByteAndChecksum()/100.0
      msg.yaw = self.readTwoByteAndChecksum()/100.0 - 360
      msg.a0 = self.readTwoByteAndChecksum()
      msg.a1 = self.readTwoByteAndChecksum()
      msg.a2 = self.readTwoByteAndChecksum()
      msg.a3 = self.readTwoByteAndChecksum()
      msg.a6 = self.readTwoByteAndChecksum()
      msg.a7 = self.readTwoByteAndChecksum()

      msg.leftMotorSetting = self.readOneByteAndChecksum()
      msg.rightMotorSetting = self.readOneByteAndChecksum()

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


