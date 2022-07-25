# MPGL

MPGL is simple and precise graphics library written entirely in the C++20 standard. It construction allows to interact with lower abstraction components without need to do everything manualy from scratch (e. g. the default shaders of build-in primitives can be changed with one method). The library is written in the way that maximilizes usage of the newest C++20 features like concepts and views. It is written using the OpenGL specification.

## Version

The current version of library does not support everything that was intended to be included. The 3D part of the library is not implemented so far as so as reading all types of files from the parsed formats (Progressive JPEG's are not suported and so on). The image saving, data compression and many more are yet to be included. Much more testing is needed to be done. For that the library is concidered to be in an alpha stage.

## External libraries

The MPGL uses the

* GLFW 3.3.4
* GLAD
