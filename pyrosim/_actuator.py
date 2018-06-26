class Mixin(object):

    def send_rotary_actuator(self,
                             joint_id,
                             max_force=-1,
                             speed=1.0,
                             control='positional'):

        actuator_id = self._num_entities

        self._send_entity('RotaryActuator',
                            joint_id,
                            max_force,
                            speed,
                            control)

        return actuator_id