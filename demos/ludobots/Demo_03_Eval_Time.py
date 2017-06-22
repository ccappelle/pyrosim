import sys
sys.path.insert(0, '../..')

import pyrosim


def run(test):
    if test:
        sim = pyrosim.Simulator(
            debug=False, play_blind=True, eval_time=1000)
    else:
        sim = pyrosim.Simulator(
            debug=True, play_blind=False, eval_time=1000)

    sim.send_cylinder()

    sim.start()
    sim.wait_to_finish()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(True)
        print('Successfully ran for 1000 time steps')
    else:
        run(False)
