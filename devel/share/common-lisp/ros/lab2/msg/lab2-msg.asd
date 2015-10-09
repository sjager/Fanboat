
(cl:in-package :asdf)

(defsystem "lab2-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "angle" :depends-on ("_package_angle"))
    (:file "_package_angle" :depends-on ("_package"))
    (:file "magnitude" :depends-on ("_package_magnitude"))
    (:file "_package_magnitude" :depends-on ("_package"))
    (:file "mux_control" :depends-on ("_package_mux_control"))
    (:file "_package_mux_control" :depends-on ("_package"))
    (:file "range" :depends-on ("_package_range"))
    (:file "_package_range" :depends-on ("_package"))
  ))