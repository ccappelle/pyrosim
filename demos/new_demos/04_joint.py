import sys
sys.path.insert(0, '../../')
import pyrosim

sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)

box = sim.send_box(position=(0, 0, 2))

# ball = sim.send_ball_and_socket_joint(-1, box, anchor=(0, 0, 3));
universal = sim.send_universal_joint(-1, box,
                    anchor=(0, 0, 3),
                    axis1=(1, 0, 0),
                    axis2=(0, 1, 0))

# toggle joint drawing by pressing 'd'
# sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)
# z = 2
# sphere = sim.send_sphere(position=(0, 0, z),
#                          radius=0.1)

# cyl = sim.send_cylinder(position=(0, -0.25, z),
#                         orientation=(0, -1, 0),
#                         length=0.5)

# box = sim.send_box(position=(0, -1, z), 
#                    sides=[0.2] * 3)

# sim.send_slider_joint(-1, sphere,
#                       axis=(1, 0, -0.01),
#                       joint_range=10)

# sim.send_hinge_joint(sphere, cyl,
#                      anchor=(0, 0, z),
#                      axis=(1, 0, 0),
#                      joint_range=None)

# sim.send_ball_and_socket_joint(cyl, box,
#                                anchor=(0, -0.75, z))

sim.start()
sim.wait_to_finish()
print(sim._raw_cerr)