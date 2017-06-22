
class Bodies(object):
    """Parent class for rigid bodies"""

    def __init__(self):
        self.position = [0]*3
        self.orientation = [0]*3
        self.color = [0]*3

    def set_position(self,new_position):
        """Set the position [x,y,z]"""
        self.position = list(new_position)

    def set_orientation(self,new_orientation):
        """Set the orientation [r1,r2,r3]"""
        self.orientation = list(orientation)

    def set_color(self,new_color):
        """Set the color [r,g,b]"""
        self.color = list(color)

    def set_parameter(self, param, value):
        """Set the parameter to the specified value"""
        setattr(param, value)


class Cylinder(Bodies):
    """Cylinders shaped rigid body"""

    def __init__(self, position=[0, 0, 0], orientation=[0, 0, 1],
                 color=[1, 1, 1], length=0.5, radius=0.05):
        super(Cylinder,self).__init__()
        self.set_position(position)
        self.set_color(color)
        self.set_orientation(orientation)

        self.length = length
        self.radius = radius

    def send_to_simulator(self,sim,position_offset=[0,0,0]):
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

        sim.send_cylinder()



