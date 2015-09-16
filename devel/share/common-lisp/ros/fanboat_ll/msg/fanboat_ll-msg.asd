
(cl:in-package :asdf)

(defsystem "fanboat_ll-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "fanboatLL" :depends-on ("_package_fanboatLL"))
    (:file "_package_fanboatLL" :depends-on ("_package"))
    (:file "fanboatMotors" :depends-on ("_package_fanboatMotors"))
    (:file "_package_fanboatMotors" :depends-on ("_package"))
  ))