.. _installation:

Installation
============

Currently, the make files in pyrosim are only set up to install ODE on Linux and Mac machines.
After you have downloaded the repo from `github <http://github.com/ccappelle/pyrosim>`_ go into
the pyrosim root folder (contains build.sh). Then run

.. code-block:: bash
    
    $ sh build.sh

This will build ODE and compile the pyrosim C++ code creating the executable `simulator`.
You can now use pyrosim as a local module or import it system wide using

.. code-block:: bash
    
    $ pip install -e .

Support for windows using MinGW should be coming shortly.