import pyrosim
import math

sim = pyrosim.Simulator(play_blind=False, play_paused=True, debug=True,
                        use_textures=True, xyz=[0, -4, 1], hpr=[90, 0, 0],
                        eval_time=200)

MAIN_HEIGHT = 1.0
MAIN_WIDTH = MAIN_HEIGHT/5.0
NUM_THRUSTERS = 4

# set up fuselage
main_body = sim.send_cylinder(x=0, y=0, z=MAIN_HEIGHT+MAIN_WIDTH,
                              r1=0, r2=0, r3=1,
                              length=MAIN_HEIGHT, radius=MAIN_WIDTH,
                              mass=100
                              )
# make thrusters
side_cyls = [0]*NUM_THRUSTERS
thrusters = [0]*NUM_THRUSTERS
mneuron = [0]*NUM_THRUSTERS

# network consists of sin input neuron connected to thrusters
fnueron = sim.send_function_neuron(math.sin)

for i in range(NUM_THRUSTERS):
    x = math.cos(2*math.pi/float(NUM_THRUSTERS)*i)*MAIN_WIDTH
    y = math.sin(2*math.pi/float(NUM_THRUSTERS)*i)*MAIN_WIDTH

    side_cyls[i] = sim.send_cylinder(x, y, MAIN_HEIGHT/3.0+MAIN_WIDTH,
                                     r1=0, r2=0, r3=1,
                                     length=MAIN_HEIGHT/3.0,
                                     radius=MAIN_WIDTH/3.0)
    sim.send_fixed_joint(main_body, side_cyls[i])

    # thrusters are attached to a body and apply a
    # directional force (x,y,z) based on the value of
    # the attached motor neuron. The effect of the force
    # on the body depends on the lo and hi parameters
    # and the total mass of the bodies attached.
    # Currently the thrusters only apply force to the
    # center of mass of the attached body.
    thrusters[i] = sim.send_thruster(side_cyls[i],
                                     x=0, y=0, z=-1,
                                     lo=0, hi=40)

    mneuron[i] = sim.send_motor_neuron(thrusters[i])
    sim.send_synapse(fnueron, mneuron[i], 1.0)

sim.film_body(main_body, 'track')
sim.start()
sim.wait_to_finish()
