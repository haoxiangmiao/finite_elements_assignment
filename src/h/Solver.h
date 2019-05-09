/*****************************************************************************/
/*  STAP++ : A C++ FEM code sharing the same input data file with STAP90     */
/*     Computational Dynamics Laboratory                                     */
/*     School of Aerospace Engineering, Tsinghua University                  */
/*                                                                           */
/*     Release 1.11, November 22, 2017                                       */
/*                                                                           */
/*     http://www.comdyn.cn/                                                 */
/*****************************************************************************/

#pragma once

#include "SkylineMatrix.h"
#include "CSRMatrix.h"
#include "SparseMatrix.h"

//!	Base class for a solver
/*	New solver should be derived from this base class, and match the storage scheme
	of the global stiffness matrix employed in Domain class. */

class CSolver
{
public:
	CSolver() {};
};

//!	LDLT solver: A in core solver using skyline storage  and column reduction scheme
class CLDLTSolver : public CSolver
{
public:
	CSkylineMatrix<double>* K;
//!	Constructor
	CLDLTSolver(CSkylineMatrix<double>* _K) : K(_K) {};

//!	Perform L*D*L(T) factorization of the stiffness matrix
	void LDLT();

//!	Reduce right-hand-side load vector and back substitute
	void BackSubstitution(double* Force); 
};

class CSRSolver : public CSolver
{

public:
	
	CSRMatrix<double>* K;
	CSRSolver(CSRMatrix<double>* _K) : K(_K) {};
	void solve(double* Force, unsigned NLCase);
};