class Mixin(object):

    def send_synapse(self, source_neuron_id, target_neuron_id, weight):
        self._send_entity('Synapse',
                          source_neuron_id,
                          target_neuron_id,
                          weight)

    def send_bias_neuron(self, value):

        neuron_id = self._num_entities
        self._send_entity('BiasNeuron', value)

        return neuron_id

    def send_sensor_neuron(self, sensor_id):

        neuron_id = self._num_entities
        self._send_entity('SensorNeuron', sensor_id)
        return neuron_id

    def send_motor_neuron(self, motor_id, alpha=0.0, tau=1.0, starting_value=0.0):

        neuron_id = self._num_entities
        self._send_entity('MotorNeuron', motor_id, alpha, tau, starting_value)

        return neuron_id

    def send_user_neuron(self, input_values):

        neuron_id = self._num_entities
        self._send_entity('UserNeuron', len(input_values), input_values)

        return neuron_id

    def send_hidden_neuron(self, alpha=1.0, tau=1.0,
                           starting_value=0.0):

        neuron_id = self._num_entities
        self._send_entity('HiddenNeuron',
                          alpha, tau, starting_value)

        return neuron_id