import numpy as np


class Bodies(object):
    """Parent class for rigid bodies"""

    def __init__(self):
        self.position = [0]*3
        self.orientation = [0]*3
        self.color = [0]*3

    def set_position(self, new_position):
        """Set the position [x,y,z]"""
        self.position = list(new_position)

    def set_orientation(self, new_orientation):
        """Set the orientation [r1,r2,r3]"""
        self.orientation = list(new_orientation)

    def set_color(self, new_color):
        """Set the color [r,g,b]"""
        self.color = list(new_color)

    def set_parameter(self, param, value):
        """Set the parameter to the specified value"""
        assert hasattr(self, param), (self.__class__.__name__ +
                                      ' has no parameter ' + param)
        setattr(self, param, value)


class Cylinder(Bodies):
    """A cylinder shaped rigid body"""

    def __init__(self, position=[0, 0, 0], orientation=[0, 0, 1],
                 mass=1.0, color=[1, 1, 1], length=0.5, radius=0.05,
                 capped=True):
        super(Cylinder, self).__init__()
        self.set_position(position)
        self.set_color(color)
        self.set_orientation(orientation)

        self.length = length
        self.radius = radius

        self.capped = capped

    def send_to_simulator(self, sim, position_offset=[0, 0, 0]):
        """Sends the cylinder to the specified simulator

        Parameters
        ----------
        sim             : Simulator instance
            The simulator to send the body to
        position_offset : float[3], optional
            The x,y,z offset for the body, effectively moving it

        Results
        -------
        int
            The id tag of the body
        """
        x, y, z = np.array(self.position) + np.array(position_offset)
        r1, r2, r3 = self.orientation
        r, g, b = self.color
        my_id = sim.send_cylinder(x, y, z,
                                  mass=self.mass,
                                  r1=r1, r2=r2, r3=r3,
                                  length=self.length,
                                  radius=self.radius,
                                  r=r, g=g, b=b,
                                  capped=self.capped)
        return my_id

class Box(Bodies):
    """A box shaped rigid body"""

    def __init__(self):
        pass


if __name__ == '__main__':
    c = Cylinder()
    c.set_parameter('value', 10)
