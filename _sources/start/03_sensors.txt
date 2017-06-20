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

.. code-block:: python
    :caption: demos/double_pendulum.py

    ### continued from before ###
    pos_sensor = sim.send_position_sensor(cyl_1)

    vestib_sensor = sim.send_vestibular_sensor(cyl_2)

    prop_sensor = sim.send_proprioceptive_sensor(slider_joint)

    sim.start()
    results = sim.wait_to_finish()

    # we can access the results matrix directly
    print results, '\n'

    # or use built in commands to get a specific sensor and svi
    # value for every time step

    pos_x_sensor_results = sim.get_sensor_data(pos_sensor, svi=0)
    pos_y_sensor_results = sim.get_sensor_data(pos_sensor, svi=1)
    pos_z_sensor_results = sim.get_sensor_data(pos_sensor, svi=2)
    vestib_sensor_results = sim.get_sensor_data(vestib_sensor)
    prop_sensor_results = sim.get_sensor_data(prop_sensor)

    for t in range(500):
        x = pos_x_sensor_results[t]
        y = pos_y_sensor_results[t]
        z = pos_z_sensor_results[t]
        v = vestib_sensor_results[t]
        p = prop_sensor_results[t]

        output = ('{:3d}:: x:{: 3.1f}, y:{: 3.1f}, z:{: 3.1f}, ' +
                  'vestib:{: 3.1f}, prop:{: 3.1f}').format(t, x, y, z, v, p)
        print output

The output should be as follows

.. literalinclude:: /code/sensor-test.txt
    :emphasize-lines: 500
    :lines: 1-50,500-

Rest coming soon :)
