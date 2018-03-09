#!/usr/bin/env python3

import math
import numpy as np
import math
import pyrosim

EPS = 0.0
#np.random.seed(0)

debug = False
play_blind = True

sen_types = [0,1,2]

def get_light_sensor_time_series(sensitivityType, logarithmic):
    seconds = 15.0
    dt = 0.05
    eval_time = int(seconds/dt)
    gravity = -0.1

    sim = pyrosim.Simulator(eval_time=eval_time, debug=debug,
                               play_paused=False,
                               gravity=gravity,
                               play_blind=play_blind,
                               use_textures=True,
                               capture=False,
                               dt=dt,
                               xyz=[3, -4, 4])

    sphere = sim.send_sphere(x=0, y=0, z=3, radius=0.1, mass=1., r=1, g=0, b=0)

    light_sensor = sim.send_light_sensor(body_id=sphere, kind_of_light=sensitivityType, logarithmic=logarithmic)

    box0 = sim.send_box(x=0, y=0, z=3.25, length=0.2, width=0.2, height=0.2, r=0, g=0, b=0)
    sim.send_fixed_joint(-1, box0)
    light_source0 = sim.send_light_source(box0, x=0, y=0, z=-0.15, kind_of_light=0)

    box1 = sim.send_box(x=-1., y=0, z=0.75, length=0.3, width=0.3, height=1.5, r=0, g=1, b=0)
    light_source1 = sim.send_light_source(box1, x=0.5, y=0, z=0, kind_of_light=1)

    box2 = sim.send_box(x=-1., y=0, z=1.65+EPS, length=0.3, width=0.3, height=0.3, r=1, g=0, b=0)
    light_source1 = sim.send_light_source(box2, x=1, y=0, z=0, kind_of_light=2)

    sim.create_collision_matrix('all')

    sim.start()

    sim.wait_to_finish()

    return sim.get_sensor_data(light_sensor)

def draw_ts(tsdata):
    import matplotlib.pyplot as plt
    fig, (ax0, ax1) = plt.subplots(nrows=2, figsize=(7, 9.6))
    for sen in sen_types:
        ts = tsdata[False][sen]
        ax0.plot(range(len(ts)), ts, label=str(sen))
        ax0.legend()
        ax0.set_ylabel('Lin sensor output')
        ax0.set_xlabel('Evaluation step')
    for sen in sen_types:
        ts = tsdata[True][sen]
        ax1.plot(range(len(ts)), ts, label=str(sen))
        ax1.legend()
        ax1.set_ylabel('Log sensor output')
        ax1.set_xlabel('Evaluation step')
    plt.show()

if __name__ == "__main__":

    lsdata = {}
    for log in [True, False]:
        lsdata[log] = {}
        for sen in sen_types:
            lsdata[log][sen] = get_light_sensor_time_series(sen, log)

    draw_ts(lsdata)
