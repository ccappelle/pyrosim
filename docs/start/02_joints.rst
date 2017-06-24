.. _joints:

Joints
======

Basics
------

Pyrosim makes use of 2 different types of joints:

#. Hinge Joints
#. Slider Joints

Hinge joints rotate around a joint axis at an anchor whereas 
slider joints slide along the direction of a joint axis.

.. image:: /img/hinge.jpg
    :width: 300 px
    :height: 300 px
    :align: center
    :alt: hinge

.. image:: /img/slider.jpg
    :width: 300 px
    :height: 300 px
    :align: center
    :alt: slider

Double Pendulum
---------------

This example creates a weighted double pendulum with a slider joint connecting
the mass at the end. This example makes use of the body ID tags and introduces
orientation in the cylinders *(r1,r2,r3)*. Setting the *debug* tag to true when 
creating the simulator draws the hinge joints as red cylinders along the joint
normals. Currently slider joints are not drawn.

.. literalinclude:: /../demos/double-pendulum/dp-basic.py
    :caption: dp-basic.py

.. raw:: html

    <iframe width="100%" height="400" src="https://www.youtube.com/embed/yMNPszsgAEE" frameborder="0" style="float:middle"></iframe>

