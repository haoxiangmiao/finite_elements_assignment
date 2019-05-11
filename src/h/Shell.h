#pragma once

#include "Element.h"

using namespace std;

class CShell : public CElement
{
public:

	//!	Constructor
	CShell();

	//!	Desconstructor
	~CShell();

	//!	Read element data from stream Input
	virtual bool Read(ifstream& Input, unsigned int Ele, CMaterial* MaterialSets, CNode* NodeList);

	//!	Write element data to stream
	virtual void Write(COutputter& output, unsigned int Ele);

	virtual void WritePlot(COutPlot& output, unsigned int Ele);

	//! Generate location matrix: the global equation number that corresponding to each DOF of the element
	//	Caution:  Equation number is numbered from 1 !
	virtual void GenerateLocationMatrix();

	//!	Calculate element stiffness matrix
	virtual void ElementStiffness(double* Matrix);

	//!	Calculate element mass
	virtual void ElementMass(double* Mass);

	//!	Calculate element stress
	virtual void ElementStress(double* stress, double* Displacement);

	// Caculate Gravity of Elements
	virtual void GravityCalculation(double* ptr_force);

	//!	Calculate the values required in the POSTPROCESS 
	virtual void ElementPostInfo(double* stress, double* Displacement, double* PrePositions, double* PostPositions);

	//!	Return the size of the element stiffness matrix (stored as an array column by column)
	virtual unsigned int SizeOfStiffnessMatrix();

	void ElementCoord(double* coord);
};