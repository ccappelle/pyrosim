import sys
sys.path.insert(0, '..')

import pyrosim


def run(test):

    if test:
        kwargs = {'debug': False, 'play_blind': True, 'eval_time': 500}
    else:
        kwargs = {'debug': True, 'play_blind': False, 'eval_time': 500}

    sims = [0]*10
    for s in range(0, 10):
        sims[s] = pyrosim.Simulator(**kwargs)
        sims[s].send_cylinder(x=0, y=0, z=0.1, r1=0, r2=1,
                              r3=0, length=1.0, radius=0.1)
        sims[s].start()

    for s in range(0, 10):
        sims[s].wait_to_finish()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        run(True)
        print('Successfully made multiple simulators')
    else:
        run(False)
