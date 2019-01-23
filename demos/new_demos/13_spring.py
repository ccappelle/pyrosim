import sys
sys.path.insert(0, '../../')
import pyrosim

def send_length_example( sim ):
    fixed_box = sim.send_box( position = ( -2, 0, 1 ) )
    sim.send_slider_joint( -1, fixed_box, joint_range = 0 )
    free_box = sim.send_box( position = ( -2.5, 0, 1 ) )
    sim.send_length_spring_joint( fixed_box, free_box,
                                  resting_length = 0.5,
                                  stiffness = 1.0 )

def send_hinge_example( sim ):
    fixed_box = sim.send_box( position = ( 0.5, 0, 1 ),
                              color = ( 1, 0, 0 ) )
    sim.send_slider_joint( -1, fixed_box, joint_range = 0 )
    free_box = sim.send_box( position = ( -0.5, 0, 1 ),
                             color = ( 1, 0, 0 ) )
    sim.send_hinge_spring_joint( fixed_box, free_box,
                             stiffness = 0.5,
                             axis1 = ( 0, 1, 0 ),
                             axis2 = ( 0, 0, 1 ),
                             damping = 0.01 )

def send_linear_example( sim ):
    box1 = sim.send_box( position = ( 2, 0, 1 ),
                         color = ( 0, 1, 0 ) )
    box2 = sim.send_box( position = ( 2.5, 0, 1 ),
                                color = ( 0, 1, 0 ) )
    sim.send_linear_spring_joint( box1, box2,
                                  stiffness = 1.0,
                                  resting_length = 0.75,
                                  damping = 0.01 )

sim = pyrosim.Simulator( eval_steps = -1, play_paused = True )
sim.set_friction( mu = 0 )

sim.set_current_collision_group( 'springs' )
send_length_example( sim )
send_linear_example( sim )
send_hinge_example( sim )

sim.set_current_collision_group( 'environment' )

# send env box
env_box = sim.send_box( position = ( -0.6, 0, 4 ),
                        color = ( 0, 0, 0 ) )
sim.assign_collision( 'springs', 'environment' )

sim.start()
sim.wait_to_finish()

print(sim._raw_cerr)