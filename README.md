IGTLPolyToPointExample
======================

Example program to receive OpenIGTLink POLYDATA message and extract points


Description
===========

This is a simple program that demonstrates how to receive a POLYDATA message from
a server program and extract the coordinates of the point. It will output
the coordinates as comma-separated values on the standard output.


Install
=======

The program is written using CMake. Run CMake to generate a Makefile (Linux/Mac)
or project file (Windows), and build. On Linux/Mac:

```
cd <working directory>
git clone https://github.com/tokjun/IGTLPolyToPointExample
mkdir IGTLPolyToPointExample-build
cd IGTLPolyToPointExample-build
cmake -DOpenIGTLink_DIR:STRING=<OpenIGTLink binary directory> ../IGTLPolyToPointExample
make
```


Tutorial using Slicer
=====================

First, set up 3D Slicer:

1. Start 3D Slicer, and load a surface model (VTK or STL file).
2. Open the "OpenIGTLink IF" module
3. In the "OpenIGTLink IF" module, create a connector and configure as a server.
4. Make sure the port number is 18944.
5. Start the OpenIGTLink server by clicking the "Active" button. The status should become "WAIT".

Then, open the terminal and star the example:

1. Move to the folder that contains the executive file for the example program.
2. Run the example program with the following command: "IGTLPolyToPointExample localhost 18944"
3. Confirm that the status on the 3D Slicer OpenIGTLink IF module becomes "ON".

Push the surface model from 3D Slicer to the example program:

1. In the OpenIGTLink IF module, scroll down to the "I/O Configuration" section. There is a tree list.
2. Under IGTLConnector in the tree list, click 'OUT'
3. Select the name of the surface model from the selector below the tree list interface.
4. Click '+' button next to the selector. The surface model shows up under 'OUT'. (It may be hidden. If you see '+' next to 'OUT', please click it to expand.)
5. Click the surface model in the tree list.
6. Click the 'Send' button below the tree list (on the right).
7. The example program will start receiving a POLYDATA message and putting the coordinates to the standard output (console).










