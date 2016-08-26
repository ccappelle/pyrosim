all: simulator

simulator: simulator.o environment.o object.o joint.o positionSensor.o proprioceptiveSensor.o touchSensor.o raySensor.o lightSensor.o vestibularSensor.o neuralNetwork.o neuron.o synapse.o
	/bin/sh ode-0.12/libtool --tag=CXX   --mode=link g++  -g -O2   -o simulator simulator.o environment.o object.o joint.o positionSensor.o proprioceptiveSensor.o touchSensor.o raySensor.o lightSensor.o vestibularSensor.o neuralNetwork.o neuron.o synapse.o ode-0.12/drawstuff/src/libdrawstuff.la ode-0.12/ode/src/libode.la -framework OpenGL -framework GLUT  -lm  -lpthread

synapse.o: synapse.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT synapse.o -MD -MP -c -o synapse.o synapse.cpp

neuron.o: neuron.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT neuron.o -MD -MP -c -o neuron.o neuron.cpp

neuralNetwork.o: neuralNetwork.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT neuralNetwork.o -MD -MP -c -o neuralNetwork.o neuralNetwork.cpp

vestibularSensor.o: vestibularSensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT vestibularSensor.o -MD -MP -c -o vestibularSensor.o vestibularSensor.cpp

lightSensor.o: lightSensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT lightSensor.o -MD -MP -c -o lightSensor.o lightSensor.cpp

raySensor.o: raySensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT raySensor.o -MD -MP -c -o raySensor.o raySensor.cpp

touchSensor.o: touchSensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT touchSensor.o -MD -MP -c -o touchSensor.o touchSensor.cpp

proprioceptiveSensor.o: proprioceptiveSensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT proprioceptiveSensor.o -MD -MP -c -o proprioceptiveSensor.o proprioceptiveSensor.cpp

positionSensor.o: positionSensor.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT positionSensor.o -MD -MP -c -o positionSensor.o positionSensor.cpp

object.o: object.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT object.o -MD -MP -c -o object.o object.cpp

joint.o: joint.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT joint.o -MD -MP -c -o joint.o joint.cpp

environment.o: environment.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT environment.o -MD -MP -c -o environment.o environment.cpp

simulator.o: simulator.cpp
	g++ -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT simulator.o -MD -MP -c -o simulator.o simulator.cpp
