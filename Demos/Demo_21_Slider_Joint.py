import pyrosim

ARM_LENGTH = 0.5
ARM_RADIUS = ARM_LENGTH / 10.0

sim = pyrosim.Simulator(play_paused=True, eval_time=500, debug=True)

cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS,
                         r1=0, r2=0, r3=1,
                         length=ARM_LENGTH, radius=ARM_RADIUS)
cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS,
                         r1=0, r2=1, r3=0,
                         length=ARM_LENGTH, radius=ARM_RADIUS)

joint = sim.send_slider_joint(first_body_id=cyl1, second_body_id=cyl2,
                              x_dir=1, y_dir=0, z_dir=0,
                              position_control=True)
fixed = sim.send_slider_joint(first_body_id=cyl1, second_body_id=-1,
                              lo=0, hi=0)

touch1 = sim.send_touch_sensor(body_id=cyl1)
touch2 = sim.send_touch_sensor(body_id=cyl1)

#fneuron = sim.send_function_neuron()
fneuron = sim.send_bias_neuron()

mneuron = sim.send_motor_neuron(joint_id=joint)

sim.send_synapse(source_neuron_id=fneuron,
                 target_neuron_id=mneuron, weight=-1.0)

sim.start()
sim.wait_to_finish()
