from pyrosim import PYROSIM

# ------------ Global variables -------------

LEG_LENGTH = 0.5

LEG_RADIUS = 0.1

# -------------- Functions ------------------

def Add_Main_Body():

	x = 0

	y = 0

	z = LEG_LENGTH

	sim.Send_Box(x=x, y=y, z=z, length=LEG_LENGTH, width=LEG_LENGTH, height=LEG_RADIUS)

# -------------- Main function -------------

sim = PYROSIM(playPaused = True , evalTime = 1000)

Add_Main_Body()

sim.Start()
