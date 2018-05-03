from __future__ import division, print_function
import math
import os
import numpy as np

from subprocess import Popen, PIPE
import subprocess

import errno
import shutil

from datetime import datetime

#simulator init constants
evaluation_time = 100;
dt = 0.05;
hpr=[121,-27.5000,0.0000];
xyz=[0.8317,-0.9817,0.8000];
gravity = -1.0;

def make_sure_path_exists(path):
    try:
        shutil.rmtree(path, ignore_errors=True)
        os.makedirs(path)
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise


class Simulator(object):
    """Python interface for ODE simulator

    Attributes
    ----------
    play_blind   : bool, optional
            If True the simulation runs without graphics (headless) else if 
            False the simulation runs with graphics (the default is False)
    play_paused : bool, optional
            If True the simulation starts paused else if False the simulation 
            starts running. With simulation window in focus use Ctrl-p to 
            toggle pausing the simulation. (the default is False)
    eval_time    : int, optional
            The number of discrete steps in the simulation (the default is 100)
    dt          : float, optional
            The time in seconds between physics world steps. Larger dt values 
            create more unstable physics. (the default is 0.05)
    gravity     : float, optional
            The gravity in the system. Negative values implies normal downward 
            force of gravity. (default is -1.0)
    window_size  : tuple or list of 2 ints, optional
            The initial window size for the visualization. Irrelevant if 
            blind=True. Default is (750, 500)
    xyz         : list of 3 floats
            The xyz position of the camera (default is [0.8317,-0.9817,0.8000])
    hpr         : float, optional
            The heading, pitch, and roll of the camera 
            (default is [121,-27.5,0.0])
    use_textures : bool, optional
            Draw default ODE textures or not during simulation. 
            (default is False)
    debug       : bool, optional
            If True print out every string command sent through the pipe to 
            the simulator (the default is False)
    capture     : bool, optional
            If True captures frames of the simulation every capture
            timesteps.  Meaningless if playing blind.  (the default is False) 
    """

    WORLD = -1
    FOREVER = -1

    def __init__(self, play_blind=False, play_paused=False,
                 eval_time=evaluation_time, dt=dt,
                 gravity=gravity,
                 window_size = (750,500),
                 xyz=xyz, hpr=hpr, use_textures=False,
                 debug=False, capture=0):
        assert play_blind == False or eval_time > 0, ('Cannot run'
                                                      ' blind forever')
        assert eval_time > 0, ('Cannot run forever: FIXXX MEEE')

        self.strings_to_send = []

        self._num_bodies = 0
        self._num_joints = 0
        self._num_sensors = 0
        self._num_neurons = 0
        self._collision_groups = []
        self._collision_matrix = None
        self._matrix_created = False

        self.play_paused = play_paused
        self.play_blind = play_blind
        self.eval_time = eval_time
        self.dt = dt
        self.gravity = gravity
        self.debug = debug
        self.use_textures = use_textures

        self.capture = capture
        if (self.capture):
            make_sure_path_exists('frame')

        self.evaluated = False
        self.collision_matrix_sent = False

        self.pyrosim_path = os.path.dirname(
            os.path.abspath(__file__))+'/simulator'

        self.body_to_follow = -1

        if debug:
            print ('Simulator exec location ', self.pyrosim_path, '\n')
            print ('Python send commands: ')

        if (self.play_paused == True and self.play_blind == True):
            self.play_paused = False

        # Initial simulator commands
        self._send('TexturePath', self.pyrosim_path+'/textures')
        self._send('EvaluationTime', self.eval_time)
        self._send('TimeInterval', self.dt)
        self._send('Gravity', self.gravity)
        self._send('WindowSize', *window_size)
        if (self.capture):
            self._send('Capture', 1)
        else:
            self._send('Capture', 0)

        if (self.debug):
            self._send('Debug', 1)
        else:
            self._send('Debug', 0)
        self.send_camera(xyz, hpr)

# ------Collisions-------------------------
    def create_collision_matrix(self, collision_type='none'):
        """Create a predefined collision matrix

        Parameters
        ----------
        collision_type : str, optional
            Creates a collision matrix specifying the type of 
            collision between groups desired. There are 4 options
            'none' - no collision
            'inter' - collisions on only between different groups
            'intra' - collisions on only within groups
            'all' - both inter and intra group collisions

        Returns
        -------
        bool
            True if successful
        """

        assert not self._matrix_created, ('Collision matrix' +
                                          ' already created')

        self._matrix_created = True

        num_groups = self.get_num_groups()

        self._collision_matrix = np.zeros((num_groups, num_groups),
                                          dtype='int8')
        if collision_type == 'all':
            self._collision_matrix += 1
        elif collision_type == 'inter':
            indices = np.diag_indices(num_groups)
            self._collision_matrix[indices] = 1
        elif collision_type == 'intra':
            self._collision_matrix += 1
            indices = np.diag_indices
            self._collision_matrix[indices] = 0
        elif collision_type == 'none':
            pass

        return True

    def assign_collision(self, group_1, group_2):
        """Create collision potential between group 1 and group 2

        Parameters
        ----------
        group_1 : str
            The string handle of group 1
        group_2 : str
            The string handle of group 2

        Returns
        -------
        bool
            True if successful
        """

        if not self._matrix_created:
            self.create_collision_matrix()
            self._matrix_created = True

        group_1 = self.get_group_id(group_1)
        group_2 = self.get_group_id(group_2)

        self._collision_matrix[group_1, group_2] = 1
        self._collision_matrix[group_2, group_1] = 1

        return True

    def remove_collision(self, group_1, group_2):
        """Turn off collision potential between group 1 and 2"""

        if not self._matrix_created:
            self.create_collision_matrix()
            self._matrix_created = True

        group1 = self.get_group_id(group1)
        group2 = self.get_group_id(group2)

        self._collision_matrix[group1, group2] = 0
        self._collision_matrix[group2, group1] = 0
