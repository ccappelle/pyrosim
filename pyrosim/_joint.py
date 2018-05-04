

class Mixin(object):

    def send_hinge_joint(self,
                         body1, body2,
                         anchor,
                         axis = (0, 0, 1),
                         joint_range = None,
                         # other parameters to be added
                         ):

        if joint_range is None:
            joint_range = (1, -1) # no stops

        try:
            len(joint_range)
        except:
            joint_range = (-joint_range, joint_range)

        assert(len(joint_range)) == 2

        self._send_entity("Hinge",
                          body1, body2,
                          *anchor,
                          *axis,
                          *joint_range)
