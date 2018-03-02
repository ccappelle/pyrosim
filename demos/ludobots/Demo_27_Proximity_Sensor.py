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
    gravity = -0.1

    sim = pyrosim.Simulator(eval_time=eval_time, debug=True,
                               play_paused=False,
                               gravity=gravity,
                               play_blind=False,
                               use_textures=True,
                               capture=False,
                               dt=dt,
                               xyz=[3, -4, 4])

    sphere = sim.send_sphere(x=0, y=0, z=3, radius=0.3, mass=1., r=1, g=0, b=0)

    proxi = sim.send_proximity_sensor(body_id=sphere, x=0, y=0, z=0, max_distance=1.)

    box1 = sim.send_box(x=-1., y=0, z=0.75, length=0.3, width=0.3, height=1.5, r=0, g=1, b=0)
    box2 = sim.send_box(x=-1., y=0, z=1.65+EPS, length=0.3, width=0.3, height=0.3, r=1, g=0, b=0)
    box3 = sim.send_box(x=0, y=-0.2, z=0.1, length=0.2, width=0.2, height=0.2, r=0, g=0, b=1)

    sim.create_collision_matrix('all')

    sim.start()

    sim.wait_to_finish()

    proxi_data = []
    for i in range(6):
        proxi_data.append(sim.get_sensor_data(proxi, svi=i))

    for t in range(int(seconds/dt)):
        print(proxi_data[0][t], proxi_data[1][t], proxi_data[2][t], proxi_data[3][t], proxi_data[4][t], proxi_data[5][t])