# ------Getters--------------------------

    def get_data(self):
        """Get all sensor data back as numpy matrix"""
        assert self.evaluated == True, 'Simulation has not run yet'
        return self.data

    def get_group_id(self, group):
        """Returns the id of the collision group"""
        try:
            index = self._collision_groups.index(group)
        except ValueError:
            index = -1
        return index

    def get_group_names(self):
        """Returns a list containing the collision groups names"""
        return list(self._collision_groups)

    def get_num_bodies(self):
        """Returns the number of bodies"""
        return self._num_bodies

    def get_num_groups(self):
        """Returns the number of collision groups"""
        return len(self._collision_groups)

    def get_num_joints(self):
        """Returns the number of joints"""
        return self._num_joints

    def get_num_neurons(self):
        """Returns the number of neurons"""
        return self._num_neurons

    def get_num_sensors(self):
        """Returns the number of sensors"""
        return self._num_sensors

    def get_sensor_data(self, sensor_id, svi=0):
        """Get the post simulation data from a specified sensor

        Parameters
        ----------
        sensor_id : int
                the sensors id tag
        svi      : int , optional
                The sensor value index. Certain sensors have multiple values 
                (e.g. the position sensor) and the svi specifies which to 
                access (e.g. for a position sensor, svi=0 corresponds to the
                x value of that sensor)

        Returns
        -------
        list of float
                Returns the list of sensor values over the simulation.
        """
        assert self.evaluated == True, 'Simulation has not run yet'
        return self.data[sensor_id, svi, :]

# -----Camera---------------------------
    def send_camera(self, xyz, hpr):
        """Sends camera position to simulator in eulerian coordinates

        Parameters
        ----------
        xyz : list of floats
                A length 3 list specifying the x,y,z position of the camera
                in simulation
        hpr : list of floats
                A length 3 list specifying the heading, pitch, 
                and roll of the camera in degrees

        Returns
        -------
        bool
                True if successful, False otherwise
        """
        self._send('Camera',
                   xyz[0], xyz[1], xyz[2],
                   hpr[0], hpr[1], hpr[2])

        return True

    def film_body(self, body_id, method='follow'):
        """Sets the camera to film a body

        Camera has two modes: 'follow' moves the camera's position based on where
        the body is moving and 'track' rotates the camera to look at the body

        Parameters
        ----------
        body_id : int
                The id tag of the body to be filmed
        method  : str, optional
                The way the camera should move to film the body. 
                Either 'follow' or 'track' (default is 'follow')

        Returns
        -------
        bool
                True if successful, False otherwise
        """
        assert body_id < self._num_bodies, 'Body with id ' + \
            str(body_id) + ' has not been sent'
        assert self.body_to_follow == -1, 'Body with id ' + \
            str(body_id) + ' has already been assigned to be filmed'
        assert (method == 'follow' or method ==
                'track'), 'Method must be `follow` or `track`'

        if method == 'follow':
            self._send('FollowBody', body_id)
        elif method == 'track':
            self._send('TrackBody', body_id)

        self.body_to_follow = body_id

        return True

