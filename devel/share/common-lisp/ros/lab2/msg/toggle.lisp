; Auto-generated. Do not edit!


(cl:in-package lab2-msg)


;//! \htmlinclude toggle.msg.html

(cl:defclass <toggle> (roslisp-msg-protocol:ros-message)
  ((toggle
    :reader toggle
    :initarg :toggle
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass toggle (<toggle>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <toggle>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'toggle)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name lab2-msg:<toggle> is deprecated: use lab2-msg:toggle instead.")))

(cl:ensure-generic-function 'toggle-val :lambda-list '(m))
(cl:defmethod toggle-val ((m <toggle>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader lab2-msg:toggle-val is deprecated.  Use lab2-msg:toggle instead.")
  (toggle m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <toggle>) ostream)
  "Serializes a message object of type '<toggle>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'toggle) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <toggle>) istream)
  "Deserializes a message object of type '<toggle>"
    (cl:setf (cl:slot-value msg 'toggle) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<toggle>)))
  "Returns string type for a message object of type '<toggle>"
  "lab2/toggle")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'toggle)))
  "Returns string type for a message object of type 'toggle"
  "lab2/toggle")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<toggle>)))
  "Returns md5sum for a message object of type '<toggle>"
  "301e9e6fc0ec5b2acb90ccde2488159e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'toggle)))
  "Returns md5sum for a message object of type 'toggle"
  "301e9e6fc0ec5b2acb90ccde2488159e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<toggle>)))
  "Returns full string definition for message of type '<toggle>"
  (cl:format cl:nil "bool toggle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'toggle)))
  "Returns full string definition for message of type 'toggle"
  (cl:format cl:nil "bool toggle~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <toggle>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <toggle>))
  "Converts a ROS message object to a list"
  (cl:list 'toggle
    (cl:cons ':toggle (toggle msg))
))
