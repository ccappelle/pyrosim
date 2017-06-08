#!/usr/bin/env python

from setuptools import setup
from setuptools.command.install import install

setup(name='pyrosim',
		version='0.1.1',
		description='Python interface for ODE simulator',
		author='Collin Cappelle',
		install_requires=['numpy'],
		packages=['pyrosim'],
		)