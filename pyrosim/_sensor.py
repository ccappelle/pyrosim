
class Mixin(object):

    def _send_sensor(self, *args):
        return self._send_entity('Sensor', *args)

    # ------------ POSITION SENSOR -----------------------------
    def send_position_sensor(self, body_id, which_dimension='x'):
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
        return self.send_position_sensor(body_id, which_dimension=0)

    def send_position_y_sensor(self, body_id):
        return self.send_position_sensor(body_id, which_dimension=1)

    def send_position_z_sensor(self, body_id):
        return self.send_position_sensor(body_id, which_dimension=2)

    # ----------- RAY SENSOR ------------------------------------
    def send_ray_sensor(self, ray_id, which_sense='d'):

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



