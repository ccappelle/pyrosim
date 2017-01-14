# pyrosim
A Python Robot Simulator.

## For Windows users.

Follow the tutorial here.

## For Mac and Linux users (currently not available for Windows):

Run build.sh:
```bash
$ sh build.sh
```
This takes three to five minutes. The output should look like this:
```
Unpacking ode-0.12.tar.bz2...done
Building ode-0.12...done
Building simulator...done
```

If you are installing from Linux and you do not get the third
`done' confirmation, install these additional packages
```
$ sudo apt-get install build-essential
$ sudo apt-get install freeglut3-dev
```
then re-install Pyrosim:
```
$ sh build.sh
```

Test the package by running
```bash
$ python Demo_00_Empty_Sim.py 
```
This should bring up an OpenGL window displaying an empty, virtual world.
The window will close on its own after a few seconds.

Start making robots [here](https://www.reddit.com/r/ludobots/wiki/pyrosim/simulation),
starting at step #3.

