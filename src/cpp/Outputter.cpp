/*****************************************************************************/
/*  STAP++ : A C++ FEM code sharing the same input data file with STAP90     */
/*     Computational Dynamics Laboratory                                     */
/*     School of Aerospace Engineering, Tsinghua University                  */
/*                                                                           */
/*     Release 1.11, November 22, 2017                                       */
/*                                                                           */
/*     http://www.comdyn.cn/                                                 */
/*****************************************************************************/

#include "Domain.h"
#include "Outputter.h"
#include "SkylineMatrix.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>



using namespace std;

//	Output current time and date
void COutputter::PrintTime(const struct tm* ptm, COutputter &output)
{
	const char* weekday[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
							 "Thursday", "Friday", "Saturday"};
	const char* month[] = {"January", "February", "March", "April", "May", "June",
						   "July", "August", "September", "October", "November", "December"};

	output << "        (";
	output << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " on ";
	output << month[ptm->tm_mon] << " " << ptm->tm_mday << ", " << ptm->tm_year + 1900 << ", "
		   << weekday[ptm->tm_wday] << ")" << endl
		   << endl;
}

COutputter* COutputter::_instance = nullptr;

//	Constructor
COutputter::COutputter(string FileName)
{
	OutputFile.open(FileName);

	if (!OutputFile)
	{
		cerr << "*** Error *** File " << FileName << " does not exist !" << endl;
		exit(3);
	}
}

//	Return the single instance of the class
COutputter* COutputter::Instance(string FileName)
{
	if (!_instance)
		_instance = new COutputter(FileName);
	return _instance;
}

//	Print program logo
void COutputter::OutputHeading()
{
	CDomain* FEMData = CDomain::Instance();

	*this << "TITLE : " << FEMData->GetTitle() << endl;

	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	PrintTime(timeinfo, *this);
}

//	Print nodal data
void COutputter::OutputNodeInfo()
{
	CDomain* FEMData = CDomain::Instance();

	CNode* NodeList = FEMData->GetNodeList();

	*this << "C O N T R O L   I N F O R M A T I O N" << endl
		  << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	unsigned int NUMNP = FEMData->GetNUMNP();
	unsigned int NUMEG = FEMData->GetNUMEG();
	unsigned int NLCASE = FEMData->GetNLCASE();
	unsigned int MODEX = FEMData->GetMODEX();

	*this << "      NUMBER OF NODAL POINTS . . . . . . . . . . (NUMNP)  =" << setw(6) << NUMNP << endl;
	*this << "      NUMBER OF ELEMENT GROUPS . . . . . . . . . (NUMEG)  =" << setw(6) << NUMEG << endl;
	*this << "      NUMBER OF LOAD CASES . . . . . . . . . . . (NLCASE) =" << setw(6) << NLCASE << endl;
	*this << "      SOLUTION MODE  . . . . . . . . . . . . . . (MODEX)  =" << setw(6) << MODEX << endl;
	*this << "         EQ.0, DATA CHECK" << endl
		  << "         EQ.1, EXECUTION" << endl
		  << endl;

	*this << " N O D A L   P O I N T   D A T A" << endl << endl;
	*this << "    NODE       BOUNDARY                          NODAL POINT" << endl
		  << "   NUMBER   CONDITION  CODES                     COORDINATES" << endl;

	for (unsigned int np = 0; np < NUMNP; np++)
//		NodeList[np].Write(*this, np);

	*this << endl;
}

//	Output equation numbers
void COutputter::OutputEquationNumber()
{
	CDomain* FEMData = CDomain::Instance();
	unsigned int NUMNP = FEMData->GetNUMNP();

	CNode* NodeList = FEMData->GetNodeList();

	*this << " EQUATION NUMBERS" << endl
		  << endl;
	*this << "   NODE NUMBER                     DEGREES OF FREEDOM" << endl;
	*this << "        N              X        Y        Z      sitax     sitay     sitaz" << endl;

	for (unsigned int np = 0; np < NUMNP; np++) // Loop over for all node
//		NodeList[np].WriteEquationNo(*this, np);

	*this << endl;
}

