.. _thrusters:

Internal Thruster
=================

Basics
------

Thrusters are objects attached to bodies which apply force based on the value of the motor neuron associated to them. 
They are like joints except they can only affect one body directly (multiple indirectly if joined or colliding).
Thrusters apply force along a specified direction which rotates as the body it is connected to rotates. 
This means thrusters, if allowed, can apply force in both the negative and positive direction specified. 
Motor neurons control thrusters much in the same way they control position control joints: when a motor neuron outputs a 1, the thruster outputs its max force and when the motor neuron outputs a -1, the thruster uses its min force. 
This means thrusters can be seamlessly integrated into any neural network.

Spaceship
---------

The natural example for thrusters is, of course, a spaceship. Here we recreate space-x by launching a rocket and having it return safely to ground (and then still thrusting upwards because it actuates on a sine wave, so it's not really like space-x but we are trying to get on the bandwagon).

.. literalinclude:: /../demos/robots/rocket_ship.py
    :caption: rocket_ship.py

.. raw:: html 

    <iframe width="100%" height="315" src="https://www.youtube.com/embed/J1Bsi3IW9VE?rel=0" frameborder="0" allowfullscreen></iframe>