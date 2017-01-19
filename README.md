# pyrosim: A python robot simulator. 

## Installation instructions.

  Download or clone the repository using the green button at the top of this page.

  If you downloaded the zipped version, unzip the directory somewhere you can access it.

### For Windows users:

  Make sure you have [Anaconda](https://www.continuum.io/downloads) installed on your machine.
  (Choose `Download for Windows', choose the Python2.7 version, and choose 64-bit if you have
   64-bit machine, and 32-bit otherwise.)

  In the pyrosim directory, open the file called pyrosim.py using your favorite text editor or IDE.

  Change line 20 from

  ```
  commandsToSend = ['./simulator']
  ```

  to

  ```
  commandsToSend = ['./simulator7.exe']
  ```

  if you have Windows 7 or 8, or change the line to 

  ```
  commandsToSend = ['./simulator10.exe']
  ```

  if you have a Windows 10 machine.

  Save the file and close it.

  Now run Demo_00_Empty_Sim.py, and you should see a new window open onto
  an empty virtual environment, [like this](https://imgur.com/a/FNQpE).

  If that does not work, follow the tutorial [here](https://youtu.be/tNGrxDftclA).

### For Mac and Linux users:

  Open a terminal window, and navigate into that directory. For example:

  ```bash
  $ cd ~/Desktop/pyrosim
  ```

  Run build.sh:

  ```bash
  $ sh build.sh
  ```

  This takes three to five minutes. The output should look like this:
  
  ```bash
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

  Once you get all three `done' confirmations, test the package by running
  ```bash
  $ python Demo_00_Empty_Sim.py 
  ```
  This should bring up an OpenGL window displaying an empty, virtual world.
  The window will close on its own after a few seconds.

### Next steps.

Now you can start making robots [here](https://www.reddit.com/r/ludobots/wiki/pyrosim/simulation), starting at step #3.