//	Output element data
void COutputter::OutputElementInfo()
{
	//	Print element group control line

	CDomain* FEMData = CDomain::Instance();

	unsigned int NUMEG = FEMData->GetNUMEG();

	*this << " E L E M E N T   G R O U P   D A T A" << endl
		  << endl
		  << endl;

	for (unsigned int EleGrp = 0; EleGrp < NUMEG; EleGrp++)
	{
		*this << " E L E M E N T   D E F I N I T I O N" << endl
			  << endl;

		ElementTypes ElementType = FEMData->GetEleGrpList()[EleGrp].GetElementType();
		unsigned int NUME = FEMData->GetEleGrpList()[EleGrp].GetNUME();

		*this << " ELEMENT TYPE  . . . . . . . . . . . . .( NPAR(1) ) . . =" << setw(5)
			  << ElementType << endl;
		*this << "     EQ.1, TRUSS ELEMENTS" << endl
			  << "     EQ.2, ELEMENTS CURRENTLY" << endl
			  << "     EQ.3, NOT AVAILABLE" << endl
			  << endl;

		*this << " NUMBER OF ELEMENTS. . . . . . . . . . .( NPAR(2) ) . . =" << setw(5) << NUME
			  << endl
			  << endl;

/*		switch (ElementType)
		{
			case ElementTypes::Bar: // Bar element
				PrintBarElementData(EleGrp);
				break;
			case ElementTypes::Q4: // 4Q element
				Print4QElementData(EleGrp);
				break;
			case ElementTypes::T3: // 4Q element
				PrintT3ElementData(EleGrp);
				break;
			case ElementTypes::H8: // 4Q element
				PrintH8ElementData(EleGrp);
				break;
			case ElementTypes::H8R: // 4Q element
				PrintH8ElementData(EleGrp);
				break;
			case ElementTypes::Plate: // Plate element
				PrintPlateElementData(EleGrp);
				break;
			case ElementTypes::Beam: //Beam element
				PrintBeamElementData(EleGrp);
				break;
			case ElementTypes::Shell: // Shell element
				PrintShellElementData(EleGrp);
			case ElementTypes::Infinite: //Infinite element
				PrintInfiniteElementData(EleGrp);
				break;
			case ElementTypes::Subpara: //Subpara element
				PrintSubparaElementData(EleGrp);
				break;
		}
*/	}
}
//	Output bar element data
void COutputter::PrintBarElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		  << endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		  << endl
		  << endl;

	*this << "  SET       YOUNG'S     CROSS-SECTIONAL" << endl
		  << " NUMBER     MODULUS          AREA" << endl
		  << "               E              A" << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		  << endl
		  << " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE       MATERIAL" << endl
		  << " NUMBER-N      I        J       SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

//Output 4Q element data
void COutputter::Print4QElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		<< endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND ELASTIC CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		<< endl
		<< endl;

	*this << "  SET       YOUNG'S    ELASTIC CONSTANTS" << endl
		<< " NUMBER     MODULUS        POISSON" << endl
		<< "               E              v" << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		<< endl
		<< " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE      NODE    MATERIAL" << endl
		<< " NUMBER-N      I        J        K        L     SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);
	*this << endl;
}

//	Output 3T element data
void COutputter::PrintT3ElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		  << endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		  << endl
		  << endl;

	*this << "  SET       YOUNG'S        POISSON " << endl
		  << " NUMBER     MODULUS         RATIO            Density         Thickness" << endl
		  << "               E              Nu               Rou               t " << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		  << endl
		  << " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE       MATERIAL" << endl
		  << " NUMBER-N      I       II      III       SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

//	Output 8H element data
void COutputter::PrintH8ElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		  << endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		  << endl
		  << endl;

	*this << "  SET       YOUNG'S        POISSON       RIGIDITY        LAME'S" << endl
		  << " NUMBER     MODULUS         RATIO         MODULUS      COEFFICIENT      Density" << endl
		  << "               E              Nu            G             Lam             Rou" << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		  << endl
		  << " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE     NODE     NODE     NODE     NODE     NODE       MATERIAL" << endl
		  << " NUMBER-N      I       II      III       IV        V       VI       VII      VIII     SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

void COutputter::PrintBeamElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();
	*this << " M A T E R I A L   D E F I N I T I O N" << endl << endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		<< endl
		<< endl;

	*this << "  SET       YOUNG'S   ELASTIC CONSTANTS      PLANE" << endl
		<< " NUMBER     MODULUS         POISSON          THICKNESS" << endl
		<< "               E            poisson               thick   " << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		<< endl
		<< " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE       MATERIAL" << endl
		<< " NUMBER-N      I        J       SET NUMBER" << endl;

	const unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

