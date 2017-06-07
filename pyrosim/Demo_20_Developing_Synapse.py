import pyrosim
import math

ARM_LENGTH = 0.5

ARM_RADIUS = ARM_LENGTH / 10.0

sim = pyrosim.Simulator(play_paused = True , eval_time = 1000, debug=True)

sim.Send_Cylinder(body_ID = 0 , x=0, y=0, z=ARM_LENGTH/2.0 + 2*ARM_RADIUS, r1=0, r2=0, r3=1, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Cylinder(body_ID = 1 , x=0, y=ARM_LENGTH/2.0, z=ARM_LENGTH + 2*ARM_RADIUS , r1=0, r2=1, r3=0, length=ARM_LENGTH, radius=ARM_RADIUS)

sim.Send_Hinge_Joint(joint_ID = 0, first_body_ID=0, second_body_ID=1, x=0, y=0, z=ARM_LENGTH + 2*ARM_RADIUS , n1=1, n2=0, n3=0, lo=-3.14159/4.0, hi=+3.14159/4.0)

sim.Send_Hinge_Joint(joint_ID = 1 , first_body_ID=0, second_body_ID=-1, x=0, y=0, z=ARM_LENGTH/2.0 + 2*ARM_RADIUS)

sim.Send_Function_Neuron(neuron_ID=0, function=math.sin) 
sim.Send_Motor_Neuron(neuron_ID=1, joint_ID=0)

#sim.Send_Synapse(sourceNeuronID=0,targetNeuronID=1,weight=1.0)
sim.Send_Developing_Synapse(source_neuron_ID=0,target_neuron_ID=1,
							start_weight=1.0, end_weight=-1.0,
							start_time=0, end_time=1.0)
sim.Start()

sim.Wait_To_Finish()
