
class Mixin(object):

    def _send_sensor(self, *args):
        return self._send_entity('Sensor', *args)

    # ------------ POSITION SENSOR -----------------------------
    def send_position_sensor(self, body_id, which_dimension='x'):
        """Send position sensor which tracks the body specified in body_id

        Parameters
        ----------
        body_id         : int
            The id tag of the specifed body
        which_dimension : str or int (optional)
            Specifies which dimension to track: 'x', 'y', or 'z'.
            You can also use 0, 1, or 2. (default is 'x')

        Returns
        -------
        int
            The id tag of the sensor
        """
        if which_dimension == 'x':
            which_dimension = 0
        elif which_dimension == 'y':
            which_dimension = 1
        elif which_dimension == 'z':
            which_dimension = 2
        else:
            which_dimension = -1

        assert which_dimension >=0 and which_dimension <=2, ('Which dimension must be x, y, z or 0, 1, 2')
        self._assert_body(body_id, 'body_id')

        return  self._send_sensor('PositionSensor', body_id, which_dimension)

    def send_position_x_sensor(self, body_id):
        """Send x position sensor which tracks the body specified in *body_id*"""
        return self.send_position_sensor(body_id, which_dimension=0)

    def send_position_y_sensor(self, body_id):
        """Send y position sensor which tracks the body specified in *body_id*"""
        return self.send_position_sensor(body_id, which_dimension=1)

    def send_position_z_sensor(self, body_id):
        """Send z position sensor which tracks the body specified in *body_id*"""
        return self.send_position_sensor(body_id, which_dimension=2)

    # ----------- RAY SENSOR ------------------------------------
    def send_ray_sensor(self, ray_id, which_sense='d'):
        """
        Sends a ray sensor to the simulator.

        Ray sensors connect to ray entities. They can return the distance
        to this object or the color of this object (r, g, b).

        Parameters
        ----------
        ray_id      :  int
            The id tag of the ray to attach to.
        which_sense : str or int (optional)
            Specifies which sense of the ray to return.
            Use 'd' for distance, 'r' for red, 'g' for green, 'b' for blue.
        
        Returns
        -------
        int
            the id tag of the sensor.
        """
        if which_sense == 'd':
            which_sense = 0
        elif which_sense == 'r':
            which_sense = 1
        elif which_sense == 'g':
            which_sense = 2
        elif which_sense == 'b':
            which_sense = 3

        assert which_sense >=0 and which_sense <=3, ('Which sense must be d, r, g, b or 0, 1, 2, 3')
        self._assert_body(ray_id, 'ray_id')
        return self._send_sensor('RaySensor', ray_id, which_sense)

    def send_ray_distance_sensor(self, ray_id):
        return self.send_ray_sensor(ray_id, which_sense='d')

    def send_ray_red_sensor(self, ray_id):
        return self.send_ray_sensor(ray_id, which_sense='r')

    def send_ray_green_sensor(self, ray_id):
        return self.send_ray_sensor(ray_id, which_sense='g')

    def send_ray_blue_sensor(self, ray_id):
        return self.send_ray_sensor(ray_id, which_sense='b')

    # --------------- TOUCH SENSOR -----------------------------------
    def send_touch_sensor(self, body_id):
        """Attach a touch sensor to a body

        Parameters
        ----------
        body_id   : int
            The id tag of the body to attach the sensor to

        Returns
        -------
        int
            The id tag of the sensor
        """

        self._assert_body(body_id)

        return self._send_sensor('TouchSensor', body_id)


    # ------------- QUATERNION SENSOR --------------------------------
    def send_quaternion_sensor(self, body_id, which_sense='a'):

        self._assert_body(body_id)
        return self._send_sensor('QuaternionSensor', which_sense)

