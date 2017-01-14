# pyrosim
A Python Robot Simulator.

1. For Windows users.

  1. Follow the tutorial [here](https://youtu.be/tNGrxDftclA).

2. For Mac and Linux users (currently not available for Windows):

  1. Run build.sh:

  `$ sh build.sh`

  This takes three to five minutes. The output should look like this:
  
  `Unpacking ode-0.12.tar.bz2...done`

  `Building ode-0.12...done`

  `Building simulator...done`

  2. If you are installing from Linux and you do not get the third
  `done' confirmation, install these additional packages
  ```
  $ sudo apt-get install build-essential
  $ sudo apt-get install freeglut3-dev
  ```
  then re-install Pyrosim:
  ```
  $ sh build.sh
  ```

Once you get all three `done' confirmations, test the package by running
```bash
$ python Demo_00_Empty_Sim.py 
```
This should bring up an OpenGL window displaying an empty, virtual world.
The window will close on its own after a few seconds.

Now you can start making robots [here](https://www.reddit.com/r/ludobots/wiki/pyrosim/simulation),
starting at step #3.

