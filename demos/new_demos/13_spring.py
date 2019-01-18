import sys
sys.path.insert(0, '../../')
import pyrosim


sim = pyrosim.Simulator( eval_steps = -1, play_paused = True )
sim.set_friction( mu = 0.0 )
# send boxes
# send linear spring example
sim.set_current_collision_group( 'linear' )
linear_box1 = sim.send_box( position = ( 1, 0, 1 ),
                            color = ( 0, 1, 0 ) )
linear_box2 = sim.send_box( position = ( 2, 0, 1 ),
                            color = ( 0, 1, 0 ) )

# send hinge joint example
sim.set_current_collision_group( 'hinge' )
hinge_box1 = sim.send_box( position = ( 0, 0, 1 ), 
                           color = ( 1, 0, 0 ) )
hinge_box2 = sim.send_box( position = ( -1, 0, 1 ), 
                           color = ( 1, 0, 0 ) )
# connect box1 to the world
sim.send_slider_joint( -1, hinge_box1, joint_range = ( 0, 0 ) )

# send box to interact
sim.set_current_collision_group( 'env' )
env_box = sim.send_box( position = ( -1.2, 0, 4 ),
                        color = ( 0, 0, 0 ) )

sim.assign_collision( 'env', 'hinge' )


# send springs
sim.send_linear_spring_joint( linear_box1, linear_box2,
                              stiffness = 1.0,
                              resting_length = 0.75 )
sim.send_hinge_spring_joint( hinge_box1, hinge_box2,
                             stiffness = 0.5,
                             axis1 = ( 0, 1, 0 ),
                             axis2 = ( 0, 0, 1 ) )
# sim.set_current_collision_group( 'spring' )

# box1 = sim.send_box( position = ( 1, 0, 1 ), color = ( 1, 0, 0) )
# # fix box to world
# sim.send_hinge_joint( -1, box1, anchor = ( 1, 0, 1 ), joint_range = ( 0, 0 ) )

# box2 = sim.send_box( position = ( 0, 0, 1 ), color = ( 1, 0, 0) )

# # sim.set_current_collision_group( 'box' )
# # box3 = sim.send_box( position = ( -0.1, 0, 4 ), density = 8.0 )
# axis1 = ( 0, 0, 1 )
# axis2 = ( 0, 1, 0 )

# sim.send_hinge_spring_joint( box1, box2, axis1 = axis1, axis2 = axis2 )

# # sim.send_linear_spring_joint( box1, box2,
# #                               stiffness = 1.0,
# #                               resting_length = 0.5 )
# sim.set_friction( mu = 0.1 ) 
# # sim.assign_collision( 'spring', 'box' )
sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)