# -----Bodies----------------------------------
    def send_box(self, x=0, y=0, z=0, mass=1.0,
                 r1=0, r2=0, r3=1,
                 length=0.1, width=0.1, height=0.1,
                 collision_group='default',
                 r=1, g=1, b=1):
        """Send box body to the simulator

        Parameters
        ----------
        x        : float, optional
                The x position coordinate of the center (default 0)
        y        : float, optional
                The y position coordinate of the center (default 0)
        z        : float, optional
                The z position coordinate of the center (default 0)
        mass  : float, optional 
                The mass of the body (default is 1.0)
        length   : float, optional
                The length of the box
        width   : float, optional
                The width of the box
        height  : float, optional
                The height of the box
        collision_group : str or int, optional
                The collision group the body is assigned to. The collision
                group determines how the body collides with other bodies.
                The default group is labeled 'default'.
        r       : float, optional
                The amount of the color red in the body (r in [0,1])
        g       : float, optional
                The amount of the color green in the body (g in [0,1])
        b       : float, optional
                The amount of the color blue in the body (b in [0,1])

        Returns
        -------
        int
                id tag of the box
        """
        assert length > 0, 'Length of Box must be positive'
        assert width > 0, 'Width of Box must be positive'
        assert height > 0, 'Height of Box must be positive'
        self._assert_non_zero('Box', r1, r2, r3)
        self._assert_color('Box', r, g, b)

        if collision_group in self._collision_groups:
            group_id = self.get_group_id(collision_group)
        else:
            group_id = self._add_group(collision_group)

        body_id = self._num_bodies
        self._num_bodies += 1

        self._send('Box',
                   body_id,
                   x, y, z,
                   r1, r2, r3,
                   length, width, height,
                   mass,
                   group_id,
                   r, g, b)

        return body_id

    def send_sphere(self,
                    x=0, y=0, z=0,
                    r1=0, r2=0, r3=1,
                    radius=0.1, mass=1.0,
                    collision_group='default',
                    r=1, g=1, b=1):
        """Sends a sphere to the simulator

        Parameters
        ----------
        x        : float, optional
                The x position of the center
        y        : float, optional
                The y position of the center
        z        : float, optional
                The z position of the center
        mass  : float, optional 
                The mass of the body (default is 1.0)
        radius   : float, optional
                The radius of the sphere (default is 0.5)
        collision_group : str or int, optional
                The collision group the body is assigned to. The collision
                group determines how the body collides with other bodies.
                The default group is labeled 'default'.
        r       : float, optional
                The amount of the color red in the body (r in [0,1])
        g       : float, optional
                The amount of the color green in the body (g in [0,1])
        b       : float, optional
                The amount of the color blue in the body (b in [0,1])

        Returns
        -------
        int
                The id tag of the sphere
        """
        assert radius >= 0, 'Radius of Sphere must be >= 0'
        self._assert_color('Sphere', r, g, b)

        if collision_group in self._collision_groups:
            group_id = self.get_group_id(collision_group)
        else:
            group_id = self._add_group(collision_group)

        body_id = self._num_bodies
        self._num_bodies += 1

        self._send('Sphere',
                   body_id,
                   x, y, z,
                   r1, r2, r3,
                   radius,
                   mass,
                   group_id,
                   r, g, b)

        return body_id

    def send_cylinder(self,
                      x=0, y=0, z=0,
                      r1=0, r2=0, r3=1,
                      length=1.0, radius=0.1,
                      mass=1.0,
                      collision_group='default',
                      r=1, g=1, b=1,
                      capped=True):
        """Send cylinder body to the simulator

        Parameters
        ----------
        x        : float, optional
                The x position coordinate of the center (default is 0)
        y        : float, optional
                The y position coordinate of the center (default is 0)
        z        : float, optional
                The z position coordinate of the center (default is 0)
        mass  : float, optional 
                The mass of the body (default is 1.0)
        r1       : float, optional
                The orientation along the x axis. The vector [r1,r2,r3]
                specify the direction of the long axis of the cylinder.
                (default is 0)
        r2       : float, optional
                The orientation along the y axis. The vector [r1,r2,r3]
                specify the direction of the long axis of the cylinder.
                (default is 0)
        r3       : float, optional
                The orientation along the z axis. The vector [r1,r2,r3]
                specify the direction of the long axis of the cylinder.
                (default is 1)
        length   : float, optional
                The length of long axis of the cylinder (default is 1.0)
        radius   : float, optional
                The radius of the short axis of the cylinder (default is 0.1)
        r       : float, optional
        collision_group : str or int, optional
                The collision group the body is assigned to. The collision
                group determines how the body collides with other bodies.
                The default group is labeled 'default'.
        r       : float, optional
                The amount of the color red in the body (r in [0,1])
        g       : float, optional
                The amount of the color green in the body (g in [0,1])
        b       : float, optional
                The amount of the color blue in the body (b in [0,1])
        capped  : bool, optional
                Use a hemisphere cap at the end of the cylinder or not.
                Collision detection in flat-ended cylinders usually takes
                more effort.

        Returns
        -------
        int
                The id tag of the cylinder
        """
        assert length >= 0, 'Length of Cylinder must be >= 0'
        assert radius >= 0, 'Radius of Cylinder must be >= 0'
        self._assert_non_zero('Cylinder', r1, r2, r3)
        self._assert_color('Cylinder', r, g, b)

        if collision_group in self._collision_groups:
            group_id = self.get_group_id(collision_group)
        else:
            group_id = self._add_group(collision_group)

        body_id = self._num_bodies
        self._num_bodies += 1

        if capped:
            name = 'Capsule'
        else:
            name = 'Cylinder'

        self._send(name,
                   body_id,
                   x, y, z,
                   r1, r2, r3,
                   length, radius,
                   mass,
                   group_id,
                   r, g, b)

        return body_id

    def send_adhesion_susceptibility(self, body_id, adhesion_kind):
        """Mark a body as susceptible to adhesion of a certain kind

        All bodies are susceptible to adhesion of zeroth kind. Mark some
        bodies as susceptible to adhesion of a certain kind to make only
        the adhesive actuators of matching and zeroth kind stick to them.

        Parameters
        ----------
        body_id        : int
                The body id of the body to be marked
        adhesion_kind : int
                Identifier of the adhesion kind

        Returns
        -------
        bool
            True if successful

        """
        assert body_id < self._num_bodies, 'Body with id ' + \
            str(body_id) + ' has not been sent'

        self._send('MakeObjectSusceptibleToAdhesionKind',
                   body_id,
                   adhesion_kind)

        return True


