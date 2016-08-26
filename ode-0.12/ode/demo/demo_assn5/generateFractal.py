from math import *

def GenerateCode(pt,angle,currDepth,maxDepth,index):

	if ( currDepth < maxDepth ):

		newPt = [0,0,0];

		newPt[0] = pt[0] + sin((angle-(45.0/(pow(1.25,currDepth))))*1.0*3.14159/180.0);
		newPt[1] = pt[1];
		newPt[2] = pt[2] + cos((angle-(45.0/(pow(1.25,currDepth))))*1.0*3.14159/180.0); 

		#print angle, pt, newPt;
		#print (pt[0]+newPt[0])/2.0, (pt[1]+newPt[1])/2.0, (pt[2]+newPt[2])/2.0;
		#print newPt[0]-pt[0], newPt[1]-pt[1], newPt[2]-pt[2]; 

		string = 'CreateCylinder(';
		string = string + str(index);
		string = string + ',' + str((pt[0]+newPt[0])/2);
                string = string + ',' + str((pt[1]+newPt[1])/2);
                string = string + ',' + str((pt[2]+newPt[2])/2);
		string = string + ',' + str( newPt[0] - pt[0] );
                string = string + ',' + str( newPt[1] - pt[1] );
                string = string + ',' + str( newPt[2] - pt[2] );
		string = string + ',0.1';
		string = string + ',1.0';
		string = string + ');';
		print string; 

		index = index + 1;

                index = GenerateCode(newPt,angle-(45.0/(pow(1.25,currDepth))),currDepth+1,maxDepth,index);

                newPt[0] = pt[0] + sin((angle+(45.0/(pow(1.25,currDepth))))*1.0*3.14159/180.0);
                newPt[1] = pt[1];
                newPt[2] = pt[2] + cos((angle+(45.0/(pow(1.25,currDepth))))*1.0*3.14159/180.0);

                #print angle, pt, newPt;
                #print (pt[0]+newPt[0])/2.0, (pt[1]+newPt[1])/2.0, (pt[2]+newPt[2])/2.0;
                #print newPt[0]-pt[0], newPt[1]-pt[1], newPt[2]-pt[2];

                string = 'CreateCylinder(';
                string = string + str(index);
                string = string + ',' + str((pt[0]+newPt[0])/2);
                string = string + ',' + str((pt[1]+newPt[1])/2);
                string = string + ',' + str((pt[2]+newPt[2])/2);
                string = string + ',' + str( newPt[0] - pt[0] );
                string = string + ',' + str( newPt[1] - pt[1] );
                string = string + ',' + str( newPt[2] - pt[2] );
                string = string + ',0.1';
                string = string + ',1.0';
                string = string + ');';
                print string;

		index = index + 1;
 
		index = GenerateCode(newPt,angle+(45.0/(pow(1.25,currDepth))),currDepth+1,maxDepth,index);

	return index;

# ------------------- Main function ------------------

startingPoint = [0,0,0];

maxDepth = 7;

index = GenerateCode(startingPoint,0,0,maxDepth,0);

