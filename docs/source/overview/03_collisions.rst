.. _collisions:

Managing Collisions
===================

.. Overview
.. --------

.. In simulators there are two distinct phases regarding collisions: detection and response.
.. In the detection phase 

Spaces
------
Spaces can help reduce the number of times collision detection is run between bodies.
By placing individual robots in their own space, collision detection will now run between the bounding box of each robot instead of the individual components of each robot.
Spaces can also help group portions of the environment to help decrease simulation time.
To debug you can press 's' to visualize the bounding box of each space as a red box.
Below is an example of how spaces work.


Collision Groups
----------------
Collision groups determine which bodies collide.
By default there is no collision in pyrosim except with the ground plane.
To allow collision to occur between two bodies you must first specify which collision group the body is a part of and then assign a collision potential between the two groups.


.. literalinclude:: ../../../demos/docs/collisions.py
