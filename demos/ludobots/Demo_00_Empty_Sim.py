import sys
sys.path.insert(0, '../..')
import pyrosim

sim = pyrosim.Simulator(debug=True, play_blind=False)
sim.start()
sim.wait_to_finish()
