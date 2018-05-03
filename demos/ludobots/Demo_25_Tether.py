import pyrosim
import math

sim = pyrosim.Simulator(play_paused=False, debug=True, use_textures=True,
                        xyz=[3.0, 2.0, 2.0], hpr=[-150.0, -15.0, 0.0],
                        eval_time=250)

box_0 = sim.send_box(x=0.0, y=0.0, z=2.0,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

box_1 = sim.send_box(x=0.0, y=0.0, z=0.5,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

world_joint = sim.send_fixed_joint(pyrosim.Simulator.WORLD, box_0)

tether = sim.send_tether(box_0, box_1,
                         force_coefficient=10., dampening_coefficient=10.)

mneuron_0 = sim.send_motor_neuron(tether, input_index=0, alpha=0.)
mneuron_1 = sim.send_motor_neuron(tether, input_index=1, alpha=0.)

fnueron = sim.send_function_neuron(lambda x: math.sin(x) )

sim.send_synapse(fnueron, mneuron_0, 1.0)
sim.send_synapse(fnueron, mneuron_1, 1.0)

sim.create_collision_matrix('all')

sim.start()
results = sim.wait_to_finish()
