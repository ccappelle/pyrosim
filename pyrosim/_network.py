class Mixin(object):

    def _send_neuron(self, *args):
        return self._send_entity('Neuron', *args)

    def _send_synapse(self, *args):
        return self._send_entity('Synapse', *args)

    def send_synapse(self, source_neuron_id, target_neuron_id, weight):

        self._assert_neuron(source_neuron_id, 'source_neuron_id')
        self._assert_neuron(target_neuron_id, 'target_neuron_id')

        return self._send_synapse('Synapse',
                          source_neuron_id,
                          target_neuron_id,
                          weight)

    def send_bias_neuron(self, value=1.0):
        
        return self._send_neuron('BiasNeuron', value)

    def send_sensor_neuron(self, sensor_id):

        self._assert_sensor(sensor_id, 'sensor_id')
        return self._send_neuron('SensorNeuron', sensor_id)

    def send_motor_neuron(self, motor_id, alpha=0.0, tau=1.0, starting_value=0.0):

        self._assert_actuator(motor_id, 'motor_id')
        return self._send_neuron('MotorNeuron', motor_id, alpha, tau, starting_value)

    def send_user_neuron(self, input_values):

        return self._send_neuron('UserNeuron', len(input_values), input_values)

    def send_hidden_neuron(self, alpha=1.0, tau=1.0,
                           starting_value=0.0):

        return self._send_neuron('HiddenNeuron',
                                 alpha, tau, starting_value)