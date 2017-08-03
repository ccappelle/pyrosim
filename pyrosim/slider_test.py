import pyrosim

# sim = pyrosim.Simulator(eval_time=10, 
#                         play_blind=False, play_paused=True,
#                         debug=True)
sim = pyrosim.Simulator(eval_time=1000, 
                        play_blind=False, play_paused=True,
                        debug=True)

box1 = sim.send_box(x=0,y=0,z=1, length=.2, width=.2, height=.2)

box2 = sim.send_box(x=0,y=0,z=.5, length=.2, width=.2, height=.2)

cyl = sim.send_cylinder(x=0,y=0,z=.95, r1=0,r2=0,r3=1, length=.05, radius=.02)

joint1 = sim.send_slider_joint(box1, pyrosim.Simulator.WORLD,
                                x=1,y=0,z=0)

joint2 = sim.send_slider_joint(cyl, box2, x=0,y=0.,z=1.)

joint3 = sim.send_hinge_joint(cyl, box1, x=0, y=0, z=1,
                                n1=0, n2=1, n3=0,
                            position_control=False)

jet1 = sim.send_jet(box2)

sim.send_position_sensor(box1)

fneuron = sim.send_function_neuron()
bneuron = sim.send_bias_neuron()

mneuron1 = sim.send_motor_neuron(joint1)
mneuron2 = sim.send_motor_neuron(joint2)
mneuron3 = sim.send_motor_neuron(joint3)


sim.send_synapse(fneuron,mneuron1, 1.0)
sim.send_synapse(fneuron,mneuron2, 1.0)
sim.send_synapse(bneuron,mneuron3, 1.0)


sim.start()
sim.wait_to_finish()