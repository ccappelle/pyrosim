import pyrosim

sim = pyrosim.Simulator(play_blind=False, play_paused=True, debug=True)

MAIN_HEIGHT = 1.0
MAIN_WIDTH = MAIN_HEIGHT/5.0

print MAIN_WIDTH

sim.send_cylinder(x=0, y=0, z=MAIN_HEIGHT+MAIN_WIDTH,
                  r1=0, r2=0, r3=1,
                  length=MAIN_HEIGHT, radius=MAIN_WIDTH,
                  )


sim.start()
sim.wait_to_finish()
