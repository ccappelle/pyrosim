class Mixin(object):

    def _send_actuator(self, *args):
        return self._send_entity('Actuator', *args)


    def send_rotary_actuator(self,
                             joint_id,
                             max_force=-1,
                             speed=1.0,
                             control='positional'):

        self._assert_joint(joint_id, 'joint_id')
        return self._send_actuator('RotaryActuator',
                                    joint_id,
                                    max_force,
                                    speed,
                                    control)