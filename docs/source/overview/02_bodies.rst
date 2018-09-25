.. _bodies:

Bodies
======

Sending Bodies
--------------
There are 3 rigid body prototypes which make up all of the dynamic bodies you can currently send to pyrosim.

* Boxes
* Spheres
* Cylinders (capped and un-capped)

You can send bodies using simple send commands as illustrated below.

.. literalinclude:: ../../../demos/docs/all_bodies.py

.. raw:: html
    
    <p align="center"><iframe width="400" height="225" src="https://www.youtube.com/embed/4SrnicM40VY?rel=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe></p>


Body Options
------------
All bodies have parameters which specify their position, orientation, color, and size.
See :ref:`code` for explicit parameter listing.
Below is an example sending multiple bodies with some of these parameters altered.


Composite Bodies
----------------
Composite bodies are bodies which use multiple rigid body prototypes in their construction.
Composite bodies are useful to efficiently create arbitrary shapes.
You can send composite bodies similar to singular bodies, however you would use 'add' commands instead off 'send' commands to attach prototypes to the composite body.
Below is an example of creating a hollow box attaching cylinders to a composite body.
The black sphere in the center is an graphical illustration of the center of mass of the composite body.

.. literalinclude:: ../../../demos/docs/composite_body.py
