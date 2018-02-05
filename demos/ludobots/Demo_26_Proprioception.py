import pyrosim
import math

# The basis - elevated fixed box and sinusoidal neuron

sim = pyrosim.Simulator(play_paused=False, debug=True, use_textures=True,
                        xyz=[4.0, 3.0, 2.0], hpr=[-150.0, -15.0, 0.0],
                        eval_time=250)

box_0 = sim.send_box(x=0.0, y=0.0, z=2.0,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

world_joint = sim.send_fixed_joint(pyrosim.Simulator.WORLD, box_0)

fnueron_0 = sim.send_function_neuron(lambda x: (math.sin(x)+0.5)/2 )
fnueron_1 = sim.send_function_neuron(lambda x: (math.sin(x+1.5)+0.5)/2 )

# Tether

box_1 = sim.send_box(x=0.0, y=0.0, z=0.5,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

tether = sim.send_tether(box_0, box_1,
                         force_coefficient=10., dampening_coefficient=10.)

mneuron_0 = sim.send_motor_neuron(tether, input_index=0)
mneuron_1 = sim.send_motor_neuron(tether, input_index=1)

sim.send_synapse(fnueron_0, mneuron_0, 1.0)
sim.send_synapse(fnueron_0, mneuron_1, 1.0)

tet_pro_sen = sim.send_proprioceptive_sensor(tether)

# Linear joint

box_2 = sim.send_box(x=2.0, y=0.0, z=2.0,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

slider = sim.send_slider_joint(box_0, box_2,
                               x=1., y=0., z=0.,
                               lo=-0.5, hi=0.5, speed=10.)

mneuron_2 = sim.send_motor_neuron(slider)
sim.send_synapse(fnueron_0, mneuron_2, 1.0)

lin_pro_sen = sim.send_proprioceptive_sensor(slider)

# Rotary joint

box_3 = sim.send_box(x=-1.0, y=0.0, z=2.0,
                     width=0.5, length=0.5, height=0.5,
                     mass=3.0)

servo = sim.send_hinge_joint(box_0, box_3,
                             x=-0.75, y=0, z=2.0,
                             n1=0., n2=1., n3=0.,
                             lo=1., hi=-1.,
                             torque=10.)

mneuron_3 = sim.send_motor_neuron(servo)
sim.send_synapse(fnueron_0, mneuron_3, 1.0)

rot_pro_sen = sim.send_proprioceptive_sensor(servo)

# Adhesive joint

box_4 = sim.send_box(x=0., y=0., z=0.25,
                     width=0.5, length=0.5, height=0.5,
                     mass=0.5)

glue = sim.send_adhesive_joint(box_1)

mneuron_4 = sim.send_motor_neuron(glue)
sim.send_synapse(fnueron_1, mneuron_4, 1.0)

adh_pro_sen = sim.send_proprioceptive_sensor(glue)

# Starting the simulation and reading the sensor data

sim.create_collision_matrix('all')
sim.start()
results = sim.wait_to_finish()

lin_data = sim.get_sensor_data(lin_pro_sen)
rot_data = sim.get_sensor_data(rot_pro_sen)
adh_data = sim.get_sensor_data(adh_pro_sen)
tet_data_0 = sim.get_sensor_data(tet_pro_sen, svi=0)
tet_data_1 = sim.get_sensor_data(tet_pro_sen, svi=1)

state_ts = zip(lin_data, rot_data, adh_data, tet_data_0, tet_data_1)
for state in state_ts:
    print('   '.join(map(str, state)))
