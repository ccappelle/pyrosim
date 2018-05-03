from __future__ import division, print_function
import numpy as np
import os
import subprocess

class Simulator(object):
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

    def add_box_to_composite(self,
                             composite_id,
                             position = (0.0, 0.0, 0.0),
                             orientation = (0.0, 0.0, 1.0),
                             sides = (0.25, 0.25, 0.25),
                             density = 1.0,
                             color = (1.0, 1.0, 1.0)):
        self._send('Add', composite_id,
                    'Box', 
                    *position,
                    *orientation,
                    *sides,
                    density,
                    *color)

    def add_cylinder_to_composite(self,
                                  composite_id,
                                  position = (0.0, 0.0, 0.0),
                                  orientation = (0.0, 0.0, 1.0),
                                  length=0.5,
                                  radius=0.05,
                                  capped=True,
                                  density = 1.0,
                                  color = (1.0, 1.0, 1.0)):
        capped = int(capped)
        self._send('Add',
                   composite_id,
                   'Cylinder',
                   *position,
                   *orientation,
                   length,
                   radius,
                   capped,
                   density,
                   *color)

    def send_box(self,
                 position = (0.0, 0.0, 0.0),
                 orientation = (0.0, 0.0, 1.0),
                 sides = (0.25, 0.25, 0.25),
                 density = 1.0,
                 color = (1.0, 1.0, 1.0),
                 space = None,
                 collision_group = None):

        body_id = self._num_entities
        self._send_entitiy('Box',
                           *position,
                           *orientation,
                           *sides,
                           density,
                           *color,
                           space,
                           collision_group)
        return body_id

    def send_cylinder(self,
                      position = (0.0, 0.0, 0.0),
                      orientation = (0.0, 0.0, 1.0),
                      length = 0.5,
                      radius = 0.05,
                      capped = True,
                      density = 1.0,
                      color = (1.0, 1.0, 1.0),
                      space = None,
                      collision_group = None):

        capped = int(capped)
        body_id = self._num_entities
        self._send_entitiy('Cylinder',
                           *position,
                           *orientation,
                           length,
                           radius,
                           capped,
                           density,
                           *color,
                           space,
                           collision_group)
        return body_id

    def send_composite_body(self, space=None, collision_group=None):

        body_id = self._num_entities
        self._send_entitiy('Composite',
                            space,
                            collision_group)

    def send_height_map(self, height_matrix,
                        position=(0, 0, 0),
                        dimensions=10.0,
                        height_scale=1.0):
        pass 

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
        self._send_parameters()
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

    def _send_entitiy(self, *args):
        self._send('Entity', *args)
        self._num_entities += 1

    def _send_parameter(self, *args):
        self._send('Parameter', *args)

    def _send_parameters(self):

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
