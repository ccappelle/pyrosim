class Mixin(object):

    def _send_neuron(self, *args):
        return self._send_entity('Neuron', *args)

    def _send_synapse(self, *args):
        return self._send_entity('Synapse', *args)

    def send_synapse(self, source_neuron_id, target_neuron_id, weight):
        """Send a synapse to the simulator to connect neurons

        Parameters
        ----------
        source_neuron_id : int
            The id tag of the source neuron
        target_neuron_id : int
            The id tag of the target neuron
        weight           : float
            The weight value of the synapse

        Returns
        -------
        int
            The id tag of the synapse
        """

        self._assert_neuron(source_neuron_id, 'source_neuron_id')
        self._assert_neuron(target_neuron_id, 'target_neuron_id')

        return self._send_synapse('Synapse',
                          source_neuron_id,
                          target_neuron_id,
                          weight)

    def send_bias_neuron(self, value=1.0):
        """Send a bias neuron to the simulator"""
        return self._send_neuron('BiasNeuron', value)

    def send_sensor_neuron(self, sensor_id):
        """Send a sensor neuron to the simulator

        Parameters
        ----------
        sensor_id : int
            The id tag of the sensor to pull values from at each time step.

        Returns
        -------
        int
            The id tag of the neuron
        """
        self._assert_sensor(sensor_id, 'sensor_id')
        return self._send_neuron('SensorNeuron', sensor_id)

    def send_motor_neuron(self, motor_id, alpha=0.0, tau=1.0, starting_value=0.0):
        """Send a motor neuron to the simulator

        The value of the motor neuron at each time step is passed to the specified
        motor in order to determine how it should actuate.

        Parameters
        ----------
        motor_id : float
            The id tag of the motor to send neuron value to
        alpa     : float (optional)
            A 'learning rate' parameter. (default is 0)
        tau      : float (optional)
            A 'learning rate' parameter. (default is 1.0)
        starting_value : float (optional)
            The starting value the neuron takes. Could be usefully when motor does
            not start at 0. (default is 0)

        Returns
        -------
        int
            The id tag of the neuron
        """
        self._assert_actuator(motor_id, 'motor_id')
        return self._send_neuron('MotorNeuron', motor_id, alpha, tau, starting_value)

    def send_user_neuron(self, input_values):
        """Send a user input neuron to the simulator.

        A user input neuron takes pre-specified input supplied by the user before
        simulation. Similar to a bias neuron that changes every time step. This is
        useful to send Central Pattern Generators and other functions.

        Parameters
        ----------
        input_values : list
            The values which the neuron will take every time step. If evaluation
            time is longer than the length of the list, the values will be repeated

        Returns
        -------
        int
            The id tag of the neuron.
        """
        return self._send_neuron('UserNeuron', len(input_values), input_values)

    def send_hidden_neuron(self, alpha=1.0, tau=1.0,
                           starting_value=0.0):

        return self._send_neuron('HiddenNeuron',
                                 alpha, tau, starting_value)