
class Robot(object):
    """docstring for Robot"""

    def __init__(self):
        self.bodies = []
        self.joints = []
        self.sensors = []

        self.network = None

    def add_cylinder(self, **kwargs):
        pass

    def add_body(self, body):
        self.bodies.append(body)

    def add_box(self, **kwargs):
        pass

    def add_sphere(self, **kwargs):
        pass

    def add_joint(self, **kwargs):
        pass

    def send_body(self, sim):


    def send_to_simulator(self, sim):
        self.send_bodies(sim)


