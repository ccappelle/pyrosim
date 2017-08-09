import numpy as np


class Body(object):
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

    def send_to_simulator(self, sim, position_offset=[0, 0, 0]):
        self.x, self.y, self.z = np.array(
            self.position) + np.array(position_offset)
        self.r1, self.r2, self.r3 = self.orientation
        self.r, self.g, self.b = self.color


class Cylinder(Body):
    """A cylinder shaped rigid body"""

    def __init__(self, position=[0, 0, 0], orientation=[0, 0, 1],
                 mass=1.0,
                 color=[1, 1, 1],
                 length=0.5, radius=0.05,
                 collision_group='default',
                 capped=True):
        super(Cylinder, self).__init__()
        self.set_position(position)
        self.set_color(color)
        self.set_orientation(orientation)

        self.length = length
        self.radius = radius

        self.capped = capped

        self.collision_group = collision_group

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
        super(Cylinder, self).send_to_simulator(sim, position_offset)
        my_id = sim.send_cylinder(self.x, self.y, self.z,
                                  mass=self.mass,
                                  r1=r1, r2=r2, r3=r3,
                                  length=self.length,
                                  radius=self.radius,
                                  r=r, g=g, b=b,
                                  capped=self.capped,
                                  collision_group=self.collision_group)
        return my_id


class Box(Body):
    """A box shaped rigid body"""

    def __init__(self, position=[0, 0, 0], orientation=[0, 0, 1],
                 mass=1.0, color=[1, 1, 1],
                 length=0.1, width=0.1, height=0.1,
                 collision_group='default'):
        super(Box, self).__init__()
        self.set_position(position)
        self.set_color(color)
        self.set_orientation(orientation)

        self.length = length
        self.width = width
        self.height = height

        self.collision_group = collision_group

    def send_to_simulator(self, sim, position_offset=[0, 0, 0]):
        """Sends the box to the specified simulator

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
        super(Box, self).send_to_simulator(sim, position_offset)
        my_id = sim.send_box(self.x, self.y, self.z,
                             mass=self.mass,
                             r1=self.r1, r2=self.r2, r3=self.r3,
                             length=self.length,
                             width=self.width,
                             height=self.height,
                             r=self.r, g=self.g, b=self.b,
                             capped=self.capped,
                             collision_group=self.collision_group
                             )
        return my_id

class Sphere(Body):
    """A sphere shaped rigid body"""

    def __init__(self, position=[0, 0, 0], orientation=[0, 0, 1],
                 mass=1.0, color=[1, 1, 1],
                 radius=0.1,
                 collision_group='default'):

        super(Sphere, self).__init__()
        self.set_position(position)
        self.set_color(color)
        self.set_orientation(orientation)

        self.radius = radius

        self.collision_group = collision_group

    def send_to_simulator(self, sim, position_offset=[0, 0, 0]):
    """Sends the box to the specified simulator

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
        super(Sphere, self).send_to_simulator(sim, position_offset)
        my_id = sim.send_box(self.x, self.y, self.z,
                             mass=self.mass,
                             r1=self.r1, r2=self.r2, r3=self.r3,
                             radius=self.radius,
                             r=self.r, g=self.g, b=self.b,
                             capped=self.capped,
                             collision_group=self.collision_group
                             )

if __name__ == '__main__':
    c = Cylinder()
    c.set_parameter('value', 10)