# --------Joints------------------------------
    def send_fixed_joint(self, first_body_id, second_body_id):
        """Fix two bodies (or a body and space) together

         This is implemented by using a hing joint and setting the hi and low 
         stop parameter to 0. This is probably not the best way to do it...

         Parameters
         ----------
         first_body_id   : int
                 The body id of the first body the joint is connected to.
                 If set equal to -1, the joint is connected to a point in
                 space 
         second_body_id  : int
                 The body id of the second body the joint is connected to.
                 If set equal to -1, the joint is connected to a point in
                 space 

         Returns
         -------
         bool
             True if successful
        """
        assert first_body_id < self._num_bodies, 'Body with id ' + \
            str(first_body_id) + ' has not been sent'
        assert second_body_id < self._num_bodies, 'Body with id ' + \
            str(second_body_id) + ' has not been sent'
        self.send_hinge_joint(first_body_id, second_body_id, lo=0, hi=0,
                              torque=10000)
        return True

    def send_hinge_joint(self, first_body_id, second_body_id, x=0, y=0, z=0,
                         n1=0, n2=0, n3=1,
                         lo=-math.pi/4.0, hi=+math.pi/4.0,
                         speed=1.0, torque=10.0, position_control=True):
        """Send a hinge joint to the simulator

        Hinge joints rotate around the axis specified by [n1,n2,n3]

        Parameters
        ----------
        first_body_id   : int
                The body id of the first body the joint is connected to.
                If set equal to -1, the joint is connected to a point in
                space 
        second_body_id  : int
                The body id of the second body the joint is connected to.
                If set equal to -1, the joint is connected to a point in
                space 
        x               : float, optional
                The x position coordinate of the joint (default is 0)
        y               : float, optional
                The y position coordinate of the joint (default is 0)
        z               : float, optional
                The z position coordinate of the joint (default is 0)
        n1              : float, optional
                The orientation along the x axis. The vector [n1,n2,n3]
                specifies the axis about which the joint rotates
                (default is 0)
        n2              : float, optional
                The orientation along the y axis. The vector [n1,n2,n3]
                specifies the axis about which the joint rotates
                (default is 0)
        n3              : float, optional
                The orientation along the z axis. The vector [n1,n2,n3]
                specifies the axis about which the joint rotates
                (default is 1)
        lo              : float, optional
                The lower limit in radians of the joint (default is -pi/4)
        hi              : float, optional
                The upper limit in radians of the joint (default is pi/4)
        speed           : float, optional
                The speed of the motor of the joint (default is 1.0)
        torque          : float, optional
                The maximum amount torque the motor in the joint can use
                (default is 10.0)
        position_control : bool, optional
                True means use position control. This means the motor neuron
                output is treated as a target angle for the joint to actuate
                to. False means the motor neuron output is treated as a target
                actuation rate.

        Returns
        -------
        int 
                The id tag for the hinge joint
        """
        assert first_body_id < self._num_bodies, 'Body with id ' + \
            str(first_body_id) + ' has not been sent'
        assert second_body_id < self._num_bodies, 'Body with id ' + \
            str(second_body_id) + ' has not been sent'
        assert speed >= 0, ('Speed of Hinge Joint must be greater'
                            'than or equal to zero')
        assert torque >= 0, ('Torque of Hinge Joint must be greater'
                             'than or equal to zero')
        assert (first_body_id >= 0 or
                second_body_id >= 0), ('Both objects cannot be the world')

        self._assert_non_zero('Hinge Joint', n1, n2, n3)

        joint_id = self._num_joints
        self._num_joints += 1
        if position_control:
            pc = 1
        else:
            pc = 0

        self._send('HingeJoint',
                   joint_id,
                   first_body_id, second_body_id,
                   x, y, z,
                   n1, n2, n3,
                   lo, hi,
                   speed, torque,
                   pc)

        return joint_id

    def send_slider_joint(self, first_body_id, second_body_id,
                          x=0, y=0, z=1,
                          lo=-.25, hi=+.25,
                          speed=1.0, strength=10.0, position_control=True):
        """Send a slider joint to the simulator

                Slider joints push and pull two bodies along the axis defined
                by [x_dir,y_dir,z_dir]

        Parameters
        ----------
        first_body_id   : int
                The body id of the first body the joint is connected to.
                If set equal to -1, the joint is connected to a point in
                space 
        second_body_id  : int
                The body id of the second body the joint is connected to.
                If set equal to -1, the joint is connected to a point in
                space 
        x          : float, optional
                The orientation along the x axis.
                (default is 0)
        y           : float, optional
                The orientation along the y axis. 
                (default is 0)
        z          : float, optional
                The orientation along the z axis. 
                (default is 1)
        lo              : float, optional
                The lower limit in simulator units of the joint
                (default is -1.0)
        hi              : float, optional
                The upper limit in simulator units of the joint 
                (default is 1.0)
        speed           : float, optional
                The speed of the motor of the joint (default is 10.0)
        strength          : float, optional
                The maximum amount of force the motor in the joint can use
                (default is 1.0)
        position_control : bool, optional
                True means use position control. This means the motor neuron
                output is treated as a position for the joint to actuate
                to. False means the motor neuron output is treated as a target
                actuation rate.

        Returns
        -------
        int 
                The id tag for the hinge joint
        """
        assert first_body_id < self._num_bodies, 'Body with id ' + \
            str(first_body_id) + ' has not been sent'
        assert second_body_id < self._num_bodies, 'Body with id ' + \
            str(second_body_id) + ' has not been sent'
        assert speed >= 0, ('Speed of Hinge Joint must be greater'
                            'than or equal to zero')
        assert strength >= 0, ('Strength must be greater'
                               'than or equal to zero')
        assert (first_body_id >= 0 or
                second_body_id >= 0), ('Both objects cannot be the world')

        joint_id = self._num_joints
        self._num_joints += 1
        if position_control:
            pc = 1
        else:
            pc = 0

        self._send('SliderJoint',
                   joint_id,
                   first_body_id, second_body_id,
                   x, y, z,
                   lo, hi,
                   speed, strength,
                   pc)

        return joint_id

    def send_adhesive_joint(self, body_id, adhesion_kind=0):
        """Send an adhesive joint to the simulator.

           Whenever it is actuated, all bodies that are touched by the body
           that has this joint will stick (form a rigid joint) to it.

        Parameters
        ----------
        body_id   : int
                The body id of the body the joint is connected to.

        adhesion_kind   :  int
                ID of the mechanism of adhesion. All bodies with no
                exceptions are adhesive to joints with adhesion_kind=0; for
                if the value is not zero, only the bodies that are marked
                as being susceptible to the adhesion of the kind with that
                id will form rigid joints.

        Returns
        -------
        int
                The id tag for the adhesive joint
        """
        assert body_id < self._num_bodies, 'Body with id ' + \
            str(body_id) + ' has not been sent'

        joint_id = self._num_joints
        self._num_joints += 1

        self._send('AdhesiveJoint',
                   joint_id,
                   body_id,
                   adhesion_kind)

        return joint_id

    def send_thruster(self, body_id, x=0, y=0, z=-1, lo=-10.0, hi=10.0):
        """Send a thruster engine to the specified body

        The thruster engine provides a linear force to the center of
        mass of the assigned body in the specified direction

        body_id : int
            The handle of the body the jet should be attached to
        x       : float, optional
            The x value of the directional vector (default is 0)
        y       : float, optional
            The y value of the directional vector (default is 0)
        z       : float, optional
            The z value of the directional vector (default is 1)
        lo      : float, optional
            The amount of force when the associated motor
            neuron is -1. Negative implies force in the 
            opposit direction. (default is -10)
        hi      : float, optioal
            The amount of force when the associated motor
            neuron is +1. (default is 10)

        Returns
        -------
        int
            The id handle of the thruster
        """
        self._assert_non_zero('Jet', x, y, z)
        assert hi >= lo, 'Hi parameter must be geq to lo parameter'
        assert body_id < self._num_bodies, 'Body must exist'

        joint_id = self._num_joints
        self._num_joints += 1

        self._send('Thruster', joint_id,
                   body_id,
                   x, y, z,
                   lo, hi)

        return joint_id