void COutputter::PrintShellElementData(unsigned int EleGrp){
	CDomain* FEMData=CDomain::Instance();
	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();
	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		  << endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		  << endl
		  << endl;

	*this << "  SET       YOUNG'S     POISSON'S      DENSITY          THICKNESS" << endl
		  << " NUMBER     MODULUS       RATIO" << endl
		  << "               E           nu            rho                t" << endl;
	*this << setiosflags(ios::scientific) << setprecision(5);
	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);
	*this << endl << endl << " E L E M E N T   I N F O R M A T I O N (INPUT IN CONTERCLOCKWISE ORDER)" << endl;
	*this << " ELEMENT     NODE     NODE     NODE     NODE       MATERIAL" << endl
		  << " NUMBER-N      I        J        K        L       SET NUMBER" << endl;

	const unsigned int NUME = ElementGroup.GetNUME();
	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

void COutputter::PrintPlateElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		<< endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		<< endl
		<< endl;

	*this << "  SET       YOUNG'S   ELASTIC CONSTANTS      PLANE" << endl
		<< " NUMBER     MODULUS         POISSON          THICKNESS" << endl
		<< "               E            poisson            thick   " << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		<< endl
		<< " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE     NODE       MATERIAL" << endl
		<< " NUMBER-N      I        J        K        L       SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

void COutputter::PrintInfiniteElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		<< endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		<< endl
		<< endl;

	*this << "  SET       YOUNG'S   ELASTIC CONSTANTS" << endl
		<< " NUMBER     MODULUS         POISSON      " << endl
		<< "               E            poisson      " << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		<< endl
		<< " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE     NODE       MATERIAL" << endl
		<< " NUMBER-N      I        J        K        L       SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}

void COutputter::PrintSubparaElementData(unsigned int EleGrp)
{
	CDomain* FEMData = CDomain::Instance();

	CElementGroup& ElementGroup = FEMData->GetEleGrpList()[EleGrp];
	unsigned int NUMMAT = ElementGroup.GetNUMMAT();

	*this << " M A T E R I A L   D E F I N I T I O N" << endl
		<< endl;
	*this << " NUMBER OF DIFFERENT SETS OF MATERIAL" << endl;
	*this << " AND CROSS-SECTIONAL  CONSTANTS  . . . .( NPAR(3) ) . . =" << setw(5) << NUMMAT
		<< endl
		<< endl;

	*this << "  SET       YOUNG'S        POSSION'S      " << endl
		<< " NUMBER     MODULUS          RATIO   " << endl
		<< "               E              nu     " << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	//	Loop over for all property sets
	for (unsigned int mset = 0; mset < NUMMAT; mset++)
		ElementGroup.GetMaterial(mset).Write(*this, mset);

	*this << endl
		<< endl
		<< " E L E M E N T   I N F O R M A T I O N" << endl;
	*this << " ELEMENT     NODE     NODE     NODE     NODE      NODE     NODE     NODE     NODE     NODE       MATERIAL" << endl
		<< " NUMBER-N      I        J        K        L        M        N        O        P        Q       SET NUMBER" << endl;

	unsigned int NUME = ElementGroup.GetNUME();

	//	Loop over for all elements in group EleGrp
	for (unsigned int Ele = 0; Ele < NUME; Ele++)
		ElementGroup[Ele].Write(*this, Ele);

	*this << endl;
}


//	Print load data
void COutputter::OutputLoadInfo()
{
	CDomain* FEMData = CDomain::Instance();

	for (unsigned int lcase = 1; lcase <= FEMData->GetNLCASE(); lcase++)
	{
		CLoadCaseData* LoadData = &FEMData->GetLoadCases()[lcase - 1];

		*this << setiosflags(ios::scientific);
		*this << " L O A D   C A S E   D A T A" << endl
			  << endl;

		*this << "     LOAD CASE NUMBER . . . . . . . =" << setw(6) << lcase << endl;
		*this << "     NUMBER OF CONCENTRATED LOADS . =" << setw(6) << LoadData->nloads << endl
			  << endl;
		*this << "    NODE       DIRECTION      LOAD" << endl
			  << "   NUMBER                   MAGNITUDE" << endl;

		LoadData->Write(*this, lcase);

		*this << endl;
	}
}

//	Print nodal displacement
void COutputter::OutputNodalDisplacement(unsigned int lcase)
{
	CDomain* FEMData = CDomain::Instance();
	CNode* NodeList = FEMData->GetNodeList();
	double* Displacement = FEMData->GetDisplacement();

	*this << " LOAD CASE" << setw(5) << lcase + 1 << endl
		  << endl
		  << endl;

	*this << setiosflags(ios::scientific);

	*this << " D I S P L A C E M E N T S" << endl
		  << endl;
	*this << "  NODE           X-DISPLACEMENT    Y-DISPLACEMENT    Z-DISPLACEMENT     X-ROTATION     Y-ROTATION     Z-ROTATION" << endl;

	for (unsigned int np = 0; np < FEMData->GetNUMNP(); np++)
//		NodeList[np].WriteNodalDisplacement(*this, np, Displacement);

	*this << endl;
}

