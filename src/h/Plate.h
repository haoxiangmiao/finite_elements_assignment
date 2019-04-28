#pragma once

#include "Element.h"

using namespace std;

//! Plane element class
class CPlate : public CElement
{
public:

//!	Constructor
	CPlate();

//!	Desconstructor
	~CPlate();

//!	Read element data from stream Input
	virtual bool Read(ifstream& Input, unsigned int Ele, CMaterial* MaterialSets, CNode* NodeList);

//!	Write element data to stream
	virtual void Write(COutputter& output, unsigned int Ele);

//! Generate location matrix: the global equation number that corresponding to each DOF of the element
//	Caution:  Equation number is numbered from 1 !
    virtual void GenerateLocationMatrix();

//!	Calculate element stiffness matrix
	virtual void ElementStiffness(double* Matrix);

//!	Calculate element stress
	virtual void ElementStress(double* stress, double* Displacement);

//! Recover element stress
	virtual void RecoverElementStress(double* Displacement, double* A);

//!	Calculate element stress for plot
	virtual void ElementStressplot1(double* newx, double* Displacement);

//!	Calculate element stiffness matrix 
	virtual void ElementMass(double* Mass);


//!	Return the size of the element stiffness matrix (stored as an array column by column)
	virtual unsigned int SizeOfStiffnessMatrix();

// Caculate Gravity of Elements
	virtual void GravityCalculation();
};