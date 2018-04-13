import sys
sys.path.insert(0, '../..')
import pyrosim # noqa

sim = pyrosim.Simulator()
sim.start()
sim.wait_to_finish()
