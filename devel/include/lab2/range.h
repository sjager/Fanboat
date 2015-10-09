// Generated by gencpp from file lab2/range.msg
// DO NOT EDIT!


#ifndef LAB2_MESSAGE_RANGE_H
#define LAB2_MESSAGE_RANGE_H


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
struct range_
{
  typedef range_<ContainerAllocator> Type;

  range_()
    : header()
    , a(0.0)
    , b(0.0)  {
    }
  range_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , a(0.0)
    , b(0.0)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef double _a_type;
  _a_type a;

   typedef double _b_type;
  _b_type b;




  typedef boost::shared_ptr< ::lab2::range_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::lab2::range_<ContainerAllocator> const> ConstPtr;

}; // struct range_

typedef ::lab2::range_<std::allocator<void> > range;

typedef boost::shared_ptr< ::lab2::range > rangePtr;
typedef boost::shared_ptr< ::lab2::range const> rangeConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::lab2::range_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::lab2::range_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace lab2

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'lab2': ['/home/jimmy/Fanboat/src/lab2/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::lab2::range_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::lab2::range_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lab2::range_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::lab2::range_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lab2::range_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::lab2::range_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::lab2::range_<ContainerAllocator> >
{
  static const char* value()
  {
    return "57d14de24ee7d78a38ce2a221c57ebc1";
  }

  static const char* value(const ::lab2::range_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x57d14de24ee7d78aULL;
  static const uint64_t static_value2 = 0x38ce2a221c57ebc1ULL;
};

template<class ContainerAllocator>
struct DataType< ::lab2::range_<ContainerAllocator> >
{
  static const char* value()
  {
    return "lab2/range";
  }

  static const char* value(const ::lab2::range_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::lab2::range_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
\n\
float64 a\n\
float64 b\n\
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

  static const char* value(const ::lab2::range_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::lab2::range_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.a);
      stream.next(m.b);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct range_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::lab2::range_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::lab2::range_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "a: ";
    Printer<double>::stream(s, indent + "  ", v.a);
    s << indent << "b: ";
    Printer<double>::stream(s, indent + "  ", v.b);
  }
};

} // namespace message_operations
} // namespace ros

#endif // LAB2_MESSAGE_RANGE_H
