// Generated by gencpp from file ball_detector/ballLocation.msg
// DO NOT EDIT!


#ifndef BALL_DETECTOR_MESSAGE_BALLLOCATION_H
#define BALL_DETECTOR_MESSAGE_BALLLOCATION_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace ball_detector
{
template <class ContainerAllocator>
struct ballLocation_
{
  typedef ballLocation_<ContainerAllocator> Type;

  ballLocation_()
    : header()
    , imageWidth(0)
    , imageHeight(0)
    , x(0.0)
    , y(0.0)
    , radius(0.0)  {
    }
  ballLocation_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , imageWidth(0)
    , imageHeight(0)
    , x(0.0)
    , y(0.0)
    , radius(0.0)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef uint32_t _imageWidth_type;
  _imageWidth_type imageWidth;

   typedef uint32_t _imageHeight_type;
  _imageHeight_type imageHeight;

   typedef double _x_type;
  _x_type x;

   typedef double _y_type;
  _y_type y;

   typedef double _radius_type;
  _radius_type radius;




  typedef boost::shared_ptr< ::ball_detector::ballLocation_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ball_detector::ballLocation_<ContainerAllocator> const> ConstPtr;

}; // struct ballLocation_

typedef ::ball_detector::ballLocation_<std::allocator<void> > ballLocation;

typedef boost::shared_ptr< ::ball_detector::ballLocation > ballLocationPtr;
typedef boost::shared_ptr< ::ball_detector::ballLocation const> ballLocationConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::ball_detector::ballLocation_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::ball_detector::ballLocation_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace ball_detector

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'ball_detector': ['/home/jimmy/Fanboat/src/ball_detector/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::ball_detector::ballLocation_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::ball_detector::ballLocation_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ball_detector::ballLocation_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ball_detector::ballLocation_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ball_detector::ballLocation_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ball_detector::ballLocation_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::ball_detector::ballLocation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "046f798a75776068674c89e287314673";
  }

  static const char* value(const ::ball_detector::ballLocation_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x046f798a75776068ULL;
  static const uint64_t static_value2 = 0x674c89e287314673ULL;
};

template<class ContainerAllocator>
struct DataType< ::ball_detector::ballLocation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ball_detector/ballLocation";
  }

  static const char* value(const ::ball_detector::ballLocation_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::ball_detector::ballLocation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
#The iamge width and height in pixels\n\
uint32 imageWidth\n\
uint32 imageHeight\n\
\n\
#The location of the ball center in x,y (width,height)\n\
float64 x\n\
float64 y\n\
#The radius of the detected ball\n\
float64 radius\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
";
  }

  static const char* value(const ::ball_detector::ballLocation_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::ball_detector::ballLocation_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.imageWidth);
      stream.next(m.imageHeight);
      stream.next(m.x);
      stream.next(m.y);
      stream.next(m.radius);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct ballLocation_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::ball_detector::ballLocation_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::ball_detector::ballLocation_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "imageWidth: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.imageWidth);
    s << indent << "imageHeight: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.imageHeight);
    s << indent << "x: ";
    Printer<double>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<double>::stream(s, indent + "  ", v.y);
    s << indent << "radius: ";
    Printer<double>::stream(s, indent + "  ", v.radius);
  }
};

} // namespace message_operations
} // namespace ros

#endif // BALL_DETECTOR_MESSAGE_BALLLOCATION_H