# -----------Neurons----------------------
    def send_bias_neuron(self):
        """Send bias neuron to simulator.

        Bias neurons emit a constant value of 1.0

        Returns
        -------
        int
                id tag of the neuron
        """
        neuron_id = self._num_neurons
        self._num_neurons += 1

        self._send('BiasNeuron',
                   neuron_id)

        return neuron_id

    def send_motor_neuron(self, joint_id=0, tau=1.0, alpha=1.0,
                          start_value=0.0):
        """Send motor neurons to simulator

        Motor neurons are neurons which connect to a specified joint and 
        determine how the joint moves every time step of simulation

        Warning
        -------
        Sending a motor neuron to a joint whose starting position
        is not in the middle of the 'hi' & 'lo' cutoffs will most likely cause
        instabilities in the simulation. For example creating a joint with
        either 'hi' or 'lo' to 0 and attaching a motor neuron to this joint
        will cause the joint to break. 

        Parameters
        ----------
        joint_id    : int, optional
                The joint id tag of the joint we want the neuron to connect to
        tau         : float, optional
                The 'learning rate' of the neuron. Increasing tau increases
                how much of value of the neuron at the current time step comes
                from external inputs vs. the value of the neuron at the 
                previous time step. (default 1)
        alpha       : float, optional
                The 'remembrance rate' of the neuron. Usually 1 or 0.
                An alpha of 1 helps reduce jitter in positionally controlled
                joints. (default is 1)
        start_value : float, optional
                The starting value of the neuron. This value is specified
                to mitigate the problem of a joint starting not on its midpoint
                for positionally controlled joints. Set to +1 or greater for 
                close to the 'hi' range and -1 or less for close to 'lo' range
                (default is 0.0)

        Returns
        -------
        int
                The id tag of the neuron
        """
        assert tau >= 0, 'Tau must be positive'
        assert joint_id < self._num_joints, 'Joint with id ' + \
            str(joint_id)+' has not been sent'

        neuron_id = self._num_neurons
        self._num_neurons += 1

        self._send('MotorNeuron',
                   neuron_id,  joint_id,
                   tau, alpha, start_value)

        return neuron_id

    def send_sensor_neuron(self, sensor_id=0, svi=0):
        """Sends a sensor neuron to the simulator

        Sensor neurons are input neurons which take the value of their 
        associated sensor

        Parameters
        ----------
        sensor_id        : int, optional
                The associated sensor id for the neuron to draw values from.
        svi : int, optional
                The sensor value index is the offset index of the sensor. 
                SVI is used for sensors which return a vector of values 
                (position, ray sensors, etc.)

        Returns
        -------
        int
                The id tag of the neuron
        """
        assert sensor_id < self._num_sensors, 'Sensor with id ' + \
            str(sensor_id)+' has not been sent'
        assert svi in range(4), 'SVI must be in [0,3]'

        neuron_id = self._num_neurons
        self._num_neurons += 1

        self._send('SensorNeuron',
                   neuron_id, sensor_id, svi)

        return neuron_id

    def send_user_input_neuron(self, in_values):
        """Send neuron to the simulator which takes user defined values

        Parameters
        ----------
        in_values : list of floats or float, optional
                The user specified values for the neuron. If length of 
                values < the number of time steps, the values are 
                continually looped through until every time step
                has a corresponding value

        Returns
        -------
        int
                The id tag of the neuron.
        """
        try:
            iter(in_values)
        except TypeError:
            in_values = [in_values]*self.eval_time

        # if values is shorter than eval_time repeat until sufficient
        if len(in_values) < self.eval_time:
            out_values = [0]*self.eval_time
            for i in range(self.eval_time):
                out_values[i] = in_values[i % len(in_values)]
        else:
            out_values = in_values

        neuron_id = self._num_neurons
        self._num_neurons += 1

        self._send('FunctionNeuron', neuron_id, *out_values)

        return neuron_id

    def send_function_neuron(self, function=math.sin):
        """Send neuron to simulator which takes its value from the  function

        The function is mapped to the specific time in the simulation based on 
        both the discrete evaluation time and the dt space between time steps. 
        For example if evalTime=100 and dt=0.05 the function will be evaluated 
        at [0,0.05,...,5]

        Parameters
        ----------
        function : function, optional
                The function which defines the neuron value. Valid functions 
                return a single float value over the time domain.

        Returns
        -------
        int
                The id tag of the neuron
        """
        assert self.eval_time >= 0, ('Cannot send function neuron'
                                     ' in infinite mode')

        end_time = self.eval_time*self.dt
        time_vals = np.arange(0, end_time, self.dt)
        output_vals = list(map(function, time_vals))

        return self.send_user_input_neuron(output_vals)

    def send_hidden_neuron(self, tau=1.0, alpha=1.0):
        """Send a hidden neuron to the simulator

        Hidden neurons are basic neurons which can have inputs and outputs. 
        They 'hidden' between input neurons (sensors, bias, function) and 
        output neurons (motors)

        Parameters
        ----------
        tau : float, optional
            The 'learning rate' of the neuron. Increasing tau increases
            how much of value of the neuron at the current time step comes
            from external inputs vs. the value of the neuron at the 
            previous time step.
        alpha    :
            The 'remembrance rate' of the neuron. Usually 1 or 0.

        Returns
        -------
        int
            The id tag of the neuron
        """
        assert tau > 0, 'Tau value of Hidden Neuron must be positive'
        neuron_id = self._num_neurons
        self._num_neurons += 1

        self._send('HiddenNeuron', neuron_id, tau, alpha)

        return neuron_id

