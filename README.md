# pyrosim: A python robot simulator. 

## Installation instructions.

  Download or clone the repository using the green button at the top of this page.

  If you downloaded the zipped version, unzip the directory somewhere you can access it.

### For Windows users:

  Install [Anaconda](https://www.continuum.io/downloads)--a package that includes Python
  and useful Python libraries--if you don't already have it by 
  choosing `Download for Windows', the Python2.7 version, and the 64-bit version if you have
  a 64-bit machine and 32-bit otherwise. (You can find out whether you have a 32-bit or 64-bit
  machine [here](https://support.microsoft.com/en-us/help/15056/windows-7-32-64-bit-faq).)

  Now open the file called pyrosim.py in the pyrosim directory using your favorite text
  editor.

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

  Now open Demo_00_Empty_Sim.py, again using your favorite text editor, and change this line

  ```
  sim = PYROSIM() 
  ```

  to this

  ```
  sim = PYROSIM(evalTime=10000) 
  ```
  
  Save the file and close it.

  Now [open a command prompt](https://www.youtube.com/watch?v=C6UkfGafBGI)
  and [navigate](https://www.youtube.com/watch?v=xNXtttJUREk) into the Pyrosim directory.

  Now run Demo_00_Empty_Sim.py by typing

  ```
  c:\pyrosim> python Demo_00_Empty_Sim.py
  ```

  If you get an error message such as ```python: command not found```, this means
  that your computer does not know where to find the python interpreter.
  [Locate the directory](https://www.youtube.com/watch?v=DQcA47Mqcj4) 
  where you installed Anaconda. There should be a file called python.exe
  in there. Now 
  [change your path environment variable](http://www.computerhope.com/issues/ch000549.htm)
  to include the directory
  that contains python.exe. Execute ```c:\pyrosim> python Demo_00_Empty_Sim.py```
  again.

  When the command works, you should see a new window open onto
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

