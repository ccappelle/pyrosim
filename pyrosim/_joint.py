
class Mixin(object):

    def _send_joint(self, *args):
        return self._send_entity('Joint', *args)

    def send_hinge_joint(self,
                         body1, body2,
                         anchor,
                         axis = (0, 0, 1),
                         joint_range = None,
                         # other parameters to be added
                         ):
        """Send a hinge joint to the simulator to attach two bodies

        A hinge joint allows bodies to rotate in relation to one another
        about the specified axis. By using -1 in place of a body id tag
        the joint will connect a point in space to the other body. Specifying
        `joint_range` places stopping limits on the joint.

        Parameters
        ----------
        body1       : int
            The id tag of the first body. Specify -1 in order to set body1
            to the 'world'
        body2       : int 
            The id tag of the second body. Specify -1 in order to set body2
            to the 'world'
        anchor      : triple float
            The location of the joint.
        axis        : triple float (optional)
            The axis the bodies rotate about. (default is about the z-axis
            [0, 0, 1])
        joint_range : float or tuple (optional)
            The range of movement the joint can take in radians. The starting
            joint position is always at 0. By specifying a single value, *a*,
            the joint range becomes (*-a*, *+a*). You can also directly specify
            the low and high stops by directly specifying the tuple. (default is
            None meaning there are no stops)
        Returns
        -------
        int
            Id tag of the joint
        """

        self._assert_body(body1, 'body1')
        self._assert_body(body2, 'body2')
        
        assert not(body1 == -1 and body2 == -1), ('Both bodies cannot be the world')
        if joint_range is None:
            joint_range = (1, -1) # no stops

        try:
            len(joint_range)
        except:
            joint_range = (-joint_range, joint_range)

        assert(len(joint_range)) == 2
        assert(len(anchor)) == 3
        assert(len(axis)) == 3

        return self._send_joint("HingeJoint",
                          body1, body2,
                          anchor,
                          axis,
                          joint_range)

    def send_slider_joint(self,
                          body1, body2,
                          axis = (0, 0, 1),
                          joint_range = 0.5):
        """Send a slider joint to the simulator
    
        Slider joints allow two bodies to move linearly in relation to one
        another according to the specified axis.

        Parameters
        ----------
        body1       : int
            The id tag of the first body. Specify -1 in order to set body1
            to the 'world'
        body2       : int 
            The id tag of the second body. Specify -1 in order to set body2
            to the 'world'
        axis        : triple of floats (optional)
            The axis of the slider joint. (default is along the z-axis
            [0, 0, 1])
        joint_range : float or tuple of floats (optional)
            The limits the slider can move. By specifying a single float, *a*,
            high and low limits are create (*-a*, *a*). (default is 0.5)

        Returns
        -------
        int
            Id tag of the joint
        """

        self._assert_body(body1, 'body1')
        self._assert_body(body2, 'body2')
        if joint_range is None:
            joint_range = (1, -1) # no stops
        try:
            len(joint_range)
        except:
            joint_range = (-joint_range, joint_range)
        else:
          assert(len(joint_range)) == 2, "joint_range must have 2 entries"

        assert(joint_range[1] >= joint_range[0]) , "Slider joint cannot have infinite range"

        return self._send_joint("SliderJoint",
                          body1, body2,
                          axis,
                          joint_range)

    def send_ball_and_socket_joint(self,
                                   body1, body2,
                                   anchor = (0, 0, 1)):
        """Send a ball and socket joint to the simulator

        Ball and sockets maintain the distance between the bodies and the joint

        Parameters
        ----------
        body1       : int
            The id tag of the first body. Specify -1 in order to set body1
            to the 'world'
        body2       : int 
            The id tag of the second body. Specify -1 in order to set body2
            to the 'world'
        anchor      : triple of floats (optional)
            The absolute starting position of the joint.
        """

        self._assert_body(body1, 'body1')
        self._assert_body(body2, 'body2')

        return self._send_joint("BallAndSocketJoint",
                                body1, body2,
                                anchor)



    def send_universal_joint(self,
                             body1, body2,
                             anchor,
                             axis1=(0, 0, 1),
                             axis2=(1, 0, 0)):

        """Send a ball and socket joint to the simulator

        Ball and sockets maintain the distance between the bodies and the joint

        Parameters
        ----------
        body1       : int
            The id tag of the first body. Specify -1 in order to set body1
            to the 'world'
        body2       : int 
            The id tag of the second body. Specify -1 in order to set body2
            to the 'world'
        anchor      : triple of floats (optional)
            The absolute starting position of the joint.
        """

        self._assert_body(body1, 'body1')
        self._assert_body(body2, 'body2')

        return self._send_joint("UniversalJoint",
                                body1, body2,
                                anchor, axis1, axis2)