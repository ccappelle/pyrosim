class Mixin(object):

    def _send_actuator(self, *args):
        """Sends entity categorized as Actuator"""
        return self._send_entity('Actuator', *args)


    def send_rotary_actuator(self,
                             joint_id,
                             max_force=-1,
                             speed=1.0,
                             control='positional'):
        """Sends a rotary actuator to acto on a hinge joint
        
        Parameters
        ----------
        joint_id  : int
            The id tag of the hinge joint to motorize
        max_force : float (optional)
            The maximum amount of torque the joint can use to move.
            A negative value indicates infinite possible torque.
            (default is infinite)
        speed     : float (optional)
            The speed multiplier used in actuation. (default is 1)
        control   : str (optional)
            There are two control schemes: 'positional' and 'velocity'.
            *positional* indicates the value inputed dictates which angle
            the motor should move to. *velocity* indicates the value inputed
            should specify the velocity of actuation.

        Returns
        -------
        int
            The id tag of the actuator
        """
        self._assert_joint(joint_id, 'joint_id')
        assert speed > 0, ("Speed must be a positive number")
        return self._send_actuator('RotaryActuator',
                                    joint_id,
                                    max_force,
                                    speed,
                                    control)

    def send_linear_actuator(self,
                             joint_id,
                             max_force=-1,
                             speed=1.0,
                             control='positional'):
        """Sends a linear actuator to acto on a slider joint
        
        Parameters
        ----------
        joint_id  : int
            The id tag of the slider joint to motorize
        max_force : float (optional)
            The maximum amount of force the joint can use to move.
            A negative value indicates infinite possible force.
            (default is infinite)
        speed     : float (optional)
            The speed multiplier used in actuation. (default is 1)
        control   : str (optional)
            There are two control schemes: 'positional' and 'velocity'.
            *positional* indicates the value inputed dictates which position
            the motor should move to. *velocity* indicates the value inputed
            should specify the velocity of actuation.

        Returns
        -------
        int
            The id tag of the actuator
        """
        self._assert_joint(joint_id, 'joint_id')
        return self._send_actuator('LinearActuator',
                                    joint_id,
                                    max_force,
                                    speed,
                                    control)

    def send_thruster(self,
                     body_id,
                     force_range=(0.0, 10.0),
                     direction=(0.0, 0.0, 1.0)):
        """Sends a thruster to a body.

        Thrusters act as forces pushing on the center of mass in the specified
        `direction`.
        
        Parameters
        ----------
        body_id     : int
            The id tag of the body to connect to.
        force_range : float tuple (optional)
            A tuple which specifies the low and high range of force
            used to apply to the attached body. (default is (0, 10))
        direction   : float triple (optional)
            The direction the force should be applied. For example,
            if you want the thruster to move the body upward you would
            set `direction` to be upward, i.e. `direction = (0, 0, 1)`

        Returns
        -------
        int
            The id tag of the thruster
        """
        assert len(force_range) == 2, ('force_range must be a tuple')
        assert len(direction) == 3, ('direction must be a triple')
        self._assert_body(body_id, 'body_id')
        return self._send_actuator('ThrusterActuator',
                                    body_id,
                                    force_range,
                                    direction)
