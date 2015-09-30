
(cl:in-package :asdf)

(defsystem "lab2-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "angle" :depends-on ("_package_angle"))
    (:file "_package_angle" :depends-on ("_package"))
    (:file "magnitude" :depends-on ("_package_magnitude"))
    (:file "_package_magnitude" :depends-on ("_package"))
  ))