# threescanner

> 3D scanning library which creates a point cloud using **structured light**,
> a method of scanning involving a projector and a camera.

With this software you can get a three dimensional point cloud of a real object.

You only need a camera (even a webcam) and a projector.

You can use the provided scanner+projector applications or include the library
in your own project using the API.

Projector and scanner are in separate processes and they communicate via tcp,
so they can reside in different computers.

*Scanner* and *projector* are implementation-agnostic, so different algorithms can
be used .

At the moment, only [*ThreePhase* by *Kyle McDonald*](https://code.google.com/p/structured-light/)
algorithm is implemented, but it's possible to add [other implementations](https://sites.google.com/site/structuredlight/techniques),
like *binary codes*, *gray codes* etc...

**THIS PROJECT ISN'T FINISHED YET**

What to do to have a working scanner:

 - input from camera (with opencv)
 - threephase: syncronization between scanner (camera) and projector
 - a GUI

## Build

### Dependencies

 - [glew](http://glew.sourceforge.net/) (tested with 1.10)
 - [glfw](http://www.glfw.org/) (tested with 3.0)
 - [Point Cloud Library](http://www.pointclouds.org) (tested with 1.7)
 - [boost](http://www.boost.org/) (tested with 1.52)

To build:

    make thirdparty # download and build dependencies
    make all

## Development

Build debug version:

    make DEBUG=1 all

## What is structured light?

Structured light is a method of 3D scanning where we project a known pattern onto an unknown surface and by analyzing the deformation (warping) of the known pattern we can mathematically reconstruct the surface virtually. 

Imagine a room full of perfectly matte white objects. If you put a projector and a camera in the room, and project a pattern such that every column (or row) has a unique color, then you can create a correspondence between what the projector "sees", and what the camera sees. This correspondence allows you to triangulate the position of every projected pixel and determine its depth. If you project one frame for every frame the camera captures, you can extrapolate 3D information at your camera's framerate.

Unfortunately, rooms are not generally filled with matte white objects. Whats more, camera-projector synchronization can be difficult outside of hardware, and most cameras distort the scene to some degree. But there are lots of techniques for projecting more information onto the scene in order to overcome these limitations.

### References:

 - <https://sites.google.com/site/structuredlight/techniques>
 - <https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxzdHJ1Y3R1cmVkbGlnaHR8Z3g6MjkyN2FkNzcyMGRlOWQ3MA>
 - <http://public.vrac.iastate.edu/~song/publications/papers/2006-oe-realtime.pdf>
 - <http://www5.informatik.uni-erlangen.de/Forschung/Publikationen/2012/Schmalz12-RSS.pdf>
 - <http://fab.cba.mit.edu/content/processes/structured_light/>
 - <http://mesh.brown.edu/3dpgp-2009/homework/hw2/hw2.html>
 - <http://www.amitkagrawal.com/cvpr11/SL/index.html>
 - <http://www.hackengineer.com/structured-light-vs-microsoft-kinect/>
 - <http://www.instructables.com/id/Structured-Light-3D-Scanning/>
