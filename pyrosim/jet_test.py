import pyrosim
import math

sim = pyrosim.Simulator(play_paused=True, play_blind=False, eval_time=500, debug=True,
                        xyz=[0,-2,1], hpr=[90,-10,0], use_textures=True)

#body = sim.send_sphere(x=0, y=0, z=1)
body = sim.send_cylinder(x=0,y=0,z=1, length=1.0,
                        radius=.15,
                        mass=100)
num_thrusters = 4
cylinders = [0]*num_thrusters
thrusters = [0]*num_thrusters
radius = .15

fneuron = sim.send_function_neuron()
bneuron = sim.send_bias_neuron()
mneurons = [0]*num_thrusters

for i in range(num_thrusters):
    theta = i/float(num_thrusters) * 2.0 * 3.14159
    x = math.cos(theta)*radius
    y = math.sin(theta)*radius

    cylinders[i] = sim.send_cylinder(x=x, y=y, z=.5, length=.2, radius=radius/2.0)
    sim.send_fixed_joint(body, cylinders[i])   
    thrusters[i] = sim.send_thruster(cylinders[i], x=0,y=0,z=-1, lo=0, hi=40)
    mneurons[i] = sim.send_motor_neuron(thrusters[i])
    sim.send_synapse(fneuron,mneurons[i],1.0)

body_sensor = sim.send_position_sensor(body)

num_side_thrusters = 3
side_cylinders = [0]*num_side_thrusters
side_thrusters = [0]*num_side_thrusters
sensor_neurons = [0]*3
for i in range(3):
    sensor_neurons[i] = sim.send_sensor_neuron(body_sensor, svi=i)

direction_neurons = [0]*num_side_thrusters

for i in range(num_side_thrusters):
    theta = i/float(num_side_thrusters) * 2.0 * 3.14159
    x = math.cos(theta)*(radius+.1)
    y = math.sin(theta)*(radius+.1)

    side_cylinders[i] = sim.send_cylinder(x=x, y=y, z=1.2, length=.05,
                                            r1=x/radius, r2=y/radius, r3=-2.0, 
                                            radius=radius/8.0, mass=0.1)
    sim.send_fixed_joint(body, side_cylinders[i])
    side_thrusters[i] = sim.send_thruster(side_cylinders[i], x=x/radius, y=y/radius, z=-2.0, lo=0, hi=10.)
    direction_neurons[i] = sim.send_motor_neuron(side_thrusters[i])
    #sim.send_synapse(sensor_neurons[i%3], direction_neurons[i], weight=1.0)
    sim.send_synapse(bneuron, direction_neurons[i], weight=1.0)

sim.film_body(body, 'follow')
sim.start()
print sim.wait_to_finish()
