.. _collision:

Collision Matrix
================

By default, collisions are turned off in pyrosim. To turn on collisions between rigid bodies they must be assigned collision groups and a collision matrix defining the interactions between those groups must be sent. 

Collision Groups
----------------

A collision group is a collection of bodies which all collide (or don't) the same way with other collision groups. For example, if you do not want a robot to collide with its body parts, but want it to collide with objects in the environment, then all the bodies in the robot would be in one collision group and all the bodies in the environment would be in another.

When sending a body, you can assign a collision group name to the body by setting the :code:`collision_group` parameter when the body is sent. The default name is 'default'. After you have sent all the bodies you want in the simulation you use the name tag to define collisions

Defining Collisions Between Groups
----------------------------------

If we imagine each collision group as a node on a network, then it is easy to see the collision matrix as an adjacency matrix for this network where there is an edge between two groups if they collide and none if they do not.
To go back to our robot/environment example above, we would want a connection between the 'robot' and 'env' collision groups and a loop connection on the 'env' node. Loops specify intra-group collision while edges between two different nodes specify inter-group collision.

If you do not want to specify every collision between every group you can use the predefined collision matrices provided by pyrosim. By using the :code:`sim.create_collision_matrix(collision_type)` command you can choose from four different collision types: 'none', 'inter', 'intra', and 'all'. They are depicted using the 'env' and 'robot' groups by the following figure:

.. image:: /img/graph-pyrosim.png
    :alt: Different predefined collisions
    :align: center
    :width: 100 %

Regardless of how groups are defined and connected, the collisions between bodies which are attached by joints are disabled automatically. If they were enabled the simulation would most likely break.

Double Pendulum Part IV
-----------------------

We have taken our double pendulum code and removed the sensors and turned the network it to a simple forward actuation about its connection in space. This simulation should show the pendulum actuate and hit a ball in the environment into the target blocks.

.. literalinclude:: /../demos/double-pendulum/dp-collisions.py
    :caption: dp-collisions.py

.. raw:: html

    <iframe width="100%" height="315" src="https://www.youtube.com/embed/TmLf08X_jB8?rel=0" frameborder="0" allowfullscreen></iframe>

