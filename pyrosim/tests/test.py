import pyrosim

from nose.tools import assert_equal
from nose.tools import assert_not_equal
from nose.tools import assert_raises
from nose.tools import raises


class Test_Pyrosim(object):

    @classmethod
    def setup_class(cls):
        pass

    @classmethod
    def teardown_class(cls):
        pass

    def setUp(self):
        self.EVAL_TIME = 1000
        self.sim = pyrosim.Simulator(play_blind=True,
                                     eval_time=self.EVAL_TIME)

    def teardown(self):
        pass

    def test_bodies(self):
        bodies = [0]*4
        pos_sensors = [0]*4

        bodies[0] = self.sim.send_box(x=0, y=0, z=0)
        bodies[1] = self.sim.send_cylinder(x=1, y=0, z=0,
                                           capped=True)
        bodies[2] = self.sim.send_cylinder(x=2, y=0, z=0,
                                           capped=False)
        bodies[3] = self.sim.send_sphere(x=3, y=0, z=0)
        for i in range(4):
            pos_sensors[i] = self.sim.send_position_sensor(bodies[i])

        self.sim.start()
        self.sim.wait_to_finish()
        data = [0]*4
        for i in range(4):
            data[i] = self.sim.get_sensor_data(pos_sensors[i], svi=2)
            print data[i][10]

        KNOWN = [0.044631,
                 1.0875,
                 0.8875,
                 0.0936]
        flag = True
        for i in range(4):
            if abs(data[i][10] - KNOWN[i]) > 0.0001:
                flag = False

        assert flag == True, 'Sending bodies incorrect'

    def test_pendulum(self):
        cyl1 = self.sim.send_cylinder(x=.5, y=0, z=2.5, 
                                      r1=1, r2=0, r3=0)
        cyl2 = self.sim.send_cylinder(x=1.5, y=0, z=2.5, 
                                      r1=1, r2=0, r3=0)
        
        hinge1 = self.sim.send_hinge_joint(pyrosim.Simulator.WORLD,
                                            cyl1, 
                                            x=0, y=0, z=2.5,
                                            n1=0, n2=1, n3=0,
                                            position_control = False)
        hinge2 = self.sim.send_hinge_joint(cyl1, cyl2,
                                            x=1, y=0, z=2.5,
                                            n1=0, n2=1, n3=0,
                                            position_control = True)
        ray_sensor = self.sim.send_ray_sensor(cyl2, x=2.0, y=0, z=2.5,
                                                r1=1, r2=0, r3=0)

        fneuron = self.sim.send_function_neuron()
        mneuron = self.sim.send_motor_neuron(hinge1)

        self.sim.send_synapse(fneuron, mneuron, weight=1.0)

        self.sim.start()
        self.sim.wait_to_finish()

        data = self.sim.get_sensor_data(ray_sensor, svi=0)
        assert abs(0.698613-data[200])<0.000001, 'Pendulum fail'