//	Calculate stresses
void COutputter::OutputElementStress(unsigned int lcase)
{
	CDomain* FEMData = CDomain::Instance();

	double* Displacement = FEMData->GetDisplacement();

	unsigned int NUMEG = FEMData->GetNUMEG();

	COutPlot* Outplot = COutPlot::Instance();
	COutPlotPost* Outpost = COutPlotPost::Instance();

	unsigned int nele = FEMData->GetNUMELE();

	Outplot->StressHead(lcase, nele);
	Outpost->StressHead(lcase, nele);

	for (unsigned int EleGrpIndex = 0; EleGrpIndex < NUMEG; EleGrpIndex++)
	{
		*this << " S T R E S S  C A L C U L A T I O N S  F O R  E L E M E N T  G R O U P" << setw(5)
			  << EleGrpIndex + 1 << endl
			  << endl;

		CElementGroup& EleGrp = FEMData->GetEleGrpList()[EleGrpIndex];
		unsigned int NUME = EleGrp.GetNUME();
		ElementTypes ElementType = EleGrp.GetElementType();
		

		switch (ElementType)
		{
			case ElementTypes::Bar: // Bar element
				*this << "  ELEMENT             FORCE            STRESS" << endl
					<< "  NUMBER" << endl;

				double stress;

				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					Element.ElementStress(&stress, Displacement);

					CBarMaterial& material = *dynamic_cast<CBarMaterial*>(Element.GetElementMaterial());
					*this << setw(5) << Ele + 1 << setw(22) << stress * material.Area << setw(18)
						<< stress << endl;
					Outplot->ElementStress(abs(stress));
					Outpost->ElementStress(abs(stress));

				}

				*this << endl;
				break;

			case ElementTypes::Q4: // 4Q element
				*this << "  ELEMENT  GAUSS    X-COORD        Y-COORD        Z-COORD         SXX            SYY            TXY" << endl
					<< "  NUMBER   POINT" << endl;


				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					C4QMaterial* material = dynamic_cast<C4QMaterial *>(Element.GetElementMaterial());
					double *stress_4Q = new double[24];
					for (unsigned int m = 0; m < 24; m++)
						stress_4Q[m] = 0;

					Element.ElementStress(stress_4Q, Displacement);

					*this << setw(5) << Ele + 1 << setw(9) << "1"
						<< setw(15) << stress_4Q[0] << setw(15) << stress_4Q[1] << setw(15) << stress_4Q[2]
						<< setw(15) << stress_4Q[3] << setw(15) << stress_4Q[4] << setw(15) << stress_4Q[5] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "2"
						<< setw(15) << stress_4Q[6] << setw(15) << stress_4Q[7] << setw(15) << stress_4Q[8]
						<< setw(15) << stress_4Q[9] << setw(15) << stress_4Q[10] << setw(15) << stress_4Q[11] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "3"
						<< setw(15) << stress_4Q[12] << setw(15) << stress_4Q[13] << setw(15) << stress_4Q[14]
						<< setw(15) << stress_4Q[15] << setw(15) << stress_4Q[16] << setw(15) << stress_4Q[17] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "4"
						<< setw(15) << stress_4Q[18] << setw(15) << stress_4Q[19] << setw(15) << stress_4Q[20]
						<< setw(15) << stress_4Q[21] << setw(15) << stress_4Q[22] << setw(15) << stress_4Q[23] << endl;
					double MStressQ4=0;
					for (unsigned int iQ4 = 0; iQ4 < 4; iQ4++)
					{
						MStressQ4 = MStressQ4 + sqrt(stress_4Q[6 * iQ4 + 3] * stress_4Q[6 * iQ4 + 3] + stress_4Q[6 * iQ4 + 4] * stress_4Q[6 * iQ4 + 4] - stress_4Q[6 * iQ4 + 3] * stress_4Q[6 * iQ4 + 4] + 3 * stress_4Q[6 * iQ4 + 5] * stress_4Q[6 * iQ4 + 5]) / 4;
					}
					Outplot->ElementStress(MStressQ4);
					Outpost->ElementStress(MStressQ4);
					delete[] stress_4Q;
				}
				*this << endl;
				break;
			
			case ElementTypes::T3:
				*this << "  ELEMENT         STRESS_XX         STRESS_YY         STRESS_XY" << endl
					<< "  NUMBER" << endl;

				double T3Stress[3];

				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CT3& Element = dynamic_cast<CT3&>(EleGrp[Ele]);
					Element.ElementStress(T3Stress, Displacement);
					*this << setw(5) << Ele + 1 << setw(18) << T3Stress[0] << setw(18) << T3Stress[1] << setw(18) << T3Stress[2] << endl;
					double MStressT3;
					MStressT3 = sqrt( T3Stress[0]*T3Stress[0] + T3Stress[1]*T3Stress[1] - T3Stress[0]*T3Stress[1] +3*T3Stress[2]*T3Stress[2] );
					Outplot->ElementStress(MStressT3);
					Outpost->ElementStress(MStressT3);
				}

				*this << endl;

				break;
				
			case ElementTypes::H8:
				*this << "  ELEMENT         X_coord         Y_coord         Z_coord         STRESS_XX         STRESS_YY         STRESS_ZZ         STRESS_YZ         STRESS_ZX         STRESS_XY" << endl
					<< "  NUMBER" << endl;
				
				double H8Stress[48];
				double H8coord[24];

				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CH8& Element = dynamic_cast<CH8&>(EleGrp[Ele]);
					Element.ElementStress(H8Stress, Displacement);
					Element.ElementCoord(H8coord);
					double MStressH8=0;
					for (unsigned int cd = 0; cd < 8; cd++)
					{
//						*this << setw(5) << Ele + 1 << setw(18) << H8coord[3*cd] << setw(18) << H8coord[3*cd+1] << setw(18) << H8coord[3*cd+2]  << setw(18) << H8Stress[6*cd] << setw(18) << H8Stress[6*cd+1] << setw(18) << H8Stress[6*cd+2] << setw(18) << H8Stress[6*cd+3] << setw(18) << H8Stress[6*cd+4] << setw(18) << H8Stress[6*cd+5] << endl;
						MStressH8 = MStressH8 + sqrt( H8Stress[6*cd]*H8Stress[6*cd] + H8Stress[6*cd+1]*H8Stress[6*cd+1] + H8Stress[6*cd+2]*H8Stress[6*cd+2] - H8Stress[6*cd]*H8Stress[6*cd+1] - H8Stress[6*cd]*H8Stress[6*cd+2] - H8Stress[6*cd+1]*H8Stress[6*cd+2] + 3*H8Stress[6*cd+3]*H8Stress[6*cd+3] + 3*H8Stress[6*cd+4]*H8Stress[6*cd+4] + 3*H8Stress[6*cd+5]*H8Stress[6*cd+5] )/8;
					}
					Outplot->ElementStress(MStressH8);
					Outpost->ElementStress(MStressH8);
				}

				*this << endl;

				break;
			
			case ElementTypes::H8R:
				*this << "  ELEMENT         X_coord         Y_coord         Z_coord         STRESS_XX         STRESS_YY         STRESS_ZZ         STRESS_YZ         STRESS_ZX         STRESS_XY" << endl
					<< "  NUMBER" << endl;

				double H8RStress[6];
				double H8Rcoord[3];

				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CH8R& Element = dynamic_cast<CH8R&>(EleGrp[Ele]);
					Element.ElementStress(H8RStress, Displacement);
					Element.ElementCoord(H8Rcoord);
					double MStressH8R;
						*this << setw(5) << Ele + 1 << setw(18) << H8Rcoord[0] << setw(18) << H8Rcoord[1] << setw(18) << H8Rcoord[2]  << setw(18) << H8RStress[0] << setw(18) << H8RStress[1] << setw(18) << H8RStress[2] << setw(18) << H8RStress[3] << setw(18) << H8RStress[4] << setw(18) << H8RStress[5] << endl;
						MStressH8R = sqrt( H8RStress[0]*H8RStress[0] + H8RStress[1]*H8RStress[1] + H8RStress[2]*H8RStress[2] - H8RStress[0]*H8RStress[1] - H8RStress[0]*H8RStress[2] - H8RStress[1]*H8RStress[2] + 3*H8RStress[3]*H8RStress[3] + 3*H8RStress[4]*H8RStress[4] + 3*H8RStress[5]*H8RStress[5] );
					Outplot->ElementStress(MStressH8R);
					Outpost->ElementStress(MStressH8R);
				}

				*this << endl;

				break;
				
			case ElementTypes::Plate: // Plate element
				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					CPlateMaterial* material =
						dynamic_cast<CPlateMaterial *>(Element.GetElementMaterial());

					double* stress_plate = new double[20];
					Element.ElementStress(stress_plate, Displacement);

					*this << " ELEMENT NUMBER :" << setw(5) << Ele + 1 << endl;
					*this << " GAUSS POINT   DEGREES    OF    FREEDOM     BENDING MOMENT   BENDING MOMENT    BENDING MOMENT" << endl
						<< "   NUMBER         X                Y                X                Y                 XY " << endl;
					*this << setw(5) << 1 << setw(18) << stress_plate[24] << setw(18) << stress_plate[25] << setw(18) << stress_plate[0]
						<< setw(18) << stress_plate[1] << setw(18) << stress_plate[2] << endl;
					*this << setw(5) << 2 << setw(18) << stress_plate[26] << setw(18) << stress_plate[27] << setw(18) << stress_plate[3]
						<< setw(18) << stress_plate[4] << setw(18) << stress_plate[5] << endl;
					*this << setw(5) << 3 << setw(18) << stress_plate[28] << setw(18) << stress_plate[29] << setw(18) << stress_plate[6]
						<< setw(18) << stress_plate[7] << setw(18) << stress_plate[8] << endl;
					*this << setw(5) << 4 << setw(18) << stress_plate[30] << setw(18) << stress_plate[31] << setw(18) << stress_plate[9]
						<< setw(18) << stress_plate[10] << setw(18) << stress_plate[11] << endl;
					
					double MStressP = 0;
					for (unsigned int iP = 0; iP < 4; iP++)
					{
						MStressP = MStressP + sqrt(stress_plate[12+3 * iP] * stress_plate[12+3 * iP] + stress_plate[12+3 * iP + 1] * stress_plate[12+3 * iP + 1] - stress_plate[12+3 * iP] * stress_plate[12+3 * iP + 1] + 3 * stress_plate[12+3 * iP + 2] * stress_plate[12+3 * iP + 2]) / 4;
					}
					Outplot->ElementStress(MStressP);
					Outpost->ElementStress(MStressP);
                    delete[] stress_plate;
				}
				break;

			case ElementTypes::Beam:
				*this << "  ELEMENT          SXX                 SYY                   SZZ" << endl
					<< "  NUMBER" << endl;


				
				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					double* beamstress = new double[3];
					Element.ElementStress(beamstress, Displacement);
					CBeamMaterial* material = dynamic_cast<CBeamMaterial*>(Element.GetElementMaterial());
