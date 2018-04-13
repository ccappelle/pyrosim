.. _sensors:

Sensors
=======

.. _sensor-basics:

Basics
------

There are currently 5 sensors:

#. Position - senses x,y,z position of a body.
#. Ray - senses the distance and color component of the body the ray hits.
#. Proprioceptive - senses the current angle of a joint.
#. Touch - senses if a body is touching another body
#. Vestibular - senses a bodies orientation in space

After the simulation terminates, the results of the sensors from every time step
is collected and returned as a numpy matrix. The matrix has indices 
(sensor_id, svi, time). Sensor ID is the returned ID tag of the sensor. Svi stands
for sensor value index. Some sensors sense more than one value. For example: the 
position sensor returns the x,y,z position of a body which corresponds to svi=0,1,2 
respectively. 

Double Pendulum Cont.
---------------------

We add sensors to the double pendulum and evaluate the print the output.
Insert the following code after the joints have been sent.

.. literalinclude:: /../demos/double-pendulum/dp-sensors.py
    :caption: dp-sensors.py
    :lines: 46-

The output should be as follows

.. literalinclude:: /code/sensor-test.txt
    :emphasize-lines: 500
    :lines: 1-50,500-
