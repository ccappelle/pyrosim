from __future__ import division, print_function
import numpy as np
import os
import subprocess

# C.C NOTE: Mixin convention - 
# mixin files should be named _name.py and
# contain one Mixin class labeled Mixin.
# see _body.py for more info


# C.C. we can probably remove this after a certain point
# this is necessary for being able to run
# both as __main__ and as package
if __package__ is None or __package__ == '':
#uses current directory visibility
  import _body
  import _joint
else:
#uses current package visibility
  from . import _body
  from . import _joint

class Simulator(_body.Mixin, _joint.Mixin):
    """Python Interface for ODE robotics simulator

    Attributes
    ----------
    TO DO
    """

    def __init__(self,
                 eval_steps=100,
                 dt=0.01,
                 play_blind=False,
                 play_paused=False,
                ):

        # location of this file
        self._this_file_path = os.path.dirname(os.path.abspath(__file__))
        # location of simulator executable
        self._simulator_path = os.path.join(self._this_file_path, 'simulator/build')

        self._num_entities = 0

        # commands to be sent
        self._strings_to_send=''

        # playback parameters
        self._play_blind = play_blind
        self._play_paused = play_paused

        # body parameters
        self._current_space = 'default'
        self._current_collision_group = 'default'

        # sim parameters
        self._eval_steps = eval_steps
        self._dt = dt

        self._raw_cerr = ''

    def start(self):
        """Start the simulation"""

        commands = [self._simulator_path + '/simulator']
        # if self._play_blind:
        #     commands.append('-blind')
        if self._play_paused:
            commands.append('-pause')

        # create pipe to simulator
        self.pipe = subprocess.Popen(
                commands,
                bufsize=0, # necessary to not halt
                stdout=subprocess.PIPE, # connects stdout
                stderr=subprocess.PIPE, # connects stderr
                stdin=subprocess.PIPE, # connects stdin
                universal_newlines=True, # necessary for 3.x
                cwd=self._simulator_path, # helps with textures
                )
        # write parameters
        self._send_simulator_parameters()
        self.pipe.stdin.write(self._strings_to_send)
        # finish by writing done
        self.pipe.stdin.write('Done\n')

    def wait_to_finish(self):
        """Communicate with pipe once sim is complete"""

        data = self.pipe.communicate()
        self._raw_cout = data[0]
        self._raw_cerr = data[1]

        #cut out annoying drawstuff commands
        start_str = 'Simulation Starting'
        end_str = 'Simulation Completed'

        start_index = self._raw_cerr.find(start_str)
        end_index = self._raw_cerr.find(end_str)


        if not (start_index == -1 or end_index == -1):
            self._raw_cerr = self._raw_cerr[:start_index+len(start_str) + 1] + '...\n' + self._raw_cerr[end_index:]

    def get_error_out(self):
        """Returns raw output from cerr"""
        return self._raw_cerr

    def _send(self, command, *args):
        assert isinstance(command, str), ('Command must be string')

        # each entry is delimited by \n
        string_to_send = command + '\n'
        for arg in args:
            string_to_send +=  str(arg) + '\n'

        self._strings_to_send += string_to_send

    def _send_add_command(self, *args):
        self._send('Add', *args)

    def _send_entity(self, *args):
        self._send('Entity', *args)
        self._num_entities += 1

    def _send_parameter(self, *args):
        self._send('Parameter', *args)

    def _send_simulator_parameters(self):

        # send camera

        # send gravity

        # send eval steps
        self._send_parameter('EvalSteps', int(self._eval_steps))
        # send DT
        self._send_parameter('DT', self._dt)
        
if __name__ == '__main__':
    
    sim = Simulator(play_blind=False, play_paused=True)

    sim.send_cylinder(capped=False)

    sim.start()

    sim.wait_to_finish()

    print(sim._raw_cerr)