# ---------PhysicalProperties--------------------
    def send_external_force(self, body_id, x, y, z, time=0):
        """Sends a directed force to a body at a specific time

        Parameters
        ----------
        body_id : int
            The body to apply the force to
        x       : float
            The amount of force in the x direction
        y       : float
            The amount of force in the y direction
        z       : float
            The amount of force in the z direction
        time    : float, optional
            When the force should be applied. (default is 0)

        Returns
        -------
        bool
            True if successful, False otherwise
        """
        assert body_id < self._num_bodies, ('Body with id ' +
                                            body_id+' has not been sent')
        assert time >= 0 and time <= self.eval_time, (
            'Time step must be within eval time')
        self._assert_non_zero('Force', x, y, z)

        self._send('ExternalForce', body_id, x, y, z, time)

        return True

    def send_light_source(self, body_id=0):
        """Attaches light source to a body in simulation

        Parameters
        ----------
        body_id : int, optional
                The body id of the body to attach the light to

        Returns
        -------
        int
                The id tag of the body the light source is attached to.
        """
        assert body_id < self._num_bodies, 'Body id ' + \
            str(body_id)+' has not been sent'

        self._send('LightSource',
                   body_id)

        return body_id

# ----------Sensors----------------------
    def send_is_seen_sensor(self, body_id):
        """Attaches a sensor which detects when a body is being hit by a ray sensor

        Parameters
        ----------
        body_id : int
            The body id to connect the sensor to

        Returns
        -------
        int
            The id tag of the sensor
        """
        assert body_id < self._num_bodies, 'Body id ' + \
            str(body_id)+' has not been sent'

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('IsSeenSensor', sensor_id, body_id)

        return sensor_id

    def send_light_sensor(self, body_id):
        """Attaches a light sensor to a body in simulation

        Parameters
        ----------
        body_id : int, optional
                The body id of the body to connect the sensor to

        Returns
        -------
        int
            The id tag of the sensor
        """
        assert body_id < self._num_bodies, 'Body id ' + \
            str(body_id)+' has not been sent'

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('LightSensor',
                   sensor_id, body_id)

        return sensor_id

    def send_position_sensor(self, body_id=0):
        """Attaches a position sensor to a body in simulation

        Parameters
        ----------
        body_id : int, optional
                The body id of the body to connect the sensor to

        Returns
        -------
        int
                The id tag of the sensor
        """
        assert body_id < self._num_bodies, 'Body with id ' + \
            str(body_id)+' has not been sent'

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('PositionSensor',
                   sensor_id, body_id)

        return sensor_id

    def send_proprioceptive_sensor(self, joint_id=0):
        """Attaches a proprioceptive sensor to a joint in simulation

        Proprioceptive sensors returns the angle of the joint at 
        each time step

        Parameters
        ----------
        joint_id : int, optional
                The joint id of the joint to connect the sensor to

        Returns
        -------
        int
                The id tag of the sensor
        """
        assert joint_id < self._num_joints, 'Joint with id ' + \
            str(joint_id)+' has not been sent'

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('ProprioceptiveSensor',
                   sensor_id, joint_id)

        return sensor_id

    def send_ray_sensor(self, body_id=0,
                        x=0, y=0, z=0,
                        r1=0, r2=0, r3=1,
                        max_distance=10):
        """Sends a ray sensor to the simulator connected to a body

        Ray sensors return four values each time step, the distance and 
        color (r,g,b).

        Parameters
        ----------
        body_id : int, optional
                The body id of the associated body the ray sensor is connected 
                to. When this body moves the ray sensor moves accordingly
        x        : float, optional
                The x position of the sensor
        y        : float, optional
                The y position of the sensor
        z        : float, optional
                The z position of the sensor
        r1       : float, optional
                The x direction of the sensor. The array [r1,r2,r3] is the 
                direction the ray sensor is pointing in the time step.
        r2       : float, optional
                The y direction of the sensor. The array [r1,r2,r3] 
                is the direction the ray sensor is pointing in the time step.
        r3       : float, optional
                The z direction of the sensor. The array [r1,r2,r3] is the 
                direction the ray sensor is pointing in the time step.
        max_distance: float, optional
                The maximum distance away the ray can sense in simulator
                units. (default is 10.0)

        Returns
        -------
        int
                The id tag of the sensor
        """
        assert body_id < self._num_bodies, 'Body with id ' + \
            str(body_id) + ' has not been sent yet'
        self._assert_non_zero('Ray Sensor', r1, r2, r3)

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('RaySensor',
                   sensor_id, body_id,
                   x, y, z,
                   r1, r2, r3,
                   max_distance)

        return sensor_id

    def send_touch_sensor(self, body_id=0):
        """Send touch sensor to a body in the simulator

        Parameters
        ----------
        body_id : int, optional
                The body id of the associated body 

        Returns
        -------
        int
                The id tag of the sensor
        """
        assert body_id < self._num_bodies, ('Body with id '+body_id+' has'
                                            ' not been sent'
                                            )
        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('TouchSensor',
                   sensor_id, body_id)

        return sensor_id

    def send_vestibular_sensor(self, body_id=0):
        """Connects a vestibular sensor to a body

        Vestibular sensors return a bodies orrientation in space

        Parameters
        ----------
        body_id : int, optional
                The body id of the associated body 

        Returns
        -------
        int
                The id tag of the sensor
        """
        assert body_id < self._num_bodies, ('Body with id '+body_id+' has'
                                            ' not been sent'
                                            )

        sensor_id = self._num_sensors
        self._num_sensors += 1

        self._send('VestibularSensor',
                   sensor_id, body_id)

        return sensor_id

