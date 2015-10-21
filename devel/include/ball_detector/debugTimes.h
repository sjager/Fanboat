// Generated by gencpp from file ball_detector/debugTimes.msg
// DO NOT EDIT!


#ifndef BALL_DETECTOR_MESSAGE_DEBUGTIMES_H
#define BALL_DETECTOR_MESSAGE_DEBUGTIMES_H


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
struct debugTimes_
{
  typedef debugTimes_<ContainerAllocator> Type;

  debugTimes_()
    : header()
    , labels()
    , times()  {
    }
  debugTimes_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , labels(_alloc)
    , times(_alloc)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _labels_type;
  _labels_type labels;

   typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _times_type;
  _times_type times;




  typedef boost::shared_ptr< ::ball_detector::debugTimes_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::ball_detector::debugTimes_<ContainerAllocator> const> ConstPtr;

}; // struct debugTimes_

typedef ::ball_detector::debugTimes_<std::allocator<void> > debugTimes;

typedef boost::shared_ptr< ::ball_detector::debugTimes > debugTimesPtr;
typedef boost::shared_ptr< ::ball_detector::debugTimes const> debugTimesConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::ball_detector::debugTimes_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::ball_detector::debugTimes_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace ball_detector

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'ball_detector': ['/home/kaitlyn/Desktop/Git_Repos/Fanboat/src/ball_detector/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::ball_detector::debugTimes_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::ball_detector::debugTimes_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ball_detector::debugTimes_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::ball_detector::debugTimes_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ball_detector::debugTimes_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::ball_detector::debugTimes_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::ball_detector::debugTimes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "24f9f017187aacc6186ce1f2d40d694e";
  }

  static const char* value(const ::ball_detector::debugTimes_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x24f9f017187aacc6ULL;
  static const uint64_t static_value2 = 0x186ce1f2d40d694eULL;
};

template<class ContainerAllocator>
struct DataType< ::ball_detector::debugTimes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "ball_detector/debugTimes";
  }

  static const char* value(const ::ball_detector::debugTimes_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::ball_detector::debugTimes_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
#string identifying the corresponding element time \n\
string[] labels\n\
#the execution time in seconds\n\
float64[] times\n\
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

  static const char* value(const ::ball_detector::debugTimes_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::ball_detector::debugTimes_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.labels);
      stream.next(m.times);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct debugTimes_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::ball_detector::debugTimes_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::ball_detector::debugTimes_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "labels[]" << std::endl;
    for (size_t i = 0; i < v.labels.size(); ++i)
    {
      s << indent << "  labels[" << i << "]: ";
      Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.labels[i]);
    }
    s << indent << "times[]" << std::endl;
    for (size_t i = 0; i < v.times.size(); ++i)
    {
      s << indent << "  times[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.times[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // BALL_DETECTOR_MESSAGE_DEBUGTIMES_H
