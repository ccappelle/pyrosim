import sys
sys.path.insert(0, '../..')
import pyrosim # noqa

pass
# ---- ONLY DO THIS IF YOU KNOW WHAT YOU ARE DOING -------

# the capture option creates a frame every time step
# you must have play_blind=False
# capture creates a directory called 'frames' and stores the image
# files there
# sim = pyrosim.Simulator(eval_time=100, play_blind=False, capture=True)

# Uncomment below to use

# sim.send_cylinder()

# sim.start()
# sim.wait_to_finish()

# sim.make_movie()
