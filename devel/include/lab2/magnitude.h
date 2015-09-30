// Generated by gencpp from file lab2/magnitude.msg
// DO NOT EDIT!


#ifndef LAB2_MESSAGE_MAGNITUDE_H
#define LAB2_MESSAGE_MAGNITUDE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace lab2
{
template <class ContainerAllocator>
struct magnitude_
{
  typedef magnitude_<ContainerAllocator> Type;

  magnitude_()
    : header()
    , magnitude(0.0)  {
    }
  magnitude_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , magnitude(0.0)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef double _magnitude_type;
  _magnitude_type magnitude;




  typedef boost::shared_ptr< ::lab2::magnitude_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::lab2::magnitude_<ContainerAllocator> const> ConstPtr;

}; // struct magnitude_

typedef ::lab2::magnitude_<std::allocator<void> > magnitude;

typedef boost::shared_ptr< ::lab2::magnitude > magnitudePtr;
typedef boost::shared_ptr< ::lab2::magnitude const> magnitudeConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::lab2::magnitude_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::lab2::magnitude_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace lab2

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'lab2': ['/home/sjager2/Github/Fanboat/src/lab2/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::lab2::magnitude_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::lab2::magnitude_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lab2::magnitude_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lab2::magnitude_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lab2::magnitude_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lab2::magnitude_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::lab2::magnitude_<ContainerAllocator> >
{
  static const char* value()
  {
    return "5abe66de36ab14d7d6aa08d9f6bfc02c";
  }

  static const char* value(const ::lab2::magnitude_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x5abe66de36ab14d7ULL;
  static const uint64_t static_value2 = 0xd6aa08d9f6bfc02cULL;
};

template<class ContainerAllocator>
struct DataType< ::lab2::magnitude_<ContainerAllocator> >
{
  static const char* value()
  {
    return "lab2/magnitude";
  }

  static const char* value(const ::lab2::magnitude_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::lab2::magnitude_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
\n\
#Pitch roll and yaw in degrees\n\
float64 magnitude \n\
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

  static const char* value(const ::lab2::magnitude_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::lab2::magnitude_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.magnitude);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct magnitude_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::lab2::magnitude_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::lab2::magnitude_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "magnitude: ";
    Printer<double>::stream(s, indent + "  ", v.magnitude);
  }
};

} // namespace message_operations
} // namespace ros

#endif // LAB2_MESSAGE_MAGNITUDE_H