# ------Synapses------------------------------
    def send_synapse(self, source_neuron_id=0, target_neuron_id=0,
                     weight=0.0):
        """Sends a synapse to the simulator

        Synapses are the edge connections between neurons

        Parameters
        ----------
        source_neuron_id : int, optional
                The id of the source neuron of the synapse
        target_neuron_id : int, optional
                The id of the target neuron of the synapse
        weight         : float, optional
                The edge weight of the synapse

        Returns
        -------
        bool
                True if successful, False otherwise
        """
        assert source_neuron_id < self._num_neurons, 'Neuron with id ' + \
            str(source_neuron_id)+' has not been sent'
        assert target_neuron_id < self._num_neurons, 'Neuron with id ' + \
            str(target_neuron_id)+' has not been sent'

        return self.send_developing_synapse(source_neuron_id=source_neuron_id,
                                            target_neuron_id=target_neuron_id,
                                            start_weight=weight,
                                            end_weight=weight,
                                            start_time=0.,
                                            end_time=0.)

    def send_developing_synapse(self, source_neuron_id=0, target_neuron_id=0,
                                start_weight=0.0, end_weight=0.0,
                                start_time=0., end_time=1.0):
        """Sends a synapse to the simulator

        Developing synapses are synapses which change over time. 
        The synapse will interpolate between the start_weight and end_weight
        over the desired time range dictated by start_time and end_time.
        start_time and end_time are in [0,1] where 0 maps to time step 0
        and 1 maps to the eval_time of the simulation. Setting start_time
        equal to end_time results in a discrete change from start_weight
        to end_weight in the synapse at the specified time step. If
        start_time >= end_time times are changed such that
        end_time = start_time.

        Parameters
        ----------
        source_neuron_id : int, optional
                The id of the source neuron of the synapse
        target_neuron_id : int, optional
                The id of the target neuron of the synapse
        start_weight       : float, optional
                The starting edge weight of the synapse
        end_weight     : float, optional
                The ending edge weight of the synapse
        start_time     : float, optional
                The starting time of development. start_time in [0,1]
        end_time       : float, optional
                The ending time of development. end_time in [0,1]

        Returns
        -------
        bool
                True if successful, False otherwise
        """
        assert source_neuron_id < self._num_neurons, ('Neuron with id ' +
                                                      str(source_neuron_id) +
                                                      ' has not been sent')
        assert target_neuron_id < self._num_neurons, ('Neuron with id ' +
                                                      str(target_neuron_id) +
                                                      ' has not been sent')

        if start_time >= end_time:
            end_time = start_time

        start_time = int(start_time * (self.eval_time-1))
        end_time = int(end_time * (self.eval_time-1))

        self._send('Synapse',
                   source_neuron_id, target_neuron_id,
                   start_weight, end_weight,
                   start_time, end_time)

        return True

