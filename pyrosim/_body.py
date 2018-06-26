# specialized class designed to be Mixin to pyrosim.py
# Contains all send and add functions related to bodies,
# geoms, physical things, etc.

class Mixin(object):
    def _send_body(self, *args):
        return self._send_entity('Body', *args)

    def add_impulse_to_body(self,
                            body_id,
                            force,
                            time_step=0):
        self._assert_body(body_id, 'body_id')
        self._send_add_command(body_id,
                               'Impulse',
                               time_step,
                               force
                               )

    def add_box_to_composite(self,
                         composite_id,
                         position = (0.0, 0.0, 0.0),
                         orientation = (0.0, 0.0, 1.0),
                         sides = (0.25, 0.25, 0.25),
                         density = 1.0,
                         color = (1.0, 1.0, 1.0)):
        self._assert_body(composite_id, 'composite_id')
        self._send_add_command(composite_id,
                               'Geom',
                               'Box', 
                               position,
                               orientation,
                               sides,
                               density,
                               color)

    def add_cylinder_to_composite(self,
                                  composite_id,
                                  position = (0.0, 0.0, 0.0),
                                  orientation = (0.0, 0.0, 1.0),
                                  length=0.5,
                                  radius=0.05,
                                  capped=True,
                                  density = 1.0,
                                  color = (1.0, 1.0, 1.0)):
        self._assert_body(composite_id, 'composite_id')
        capped = int(capped)
        self._send_add_command(composite_id,
                               'Geom',
                               'Cylinder',
                               position,
                               orientation,
                               length,
                               radius,
                               capped,
                               density,
                               color)

    def add_sphere_to_composite(self,
                                composite_id,
                                position = (0.0, 0.0, 0.0),
                                orientation = (0.0, 0.0, 1.0),
                                radius=0.25,
                                density=1.0,
                                color=(1.0, 1.0, 1.0)):
        self._assert_body(composite_id, 'composite_id')
        self._send_add_command(composite_id,
                               'Geom',
                               'Sphere',
                               position,
                               orientation,
                               radius,
                               density,
                               color)

    def send_ray(self,
                 body_id,
                 position,
                 orientation,
                 max_length=10.0):
        self._assert_body(body_id, 'composite_id')
        return self._send_body('Ray', body_id, position, orientation, max_length)

    def send_box(self,
                 position = (0.0, 0.0, 0.0),
                 orientation = (0.0, 0.0, 1.0),
                 sides = (0.25, 0.25, 0.25),
                 density = 1.0,
                 color = (1.0, 1.0, 1.0),
                 space = None,
                 collision_group = None):

        if collision_group is None:
            collision_group = self._current_collision_group

        if space is None:
            space = self._current_space

        return self._send_body('Box',
                               position,
                               orientation,
                               sides,
                               density,
                               color,
                               space,
                               collision_group)

    def send_cylinder(self,
                      position = (0.0, 0.0, 0.0),
                      orientation = (0.0, 0.0, 1.0),
                      length = 0.5,
                      radius = 0.05,
                      capped = True,
                      density = 1.0,
                      color = (1.0, 1.0, 1.0),
                      space = None,
                      collision_group = None):

        if collision_group is None:
            collision_group = self._current_collision_group

        if space is None:
            space = self._current_space

        capped = int(capped)

        return self._send_body('Cylinder',
                               position,
                               orientation,
                               length,
                               radius,
                               capped,
                               density,
                               color,
                               space,
                               collision_group)

    def send_composite_body(self, space=None, collision_group=None):

        if collision_group is None:
            collision_group = self._current_collision_group

        if space is None:
            space = self._current_space

        return self._send_body('Composite',
                               space,
                               collision_group)

    def send_height_map(self, height_matrix,
                        position=(0, 0, 0),
                        size=10.0,
                        height_scale=1.0):
        import numpy as np
        M, N= np.shape(height_matrix)

        try:
            len(size)
        except:
            size = (size, size)

        assert (len(size) == 2)

        height_vec = height_matrix.ravel('C')

        return self._send_entity('Entity',
                                 'HeightMap',
                                  position,
                                  M, N,
                                  height_vec,
                                  size,
                                  height_scale,
                                  0.0, # offset (unecessary with position)
                                  1.0, # min aabb thickness
                                  0, # infinite wrap, not implemented
                                  )

    def send_sphere(self,
                    position = (0.0, 0.0, 0.0),
                    orientation = (0.0, 0.0, 1.0),
                    radius = 0.25,
                    density = 1.0,
                    color = (1.0, 1.0, 1.0),
                    space = None,
                    collision_group = None):

        if collision_group is None:
            collision_group = self._current_collision_group

        if space is None:
            space = self._current_space

        return self._send_body('Sphere',
                               position,
                               orientation,
                               radius,
                               density,
                               color,
                               space,
                               collision_group)