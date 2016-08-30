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

def Add_Left_Leg():

	x = -LEG_LENGTH

	y = 0

	z = LEG_LENGTH

	length = LEG_LENGTH

	radius = LEG_RADIUS

	r1 = -1

	r2 = 0

	r3 = 0

	sim.Send_Cylinder(x = x , y = y , z = z , length = length , radius = radius , r1 = r1 , r2 = r2, r3 = r3)

# -------------- Main function -------------

sim = PYROSIM(playPaused = True , evalTime = 1000)

Add_Main_Body()

Add_Left_Leg()

sim.Start()