# -----I/OCommands----------------------------
    def make_movie(self,  movie_name=''):
        """Takes captured image files and converts them into a movie

        Uses ffmpeg to convert images. ffmpeg needs to be installed
        for this command to work. Takes images from 'frame' directory

        Parameters
        ----------
        movie_name  : string, optional
            The movies file name. Must include a proper extenison.
            If left blank, default is to make a time stamped file name:
            '%Y-%m-%d-%H-%M-%S_movie.mp4'

        Returns
        -------
        bool
            True if successful, False otherwise
        """
        assert self.capture == True, (
            'No frames captured, set capture to true')

        if movie_name == '':
            time_stamp = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
            movie_name = time_stamp + '_movie.mp4'

        movie_command = 'ffmpeg -r 30 -i %04d.ppm -vcodec' + \
                        ' libx264 -pix_fmt yuv420p ' + movie_name

        try:
            os.chdir('frame')
            subprocess.call(movie_command, shell=True)

        except Exception:
            print('Command sent was: ' + movie_command)
            print('Must have ffmpeg installed')
            return False

        return True

    def start(self):
        """Starts the simulation"""

        assert self.evaluated == False, (
            'Simulation has already been evaluated')

        if (not self.collision_matrix_sent and self.get_num_groups() != 0):
            self._send_collision_matrix()

        # build initial commands
        commands = [self.pyrosim_path + '/simulator']
        if (self.play_blind == True):
            commands.append('-blind')
        else:
            if self.use_textures == False:
                commands.append('-notex')

        if (self.play_paused == True):
            commands.append('-pause')

        self.pipe = Popen(commands, bufsize=0,stdout=PIPE, stdin=PIPE, stderr=PIPE, universal_newlines=True)

        for string_to_send in self.strings_to_send:
            self.pipe.stdin.write(string_to_send)

        self.pipe.stdin.write('Done\n')
        if self.debug:
            print ('Done \n')
            print ('Pipe open with commands: ', commands)
            print ('Starting simulation \n')
            print ('C++ receive commands: ')

        return True

    def wait_to_finish(self):
        """Waits to for the simulation to finish and collects data

        Returns
        -------
        numpy matrix
                A matrix of the sensor values for each time step of 
                the simulation
        """

        data_from_simulator = self.pipe.communicate()

        if self.eval_time >= 0:
            self._collect_sensor_data(data_from_simulator)
            self.evaluated = True

            return self.data
        else:
            self.evaluated = True
            print (data_from_simulator[0])
            print (data_from_simulator[1])
            return 'No results during infinite run'

# --------------------- Private methods ---------------------------
    def _add_group(self, group):
        """Appends group handle to list and returns index"""
        index = len(self._collision_groups)
        self._collision_groups.append(group)

        return index

    def _assert_color(self, name, r, g, b):
        """Error checks so color params are between [0,1]"""

        colors = [r, g, b]
        for color in colors:
            assert color >= 0 and color <= 1, 'Color parameter of ' + \
                name + ' must be in [0,1]'

    def _assert_non_zero(self, name, *args):
        """Error checks vectors so they are not equal to zero"""

        flag = False
        for arg in args:
            if arg != 0:
                flag = True

        assert flag == True, ('Vector parameters of ' + name +
                              ' cannot be all zeros')

    def _collect_sensor_data(self, data_from_simulator):
        """Get sensor data back from ODE and store it in numpy array"""

        self.data = np.zeros([self._num_sensors, 4,
                              self.eval_time], dtype='f')

        debug_output = data_from_simulator[1]

        if self.debug:
            chop_start = debug_output.find('Simulation test environment')
            chop_end = debug_output.find('sideways and up')+15
            if chop_start > 0:
                print (debug_output[:chop_start], debug_output[chop_end:-1])
            else:
                print (debug_output)

        data_from_simulator = data_from_simulator[0]
        data_from_simulator = data_from_simulator.split()

        if (data_from_simulator == []):
            return

        index = 0
        while (data_from_simulator[index] != 'Done'):
            sensor_id = int(data_from_simulator[index])
            index = index + 1

            num_sensor_vals = int(data_from_simulator[index])
            index = index + 1

            for t in range(0, self.eval_time):  # time step
                for s in range(0, num_sensor_vals):  # svi
                    try:
                        sensor_value = float(data_from_simulator[index])
                    except IndexError:
                        print (index, data_from_simulator)
                        print (debug_output)
                        raise IndexError


                    self.data[sensor_id, s, t] = sensor_value
                    index = index + 1

    def _send_collision_matrix(self):
        """sends the collision matrix"""

        self.collision_matrix_sent = True

        if not self._matrix_created:
            self.create_collision_matrix()
            self._matrix_created = True

        send_string = []

        upper_tri = np.triu_indices(self.get_num_groups())
        iterator = np.nditer(self._collision_matrix[upper_tri])

        for element in iterator:
            send_string.append(element)

        self._send('CollisionMatrix', self.get_num_groups(), *send_string)
        return True

    def _send(self, command_string, *args):
        """Send a command to the simulator"""

        # first argument should be a string
        assert isinstance(command_string, str), ('Command must be string')
        string_to_send = command_string
        for arg in args:
            string_to_send += ' ' + str(arg)
        string_to_send += '\n'

        if self.debug:
            print (string_to_send,)
        self.strings_to_send.append(string_to_send)
