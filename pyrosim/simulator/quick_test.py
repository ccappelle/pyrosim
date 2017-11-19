import pyrosim


sim = pyrosim.Simulator(play_blind=False,
                        eval_time=100, dt=0.05,
                        window_size=(1000, 750),
                        debug=True)

cyl = sim.send_cylinder(x=0, y=0, z=2, 
                    r1=1, r2=0, r3=0)

sim.send_hinge_joint(-1, cyl, x=.5, y=0, z=2,
                     n1=0, n2=1, n3=0,
                     position_control=False)
sim.send_ray_sensor(cyl, x=-.5, y=0, z=2,
                    r1=-1, r2=0, r3=0)

box = sim.send_box(x=0, y=0, z=.4, width=.2, height=.2, length=.2)
# seen_sensor = sim.send_is_seen_sensor(box)


sim.start()
sim.wait_to_finish()

# data = sim.get_sensor_data(seen_sensor)


# print data