//					*this << setw(5) << Ele + 1 << setw(22) << beamstress[0] << setw(22)
//						<< beamstress[1] << setw(22) << beamstress[2] << endl;
					double Beam_mises(0);
					Beam_mises = sqrt(beamstress[0] * beamstress[0] + beamstress[1] * beamstress[1] + beamstress[2] * beamstress[2]);
					Outplot->ElementStress(Beam_mises);
					Outpost->ElementStress(Beam_mises);
					delete[] beamstress;

				}

				*this << endl;
				break;
			case ElementTypes::Shell:
				*this<<"ELEMENT      X-COORD       Y-COORD      Z-COORD       SXX            SYY          SZZ          TXY          TYZ         TZX"<<endl;
				double shellstress[48];
				double gaussposition[24];
				for (unsigned int Ele = 0; Ele < NUME; Ele++){
					CShell& Element = dynamic_cast<CShell&>(EleGrp[Ele]);
					Element.ElementStress(shellstress, Displacement);
					Element.ElementCoord(gaussposition);
/*					for(unsigned int loop=0;loop<8;loop++){
						*this << setw(5) << Ele + 1 << setw(20) <<gaussposition[3*loop]<<setw(20)<<gaussposition[3*loop+1]
							<<setw(20)<<gaussposition[3*loop+2]<< setw(20)<<shellstress[6*loop] << setw(20)
							<< shellstress[6*loop+1] << setw(20) << shellstress[6*loop+2] << setw(20)<<shellstress[6*loop+3]
							<<setw(20)<<shellstress[6*loop+4]<<setw(20)<<shellstress[6*loop+5]<<endl;
					}
*/					double MStressShell = 0;
					for (unsigned int iSh = 0; iSh < 8; iSh++)
					{    
						double SXY2 = (shellstress[6 * iSh]- shellstress[6 * iSh + 1]) * (shellstress[6 * iSh] - shellstress[6 * iSh + 1]);
						double SXZ2 = (shellstress[6 * iSh] - shellstress[6 * iSh + 2]) * (shellstress[6 * iSh] - shellstress[6 * iSh + 2]);
						double SYZ2 = (shellstress[6 * iSh + 1] - shellstress[6 * iSh + 2]) * (shellstress[6 * iSh + 1] - shellstress[6 * iSh + 2]);
						double T = 6 * (shellstress[6 * iSh + 3] * shellstress[6 * iSh + 3] + shellstress[6 * iSh + 4] * shellstress[6 * iSh + 4] + shellstress[6 * iSh + 5] * shellstress[6 * iSh + 5]);
						MStressShell = MStressShell + sqrt((SXY2 + SXZ2 + SYZ2 + T)/2) / 8;
					}
					Outplot->ElementStress(MStressShell);
					Outpost->ElementStress(MStressShell);
				}
				break;
			case ElementTypes::Infinite:
				*this << "  ELEMENT  GAUSS    X-COORD        Y-COORD        Z-COORD         SXX            SYY            TXY" << endl
					<< "  NUMBER   POINT" << endl;


				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					CInfiMaterial* material =
						dynamic_cast<CInfiMaterial *>(Element.GetElementMaterial());
					double *stress_Infi = new double[24];
					for (unsigned int m = 0; m < 24; m++)
						stress_Infi[m] = 0;

					Element.ElementStress(stress_Infi, Displacement);

					*this << setw(5) << Ele + 1 << setw(9) << "1"
						<< setw(15) << stress_Infi[0] << setw(15) << stress_Infi[1] << setw(15) << stress_Infi[2]
						<< setw(15) << stress_Infi[3] << setw(15) << stress_Infi[4] << setw(15) << stress_Infi[5] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "2"
						<< setw(15) << stress_Infi[6] << setw(15) << stress_Infi[7] << setw(15) << stress_Infi[8]
						<< setw(15) << stress_Infi[9] << setw(15) << stress_Infi[10] << setw(15) << stress_Infi[11] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "3"
						<< setw(15) << stress_Infi[12] << setw(15) << stress_Infi[13] << setw(15) << stress_Infi[14]
						<< setw(15) << stress_Infi[15] << setw(15) << stress_Infi[16] << setw(15) << stress_Infi[17] << endl;
					*this << setw(5) << Ele + 1 << setw(9) << "4"
						<< setw(15) << stress_Infi[18] << setw(15) << stress_Infi[19] << setw(15) << stress_Infi[20]
						<< setw(15) << stress_Infi[21] << setw(15) << stress_Infi[22] << setw(15) << stress_Infi[23] << endl;
					double MStressInfi = 0;
					for (unsigned int iIn = 0; iIn < 4; iIn++)
					{
						MStressInfi = MStressInfi + sqrt(stress_Infi[6 * iIn + 3] * stress_Infi[6 * iIn + 3] + stress_Infi[6 * iIn + 4] * stress_Infi[6 * iIn + 4] - stress_Infi[6 * iIn + 3] * stress_Infi[6 * iIn + 4] + 3 * stress_Infi[6 * iIn + 5] * stress_Infi[6 * iIn + 5]) / 4;

					}
					Outplot->ElementStress(MStressInfi);
					Outpost->ElementStress(MStressInfi);
					delete[] stress_Infi;
				}
				*this << endl;
				break;

			case ElementTypes::Subpara: // Subparameter element
			{
				*this << "  ELEMENT       X-COORD        Y-COORD         SXX            SYY            TXY" << endl
					<< "NUMBER" << endl;
				double* stress_Subpara = new double[45];
				for (unsigned int Ele = 0; Ele < NUME; Ele++)
				{
					CElement& Element = EleGrp[Ele];
					Element.ElementStress(stress_Subpara, Displacement);
					for (unsigned int ii = 0; ii < 9; ii++)
					{
						unsigned int temp1 = 27 + 2 * ii;  unsigned int temp2 = 3 * ii;
						*this << setw(5) << Ele + 1 << setw(22) << stress_Subpara[temp1] << setw(18) << stress_Subpara[temp1 + 1] << setw(18) << stress_Subpara[temp2] << setw(18) << stress_Subpara[temp2 + 1] << setw(18) << stress_Subpara[temp2 + 2] << endl;
					}
					double MStressSub = 0;
					for (unsigned int iSu = 0; iSu < 4; iSu++)
					{
						unsigned int temp1 = 27 + 2 * iSu;  unsigned int temp2 = 3 * iSu;
						MStressSub = MStressSub + sqrt(stress_Subpara[temp2] * stress_Subpara[temp2] + stress_Subpara[temp2 + 1] * stress_Subpara[temp2 + 1] - stress_Subpara[temp2] * stress_Subpara[temp2+1] + 3 * stress_Subpara[temp2 + 2] * stress_Subpara[temp2 + 2]) / 4;

					}
					Outplot->ElementStress(MStressSub);
					Outpost->ElementStress(MStressSub);
					delete[] stress_Subpara;
				}
				
			}
			default: // Invalid element type
				cerr << "*** Error *** Elment type " << ElementType
					<< " has not been implemented.\n\n";
		}
	}
}


