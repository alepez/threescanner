# threescanner

> 3D scanning library which creates a point cloud using a method of scanning
> involving a projector and a camera.

It's a C++ implementation of [Kyle McDonald's structured light](https://code.google.com/p/structured-light/).

With this software you can get a three dimensional point cloud of a real object.

You only need a camera (even a webcam) and a projector.


## Build

    make all

## Install

    make install

## Development

Build debug version:

    make DEBUG=1 all

Build documents:

    make doc
