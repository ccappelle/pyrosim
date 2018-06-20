class Mixin(object):

    def send_rotary_actuator(self,
                             joint_id,
                             max_force=-1,
                             speed=1.0,
                             control='positional'):

        actuator_id = self._num_actuators

        self._send_actuator('Rotary',
                            joint_id,
                            max_force,
                            speed,
                            control)

        return actuator_id