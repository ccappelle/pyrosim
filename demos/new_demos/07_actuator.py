import sys
sys.path.insert(0, '../../')
import pyrosim

# toggle joint drawing by pressing 'd'
sim = pyrosim.Simulator(eval_steps=-1, play_paused=True)

cyl = sim.send_cylinder(position=(0.25, 0, 1),
                        orientation=(1, 0, 0),
                        length=0.5)
sphere = sim.send_sphere(position=(0.5, 0, 1),
                         radius=0.1)

hinge = sim.send_hinge_joint(-1, cyl,
                     anchor=(0, 0, 1),
                     axis=(0, 1, 0),
                     joint_range=None)

slider = sim.send_slider_joint(cyl, sphere,
                      axis=(1, 0, 0),
                      joint_range=0.3)

sim.send_rotary_actuator(hinge)
sim.send_linear_actuator(slider)

sim.start()
sim.wait_to_finish()