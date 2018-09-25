# specialized class designed to be Mixin to pyrosim.py
# Contains all send and add functions related to bodies,
# geoms, physical things, etc.

class Mixin(object):
    def _send_body(self, *args):
        """Sends an entity categorized as Body"""
        return self._send_entity('Body', *args)

    def add_impulse_to_body(self,
                            body_id,
                            force,
                            time_step=0):
        """Adds an impulse force to a body at specified time step

        Parameters
        ----------
        body_id   : int
            The id tag of the target body
        force     : triple float
            The force vector to apply
        time_step : int (optional)
            The tme step to apply the specified force
        """
        self._assert_body(body_id, 'body_id')
        assert len(force) == 3, ('Force must be size 3')
        assert time_step >= 0, ('time_step must be >= 0')

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
        """Adds box geometry to a composite body

        Parameters
        ----------
        composite_id : int
            The id tag of the target composite body
        position     : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation  : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        sides        : triple float (optional)
            The length, width, and height of the body. (default [.25, .25, .25])
        density      : float (optional)
            The density of the body
        color        : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        """

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
        """Adds cylinder geometry to a composite body

        Parameters
        ----------
        composite_id : int
            The id tag of the target composite body
        position     : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation  : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        length       : float (optional)
            The length of the long axis of the cylinder. (default 0.5)
        radius       : float (optional)
            The radius of the cylinder. (default 0.05)
        capped       : boolean (optional)
            If true, the cylinder has rounded edges. If false, the cylinder
            has flat edges. (default True)
        density      : float (optional)
            The density of the body
        color        : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        """
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
        """Adds sphere geometry to a composite body

        Parameters
        ----------
        composite_id : int
            The id tag of the target composite body
        position     : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation  : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        radius       : float (optional)
            The radius of the sphere. (default 0.25)
        density      : float (optional)
            The density of the body
        color        : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        """
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
        """Send a box geometry to the simulator

        Parameters
        ----------
        position        : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation     : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        sides           : triple float (optional)
            The length, width, and height of the body. (default [.25, .25, .25])
        density         : float (optional)
            The density of the body
        color           : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        space           : str (optional)
            The string name of the space in which to place the body.
            (default is to use the string set by :func:`set_current_space()` function)
        collision_group : str (optional)
            The string name of the collision group in which to place the body.
            Collision groups specify how bodies collide. (default is to use
            the string set by :func:`set_current_collision_group()` function)
        Returns
        -------
        int
            The id tag of the box
        """

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
        """Send a cylinder geometry to the simulator

        Parameters
        ----------
        position        : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation     : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        length       : float (optional)
            The length of the long axis of the cylinder. (default 0.5)
        radius       : float (optional)
            The radius of the cylinder. (default 0.05)
        capped       : boolean (optional)
            If true, the cylinder has rounded edges. If false, the cylinder
            has flat edges. (default True)
        density         : float (optional)
            The density of the body
        color           : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        space           : str (optional)
            The string name of the space in which to place the body.
            (default is to use the string set by :func:`set_current_space()` function)
        collision_group : str (optional)
            The string name of the collision group in which to place the body.
            Collision groups specify how bodies collide. (default is to use
            the string set by :func:`set_current_collision_group()` function)
        Returns
        -------
        int
            The id tag of the cylinder
        """
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
        """Send a composite body base

        Parameters
        ----------
        space           : str (optional)
            The string name of the space in which to place the body.
            (default is to use the string set by :func:`set_current_space()` function)
        collision_group : str (optional)
            The string name of the collision group in which to place the body.
            Collision groups specify how bodies collide. (default is to use
            the string set by :func:`set_current_collision_group()` function)
        Returns
        -------
        int
            The id tag of the composite body
        """
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
        """Send a height map to the simulator

        Parameters
        ----------
        height_matrix : 2D numpy matrix
            A matrix filled in with the height (z) values. Each dimension
            must be at least size 2
        position      : triple float (optional)
            The center position of the height map. (default is [0, 0, 0])
        size          : float, tuple float (optional)
            The length of each side. When a singular value, height map is
            a square. (default is (10, 10))
        height_scale  : float (optional)
            Height multiplier. (default is 1.0)
        """
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
        """Send a sphere geometry to the simulator

        Parameters
        ----------
        position        : triple float (optional)
            The global position of the body. (default [0, 0, 0])
        orientation     : triple float (optional)
            The gloabl orientation of the body. (default [0, 0, 1])
        radius       : float (optional)
            The radius of the sphere. (default 0.25)
        density         : float (optional)
            The density of the body
        color           : triple float (optional)
            Color of the body. (default is [1, 1, 1])
        space           : str (optional)
            The string name of the space in which to place the body.
            (default is to use the string set by :func:`set_current_space()` function)
        collision_group : str (optional)
            The string name of the collision group in which to place the body.
            Collision groups specify how bodies collide. (default is to use
            the string set by :func:`set_current_collision_group()` function)
        Returns
        -------
        int
            The id tag of the shpere
        """
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