.. _forces:

External Forces
===============

Basics
------

In pyrosim you can add external forces to any body at any time step within the simulation.
For example if you wanted to throw a block at time step fifty in the *z* (up) direction with a force of *5 newtons`, you would first create the block using :code:`box_id = sim.create_box(**kwargs)` and then send the external force using :code:`sim.send_external_force(box_id, x=0, y=0, z=5, time=50)`.

Here :code:`x,y,z` refers to the force vector applied to the body. It is important to remember that larger mass bodies will need more force to move. 

Quadruped
---------

Here we present our famed mascot being hit by a box thrown with external forces. This example also showcases :ref:`developing synapses <synapses>`. 

.. literalinclude:: /../demos/robots/quadruped.py
    :caption: quadruped.py

.. raw:: html
    
    <iframe width="100%" height="315" src="https://www.youtube.com/embed/r7pnT__eFaw?rel=0" frameborder="0" allowfullscreen></iframe>
