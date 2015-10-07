; Auto-generated. Do not edit!


(cl:in-package lab2-msg)


;//! \htmlinclude mux_control.msg.html

(cl:defclass <mux_control> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (state
    :reader state
    :initarg :state
    :type cl:fixnum
    :initform 0))
)

(cl:defclass mux_control (<mux_control>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <mux_control>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'mux_control)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name lab2-msg:<mux_control> is deprecated: use lab2-msg:mux_control instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <mux_control>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader lab2-msg:header-val is deprecated.  Use lab2-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'state-val :lambda-list '(m))
(cl:defmethod state-val ((m <mux_control>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader lab2-msg:state-val is deprecated.  Use lab2-msg:state instead.")
  (state m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <mux_control>) ostream)
  "Serializes a message object of type '<mux_control>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'state)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <mux_control>) istream)
  "Deserializes a message object of type '<mux_control>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<mux_control>)))
  "Returns string type for a message object of type '<mux_control>"
  "lab2/mux_control")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'mux_control)))
  "Returns string type for a message object of type 'mux_control"
  "lab2/mux_control")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<mux_control>)))
  "Returns md5sum for a message object of type '<mux_control>"
  "6b8ba4e04f882f0e14fe3dd848d01f25")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'mux_control)))
  "Returns md5sum for a message object of type 'mux_control"
  "6b8ba4e04f882f0e14fe3dd848d01f25")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<mux_control>)))
  "Returns full string definition for message of type '<mux_control>"
  (cl:format cl:nil "Header header~%~%int8 state~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'mux_control)))
  "Returns full string definition for message of type 'mux_control"
  (cl:format cl:nil "Header header~%~%int8 state~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <mux_control>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <mux_control>))
  "Converts a ROS message object to a list"
  (cl:list 'mux_control
    (cl:cons ':header (header msg))
    (cl:cons ':state (state msg))
))
