#ifndef _HEIGHTMAP_HPP
#define _HEIGHTMAP_HPP

#pragma once

#include <ode/ode.h>

#include "entity.hpp"
#include "pythonReader.hpp"

// hard code here for both simulator efficiency
// and convenience
#define MAX_SAMPLES 50 

class HeightMap : public Entity{
protected:
    dHeightfieldDataID heightMapID;
    dGeomID geom;
    double heightData[MAX_SAMPLES * MAX_SAMPLES]; // N x M matrix
    int N, M;
    dReal realDim[2];
    dReal scale, thickness, offset;
    int wrap;
    dReal position[3];
    std::string spaceName;
public:
    HeightMap(){this->drawName="Body";};

    void readFromPython(void){
        readValueFromPython<dReal>(this->position, 3);

        // read in size
        readValueFromPython<int>(&this->M, "M");
        readValueFromPython<int>(&this->N, "N");
        
        // read in each row and column into one array
        readValueFromPython<double>(this->heightData,
                                    this->N * this->M,
                                    "Height Data");

        readValueFromPython<dReal>(this->realDim, 2, "Real Dimension");

        readValueFromPython<dReal>(&this->scale, "Scale");
        readValueFromPython<dReal>(&this->offset, "Offset");
        readValueFromPython<dReal>(&this->thickness, "Thickness");
        
        readValueFromPython<int>(&this->wrap, "Wrap");
    }

    void create(Environment *environment){
        std::cerr << "Creating HeightMap " << std::endl;
        dSpaceID space = environment->getSpace(this->spaceName);

        this->heightMapID = dGeomHeightfieldDataCreate (); 
        const double *constData = this->heightData;

        dGeomHeightfieldDataBuildDouble(this->heightMapID,
                                        constData,
                                        0,
                                        this->realDim[0],
                                        this->realDim[1],
                                        this->M,
                                        this->N,
                                        this->scale,
                                        this->offset,
                                        this->thickness,
                                        this->wrap);

        // necessary to set bounds
        // TEMPORARY HARD CODE FOR TESTING
        dGeomHeightfieldDataSetBounds(this->heightMapID, (dReal) 0.0, (dReal) 4.0);

        // create geom
        this->geom = dCreateHeightfield(space, this->heightMapID, 1);

        // rotate so z is up
        dMatrix3 R;
        dRSetIdentity(R);
        // get rotation from {0, 1, 0} to {0, 0, 1}
        dRFromAxisAndAngle( R, 1, 0, 0, 3.14159265359 / 2.0);

        // place geom
        dGeomSetRotation(this->geom, R);
        dGeomSetPosition(this->geom,
                         this->position[0],
                         this->position[1],
                         this->position[2]);
    }

    void draw(void){
        // set color to green, arbitrary
        dsSetColorAlpha(0.2, 0.7, 0.2, 1.0);

        const dReal *pos = dGeomGetPosition(this->geom);
        const dReal *rot = dGeomGetRotation(this->geom);

        dReal xStep = this->realDim[0] / (this->N - 1);
        dReal yStep = this->realDim[1] / (this->M - 1);
        // draw triangles which make up the mesh

        // x starting point for grid
        dReal currX = -this->realDim[0] / 2.0f + pos[0];

        for(int i=0; i < this->N - 1; ++i){
            // loop through x direction
            // y starting point for grid
            dReal currY = -this->realDim[1] / 2.0f + pos[1];
            for(int j=0; j < this->M - 1; ++j){
                // loop through y direction
                // triangle points constituting square of grid
                dReal a[3], b[3], c[3], d[3]; 

                // NOTE: Default height map has Y coordinate (e.g. a[1])
                // this triangle will be rotated same as the geom

                // starting bottom left (SE)
                a[0] = currX;
                a[2] = currY;
                a[1] = this->heightData[i * this->M + j] * this->scale;
                // point to bottom right (SW)
                b[0] = currX + xStep;
                b[2] = currY;
                b[1] = this->heightData[(i + 1) * this->M + j] * this->scale;
                // point to top left (NE)
                c[0] = currX;
                c[2] = currY + yStep;
                c[1] = this->heightData[i * this->M + (j + 1)] * this->scale;
                // point to top right
                d[0] = currX + xStep;
                d[2] = currY + yStep;
                d[1] = this->heightData[(i + 1) * this->M + (j + 1)] * this->scale;

                // draw square, winding order determines face
                // culling so draw both sides
                dsDrawTriangle(pos, rot, a, b, c, 1); // lower triangle
                dsDrawTriangle(pos, rot, b, c, d, 1); // upper triangle
                dsDrawTriangle(pos, rot, a, c, b, 1); // lower triangle
                dsDrawTriangle(pos, rot, b, d, c, 1); // upper triangle

                currY += yStep;
            }
            currX += xStep;
        }
    };

    void writeToPython(void){
        // write out data in meaningful way
    };
};

#endif