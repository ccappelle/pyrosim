.. _start:

Getting Started
===============

The following examples provide a background in the different features and options in pyrosim.
All demos assume you have imported pyrosim correctly. If you have installed pyrosim using
the pip method in the pyrosim root directory::

   $ pip install -e .

you should be able to simply use

.. code-block:: python
    
   import pyrosim

If you do not want to use pip you will have to append the root directory of pyrosim to your 
path before importing it (the root directory contains the setup.py file).
This can simply be done by using

.. code-block:: python

   import sys
   sys.path.insert(0,'path/to/pyrosim/root')
   import pyrosim

.. toctree::
   :glob:

   start/*