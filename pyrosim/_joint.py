

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

        joint_id = self._num_entities

        self._send_entity("Hinge",
                          body1, body2,
                          anchor,
                          axis,
                          joint_range)

        return joint_id

    def send_slider_joint(self,
                          body1, body2,
                          axis = (0, 0, 1),
                          joint_range = 0.5):

        if joint_range is None:
            joint_range = (1, -1) # no stops

        try:
            len(joint_range)
        except:
            joint_range = (-joint_range, joint_range)
        else:
          assert(len(joint_range)) == 2, "joint_range must have 2 entries"
        assert(joint_range[1] >= joint_range[0]) , "Slider joint cannot have infinite range"

        joint_id = self._num_entities

        self._send_entity("Slider",
                          body1, body2,
                          axis,
                          joint_range)

        return joint_id
