import pyrosim
import math

def send_omni_wheel(sim, position, orientation, radius=0.25, thickness=0.1, n_small_wheels=8):
    base = sim.send_cylinder(position=position,
                             orientation=orientation,
                             length=thickness,
                             radius=radius,
                             capped=False)

    for i in range(n_small_wheels):
        theta = i / n_small_wheels * (math.pi * 2.0)
        x, y = math.cos(theta) * radius, math.sin(theta) * radius

        cylinder_orientation = [-y, x, 0]

        # new_position = []
        small = sim.send_cylinder(position=[x, y, position[2]],
                                  orientation=[-y, x, 0],
                                  length=4.0 * (radius / n_small_wheels),
                                  radius=thickness / 2.0)

        sim.send_hinge_joint(base, small,
                             anchor=[x, y, position[2]],
                             axis=cylinder_orientation,
                             joint_range=-1)
if __name__ == '__main__':
    sim = pyrosim.Simulator(play_paused=True)

    send_omni_wheel(sim, position=[0, 0, 1], orientation=[0, 0, 1])
    sim.start()
    sim.wait_to_finish()