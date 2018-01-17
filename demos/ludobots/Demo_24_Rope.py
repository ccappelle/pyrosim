import pyrosim
import math

sim = pyrosim.Simulator(play_paused=False, debug=True, use_textures=True,
                        xyz=[3.0, 2.0, 2.0], hpr=[-150.0, -15.0, 0.0],
                        eval_time=1000)

cyl_1 = sim.send_cylinder(x=-0.25, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.5, radius=0.07)
cyl_2 = sim.send_cylinder(x=-0.5, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.0, radius=0.07)

cyl_3 = sim.send_cylinder(x=-0.68, y=0.0, z=2.0,
                          r1=1, r2=0, r3=0,
                          length=0.0, radius=0.07)
box = sim.send_box(x=-1.0, y=0.0, z=2.0,
                   width=0.5, length=0.5, height=0.5,
                   mass=3.0)

world_joint = sim.send_hinge_joint(pyrosim.Simulator.WORLD, cyl_1,
                                   x=0.0, y=0.0, z=2.0,
                                   n1=0, n2=1, n3=0,
                                   position_control=False,
                                   torque=1000, speed=3)

fixed_joint_1 = sim.send_fixed_joint(cyl_1, cyl_2)
fixed_joint_2 = sim.send_fixed_joint(cyl_3, box)

rope = sim.send_rope(cyl_2, cyl_3,
                     length=0.3, spring_coefficient=100., dampening_coefficient=10.)

sim.create_collision_matrix('all')

sim.start()
results = sim.wait_to_finish()
