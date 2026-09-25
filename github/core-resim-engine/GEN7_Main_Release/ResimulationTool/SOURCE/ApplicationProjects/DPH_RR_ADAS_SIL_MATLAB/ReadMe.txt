
(1) Simulink Environment for doing the mapping between tObjects and detections.


(2) To use the environment, start Matlab and load the mat-File srr_workspace.mat into the base workspace.


(3) Open the Model tObjectsSimEnv.slx. You will find a constant specifiying a bus object. 
    You can overwrite some signals in this "tObjects" bus object in order to simulate one or more static objects. As the 5th input to the
	S-Function there is a bus object of type "SrrFasCanInBus". With the bus assignment block you can overwrite bus signal values.

(4) You will find a Script "CompileCode.m" which you can use to build your s-Function, so that you can integrate your mapping and functional
    code by providing static objects from the simulation environment.

(6) If you use Visual Studio, it is easy to build the s-Function with debug symbols and set break points in the s-function code in order
    to develop your mapping and radar functionalities.

(7) We implemented an input mapping which converts the physical values which are available as floating point values into integer values. The conversíon
	cuts the positions after the decimal point.
(8) Now the S-Function has two more output buses besides the FAS-CAN bus which already existed in the previous release. The second output of the s function is a bus for the FASL-CAN, the third output is the one for the FASR-CAN. 


