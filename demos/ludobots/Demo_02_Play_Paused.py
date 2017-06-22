import sys
sys.path.insert(0, '..')

import pyrosim

def run(test):
    if test:
        # note, the simulation won't pause because it is blind
        # this is normal
        sim = pyrosim.Simulator(
            debug=False, play_blind=True, play_paused=True)
    else:
        sim = pyrosim.Simulator(
            debug=True, play_blind=False, play_paused=True)

    sim.send_cylinder()

    sim.start() # press ctrl-p to unpause
    sim.wait_to_finish()
    
if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(True)
        print ('Successfully unpaused when blind')
    else:
        run(False)
