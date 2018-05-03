#!/usr/bin/env python3

import math
import numpy as np
import math
import pyrosim

EPS = 0.0
#np.random.seed(0)

if __name__ == "__main__":

    seconds = 15.0
    dt = 0.05
    eval_time = int(seconds/dt)
    print(eval_time)
    gravity = 0.0

    sim = pyrosim.Simulator(eval_time=eval_time, debug=True,
                               play_paused=False,
                               gravity=gravity,
                               play_blind=False,
                               use_textures=True,
                               capture=False,
                               dt=dt,
                               xyz=[3, -4, 4])

    max_torque = 0.5

    box0 = sim.send_box(x=0, y=0, z=2., length=1.6, width=1.0, height=0.6, r=0, g=1, b=0)
    rcw0 = sim.send_reaction_control_wheel(box0, max_torque=max_torque)
    mneuron0 = sim.send_motor_neuron(rcw0, input_index=0, tau=0.1)

    fneuron = sim.send_function_neuron(lambda x: 0.5 + 0.1*math.sin(x*3))
    sim.send_synapse(fneuron, mneuron0, 1.0)

    sim.create_collision_matrix('all')
    sim.start()
    sim.wait_to_finish()

#    proxi_data = []
#    for i in range(6):
#        proxi_data.append(sim.get_sensor_data(proxi, svi=i))

#    for t in range(int(seconds/dt)):
#        print(proxi_data[0][t], proxi_data[1][t], proxi_data[2][t], proxi_data[3][t], proxi_data[4][t], proxi_data[5][t])