//	Print total system data
void COutputter::OutputTotalSystemData()
{
	CDomain* FEMData = CDomain::Instance();

	*this << "	TOTAL SYSTEM DATA" << endl
		  << endl;

	*this << "     NUMBER OF EQUATIONS . . . . . . . . . . . . . .(NEQ) = " << FEMData->GetNEQ()
		  << endl
		  << "     NUMBER OF MATRIX ELEMENTS . . . . . . . . . . .(NWK) = " << FEMData->GetStiffnessMatrix()->size()
		  << endl
		  << "     MAXIMUM HALF BANDWIDTH  . . . . . . . . . . . .(MK ) = " << FEMData->GetStiffnessMatrix()->GetMaximumHalfBandwidth()
		  << endl
		  << "     MEAN HALF BANDWIDTH . . . . . . . . . . . . . .(MM ) = " << FEMData->GetStiffnessMatrix()->size() / FEMData->GetNEQ() << endl
		  << endl
		  << endl;
}

#ifdef _DEBUG_

//	Print column heights for debuging
void COutputter::PrintColumnHeights()
{
	*this << "*** _Debug_ *** Column Heights" << endl;

	CDomain* FEMData = CDomain::Instance();

	unsigned int NEQ = FEMData->GetNEQ();
	CSkylineMatrix<double> *StiffnessMatrix = FEMData->GetStiffnessMatrix();
	unsigned int* ColumnHeights = StiffnessMatrix->GetColumnHeights();

	for (unsigned int col = 0; col < NEQ; col++)
	{
		if (col + 1 % 10 == 0)
		{
			*this << endl;
		}

		*this << setw(8) << ColumnHeights[col];
	}

	*this << endl
		  << endl;
}

