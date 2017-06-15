import sys
sys.path.insert(0, '..')

import pyrosim


def run(test):
    if test:
        sim = pyrosim.Simulator(
            debug=False, play_blind=True)
    else:
        sim = pyrosim.Simulator(
            debug=True, play_blind=False)

    sim.send_cylinder(r1=0, r2=1, r3=0)

    sim.start()
    sim.wait_to_finish()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(True)
        print('Successfully oriented cylinder')
    else:
        run(False)
