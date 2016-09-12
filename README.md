# pyrosim
A Python Robot Simulator.

To install, navigate into the pyrosim directory and run

> $ sh build.sh

If all goes well, the output should look like this:

> Downloading ode-0.12...done
> Unpacking ode-0.12.tar.bz2...done
> Building ode-0.12...done
> Building simulator...done

When done, test the package by running

> python empty.py 

This should bring up an OpenGL window into a desolate virtual world for about 3 seconds. Do not avert your eyes!

For now, the installation script is Linux only. Should work on MacOS if wget is installed (by default it isn't).