//	Print address of diagonal elements for debuging
void COutputter::PrintDiagonalAddress()
{
	*this << "*** _Debug_ *** Address of Diagonal Element" << endl;

	CDomain* FEMData = CDomain::Instance();

	unsigned int NEQ = FEMData->GetNEQ();
	CSkylineMatrix<double> *StiffnessMatrix = FEMData->GetStiffnessMatrix();
	unsigned int* DiagonalAddress = StiffnessMatrix->GetDiagonalAddress();

	for (unsigned int col = 0; col <= NEQ; col++)
	{
		if (col + 1 % 10 == 0)
		{
			*this << endl;
		}

		*this << setw(8) << DiagonalAddress[col];
	}

	*this << endl
		  << endl;
}

//	Print banded and full stiffness matrix for debuging
void COutputter::PrintStiffnessMatrix()
{
	*this << "*** _Debug_ *** Banded stiffness matrix" << endl;

	CDomain* FEMData = CDomain::Instance();

	unsigned int NEQ = FEMData->GetNEQ();
	CSkylineMatrix<double> *StiffnessMatrix = FEMData->GetStiffnessMatrix();
	unsigned int* DiagonalAddress = StiffnessMatrix->GetDiagonalAddress();

	*this << setiosflags(ios::scientific) << setprecision(5);

	for (unsigned int i = 0; i < DiagonalAddress[NEQ] - DiagonalAddress[0]; i++)
	{
		*this << setw(14) << (*StiffnessMatrix)(i);

		if ((i + 1) % 6 == 0)
		{
			*this << endl;
		}
	}

	*this << endl
		  << endl;

	*this << "*** _Debug_ *** Full stiffness matrix" << endl;

	for (int I = 1; I <= NEQ; I++)
	{
		for (int J = 1; J <= NEQ; J++)
		{
			int H = DiagonalAddress[J] - DiagonalAddress[J - 1];
			if (J - I - H >= 0)
			{
				*this << setw(14) << 0.0;
			}
			else
			{
				*this << setw(14) << (*StiffnessMatrix)(I, J);
			}
		}

		*this << endl;
	}

	*this << endl;
}

//	Print displacement vector for debuging
void COutputter::PrintDisplacement(unsigned int loadcase)
{
	*this << "*** _Debug_ *** Displacement vector" << endl;

	CDomain* FEMData = CDomain::Instance();

	unsigned int NEQ = FEMData->GetNEQ();
	double* Force = FEMData->GetForce();

	*this << "  Load case = " << loadcase << endl;

	*this << setiosflags(ios::scientific) << setprecision(5);

	for (unsigned int i = 0; i < NEQ; i++)
	{
		if ((i + 1) % 6 == 0)
		{
			*this << endl;
		}

		*this << setw(14) << Force[i];
	}

	*this << endl
		  << endl;
}

#endif
