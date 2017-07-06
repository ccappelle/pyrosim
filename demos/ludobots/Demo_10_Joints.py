import sys
sys.path.insert(0, '../..')

import pyrosim

# debug option draws joints and prints out sent commands
sim = pyrosim.Simulator(eval_time=500, debug=True)

ARM_LENGTH = 0.5
ARM_RADIUS = ARM_LENGTH / 10.0

# sending object returns an ID tag. Use this to later refer to the body
cyl1 = sim.send_cylinder(x=0, y=0, z=ARM_LENGTH/2.0 + ARM_RADIUS,
                         r1=0, r2=0, r3=1, length=ARM_LENGTH,
                         radius=ARM_RADIUS)

cyl2 = sim.send_cylinder(x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + ARM_RADIUS,
                         r1=0, r2=1, r3=0, length=ARM_LENGTH,
                         radius=ARM_RADIUS)

# when in debug mode, hinge joints are depicted as a red cylinder. This
# is only graphical and does not alter the simulation
sim.send_hinge_joint(first_body_id=cyl1, second_body_id=cyl2,
                     x=0, y=0, z=ARM_LENGTH + ARM_RADIUS,
                     n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

sim.start()
sim.wait_to_finish()
