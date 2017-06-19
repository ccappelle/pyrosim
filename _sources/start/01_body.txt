.. _bodies:

Bodies
======

Basics
------

Pyrosim uses ODE to simulate rigid bodies. Rigid bodies cannot
bend or deform and make hard contacts when they collide.
Currently there are three types of rigid bodies in pyrosim:

#. Boxes
#. Cylinders
#. Spheres

This demo provides a quick introduction on the different bodies
and how they are used in pyrosim.


Sending bodies
--------------

The following code sends the default of each body shape
at different x-coordinate positions.

.. code-block:: python
    
    sim = pyrosim.Simulator(play_paused=True)

    sim.send_cylinder(x=-1)
    sim.send_box(x=0)
    sim.send_sphere(x=1)

    sim.start()

.. image:: /img/bodies-init.png
    :alt: Empty simulation
    :align: center
    :width: 300 px
    :height: 300 px

Note that when you play the simulation, because the bodies are embedded
in the ground initially, they will shoot up. To rectify this we must
change the objects z-coordinate. 


Body Position
-------------

We can also easily send multiple objects
by using loops.

.. code-block:: python
    
    sim = pyrosim.Simulator(play_paused=True)

    for i in range(3):
        sim.send_cylinder(x=-1, y=i, z=.6,length=i)
        sim.send_box(x=0, y=i, z=0.05)
        sim.send_sphere(x=1, y=i, z=0.1)

    sim.start()

.. image:: /img/bodies-multiple.png
    :alt: Empty simulation
    :align: center
    :width: 300 px
    :height: 300 px

Body Dimensions
---------------

Object dimensions can be set using the natural dimension parameters
of each body.

.. code-block:: python
    
    sim = pyrosim.Simulator(play_paused=True)

    for i in range(3):
        size = (i+1)/6.0
        z = size/2.0+0.1
        sim.send_cylinder(x=-1, y=i, z=z,length=size)
        sim.send_box(x=0, y=i, z=z, height=size)
        sim.send_sphere(x=1, y=i, z=z, radius=size/2.0)

    sim.start()

.. image:: /img/bodies-size.png
    :alt: Empty simulation
    :align: center
    :width: 300 px
    :height: 300 px

A more complete list of the different body parameters can be found in the :ref:`code`

Body IDs
--------

Every time a body is sent to the simulator, an integeger ID tag is returned.
This ID is used as a handle for the body when connecting joints and sensors 
which are covered in later demos. 

