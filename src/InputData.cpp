/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	еееееееееееееееееееееееееееееееееееееееее
	InputData.cpp

	Class member functions related to InputData
	Brett Bode - February 1996
	Changed InputeFileData uses to BufferFile calls 8-97
*/
#define		kPGroupStrings		305
#define		kMaxPGroups			15
#define		kDFTGridFunctionalStrings		311
#define		kDFTGridFunctionalMaxStrings	21
#define		kDFTGridFreeFunctionalStrings		312
#define		kDFTGridFreeFunctionalMaxStrings	21

#include "Globals.h"
#include "InputData.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "myFiles.h"
#include "GlobalExceptions.h"
#include "BasisSet.h"
#include "Internals.h"
#include "Prefs.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "XML.hpp"
#include "CML.h"
#include <cctype>
#include <sstream>

	//InputData functions
InputData::InputData(void) {
	//Always create Control, System, Basis, and Data groups
	Control = new ControlGroup;
	System = new SystemGroup;
	Basis = new BasisGroup;
	Data = new DataGroup;
	StatPt = new StatPtGroup;
	Guess = new GuessGroup;
	SCF = new SCFGroup;
	MP2 = NULL;
	Hessian = NULL;
}
InputData::InputData(InputData *Copy) {
	//Always create Control, System, Basis, and Data groups
	Control = new ControlGroup(Copy->Control);	//Create the new group and copy over the data
	System = new SystemGroup(Copy->System);
	Basis = new BasisGroup(Copy->Basis);
	Data = new DataGroup(Copy->Data);
	StatPt = new StatPtGroup(Copy->StatPt);
	if (Copy->Guess) Guess = new GuessGroup(Copy->Guess);
	else Guess = new GuessGroup;
	if (Copy->SCF) SCF = new SCFGroup(Copy->SCF);
	else SCF = new SCFGroup;
	if (Copy->MP2) MP2 = new MP2Group(Copy->MP2);
	else MP2 = NULL;
	if (Copy->Hessian) Hessian = new HessianGroup(Copy->Hessian);
	else Hessian = NULL;
	DFT = Copy->DFT;
	EFP = Copy->EFP;
	FMO = Copy->FMO;
}
InputData::~InputData(void) {	//destructor
	if (Control) delete Control;	//simply delete all groups present
	if (System) delete System;
	if (Basis) delete Basis;
	if (Data) delete Data;
	if (Guess) delete Guess;
	if (SCF) delete SCF;
	if (MP2) delete MP2;
	if (Hessian) delete Hessian;
	if (StatPt) delete StatPt;
}
void InputData::WriteXML(XMLElement * parent, MoleculeData * p) const {
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_InputOptionsElement));
	
	if (Control) {
		Control->WriteXML(Ele);
	}
	if (System) {
		System->WriteXML(Ele);
	}
	if (Basis) {
		Basis->WriteXML(Ele);
	}
	if (Data) {
		Data->WriteXML(Ele);
	}
	if (Guess) {
		Guess->WriteXML(Ele);
	}
	if (SCF) {
		SCF->WriteXML(Ele);
	}
	if (MP2) {
		MP2->WriteXML(Ele);
	}
	if (Hessian) {
		Hessian->WriteXML(Ele);
	}
	if (StatPt) {
		StatPt->WriteXML(Ele);
	}
	DFT.WriteXML(Ele);
	EFP.WriteXML(Ele);
	FMO.WriteXML(Ele, p);
}
void InputData::ReadXML(XMLElement * parent, MoleculeData * p) {
	XMLElementList * ipxml = parent->getChildren();
	if (ipxml) {
		if (ipxml->length() > 0) {
			XMLElement * ipx = ipxml->item(0);
			if (ipx) {
				if (!strcmp(ipx->getName(),CML_convert(MMP_InputOptionsElement))) {
					XMLElementList * children = ipx->getChildren();
					if (children) {
						for (int i=0; i<children->length(); i++) {
							XMLElement * child = children->item(i);
							MMP_InputOptionsNS IOchild;
							if (CML_convert(child->getName(), IOchild)) {
								switch (IOchild) {
									case MMP_IOControlGroupElement:
										if (Control == NULL) Control = new ControlGroup;
										if (Control) Control->ReadXML(child);
										break;
									case MMP_IOSystemGroupElement:
										if (System == NULL) System = new SystemGroup;
										if (System) System->ReadXML(child);
										break;
									case MMP_IOBasisGroupElement:
										if (Basis == NULL) Basis = new BasisGroup;
										if (Basis) Basis->ReadXML(child);
										break;
									case MMP_IODataGroupElement:
										if (Data == NULL) Data = new DataGroup;
										if (Data) Data->ReadXML(child);
										break;
									case MMP_IOGuessGroupElement:
										if (Guess == NULL) Guess = new GuessGroup;
										if (Guess) Guess->ReadXML(child);
										break;
									case MMP_IOSCFGroupElement:
										if (SCF == NULL) SCF = new SCFGroup;
										if (SCF) SCF->ReadXML(child);
										break;
									case MMP_IOMP2GroupElement:
										if (MP2 == NULL) MP2 = new MP2Group;
										if (MP2) MP2->ReadXML(child);
										break;
									case MMP_IOHessianGroupElement:
										if (Hessian == NULL) Hessian = new HessianGroup;
										if (Hessian) Hessian->ReadXML(child);
										break;
									case MMP_IOStatPtGroupElement:
										if (StatPt == NULL) StatPt = new StatPtGroup;
										if (StatPt) StatPt->ReadXML(child);
										break;
									case MMP_IODFTGroupElement:
										DFT.ReadXML(child);
										break;
									case MMP_IOEFPGroupElement:
										EFP.ReadXML(child);
										break;
									case MMP_IOFMOGroupElement:
										FMO.ReadXML(child, p);
										break;
								}
							}
						}
						delete children;
					}
				}
			}
		}
		delete ipxml;
	}
}

	//ControlGroup functions
ControlGroup::ControlGroup(void) {
	ExeType = NULL;
	Options=0;
	InitControlPaneData();
	InitProgPaneData();
	NPrint=ITol=ICut=0;
}
ControlGroup::ControlGroup(ControlGroup *Copy) {
	if (Copy == NULL) return;
	*this = *Copy;
	ExeType = NULL;
	if (Copy->ExeType) {
		ExeType = new char[1+strlen(Copy->ExeType)];
		if (ExeType) strcpy(ExeType, Copy->ExeType);
	}
}
ControlGroup::~ControlGroup(void) {
	if (ExeType) delete [] ExeType;
}
void ControlGroup::InitControlPaneData(void) {
	if (ExeType) {
		delete [] ExeType;
		ExeType = NULL;
	}
	SCFType=GAMESSDefaultSCFType;
	MPLevelCIType=MaxIt=Charge=Multiplicity=0;
	Local=GAMESS_No_Localization;
	RunType=InvalidRunType;
	CCType = CC_None;
}
void ControlGroup::InitProgPaneData(void) {
	Friend=Friend_None;
	SetMolPlot(false);
	SetPlotOrb(false);
	SetAIMPAC(false);
	SetRPAC(false);
}
GAMESS_SCFType ControlGroup::SetSCFType(GAMESS_SCFType NewSCFType) {
	if ((NewSCFType >= GAMESSDefaultSCFType)||(NewSCFType<NumGAMESSSCFTypes))
		SCFType = NewSCFType;
	return SCFType;
}
const char * ControlGroup::GAMESSSCFTypeToText(GAMESS_SCFType t) {
	switch (t) {
		case GAMESS_RHF:
			return "RHF";
		case GAMESS_UHF:
			return "UHF";
		case GAMESS_ROHF:
			return "ROHF";
		case GAMESS_GVB:
			return "GVB";
		case GAMESS_MCSCF:
			return "MCSCF";
		case GAMESS_NO_SCF:
			return "NONE";
	}
	return "invalid";
}
GAMESS_SCFType ControlGroup::SetSCFType(const char *SCFText) {
	GAMESS_SCFType temp = GAMESS_Invalid_SCFType;
	for (int i=1; i<=NumGAMESSSCFTypes; i++) {
		if (!strcasecmp(SCFText, GAMESSSCFTypeToText((GAMESS_SCFType) i))) {
			temp = (GAMESS_SCFType) i;
			break;
		}
	}
	if (temp != GAMESS_Invalid_SCFType) SCFType = temp;
	return temp;
}
long ControlGroup::SetMPLevel(short NewMPLevel) {
	if ((NewMPLevel!=0)&&(NewMPLevel!=2)) return -1;

	MPLevelCIType = (MPLevelCIType & 0xFFF0) + NewMPLevel;
	return (MPLevelCIType & 0x0F);
}
short ControlGroup::GetMPLevel(void) const {	//return the appropriate MP value based on SCF and Run types
	short result=-1;
		//MP2 energy and gradients are available for RHF, UHF and ROHF
	if ((SCFType >= GAMESSDefaultSCFType)&&(SCFType <= GAMESS_ROHF)) result = (MPLevelCIType & 0x0F);
		//MrMP2 energies are also available
	else if ((SCFType==GAMESS_MCSCF)&&
		(RunType!=GradientRun)&&(RunType!=HessianRun)&&(RunType!=OptimizeRun)&&
		(RunType!=SadPointRun)&&(RunType!=IRCRun)&&(RunType!=GradExtrRun)&&(RunType!=DRCRun)) result=(MPLevelCIType & 0x0F);
	if (MPLevelCIType & 0xF0) result = -1;	//deactivate MP2 when CI is requested
	if (GetCCType() != CC_None) result = -1;
	if (GetRunType() == G3MP2) result = -1;
	return result;
}
CIRunType ControlGroup::SetCIType(CIRunType NewVal) {
	MPLevelCIType = (MPLevelCIType&0x0F) + (NewVal<<4);
	return (CIRunType)(MPLevelCIType & 0xF0);
}
CIRunType ControlGroup::GetCIType(void) const {
	short result = ((MPLevelCIType & 0xF0)>>4);
	if (GetSCFType() == 2) result = 0;
	return (CIRunType) result;
};
TypeOfRun ControlGroup::SetRunType(const TypeOfRun & NewRunType) {
	if ((NewRunType<=0)||(NewRunType>NumGAMESSRunTypes)) return InvalidRunType;
	
	RunType = NewRunType;
	return RunType;
}
TypeOfRun ControlGroup::SetRunType(const char *RunText) {
	TypeOfRun NewType = InvalidRunType;

	for (int i=1; i<NumGAMESSRunTypes; i++) {
		const char * test = GetGAMESSRunText((TypeOfRun)i);
		if (-1<LocateKeyWord(RunText, test, strlen(test), 9)) {
			NewType = (TypeOfRun)i;
			if (NewType == TDHFRun) {
				test = GetGAMESSRunText(TDHFXRun);
				if (-1<LocateKeyWord(RunText, test, strlen(test), 9)) {
					NewType = TDHFXRun;
				}
			}
			break;
		}
	}

	if (NewType<=0) return InvalidRunType;
	
	RunType = NewType;
	return RunType;
}
const char * ControlGroup::GetGAMESSRunText(const TypeOfRun & r) {
	switch (r) {
		case Energy:
			return "ENERGY";
		case GradientRun:
			return "GRADIENT";
		case HessianRun:
			return "HESSIAN";
		case OptimizeRun:
			return "OPTIMIZE";
		case TrudgeRun:
			return "TRUDGE";
		case SadPointRun:
			return "SADPOINT";
		case MinEnergyCrossing:
			return "MEX";
		case IRCRun:
			return "IRC";
		case MolDynamics:
			return "MD";
		case GradExtrRun:
			return "GRADEXTR";
		case DRCRun:
			return "DRC";
		case SurfaceRun:
			return "SURFACE";
		case G3MP2:
			return "G3MP2";
		case PropRun:
			return "PROP";
		case MorokumaRun:
			return "MOROKUMA";
		case TransitnRun:
			return "TRANSITN";
		case SpinOrbitRun:
			return "SPINORBT";
		case FFieldRun:
			return "FFIELD";
		case TDHFRun:
			return "TDHF";
		case TDHFXRun:
			return "TDHFX";
		case GLOBOPRun:
			return "GLOBOP";
		case VSCFRun:
			return "VSCF";
		case OptFMORun:
			return "OPTFMO";
		case RamanRun:
			return "RAMAN";
		case NMRRun:
			return "NMR";
		case MakeEFPRun:
			return "MAKEFP";
		case FreeStateFMORun:
			return "FMO0";
		default:
			return "unknown";
	}
}
const char * ControlGroup::GetGAMESSCCType(const CCRunType & r) {
	switch (r) {
		case CC_None:
			return "NONE";
		case CC_LCCD:
			return "LCCD";
		case CC_CCD:
			return "CCD";
		case CC_CCSD:
			return "CCSD";
		case CC_CCSDT:
			return "CCSD(T)";
		case CC_RCC:
			return "R-CC";
		case CC_CRCC:
			return "CR-CC";
		case CC_CRCCL:
			return "CR-CCL";
		case CC_CCSDTQ:
			return "CCSD(TQ)";
		case CC_CRCCQ:
			return "CR-CC(Q)";
		case CC_EOMCCSD:
			return "EOM-CCSD";
		case CC_CREOM:
			return "CR-EOM";
		case CC_CREOML:
			return "CR-EOML";
		case CC_IP_EOM2:
			return "IP-EOM2";
		case CC_IP_EOM3A:
			return "IP-EOM3A";
		case CC_EA_EOM2:
			return "EA-EOM2";
		case CC_EA_EOM3A:
			return "EA-EOM3A";
		default:
			return "unknown";
	}
}
CCRunType ControlGroup::SetCCType(CCRunType n) {
	CCType = n;
	return CCType;
}
CCRunType ControlGroup::SetCCType(const char * n) {
	CCRunType NewType = CC_None;
	
	for (int i=1; i<NumCCTypes; i++) {
		const char * test = GetGAMESSCCType((CCRunType)i);
		if (-1<LocateKeyWord(n, test, strlen(test), 8)) {
			NewType = (CCRunType)i;
			break;
		}
	}
	
	if (NewType<=0) return CC_None;
	
	CCType = NewType;
	return CCType;
}
CCRunType ControlGroup::GetCCType(void) const {
	CCRunType result = CCType;
	
	if (GetSCFType() > 1) result = CC_None;
	if (GetCIType() > 0) result = CC_None;
	return result;
}
short ControlGroup::GetExeType(void) {
	if ((ExeType==NULL)||(0<=LocateKeyWord(ExeType, "RUN", 3,3))) return 0;	//Normal run
	if (0<=LocateKeyWord(ExeType, "CHECK", 5,5)) return 1;
	if (0<=LocateKeyWord(ExeType, "DEBUG", 5,5)) return 2;
	return 3;
}
short ControlGroup::SetExeType(const char *ExeText) {
	if (ExeText==NULL) return 0;
	long nchar = strlen(ExeText);
	if (ExeType) {
		delete [] ExeType;
		ExeType = NULL;
	}
	ExeType = new char[nchar+1];
	strcpy(ExeType, ExeText);
	return nchar;
}
short ControlGroup::SetExeType(short NewType) {
	if ((NewType < 0)||(NewType > 2)) return -1;
	if (ExeType) {
		delete [] ExeType;
		ExeType = NULL;
	}
	if (NewType==1) {
		ExeType = new char[6];
		strcpy(ExeType, "CHECK");
	} else if (NewType == 2) {
		ExeType = new char[6];
		strcpy(ExeType, "DEBUG");
	}
	return NewType;
}
CIRunType ControlGroup::SetCIType(const char * CIText) {
	CIRunType newType = CI_None;
	if (-1<FindKeyWord(CIText, "GUGA", 4)) newType = CI_GUGA;
	else if (-1<FindKeyWord(CIText, "ALDET", 5)) newType = CI_ALDET;
	else if (-1<FindKeyWord(CIText, "ORMAS", 5)) newType = CI_ORMAS;
	else if (-1<FindKeyWord(CIText, "CIS", 3)) newType = CI_CIS;
	else if (-1<FindKeyWord(CIText, "FSOCI", 5)) newType = CI_FSOCI;
	else if (-1<FindKeyWord(CIText, "GENCI", 5)) newType = CI_GENCI;
	return SetCIType(newType);
}
const char * ControlGroup::GetCIType(const CIRunType & citype) const {
	switch (citype) {
		default:
			return "NONE";
		case CI_GUGA:
			return "GUGA";
		case CI_ALDET:
			return "ALDET";
		case CI_ORMAS:
			return "ORMAS";
		case CI_CIS:
			return "CIS";
		case CI_FSOCI:
			return "FSOCI";
		case CI_GENCI:
			return "GENCI";
	}
}
CIRunType ControlGroup::GetCIType(char * outText) const {
	CIRunType temp = GetCIType();
	if (outText != NULL) {
		strcpy(outText, GetCIType(temp));
	}
	return temp;
}
short ControlGroup::SetMaxIt(short NewVal) {
	if (NewVal>=0) MaxIt = NewVal;
	return MaxIt;
}
GAMESS_Localization ControlGroup::SetLocal(GAMESS_Localization NewVal) {
	if ((NewVal>=GAMESS_No_Localization)&&(NewVal<NumGAMESSLocalizations)) Local = NewVal;
	return Local;
}
GAMESS_Localization ControlGroup::SetLocal(const char * t) {
	GAMESS_Localization temp = Invalid_Localization;
	for (int i=0; i<NumGAMESSLocalizations; i++) {
		if (!strcasecmp(t, GAMESSLocalizationToText((GAMESS_Localization) i))) {
			temp = (GAMESS_Localization) i;
			break;
		}
	}
	if (temp != Invalid_Localization) Local = temp;
	return temp;
}
const char * ControlGroup::GAMESSLocalizationToText(GAMESS_Localization t) {
	switch (t) {
		case GAMESS_No_Localization:
			return "NONE";
		case GAMESS_BOYS_Localization:
			return "BOYS";
		case GAMESS_RUEDNBRG_Localization:
			return "RUEDNBRG";
		case GAMESS_POP_Localization:
			return "POP";
		case GAMESS_SVD_Localization:
			return "SVD";
	}
	return "invalid";
}
const char * ControlGroup::GetFriendText(FriendType f) {
	switch (f) {
		case Friend_HONDO:
			return "HONDO";
		case Friend_MELDF:
			return "MELDF";
		case Friend_GAMESSUK:
			return "GAMESSUK";
		case Friend_GAUSSIAN:
			return "GAUSSIAN";
		case Friend_ALL:
			return "ALL";
	}
	return "invalid";	//Getting to here indicates a bad value
}
FriendType ControlGroup::TextToFriend(const char * c) {
	FriendType result = Friend_None;
	for (int i=0; i<NumFriendTypes; i++) {
		if (!strcasecmp(c, GetFriendText((FriendType) i))) {
			result = (FriendType) i;
			break;
		}
	}
	return result;
}
FriendType ControlGroup::SetFriend(FriendType NewValue) {
	if ((NewValue >= Friend_None)&&(NewValue < NumFriendTypes)) Friend = NewValue;
	return (FriendType)Friend;
}
FriendType ControlGroup::SetFriend(const char * c) {
	return SetFriend(TextToFriend(c));
}
short ControlGroup::SetCharge(short NewCharge) {
	Charge = NewCharge;
	return Charge;
}
short ControlGroup::SetMultiplicity(short NewMult) {
	Multiplicity = NewMult;
	return Multiplicity;
}
bool ControlGroup::SetMolPlot(bool State) {
	if (Options & 1) Options -= 1;
	if (State) Options += 1;
	return ((Options & 1)?true:false);
}
bool ControlGroup::SetPlotOrb(bool State) {
	if (Options & (1<<1)) Options -= (1<<1);
	if (State) Options += (1<<1);
	return ((Options & (1<<1))?true:false);
}
bool ControlGroup::SetAIMPAC(bool State) {
	if (Options & (1<<2)) Options -= (1<<2);
	if (State) Options += (1<<2);
	return ((Options & (1<<2))?true:false);
}
bool ControlGroup::SetRPAC(bool State) {
	if (Options & (1<<3)) Options -= (1<<3);
	if (State) Options += (1<<3);
	return ((Options & (1<<3))?true:false);
}
bool ControlGroup::UseSphericalHarmonics(bool State) {
	if (Options & (1<<5)) Options -= (1<<5);
	if (State) Options += (1<<5);
	return ((Options & (1<<5))?true:false);
}
bool ControlGroup::SetNormF(bool State) {
	if (Options & (1<<6)) Options -= (1<<6);
	if (State) Options += (1<<6);
	return ((Options & (1<<6))?true:false);
}
bool ControlGroup::UseDFT(bool State) {
	if (Options & (1<<4)) Options -= (1<<4);
	if (State) Options += (1<<4);
	return (UseDFT());
}
bool ControlGroup::UseDFT(void) const {
	bool result = false;
	result = ((Options & (1<<4))?true:false);
	if (GetSCFType() > 3) result = false;
	if (GetMPLevel() > 0) result = false;
	if (GetCIType() > 0) result = false;
	if (GetCCType() != CC_None) result = false;
	return result;
}
bool ControlGroup::SetNormP(bool State) {
	if (Options & (1<<7)) Options -= (1<<7);
	if (State) Options += (1<<7);
	return GetNormP();
}
void ControlGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOControlGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOCGSCFType:
					{
						const char * v = child->getValue();
						if (v) SetSCFType(v);
					}
						break;
					case MMP_IOCGRunType:
					{
						const char * v = child->getValue();
						if (v) SetRunType(v);
					}
						break;
					case MMP_IOCGExeType:
					{
						const char * v = child->getValue();
						if (v) SetExeType(v);
					}
						break;
					case MMP_IOCGMPLevel:
					{
						const char * v = child->getValue();
						if (v) {
							int mpl;
							sscanf(v, "%d", &mpl);
							SetMPLevel(mpl);
						}
					}
						break;
					case MMP_IOCGCIType:
					{
						const char * v = child->getValue();
						if (v) SetCIType(v);
					}
						break;
					case MMP_IOCGCCType:
					{
						const char * v = child->getValue();
						if (v) SetCCType(v);
					}
						break;
					case MMP_IOCGMaxIterations:
					{
						const char * v = child->getValue();
						if (v) {
							int mit;
							sscanf(v, "%d", &mit);
							SetMaxIt(mit);
						}
					}
						break;
					case MMP_IOCGCharge:
					{
						const char * v = child->getValue();
						if (v) {
							int chg;
							sscanf(v, "%d", &chg);
							SetCharge(chg);
						}
					}
						break;
					case MMP_IOCGMultiplicity:
					{
						const char * v = child->getValue();
						if (v) {
							int mul;
							sscanf(v, "%d", &mul);
							SetMultiplicity(mul);
						}
					}
						break;
					case MMP_IOCGLocalization:
					{
						const char * v = child->getValue();
						if (v) SetLocal(v);
					}
						break;
					case MMP_IOCGFriend:
					{
						const char * v = child->getValue();
						if (v) SetFriend(v);
					}
						break;
					case MMP_IOCGPrintOption:
					{
						const char * v = child->getValue();
						if (v) {
							int np;
							sscanf(v, "%d", &np);
							NPrint = np;
						}
					}
						break;
					case MMP_IOCGTolerance:
					{
						const char * v = child->getValue();
						if (v) {
							int it;
							sscanf(v, "%d", &it);
							ITol = it;
						}
					}
						break;
					case MMP_IOCGCutoff:
					{
						const char * v = child->getValue();
						if (v) {
							int it;
							sscanf(v, "%d", &it);
							ICut = it;
						}
					}
						break;
					case MMP_IOCGMolPlt:
						if (child->getBoolValue(tb))
							SetMolPlot(tb);
						break;
					case MMP_IOCGPlotOrb:
						if (child->getBoolValue(tb))
							SetPlotOrb(tb);
						break;
					case MMP_IOCGAIMPac:
						if (child->getBoolValue(tb))
							SetAIMPAC(tb);
						break;
					case MMP_IOCGRPac:
						if (child->getBoolValue(tb))
							SetRPAC(tb);
						break;
					case MMP_IOCGDFTActive:
						if (child->getBoolValue(tb))
							UseDFT(tb);
						break;
					case MMP_IOCGSphericalHarm:
						if (child->getBoolValue(tb))
							UseSphericalHarmonics(tb);
						break;
					case MMP_IOCGNormF:
						if (child->getBoolValue(tb))
							SetNormF(tb);
						break;
					case MMP_IOCGNormP:
						if (child->getBoolValue(tb))
							SetNormP(tb);
						break;
				}
			}
		}
		delete children;
	}
}
void ControlGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOControlGroupElement));
	if (SCFType)
		Ele->addChildElement(CML_convert(MMP_IOCGSCFType), GetSCFTypeText());
	if (ExeType) Ele->addChildElement(CML_convert(MMP_IOCGExeType), ExeType);
	Ele->addChildElement(CML_convert(MMP_IOCGCIType), GetCIType(GetCIType()));
	Ele->addChildElement(CML_convert(MMP_IOCGRunType), GetGAMESSRunText(GetRunType()));
	snprintf(line, kMaxLineLength, "%d", GetMPLevel());
	Ele->addChildElement(CML_convert(MMP_IOCGMPLevel), line);
	if (GetCCType() != CC_None)
		Ele->addChildElement(CML_convert(MMP_IOCGCCType), GetGAMESSCCType(GetCCType()));
	if (MaxIt) {
		snprintf(line, kMaxLineLength, "%d", MaxIt);
		Ele->addChildElement(CML_convert(MMP_IOCGMaxIterations), line);
	}
	if (Charge) {
		snprintf(line, kMaxLineLength, "%d", Charge);
		Ele->addChildElement(CML_convert(MMP_IOCGCharge), line);
	}
	if (Multiplicity) {
		snprintf(line, kMaxLineLength, "%d", Multiplicity);
		Ele->addChildElement(CML_convert(MMP_IOCGMultiplicity), line);
	}
	if (Local) {
		Ele->addChildElement(CML_convert(MMP_IOCGLocalization), GetLocalText());
	}
	if (Friend) {	//punchs out input to other programs, disables exetype (forces check run)
		Ele->addChildElement(CML_convert(MMP_IOCGFriend), GetFriendText());
	}
	if (NPrint) {
		snprintf(line, kMaxLineLength, "%d", NPrint);
		Ele->addChildElement(CML_convert(MMP_IOCGPrintOption), line);
	}
	if (ITol) {
		snprintf(line, kMaxLineLength, "%d", ITol);
		Ele->addChildElement(CML_convert(MMP_IOCGTolerance), line);
	}
	if (ICut) {
		snprintf(line, kMaxLineLength, "%d", ICut);
		Ele->addChildElement(CML_convert(MMP_IOCGCutoff), line);
	}
	if (GetMolPlot()) Ele->addChildElement(CML_convert(MMP_IOCGMolPlt), trueXML);
	if (GetPlotOrb()) Ele->addChildElement(CML_convert(MMP_IOCGPlotOrb), trueXML);
	if (GetAIMPAC()) Ele->addChildElement(CML_convert(MMP_IOCGAIMPac), trueXML);
	if (GetRPAC()) Ele->addChildElement(CML_convert(MMP_IOCGRPac), trueXML);
	if (UseDFT()) Ele->addChildElement(CML_convert(MMP_IOCGDFTActive), trueXML);
	if (UseSphericalHarmonics()) Ele->addChildElement(CML_convert(MMP_IOCGSphericalHarm), trueXML);
	if (GetNormF()) Ele->addChildElement(CML_convert(MMP_IOCGNormF), trueXML);
	if (GetNormP()) Ele->addChildElement(CML_convert(MMP_IOCGNormP), trueXML);
}
void ControlGroup::WriteToFile(BufferFile *File, InputData *IData, long NumElectrons) {
	char	Out[133], textVal[133];

		//Punch the group label
	File->WriteLine(" $CONTRL ", false);
		//punch the SCF type and Run type
	if (SCFType) {
		sprintf(Out,"SCFTYP=%s ",GetSCFTypeText());
		File->WriteLine(Out, false);
	} else {	//Punch out the default RHF/ROHF wavefunction
		if (NumElectrons & 1) sprintf(Out, "SCFTYP=ROHF ");
		else sprintf(Out, "SCFTYP=RHF ");
		File->WriteLine(Out, false);
	}
	TypeOfRun t = GetRunType();
	if (t <= InvalidRunType) t = Energy;
	sprintf(Out,"RUNTYP=%s ", GetGAMESSRunText(t));
	File->WriteLine(Out, false);
	if ((ExeType)&&(!Friend)) {	//punch out ExeType if it is other than run
		sprintf(Out, "EXETYP=%s ", ExeType);
		File->WriteLine(Out, false);
	}
	if (GetMPLevel() > 0) {	//Write out MP level only if > zero
		sprintf(Out,"MPLEVL=2 ");
		File->WriteLine(Out, false);
	}
	if (GetCIType() || (GetSCFType() == 6)) {	//punch CIType if CI requested
		GetCIType(textVal);
		sprintf(Out, "CITYP=%s ", textVal);
		File->WriteLine(Out, false);
	}
	if (GetCCType() != CC_None) {
		sprintf(Out, "CCTYP=%s ", GetGAMESSCCType(CCType));
		File->WriteLine(Out, false);
	}

	if (UseDFT()) {
		sprintf(Out, "DFTTYP=%s ", IData->DFT.GetFunctionalText());
		File->WriteLine(Out, false);
	}
	
	if (MaxIt) {	//Punch Maxit if non-default value
		sprintf(Out, "MAXIT=%d ",MaxIt);
		File->WriteLine(Out, false);
	}
	if (Charge) {
		sprintf(Out, "ICHARG=%d ", Charge);
		File->WriteLine(Out, false);
	}
	if (Multiplicity) {
		sprintf(Out, "MULT=%d ", Multiplicity);
		File->WriteLine(Out, false);
	} else if (NumElectrons & 1) {	//for odd electron systems punch out a default doublet
		sprintf(Out, "MULT=2 ");
		File->WriteLine(Out, false);
	}
	if (Local) {
		sprintf(Out, "LOCAL=%s ", GetLocalText());
 		File->WriteLine(Out, false);
	}
	if (IData->Basis) {
		if (IData->Basis->GetECPPotential()) {
			sprintf(Out, "PP=%s ",IData->Basis->GetECPPotentialText());
	 		File->WriteLine(Out, false);
		}
	}
	bool tempSphere = UseSphericalHarmonics();
	if (!tempSphere && IData->Basis) {
		if ((IData->Basis->GetBasis()>=GAMESS_BS_CC_PVDZ)&&(IData->Basis->GetBasis()<=GAMESS_BS_APC4))
			tempSphere = true;
	}
	if (tempSphere) {
		sprintf(Out, "ISPHER=1 ");
		File->WriteLine(Out, false);
	}
	if (IData->Data) {
		if (IData->Data->GetCoordType()) {
			sprintf(Out, "COORD=%s ", IData->Data->GetCoordText());
	 		File->WriteLine(Out, false);
		}
		if (IData->Data->GetUnits()) {
			sprintf(Out, "UNITS=BOHR ");
	 		File->WriteLine(Out, false);
		}
		if (IData->Data->GetNumZVar()) {
			sprintf(Out, "NZVAR=%d ",IData->Data->GetNumZVar());
	 		File->WriteLine(Out, false);
		}
		if (!IData->Data->GetUseSym()) {
			sprintf(Out, "NOSYM=1 ");
	 		File->WriteLine(Out, false);
		}
	}
	if (Friend) {	//punchs out input to other programs, disables exetype (forces check run)
		sprintf(Out, "FRIEND=%s ", GetFriendText());
 		File->WriteLine(Out, false);
	}
	if (GetMolPlot()) {
		sprintf(Out, "MOLPLT=.TRUE. ");
 		File->WriteLine(Out, false);
	}
	if (GetPlotOrb()) {
		sprintf(Out, "PLTORB=.TRUE. ");
 		File->WriteLine(Out, false);
	}
	if ((1!=GetExeType())&&(Friend==0)) {
		if (GetAIMPAC()) {
			sprintf(Out, "AIMPAC=.TRUE. ");
	 		File->WriteLine(Out, false);
		}
		if (GetRPAC()) {
			sprintf(Out, "RPAC=.TRUE. ");
	 		File->WriteLine(Out, false);
		}
	}

	File->WriteLine("$END", true);
}
void ControlGroup::RevertControlPane(ControlGroup *OldData) {
	RunType = OldData->RunType;
	SCFType = OldData->SCFType;
	SetMPLevel(OldData->GetMPLevel());
	UseDFT(OldData->UseDFT());
	SetCIType(OldData->GetCIType());
	SetCCType(OldData->GetCCType());
	MaxIt = OldData->MaxIt;
	if (ExeType) {
		delete [] ExeType;
		ExeType = NULL;
	}
	SetExeType(OldData->ExeType);
	Local = OldData->Local;
	Charge = OldData->Charge;
	Multiplicity = OldData->Multiplicity;
}
void ControlGroup::RevertProgPane(ControlGroup *OldData) {
	SetMolPlot(OldData->GetMolPlot());
	SetPlotOrb(OldData->GetPlotOrb());
	SetAIMPAC(OldData->GetAIMPAC());
	SetRPAC(OldData->GetRPAC());
	SetFriend(OldData->GetFriend());
}
#pragma mark SystemGroup
		//SystemGroup member functions
long SystemGroup::SetTimeLimit(long NewTime) {
	if (NewTime >= 0) TimeLimit = NewTime;
	return TimeLimit;
}
const char * MemoryUnitToText(const MemoryUnit & mu) {
	switch (mu) {
		case wordsUnit:
			return "words";
		case bytesUnit:
			return "bytes";
		case megaWordsUnit:
			return "Mwords";
		case megaBytesUnit:
			return "MB";
		case gigaWordsUnit:
			return "Gwords";
		case gigaBytesUnit:
			return "GB";
	}
	return "invalid";
}
bool TextToMemoryUnit(const char * t, MemoryUnit & mu) {
	if (!t || !*t) return false;
	for (int i = (int) wordsUnit; i != (int) NumberMemoryUnits; ++i) {
		if (strcasecmp(t, MemoryUnitToText((MemoryUnit) i)) == 0) {
			mu = (MemoryUnit) i;
			return true;
		}
	}
	return false;
}
const char * TimeUnitToText(const TimeUnit & tu) {
	switch (tu) {
		case secondUnit:
			return "sec";
		case minuteUnit:
			return "min";
		case hourUnit:
			return "hr";
		case dayUnit:
			return "days";
		case weekUnit:
			return "weeks";
		case yearUnit:
			return "years";
		case milleniaUnit:
			return "millenia";
	}
	return "invalid";
}
bool TextToTimeUnit(const char * t, TimeUnit & tu) {
	if (!t || !*t) return false;
	for (int i = (int) secondUnit; i != (int) NumberTimeUnits; ++i) {
		if (strcasecmp(t, TimeUnitToText((TimeUnit) i)) == 0) {
			tu = (TimeUnit) i;
			return true;
		}
	}
	return false;
}
TimeUnit SystemGroup::SetTimeUnits(TimeUnit NewUnits) {
	if ((NewUnits >= secondUnit)&&(NewUnits<NumberTimeUnits)) TimeUnits = NewUnits;
	return TimeUnits;
}
float SystemGroup::GetConvertedTime(void) const {
	float result, factor=1.0f;

	if (TimeLimit) result = TimeLimit;
	else result = 525600.0f;

	switch (TimeUnits) {
		case milleniaUnit:
			factor = 1.0/1000.0;
		case yearUnit:
			factor *= 1/52.0;
		case weekUnit:
			factor *= 1/7.0;
		case dayUnit:
			factor *= 1/24.0;
		case hourUnit:
			factor *= 1/60.0;
		case minuteUnit:
		break;
		case secondUnit:
			factor = 60.0f;
		break;
	}
	result *= factor;
	return result;
}
long SystemGroup::SetConvertedTime(float NewTime) {
	long	result, factor = 1;

	switch (TimeUnits) {
		case milleniaUnit:
			factor = 1000;
		case yearUnit:
			factor *= 52;
		case weekUnit:
			factor *= 7;
		case dayUnit:
			factor *= 24;
		case hourUnit:
			factor *= 60;
		case minuteUnit:
			result = (long)(NewTime * factor);
		break;
		case secondUnit:
			result = (long)(NewTime/60.0);
		break;
	}
	if (result >= 0) TimeLimit = result;
	return TimeLimit;
}
double SystemGroup::SetMemory(double NewMemory) {
	if (NewMemory > 0.0) Memory = NewMemory;
	return Memory;
}
MemoryUnit SystemGroup::SetMemUnits(MemoryUnit NewUnits) {
	if ((NewUnits>=wordsUnit)&&(NewUnits<NumberMemoryUnits)) MemUnits = NewUnits;
	return MemUnits;
}
double SystemGroup::GetConvertedMem(void) const {
	double result, factor=1.0;

	if (Memory) result = Memory;
	else result = 1000000;

	switch (MemUnits) {
		case bytesUnit:
			factor = 8.0;
		break;
		case megaWordsUnit:
			factor = 1.0/1000000.0;
		break;
		case megaBytesUnit:
			factor = 8.0/(1024*1024);
		break;
	}
	result *= factor;
	return result;
}
double SystemGroup::SetConvertedMem(double NewMem) {
	double	result, factor = 1;

	switch (MemUnits) {
		case megaBytesUnit:
			factor *= 1024*1024;
		case bytesUnit:
			result = (long)(factor*NewMem/8.0);
		break;
		case megaWordsUnit:
			factor *= 1000000;
		case wordsUnit:
			result = (long)(factor*NewMem);
		break;
	}
	if (result >= 0) Memory = result;
	return Memory;
}
double SystemGroup::SetMemDDI(double NewMemory) {
	if (NewMemory >= 0.0) MemDDI = NewMemory;
	return Memory;
}
MemoryUnit SystemGroup::SetMemDDIUnits(MemoryUnit NewUnits) {
	if ((NewUnits>=megaWordsUnit)&&(NewUnits<NumberMemoryUnits)) MemDDIUnits = NewUnits;
	return MemDDIUnits;
}
double SystemGroup::GetConvertedMemDDI(void) const {
	double result, factor=1.0;
	
	result = MemDDI;	//memDDI is stored in MW
	
	switch (MemDDIUnits) {
		case megaBytesUnit:
			factor = 8.0;
			break;
		case gigaWordsUnit:
			factor = 1.0/1000.0;
			break;
		case gigaBytesUnit:
			factor = 8.0/(1000.0);
			break;
	}
	result *= factor;
	return result;
}
double SystemGroup::SetConvertedMemDDI(double NewMem) {
	double	result, factor = 1;
	
	switch (MemDDIUnits) {
		case megaBytesUnit:
			factor = 1.0/8.0;
			break;
		case gigaWordsUnit:
			factor = 1000.0;
			break;
		case gigaBytesUnit:
			factor = 1000.0/8.0;
			break;
	}
	result = NewMem*factor;
	if (result >= 0) MemDDI = result;
	return MemDDI;
}

char SystemGroup::SetDiag(char NewMethod) {
	if ((NewMethod>=0)&&(NewMethod<4)) KDiag = NewMethod;
	return KDiag;
}
bool SystemGroup::SetCoreFlag(bool State) {
	if (Flags & 1) Flags --;
	if (State) Flags ++;
	return GetCoreFlag();
}
bool SystemGroup::SetBalanceType(bool Type) {
	if (Flags & 2) Flags -= 2;
	if (Type) Flags += 2;
	return GetBalanceType();
}
bool SystemGroup::SetXDR(bool State) {
	if (Flags & 4) Flags -= 4;
	if (State) Flags += 4;
	return GetXDR();
}
bool SystemGroup::SetParallel(bool State) {
	if (Flags & 8) Flags -= 8;
	if (State) Flags += 8;
	return GetParallel();
}
SystemGroup::SystemGroup(void) {
	InitData();
}
SystemGroup::SystemGroup(SystemGroup *Copy) {
	if (Copy) *this=*Copy;
}
void SystemGroup::InitData(void) {
	TimeLimit = 0;
	Memory = 0.0;
	MemDDI = 0.0;
	KDiag = 0;
	TimeUnits = minuteUnit;
	MemUnits = wordsUnit;
	MemDDIUnits = megaWordsUnit;
	Flags = 0;
}
void SystemGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOSystemGroupElement));
	if (TimeLimit) {
		snprintf(line, kMaxLineLength, "%f", GetConvertedTime());
		XMLElement * t = Ele->addChildElement(CML_convert(MMP_IOSGTimeLimit), line);
		t->addAttribute(CML_convert(MMP_IOSGTimeUnits), TimeUnitToText(TimeUnits));
	}
	if (Memory) {
		snprintf(line, kMaxLineLength, "%lf", GetConvertedMem());
		XMLElement * t = Ele->addChildElement(CML_convert(MMP_IOSGMemory), line);
		t->addAttribute(CML_convert(MMP_IOSGMemoryUnits), MemoryUnitToText(MemUnits));
	}
	if (MemDDI) {
		snprintf(line, kMaxLineLength, "%lf", GetConvertedMemDDI());
		XMLElement * t = Ele->addChildElement(CML_convert(MMP_IOSGMemDDI), line);
		t->addAttribute(CML_convert(MMP_IOSGMemoryUnits), MemoryUnitToText(MemDDIUnits));
	}
	if (KDiag) {
		snprintf(line, kMaxLineLength, "%d", KDiag);
		Ele->addChildElement(CML_convert(MMP_IOSGKDiag), line);
	}
	if (GetParallel()) Ele->addChildElement(CML_convert(MMP_IOSGParallel), trueXML);
	if (GetCoreFlag()) Ele->addChildElement(CML_convert(MMP_IOSGCoreFlag), trueXML);
	if (GetBalanceType()) Ele->addChildElement(CML_convert(MMP_IOSGBalanceType), trueXML);
	if (GetXDR()) Ele->addChildElement(CML_convert(MMP_IOSGXDR), trueXML);
}
void SystemGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOSystemGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOSGTimeLimit:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							const char * u = child->getAttributeValue(CML_convert(MMP_IOSGTimeUnits));
							if (u) {
								TimeUnit t;
								if (TextToTimeUnit(u, t)) SetTimeUnits(t);
							}
							SetConvertedTime(temp);
						}
					}
						break;
					case MMP_IOSGMemory:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							const char * u = child->getAttributeValue(CML_convert(MMP_IOSGMemoryUnits));
							if (u) {
								MemoryUnit t;
								if (TextToMemoryUnit(u, t)) SetMemUnits(t);
							}
							SetConvertedMem(temp);
						}
					}
						break;
					case MMP_IOSGKDiag:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetDiag(temp);
						}
					}
						break;
					case MMP_IOSGCoreFlag:
						if (child->getBoolValue(tb))
							SetCoreFlag(tb);
						break;
					case MMP_IOSGBalanceType:
						if (child->getBoolValue(tb))
							SetBalanceType(tb);
						break;
					case MMP_IOSGXDR:
						if (child->getBoolValue(tb))
							SetXDR(tb);
						break;
					case MMP_IOSGMemDDI:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							const char * u = child->getAttributeValue(CML_convert(MMP_IOSGMemoryUnits));
							if (u) {
								MemoryUnit t;
								if (TextToMemoryUnit(u, t)) SetMemDDIUnits(t);
							}
							SetConvertedMemDDI(temp);
						}
					}
						break;
					case MMP_IOSGParallel:
						if (child->getBoolValue(tb))
							SetParallel(tb);
						break;
				}
			}
		}
		delete children;
	}
}
void SystemGroup::WriteToFile(BufferFile *File) {
	long	test;
	char	Out[133];

		//Punch the group label
	File->WriteLine(" $SYSTEM ", false);
		//Time limit
	test = TimeLimit;
	if (test==0) test = 600;
	sprintf(Out,"TIMLIM=%ld ",test);
	File->WriteLine(Out, false);
		//Memory
	if (Memory) {
		sprintf(Out, "MEMORY=%ld ", (long)Memory);
		File->WriteLine(Out, false);
	}
	if (MemDDI) {
		sprintf(Out, "MEMDDI=%ld ", (long)MemDDI);
		File->WriteLine(Out, false);
	}	//PARALL
	if (GetParallel()) {
		sprintf(Out, "PARALL=.TRUE. ");
		File->WriteLine(Out, false);
	}	//diag method
	if (KDiag) {
		sprintf(Out, "KDIAG=%d ", KDiag);
		File->WriteLine(Out, false);
	}	//core flag
	if (GetCoreFlag()) {
		sprintf(Out, "COREFL=.TRUE. ");
		File->WriteLine(Out, false);
	}	//Balance type
	if (GetBalanceType()) {
		sprintf(Out, "BALTYP=NXTVAL ");
		File->WriteLine(Out, false);
	}	//XDR
	if (GetXDR()) {
		sprintf(Out, "XDR=.TRUE. ");
		File->WriteLine(Out, false);
	}
	File->WriteLine("$END", true);
}
#pragma mark BasisGroup
		//BasisGroup member functions
BasisGroup::BasisGroup(void) {
	InitData();
}
BasisGroup::BasisGroup(BasisGroup *Copy) {
	if (Copy) {
		*this = *Copy;
	}
}
void BasisGroup::InitData(void) {
	Split2[0]=Split2[1]=0.0f;
	Split3[0]=Split3[1]=Split3[2]=0.0f;
	Basis=NumGauss=NumHeavyFuncs=NumPFuncs=ECPPotential=0;
	Polar = GAMESS_BS_No_Polarization;
	Flags = 0;
}
const char * BasisGroup::GAMESSBasisSetToText(GAMESS_BasisSet bs) {
	switch (bs) {
		case GAMESS_BS_MINI:
			return "MINI";
		case GAMESS_BS_MIDI:
			return "MIDI";
		case GAMESS_BS_STO:
			return "STO";
		case GAMESS_BS_N21:
			return "N21";
		case GAMESS_BS_N31:
			return "N31";
		case GAMESS_BS_N311:
			return "N311";
		case GAMESS_BS_DZV:
			return "DZV";
		case GAMESS_BS_DH:
			return "DH";
		case GAMESS_BS_BC:
			return "BC";
		case GAMESS_BS_TZV:
			return "TZV";
		case GAMESS_BS_MC:
			return "MC";
		case GAMESS_BS_CC_PVDZ:
			return "CCD";
		case GAMESS_BS_CC_PVTZ:
			return "CCT";
		case GAMESS_BS_CC_PVQZ:
			return "CCQ";
		case GAMESS_BS_CC_PV5Z:
			return "CC5";
		case GAMESS_BS_CC_PV6Z:
			return "CC6";
		case GAMESS_BS_AUG_CC_PVDZ:
			return "ACCD";
		case GAMESS_BS_AUG_CC_PVTZ:
			return "ACCT";
		case GAMESS_BS_AUG_CC_PVQZ:
			return "ACCQ";
		case GAMESS_BS_AUG_CC_PV5Z:
			return "ACC5";
		case GAMESS_BS_AUG_CC_PV6Z:
			return "ACC6";
		case GAMESS_BS_CC_PCVDZ:
			return "CCDC";
		case GAMESS_BS_CC_PCVTZ:
			return "CCTC";
		case GAMESS_BS_CC_PCVQZ:
			return "CCQC";
		case GAMESS_BS_CC_PCV5Z:
			return "CC5C";
		case GAMESS_BS_CC_PCV6Z:
			return "CC6C";
		case GAMESS_BS_AUG_CC_PCVDZ:
			return "ACCDC";
		case GAMESS_BS_AUG_CC_PCVTZ:
			return "ACCTC";
		case GAMESS_BS_AUG_CC_PCVQZ:
			return "ACCQC";
		case GAMESS_BS_AUG_CC_PCV5Z:
			return "ACC5C";
		case GAMESS_BS_AUG_CC_PCV6Z:
			return "ACC6C";
		case GAMESS_BS_PC0:
			return "PC0";
		case GAMESS_BS_PC1:
			return "PC1";
		case GAMESS_BS_PC2:
			return "PC2";
		case GAMESS_BS_PC3:
			return "PC3";
		case GAMESS_BS_PC4:
			return "PC4";
		case GAMESS_BS_APC0:
			return "APC0";
		case GAMESS_BS_APC1:
			return "APC1";
		case GAMESS_BS_APC2:
			return "APC2";
		case GAMESS_BS_APC3:
			return "APC3";
		case GAMESS_BS_APC4:
			return "APC4";
		case GAMESS_BW_SPK_DZP:
			return "SPK-DZP";
		case GAMESS_BW_SPK_TZP:
			return "SPK-TZP";
		case GAMESS_BW_SPK_QZP:
			return "SPK-QZP";
		case GAMESS_BW_SPK_ADZP:
			return "SPK-ADZP";
		case GAMESS_BW_SPK_ATZP:
			return "SPK-ATZP";
		case GAMESS_BW_SPK_AQZP:
			return "SPK-AQZP";
		case GAMESS_BW_SPKRDZP:
			return "SPKRDZP";
		case GAMESS_BW_SPKRTZP:
			return "SPKRTZP";
		case GAMESS_BW_SPKRQZP:
			return "SPKRQZP";
		case GAMESS_BW_SPKRADZP:
			return "SPKRADZP";
		case GAMESS_BW_SPKRATZP:
			return "SPKRATZP";
		case GAMESS_BW_SPKRAQZP:
			return "SPKRAQZP";
		case GAMESS_BW_KTZV:
			return "KTZV";
		case GAMESS_BW_KTZVP:
			return "KTZVP";
		case GAMESS_BW_KTZVPP:
			return "KTZVPP";
			
		case GAMESS_BS_SBKJC:
			return "SBKJC";
		case GAMESS_BS_HW:
			return "HW";
		case GAMESS_BS_MCP_DZP:
			return "MCP-DZP";
		case GAMESS_BS_MCP_TZP:
			return "MCP-TZP";
		case GAMESS_BS_MCP_QZP:
			return "MCP-QZP";
		case GAMESS_BS_MCP_ATZP:
			return "MCP-ATZP";
		case GAMESS_BS_MCP_AQZP:
			return "MCP-AQZP";
		case GAMESS_BS_MCPCDZP:
			return "MCPCDZP";
		case GAMESS_BS_MCPCTZP:
			return "MCPCTZP";
		case GAMESS_BS_MCPCQZP:
			return "MCPCQZP";
		case GAMESS_BS_MCPACDZP:
			return "MCPACDZP";
		case GAMESS_BS_MCPACTZP:
			return "MCPACTZP";
		case GAMESS_BS_MCPACQZP:
			return "MCPACQZP";
		case GAMESS_BS_IMCP_SR1:
			return "IMCP-SR1";
		case GAMESS_BS_IMCP_SR2:
			return "IMCP-SR2";
		case GAMESS_BS_IMCP_NR1:
			return "IMCP-NR1";
		case GAMESS_BS_IMCP_NR2:
			return "IMCP-NR2";
		case GAMESS_BS_ZFK3_DK3:
			return "ZFK3-DK3";
		case GAMESS_BS_ZFK4_DK3:
			return "ZFK4-DK3";
		case GAMESS_BS_ZFK5_DK3:
			return "ZFK5-DK3";
		case GAMESS_BS_ZFK3LDK3:
			return "ZFK3LDK3";
		case GAMESS_BS_ZFK4LDK3:
			return "ZFK4LDK3";
		case GAMESS_BS_ZFK5LDK3:
			return "ZFK5LDK3";			
			
		case GAMESS_BS_MNDO:
			return "MNDO";
		case GAMESS_BS_AM1:
			return "AM1";
		case GAMESS_BS_PM3:
			return "PM3";
		case GAMESS_BS_RM1:
			return "RM1";
	}
	return "invalid";
}
short BasisGroup::SetBasis(const char *BasisText) {
	short NewBasis = -1;

	for (int i=GAMESS_BS_None; i<NumGAMESSBasisSetsItem; i++) {
		if (!strcasecmp(BasisText, GAMESSBasisSetToText((GAMESS_BasisSet)i))) {
			NewBasis = i;
			break;
		}
	}
	if (NewBasis<0) return -1;

	Basis = NewBasis;
	return Basis;
}
short BasisGroup::SetBasis(short NewBasis) {
	if ((NewBasis<-1)||(NewBasis>NumGAMESSBasisSetsItem)) return -1;
	
	Basis = NewBasis;
	return Basis;
}
const char * BasisGroup::GetBasisText(void) const {
	short temp = Basis;
	if (temp <= 0) temp = 1;

	return GAMESSBasisSetToText((GAMESS_BasisSet) temp);
}
short BasisGroup::GetBasis(void) const {
	return Basis;
}
short BasisGroup::SetNumGauss(short NewNumGauss) {
	if ((NewNumGauss<0)||(NewNumGauss>6)) return -1;
	if ((Basis==4)&&(NewNumGauss!=3)&&(NewNumGauss!=6)) return -1;
	if ((Basis==5)&&(NewNumGauss<4)) return -1;
	if ((Basis==6)&&(NewNumGauss!=6)) return -1;

	NumGauss = NewNumGauss;
	return NumGauss;
}
short BasisGroup::GetNumGauss(void) const {
	return NumGauss;
}
short BasisGroup::SetNumDFuncs(short NewNum) {
	if (NewNum > 3) return -1;

	NumHeavyFuncs = NewNum + (NumHeavyFuncs & 0xF0);
	return (NumHeavyFuncs & 0x0F);
}
short BasisGroup::GetNumDFuncs(void) const {
	return (NumHeavyFuncs & 0x0F);
}
short BasisGroup::SetNumFFuncs(short NewNum) {
	if (NewNum > 3) return -1;

	NumHeavyFuncs = (NewNum<<4) + (NumHeavyFuncs & 0x0F);
	return ((NumHeavyFuncs & 0xF0)>>4);
}
short BasisGroup::GetNumFFuncs(void) const {
	return ((NumHeavyFuncs & 0xF0)>>4);
}
short BasisGroup::SetNumPFuncs(short NewNum) {
	if (NewNum > 3) return -1;

	NumPFuncs = NewNum;
	return NumPFuncs;
}
short BasisGroup::GetNumPFuncs(void) const {
	return NumPFuncs;
}
short BasisGroup::SetDiffuseSP(bool state) {
	if (state && (!(Flags & 1))) Flags += 1;
	else if (!state && (Flags & 1)) Flags -= 1;

	return state;
}
short BasisGroup::SetDiffuseS(bool state) {
	if (state && (!(Flags & 2))) Flags += 2;
	else if (!state && (Flags & 2)) Flags -= 2;

	return state;
}
GAMESS_BS_Polarization BasisGroup::SetPolar(GAMESS_BS_Polarization NewPolar) {
	if ((NewPolar>=GAMESS_BS_No_Polarization)||(NewPolar<NumGAMESSBSPolarItems)) {
		Polar = NewPolar;
	}
	return Polar;
}
GAMESS_BS_Polarization BasisGroup::SetPolar(const char *PolarText) {
	GAMESS_BS_Polarization NewPolar = GAMESS_BS_Invalid_Polar;

	for (int i=GAMESS_BS_No_Polarization; i<NumGAMESSBSPolarItems; i++) {
		if (!strcasecmp(PolarText, PolarToText((GAMESS_BS_Polarization)i))) {
			NewPolar = (GAMESS_BS_Polarization) i;
			break;
		}
	}
	if (NewPolar == GAMESS_BS_Invalid_Polar) {	//test against the old POPLE flag
		if (!strcasecmp(PolarText, PolarToText(GAMESS_BS_Pople_Polar))) {
			NewPolar = GAMESS_BS_Common_Polar;
		}
	}
	if (NewPolar>=0) Polar = NewPolar;
	return NewPolar;
}
const char * BasisGroup::PolarToText(GAMESS_BS_Polarization p) {
	switch (p) {
		case GAMESS_BS_Pople_Polar:	//this is retained to read old output
			return "POPLE";
		case GAMESS_BS_No_Polarization:
			return "none";
		case GAMESS_BS_Common_Polar:
			return "COMMON";
		case GAMESS_BS_PopN31_Polar:
			return "POPN31";
		case GAMESS_BS_PopN311_Polar:
			return "POPN311";
		case GAMESS_BS_Dunning_Polar:
			return "DUNNING";
		case GAMESS_BS_Huzinaga_Polar:
			return "HUZINAGA";
		case GAMESS_BS_Hondo7_Polar:
			return "HONDO7";
	}
	return "invalid";
}
const char * BasisGroup::GAMESSECPToText(GAMESS_BS_ECPotential p) {
	switch (p) {
		case GAMESS_BS_ECP_None:
			return "NONE";
		case GAMESS_BS_ECP_Read:
			return "READ";
		case GAMESS_BS_ECP_SBKJC:
			return "SBKJC";
		case GAMESS_BS_ECP_HW:
			return "HW";
		case GAMESS_BS_ECP_MCP:
			return "MCP";
	}
	return "invalid";
}
GAMESS_BS_ECPotential BasisGroup::SetECPPotential(const char *ECPText) {
	GAMESS_BS_ECPotential NewPot = GAMESS_BS_Invalid_ECP;
	
	for (int i=GAMESS_BS_ECP_None; i<NumGAMESSBSECPItems; i++) {
		if (!strcasecmp(ECPText, GAMESSECPToText((GAMESS_BS_ECPotential)i))) {
			NewPot = (GAMESS_BS_ECPotential) i;
			break;
		}
	}
	if (NewPot == GAMESS_BS_Invalid_ECP) {	//test against the old spelling of sbkjc
		if (!strcasecmp(ECPText, "SBK")) {
			NewPot = GAMESS_BS_ECP_SBKJC;
		}
	}
	if (NewPot>=0) ECPPotential = NewPot;
	return NewPot;
}
short BasisGroup::GetECPPotential(void) const {
	short value = ECPPotential;
	if (value <= GAMESS_BS_ECP_None) {
		if (Basis == GAMESS_BS_SBKJC) value = GAMESS_BS_ECP_SBKJC;
		if (Basis == GAMESS_BS_HW) value = GAMESS_BS_ECP_HW;
		if ((Basis >= GAMESS_BS_MCP_DZP)&&(Basis <= GAMESS_BS_IMCP_NR2))
			value = GAMESS_BS_ECP_MCP;
	}
	return value;
}
const char * BasisGroup::GetECPPotentialText(void) const {
	return GAMESSECPToText((GAMESS_BS_ECPotential) GetECPPotential());
}
short BasisGroup::SetECPPotential(short NewType) {
	if ((NewType<GAMESS_BS_ECP_None)||(NewType>NumGAMESSBSECPItems)) return -1;
	ECPPotential = NewType;
	return ECPPotential;
}
void BasisGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOBasisGroupElement));
	if (GetBasis() != 0) {
		Ele->addChildElement(CML_convert(MMP_IOBGBasisSet), GetBasisText());
	}
	if (NumGauss) {
		snprintf(line, kMaxLineLength, "%d", NumGauss);
		Ele->addChildElement(CML_convert(MMP_IOBGNumGauss), line);
	}
	if (GetNumDFuncs()) {
		snprintf(line, kMaxLineLength, "%d", GetNumDFuncs());
		Ele->addChildElement(CML_convert(MMP_IOBGNumDFuncs), line);
	}
	if (GetNumFFuncs()) {
		snprintf(line, kMaxLineLength, "%d", GetNumFFuncs());
		Ele->addChildElement(CML_convert(MMP_IOBGNumFFuncs), line);
	}
	if (GetNumPFuncs()) {
		snprintf(line, kMaxLineLength, "%d", GetNumPFuncs());
		Ele->addChildElement(CML_convert(MMP_IOBGNumPFuncs), line);
	}
	if (GetPolar() != 0) {
		Ele->addChildElement(CML_convert(MMP_IOBGPolar), GetPolarText());
	}
	if (GetECPPotential() != 0) {
		Ele->addChildElement(CML_convert(MMP_IOBGECPPotential), GetECPPotentialText());
	}
	if (GetDiffuseSP()) Ele->addChildElement(CML_convert(MMP_IOBGDiffuseSP), trueXML);
	if (GetDiffuseS()) Ele->addChildElement(CML_convert(MMP_IOBGDiffuseS), trueXML);
	if (CheckBasis()) Ele->addChildElement(CML_convert(MMP_IOBGDisableBS), trueXML);
}
void BasisGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOBasisGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOBGBasisSet:
					{
						const char * v = child->getValue();
						if (v) SetBasis(v);
					}
						break;
					case MMP_IOBGNumGauss:
					{
						long temp;
						if (child->getLongValue(temp)) SetNumGauss(temp);
					}
						break;
					case MMP_IOBGNumDFuncs:
					{
						long temp;
						if (child->getLongValue(temp)) SetNumDFuncs(temp);
					}
						break;
					case MMP_IOBGNumFFuncs:
					{
						long temp;
						if (child->getLongValue(temp)) SetNumFFuncs(temp);
					}
						break;
					case MMP_IOBGNumPFuncs:
					{
						long temp;
						if (child->getLongValue(temp)) SetNumPFuncs(temp);
					}
						break;
					case MMP_IOBGPolar:
					{
						const char * v = child->getValue();
						if (v) SetPolar(v);
					}
						break;
					case MMP_IOBGECPPotential:
					{
						const char * v = child->getValue();
						if (v) SetECPPotential(v);
					}
						break;
					case MMP_IOBGDiffuseSP:
						if (child->getBoolValue(tb))
							SetDiffuseSP(tb);
						break;
					case MMP_IOBGDiffuseS:
						if (child->getBoolValue(tb))
							SetDiffuseS(tb);
						break;
					case MMP_IOBGDisableBS:
						if (child->getBoolValue(tb))
							CheckBasis(tb);
						break;
				}
			}
		}
		delete children;
	}
}
long BasisGroup::WriteToFile(BufferFile *File, MoleculeData * lData) {
	char	Out[133];
		//if a general basis set is present don't punch the $Basis group
	if (lData->GetBasisSet() && (GetBasis() == 0)) return 1;
		//Punch the group label
	File->WriteLine(" $BASIS ", false);
		//Basis Set
	sprintf(Out,"GBASIS=%s ", GetBasisText());
	File->WriteLine(Out, false);
		//Number of Gaussians
	if (NumGauss) {
		sprintf(Out, "NGAUSS=%d ", NumGauss);
		File->WriteLine(Out, false);
	}	//number of heavy atom polarization functions
	if (GetNumDFuncs()) {
		sprintf(Out, "NDFUNC=%d ", GetNumDFuncs());
		File->WriteLine(Out, false);
	}	//number of heavy atom f type polarization functions
	if (GetNumFFuncs()) {
		sprintf(Out, "NFFUNC=%d ", GetNumFFuncs());
		File->WriteLine(Out, false);
	}	//number of light atom polarization functions
	if (NumPFuncs) {
		sprintf(Out, "NPFUNC=%d ", NumPFuncs);
		File->WriteLine(Out, false);
	}	//type of Polarization functions
	if ((Polar)&&((NumHeavyFuncs)||(NumPFuncs))) {
		sprintf(Out, "POLAR=%s ", GetPolarText());
		File->WriteLine(Out, false);
	}
	if (GetDiffuseSP()) {
		sprintf(Out, "DIFFSP=.TRUE. ");
		File->WriteLine(Out, false);
	}
	if (GetDiffuseS()) {
		sprintf(Out, "DIFFS=.TRUE. ");
		File->WriteLine(Out, false);
	}
	File->WriteLine("$END", true);
	return 0;
}
#pragma mark DataGroup
		// Data Group member functions
DataGroup::DataGroup(void) {
	InitData();
}
DataGroup::DataGroup(DataGroup *Copy) {
	if (Copy) {
		*this = *Copy;
		Title = NULL;
		if (Copy->Title) {
			Title = new char[1+strlen(Copy->Title)];
			if (Title) strcpy(Title, Copy->Title);
		}
	}
}
DataGroup::~DataGroup(void) {
	if (Title) delete [] Title;
}
void DataGroup::InitData(void) {
	Title = 0;
	Coord = NumZVar = 0;
	PointGroup = 1;
	PGroupOrder = Options = 0;
	SetUseSym(true);
}
short DataGroup::SetPointGroup(GAMESSPointGroup NewPGroup) {
	if ((NewPGroup<invalidPGroup)||(NewPGroup>NumberGAMESSPointGroups)) return -1;

	PointGroup = NewPGroup;
	return PointGroup;
}
const char * DataGroup::GetGAMESSPointGroupText(GAMESSPointGroup p) {
	switch (p) {
		case GAMESS_C1:
			return "C1";
		case GAMESS_CS:
			return "CS";
		case GAMESS_CI:
			return "CI";
		case GAMESS_CNH:
			return "CNH";
		case GAMESS_CNV:
			return "CNV";
		case GAMESS_CN:
			return "CN";
		case GAMESS_S2N:
			return "S2N";
		case GAMESS_DND:
			return "DND";
		case GAMESS_DNH:
			return "DNH";
		case GAMESS_DN:
			return "DN";
		case GAMESS_TD:
			return "TD";
		case GAMESS_TH:
			return "TH";
		case GAMESS_T:
			return "T";
		case GAMESS_OH:
			return "OH";
		case GAMESS_O:
			return "O";
	}
	return "invalid";
}
short DataGroup::SetPointGroup(char *GroupText) {
	GAMESSPointGroup NewPGroup=invalidPGroup;
	
	for (int i=0; i<5; i++) {
		if (!GroupText[i]) break;
		if ((GroupText[i]>96)&&(GroupText[i]<123)) GroupText[i] = GroupText[i] - 32;
	}
	if (std::toupper(GroupText[0]) == 'S') {
		PGroupOrder = GroupText[2] - 48;
		GroupText[2]='N';
	} else {
		int i=0;
		while (GroupText[i]&&(GroupText[i]!=' ')) {
			if (isdigit(GroupText[i])&&(GroupText[i]!='1')) {
				PGroupOrder = GroupText[i] - 48;	//single digit coverted to decimal digit
				GroupText[i]='N';
			}
			i++;
		}
	}

	for (int i=1; i<NumberGAMESSPointGroups; i++) {
		if (strcasecmp(GroupText, GetGAMESSPointGroupText((GAMESSPointGroup) i))==0) {
			NewPGroup = (GAMESSPointGroup) i;
			break;
		}
	}
	if (NewPGroup<=invalidPGroup) return invalidPGroup;

	if (NewPGroup<0) return -1;

	PointGroup = NewPGroup;
	return PointGroup;
}
short DataGroup::SetPointGroupOrder(short NewOrder) {
	if (NewOrder > 0) PGroupOrder = NewOrder;
	return PGroupOrder;
}
short DataGroup::SetTitle(const char *NewTitle, long length) {
	if (Title) delete[] Title;
	Title = NULL;

	if (length == -1) length = strlen(NewTitle);

		long TitleStart=0, TitleEnd=length-1, i, j;
		//Strip blanks of both ends of title
	while ((NewTitle[TitleStart] <= ' ')&&(TitleStart<length)) TitleStart ++;
	while ((NewTitle[TitleEnd] <= ' ')&&(TitleEnd>0)) TitleEnd --;
	length = TitleEnd - TitleStart + 1;

	if (length <= 0) return 0;
	if (length > 132) return -1;	//Title card is limited to one line

	Title = new char[length + 1];
	if (Title == NULL) throw MemoryError();
	j=0;
	for (i=TitleStart; i<=TitleEnd; i++) {
		if ((NewTitle[i] == '\n')||(NewTitle[i] == '\r')) {
			Title[j] = 0;
			break;
		}
		Title[j] = NewTitle[i];
		j++;
	}
	Title[j]=0;
	return j;
}
const char * DataGroup::GetTitle(void) const {
	return Title;
}
CoordinateType DataGroup::GetCoordType(void) const {
	return (CoordinateType) Coord;
}
const char * DataGroup::GetCoordTypeText(CoordinateType t) {
	switch (t) {
		case UniqueCoordType:
			return "UNIQUE";
		case HINTCoordType:
			return "HINT";
		case CartesianCoordType:
			return "CART";
		case ZMTCoordType:
			return "ZMT";
		case ZMTMPCCoordType:
			return "ZMTMPC";
	}
	return "invalid";
}
CoordinateType DataGroup::SetCoordType(const char * CoordText) {
	CoordinateType NewCoord = invalidCoordinateType;
	for (int i=1; i<NumberCoordinateTypes; i++) {
		if (strcasecmp(CoordText, GetCoordTypeText((CoordinateType) i))==0) {
			NewCoord = (CoordinateType) i;
			break;
		}
	}
	if (NewCoord<=invalidCoordinateType) return invalidCoordinateType;
	Coord = NewCoord;
	return (CoordinateType) Coord;
}
CoordinateType DataGroup::SetCoordType(CoordinateType NewType) {
	if ((NewType<UniqueCoordType)&&(NewType>NumberCoordinateTypes)) return invalidCoordinateType;
	Coord = NewType;
	return (CoordinateType) Coord;
}
bool DataGroup::SetUnits(bool NewType) {
	if (Options & 1) Options -= 1;
	if (NewType) Options += 1;
	return GetUnits();
}
int DataGroup::SetUnits(const char * u) {
	int rval = -1;
	if (!strcasecmp(u, "ANGS")) {
		SetUnits(false);
		rval = 0;
	} else if (!strcasecmp(u, "BOHR")) {
		SetUnits(true);
		rval = 1;
	}
	return rval;
}
bool DataGroup::SetUseSym(bool State) {
	if (Options & (1<<1)) Options -= (1<<1);
	if (State) Options += (1<<1);
	return GetUseSym();
}
short DataGroup::SetNumZVar(short NewNum) {
	if (NewNum<0) return -1;	//bad number
	NumZVar = NewNum;
	return NumZVar;
}
void DataGroup::WriteToFile(BufferFile *File, MoleculeData * MainData, WinPrefs * Prefs, long BasisTest) {
	char	Out[133];

	Frame * cFrame = MainData->GetCurrentFramePtr();
	BasisSet * lBasis = MainData->GetBasisSet();
	BasisTest = BasisTest && lBasis;	//Make sure there really is a basis set defined
	float unitConversion = 1.0f;
	if (MainData->InputOptions->Data->GetUnits()) unitConversion = kAng2BohrConversion;
		//Punch the group label
	//Check the number of ab initio atoms. If zero and we have effective fragments force
	//Coord = fragonly
	long abInitioCount=0, fragAtomCount=0;
	for (long iatom=0; iatom<cFrame->NumAtoms; iatom++) {
		if (cFrame->Atoms[iatom].IsEffectiveFragment()) fragAtomCount++;
		else abInitioCount++;
	}
	if ((abInitioCount==0)&&(fragAtomCount>0))
		File->WriteLine(" $CONTRL COORD=FRAGONLY $END", true);
	File->WriteLine(" $DATA ", true);
		//title
	if (Title == NULL) File->WriteLine("Title goes here", true);
	else File->WriteLine(Title, true);
		//Point Group
	if (((int) PointGroup > GAMESS_CI) && ((int) PointGroup < GAMESS_TD)) {
		sprintf(Out, "%s %d", GetPointGroupText(), PGroupOrder);
	} else sprintf(Out, "%s", GetPointGroupText());
	File->WriteLine(Out, true);
	if ((PointGroup!=0)&&(PointGroup!=1)) File->WriteLine("", true);
	if (! MainData->InputOptions->FMO.IsFMOActive()) {
			//coordinates
		if (Coord == ZMTCoordType) {	//"normal" style z-matrix
			Internals * IntCoords = MainData->GetInternalCoordinates();
			if (!IntCoords) {
				MainData->InitializeInternals();
				IntCoords = MainData->GetInternalCoordinates();
			}
			if (IntCoords) IntCoords->WriteCoordinatesToFile(File, MainData, Prefs);
		} else if (Coord == ZMTMPCCoordType) {
			Internals * IntCoords = MainData->GetInternalCoordinates();
			if (!IntCoords) {
				MainData->InitializeInternals();
				IntCoords = MainData->GetInternalCoordinates();
			}
			if (IntCoords) IntCoords->WriteMPCZMatCoordinatesToFile(File, MainData, Prefs);
		} else {
			if (Coord <= UniqueCoordType) MainData->GenerateSymmetryUniqueAtoms(1.0E-3);
			for (int iatom=0; iatom<cFrame->NumAtoms; iatom++) {
				if (!cFrame->Atoms[iatom].IsEffectiveFragment()) {
					if ((Coord > UniqueCoordType)||(cFrame->Atoms[iatom].IsSymmetryUnique())) {
						Str255 AtomLabel;
						Prefs->GetAtomLabel(cFrame->Atoms[iatom].GetType()-1, AtomLabel);
						AtomLabel[AtomLabel[0]+1] = 0;
						sprintf(Out, "%s   %5.1f  %10.5f  %10.5f  %10.5f",
							(char *) &(AtomLabel[1]), (float) (cFrame->Atoms[iatom].GetType()), 
							cFrame->Atoms[iatom].Position.x*unitConversion,
							cFrame->Atoms[iatom].Position.y*unitConversion,
							cFrame->Atoms[iatom].Position.z*unitConversion);
						File->WriteLine(Out, true);
						if (BasisTest) lBasis->WriteBasis(File, iatom);
					}
				}
			}
		}
	} else {
		//For FMO runs instead of the coordinates emit one line per atom type
		//First determine the list of unique atom types
		std::vector<int>	atomTypes;
		for (long i=0; i<cFrame->NumAtoms; i++) {
			bool unique=true;
			int atomT = cFrame->Atoms[i].GetType();
			for (int j=0; j<atomTypes.size(); j++) {
				if (atomTypes[j] == atomT) {
					unique = false;
					break;
				}
			}
			if (unique) atomTypes.push_back(atomT);
		}
		for (int i=0; i<atomTypes.size(); i++) {
			Str255 AtomLabel;
			Prefs->GetAtomLabel(atomTypes[i]-1, AtomLabel);
			AtomLabel[AtomLabel[0]+1] = 0;
			// the position is unnescessary, these are here to generate the basis set only.
			sprintf(Out, "%s   %d", (char *) &(AtomLabel[1]), atomTypes[i]);
			File->WriteLine(Out, true);
		}
	}
	
	File->WriteLine(" $END", true);
	if (NumZVar) {	//punch out the current connectivity in a $ZMAT group
		Internals * IntCoords = MainData->GetInternalCoordinates();
		if (IntCoords) IntCoords->WriteZMATToFile(File);
	}
	if (fragAtomCount > 0) {	//Now add on any effective fragments
		File->WriteLine(" $EFRAG ", true);
		File->WriteLine("COORD=CART", true);

		long last_frag = 0;
		long curr_frag;
		long atomsWritten=0, HydrogenIndex;
		const char *frag_name;
		for (int iatom=0; iatom<cFrame->NumAtoms; iatom++) {
			if (cFrame->Atoms[iatom].IsEffectiveFragment()) {
				curr_frag = cFrame->Atoms[iatom].GetFragmentNumber();
				frag_name = MainData->GetFragmentName(curr_frag - 1);
				if (curr_frag != last_frag) {
					//start of a new fragment, punch FRAGNAME
					last_frag = curr_frag;
					File->WriteLine("FRAGNAME=", false);
					File->WriteLine(frag_name, true);
					atomsWritten=0;
					HydrogenIndex=2;
				}
				EFrag *frag = NULL;
				if (strcmp(frag_name, "H2ORHF") != 0 &&
					strcmp(frag_name, "H2ODHT") != 0) {
					std::map<std::string, EFrag>::iterator frag_entry;
					frag_entry = MainData->efrags.find(std::string(frag_name));
					if (frag_entry != MainData->efrags.end()) {
						frag = &(frag_entry->second);
					}
				}
				if (atomsWritten < 3) {	//the EFRAG group only punchs the first three atoms
					//special case the atom labels for now. Eventually I think this should
					//be done via a FRAGNAME database. 
					if (!frag) {
						if (cFrame->Atoms[iatom].GetType() == 8)
							File->WriteLine("O1 ", false);
						else {
							sprintf(Out, "H%ld ", HydrogenIndex);
							File->WriteLine(Out, false);
							HydrogenIndex++;
						}
					} else {
						File->WriteLine(frag->GetAtoms()[atomsWritten].GetLabel().c_str(), false);
					}
					sprintf(Out, "%10.5f  %10.5f  %10.5f",
							cFrame->Atoms[iatom].Position.x, cFrame->Atoms[iatom].Position.y,
							cFrame->Atoms[iatom].Position.z);
					File->WriteLine(Out, true);
					atomsWritten++;
				}
			}
		}
		File->WriteLine(" $END", true);

		std::map<std::string, EFrag>::const_iterator frag;
		for (frag = MainData->efrags.begin();
			 frag != MainData->efrags.end();
			 ++frag) {
			const std::string& text = frag->second.GetText();
			File->Write(text.c_str(), text.size());
			File->WriteLine("", true);
		}
	}
	if (MainData->InputOptions->FMO.IsFMOActive()) {
		//FMO runs place all the coordinates in the FMOXYZ group
		File->WriteLine(" $FMOXYZ ", true);
		for (int iatom=0; iatom<cFrame->NumAtoms; iatom++) {
			if (!cFrame->Atoms[iatom].IsEffectiveFragment()) {
				Str255 AtomLabel;
				Prefs->GetAtomLabel(cFrame->Atoms[iatom].GetType()-1, AtomLabel);
				AtomLabel[AtomLabel[0]+1] = 0;
				sprintf(Out, "%s   %5.1f  %10.5f  %10.5f  %10.5f",
						(char *) &(AtomLabel[1]), (float) (cFrame->Atoms[iatom].GetType()), 
						cFrame->Atoms[iatom].Position.x*unitConversion,
						cFrame->Atoms[iatom].Position.y*unitConversion,
						cFrame->Atoms[iatom].Position.z*unitConversion);
				File->WriteLine(Out, true);
			}
		}
		File->WriteLine(" $END", true);
	}
}
void DataGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IODataGroupElement));
	if (Title) Ele->addChildElement(CML_convert(MMP_IODGTitle), Title);
	if (PointGroup) {
		snprintf(line, kMaxLineLength, "%d", PointGroup);
		XMLElement * t = Ele->addChildElement(CML_convert(MMP_IODGPointGroup), line);
		if (PGroupOrder) {
			snprintf(line, kMaxLineLength, "%d", PGroupOrder);
			t->addAttribute(CML_convert(MMP_IODGPointGroupOrder), line);
		}
	}
	if (Coord) Ele->addChildElement(CML_convert(MMP_IODGCoordType), GetCoordTypeText((CoordinateType) Coord));
	if (GetNumZVar()) {
		snprintf(line, kMaxLineLength, "%d", GetNumZVar());
		Ele->addChildElement(CML_convert(MMP_IODGNumZVars), line);
	}
	if (GetUnits()) Ele->addChildElement(CML_convert(MMP_IODGUnits), trueXML);
	if (GetUseSym()) Ele->addChildElement(CML_convert(MMP_IODGNoSymFlag), trueXML);
}
void DataGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IODataGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IODGTitle:
					{
						const char * v = child->getValue();
						if (v) SetTitle(v);
					}
						break;
					case MMP_IODGPointGroup:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetPointGroup((GAMESSPointGroup)temp);
							if (child->getAttributeValue(CML_convert(MMP_IODGPointGroupOrder), temp)) 
								SetPointGroupOrder(temp);
						}
					}
						break;
					case MMP_IODGCoordType:
					{
						const char * v = child->getValue();
						if (v) SetCoordType(v);
					}
						break;
					case MMP_IODGNumZVars:
					{
						long temp;
						if (child->getLongValue(temp)) SetNumZVar(temp);
					}
						break;
					case MMP_IODGUnits:
						if (child->getBoolValue(tb))
							SetUnits(tb);
						break;
					case MMP_IODGNoSymFlag:
						if (child->getBoolValue(tb))
							SetUseSym(tb);
						break;
				}
			}
		}
		delete children;
	}
}
#pragma mark GuessGroup
	//Guess Group functions
//This function is here to provide a default value before returning the string
const char * GuessGroup::GetGuessText(void) const {
	short value = GetGuess();
	if (value == 0) value = 1;

	return ConvertGuessType(value);
}
short GuessGroup::SetGuess(const char * GuessText) {
	short NewGuess = -1;

	for (int i=1; i<NumberGuessTypes; i++) {
		const char * val = ConvertGuessType(i);
		if (-1<LocateKeyWord(GuessText, val, strlen(val), 7)) {
			NewGuess = i;
			break;
		}
	}
	if (NewGuess<0) return -1;
	NewGuess = SetGuess(NewGuess);
	return NewGuess;
}
const char * GuessGroup::ConvertGuessType(const int & type) {
	switch (type) {
		case HUCKELGuessType:
			return "HUCKEL";
		case HCOREGuessType:
			return "HCORE";
		case MOREADGuessType:
			return "MOREAD";
		case MOSAVEDGuessType:
			return "MOSAVED";
		case SkipGuessType:
			return "SKIP";	//By hand later?
		default:
			return "invalid";
	}
	return NULL;
}
GuessGroup::GuessGroup(void) {
	InitData();
}
GuessGroup::GuessGroup(GuessGroup *Copy) {	//copy constructor
	if (Copy) {
		*this = *Copy;
		IOrder = JOrder = NULL;
			//check and copy I & J order here
	}
}
void GuessGroup::InitData(void) {
	MOTolZ = MOTolEquil = 0.0f;
	IOrder = JOrder = NULL;
	NumOrbs = 0;
	VecSource = 0;
	GuessType = 0;
	Options = 0;
}
void GuessGroup::WriteToFile(BufferFile *File, InputData *IData, MoleculeData * MainData) {
	long	test=false;
	char	Out[133];

	Frame * lFrame = MainData->GetCurrentFramePtr();
		//first determine wether or not the Guess group needs to be punched
	if (GetGuess()) test = true;
	if (GetPrintMO()) test = true;
	if (GetMix()&&IData->Control->GetMultiplicity()&&
		(IData->Control->GetSCFType()==2)) test = true;

	if (!test) return;

		//Punch the group label
	File->WriteLine(" $GUESS ", false);
		//Guess Type
	if (GetGuess()) {
		sprintf(Out,"GUESS=%s ", GetGuessText());
		File->WriteLine(Out, false);
	}
		//NumOrbs
	if (GetGuess()==3) {
		long nOrbs = GetNumOrbs();
		if (!nOrbs) {	//Make a guess if the guess comes from local orbs
			short tempVec = GetVecSource();
			const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
			if (Orbs->size() > 0) {
				if ((tempVec<=0)||(tempVec>Orbs->size() + 2)) tempVec = 2;
				if (tempVec > 1) {
					OrbitalRec * OrbSet = (*Orbs)[tempVec-2];
					nOrbs = OrbSet->getNumOccupiedAlphaOrbitals();
					if (nOrbs <= 0) nOrbs = OrbSet->getNumAlphaOrbitals();
				}
			}
		}
		sprintf(Out, "NORB=%ld ", nOrbs);
		File->WriteLine(Out, false);
	}	//PrintMO
	if (GetPrintMO()) {
		sprintf(Out, "PRTMO=.TRUE. ");
		File->WriteLine(Out, false);
	}	//Mix
	if (GetMix()&&((IData->Control->GetMultiplicity()==1)||
			(IData->Control->GetMultiplicity()==0))&&(IData->Control->GetSCFType()==2)) {
		sprintf(Out, "MIX=.TRUE. ");
		File->WriteLine(Out, false);
	}
	File->WriteLine("$END", true);
}
void GuessGroup::WriteVecGroup(BufferFile *File, MoleculeData * lData) {
		//prepare to punch out $Vec information if Guess=MORead
	if (GetGuess() == 3) {
		Frame * lFrame = lData->GetCurrentFramePtr();
		BasisSet * lBasis = lData->GetBasisSet();
		long NumBasisFuncs = lBasis->GetNumBasisFuncs(false);
		short tempVec = GetVecSource();
		const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
		if ((tempVec != 1)&&(Orbs->size() > 0)) {
			if ((tempVec<=0)||(tempVec > (Orbs->size() + 2))) tempVec = 2;
			if (tempVec > 1) {
				OrbitalRec * OrbSet = (*Orbs)[tempVec-2];
				long nOrbs = GetNumOrbs();
				if (nOrbs <= 0) {	//Setup the default value for the orbital count
					nOrbs = OrbSet->getNumOccupiedAlphaOrbitals();
					if (nOrbs <= 0) nOrbs = OrbSet->getNumAlphaOrbitals();
				}
				OrbSet->WriteVecGroup(File, NumBasisFuncs, nOrbs);
			}
		} else {
			File->WriteLine("You must provide a $VEC group here!", true);
		}
	}
}
void GuessGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOGuessGroupElement));
	if (GuessType) Ele->addChildElement(CML_convert(MMP_IOGGGuessType), ConvertGuessType(GuessType));
	if (NumOrbs) {
		snprintf(line, kMaxLineLength, "%ld", NumOrbs);
		Ele->addChildElement(CML_convert(MMP_IOGGNumOrbs), line);
	}
	if (VecSource) {
		snprintf(line, kMaxLineLength, "%d", VecSource);
		Ele->addChildElement(CML_convert(MMP_IOGGVecSource), line);
	}
	if (GetPrintMO()) Ele->addChildElement(CML_convert(MMP_IOGGPrintMO), trueXML);
	if (GetNOrder()) Ele->addChildElement(CML_convert(MMP_IOGGOrbReorder), trueXML);
	if (GetMix()) Ele->addChildElement(CML_convert(MMP_IOGGOrbMix), trueXML);
	if (MOTolZ) {
		snprintf(line, kMaxLineLength, "%f", MOTolZ);
		Ele->addChildElement(CML_convert(MMP_IOGGMOTolZ), line);
	}
	if (MOTolEquil) {
		snprintf(line, kMaxLineLength, "%f", MOTolEquil);
		Ele->addChildElement(CML_convert(MMP_IOGGMOTolEquil), line);
	}
}
void GuessGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOGuessGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOGGGuessType:
					{
						const char * v = child->getValue();
						if (v) SetGuess(v);
					}
						break;
					case MMP_IOGGNumOrbs:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetNumOrbs(temp);
						}
					}
						break;
					case MMP_IOGGVecSource:
					{
						long temp;
						if (child->getLongValue(temp)) {
							VecSource = temp;
						}
					}
						break;
					case MMP_IOGGPrintMO:
						if (child->getBoolValue(tb))
							SetPrintMO(tb);
						break;
					case MMP_IOGGOrbReorder:
						if (child->getBoolValue(tb))
							SetNOrder(tb);
						break;
					case MMP_IOGGOrbMix:
						if (child->getBoolValue(tb))
							SetMix(tb);
						break;
					case MMP_IOGGMOTolZ:
					{
						double temp;
						if (child->getDoubleValue(temp)) MOTolZ = temp;
					}
						break;
					case MMP_IOGGMOTolEquil:
					{
						double temp;
						if (child->getDoubleValue(temp)) MOTolEquil = temp;
					}
						break;
				}
			}
		}
		delete children;
	}
}
#pragma mark SCFGroup
SCFGroup::SCFGroup(void) {
	InitData();
}
SCFGroup::SCFGroup(SCFGroup *Copy) {
	if (Copy)
		*this = *Copy;
	else
		SCFGroup();
}
void SCFGroup::InitData(void) {
	SOGTolerance = EnErrThresh = DEMCutoff = DampCutoff = 0.0;
	ConvCriteria = MaxDIISEq = MVOCharge = 0;
	Punch = Options1 = ConverganceFlags = 0;
	GVBNumCoreOrbs = GVBNumOpenShells = GVBNumPairs = 0;
		//default Direct SCF to true. This is not the GAMESS default
		//but is better in most cases.
	SetDirectSCF(true);
	SetFockDiff(true);
}
bool SCFGroup::SetDirectSCF(bool State) {
	if (Options1 & 1) Options1--;
	if (State) Options1 ++;
	return GetDirectSCF();
}
bool SCFGroup::SetFockDiff(bool State) {
	if (Options1 & 2) Options1 -= 2;
	if (State) Options1 += 2;
	return GetFockDiff();
}
bool SCFGroup::SetUHFNO(bool State) {
	if (Options1 & 4) Options1 -= 4;
	if (State) Options1 += 4;
	return GetUHFNO();
}
short SCFGroup::SetConvergance(short NewConv) {
	if (NewConv > 0) ConvCriteria = NewConv;
	return ConvCriteria;
}
void SCFGroup::WriteToFile(BufferFile *File, InputData *IData) {
	long	test=false;
	char	Out[133];

		//first determine wether or not the SCF group needs to be punched
	if (IData->Control->GetSCFType() > 4) return;	//not relavent to the selected SCF type
	if (ConvCriteria > 0) test = true;
	if (GetDirectSCF()) test = true;
	if (IData->Control->GetSCFType() == GAMESS_GVB) test = true;
	
	if (!test) return;

		//Punch the group label
	File->WriteLine(" $SCF ", false);
		//Direct SCF
	if (GetDirectSCF()) {
		sprintf(Out,"DIRSCF=.TRUE. ");
		File->WriteLine(Out, false);
		if (!GetFockDiff()) {	//Fock Differencing requires direct SCF
			sprintf(Out,"FDIFF=.FALSE. ");
			File->WriteLine(Out, false);
		}
	}
		//convergance
	if (ConvCriteria > 0) {
		sprintf(Out, "NCONV=%d ", ConvCriteria);
		File->WriteLine(Out, false);
	}
	if (GetDIIS()) File->WriteLine("DIIS=.T. ", false);
	if (GetSOSCF()) File->WriteLine("SOSCF=.T. ", false);
	if (GetExtrapolation()) File->WriteLine("EXTRAP=.T. ", false);
	if (GetDamp()) File->WriteLine("DAMP=.T. ", false);
	if (GetShift()) File->WriteLine("SHIFT=.T. ", false);
	if (GetRestriction()) File->WriteLine("RSTRCT=.T. ", false);
	if (GetDEM()) File->WriteLine("DEM=.T. ", false);
		//UHF Natural Orbitals
	if (GetUHFNO()) {
		sprintf(Out, "UHFNOS=.TRUE. ");
		File->WriteLine(Out, false);
	}
	//GVB related items if this is a GVB run
	if (IData->Control->GetSCFType() == GAMESS_GVB) {
		sprintf(Out, "NCO=%ld ", GVBNumCoreOrbs);
		File->WriteLine(Out, false);
		if (GVBNumPairs>0) {
			sprintf(Out, "NPAIR=%ld ", GVBNumPairs);
			File->WriteLine(Out, false);
		}
		if (GVBNumOpenShells>0) {
			sprintf(Out, "NSETO=%ld ", GVBNumOpenShells);
			File->WriteLine(Out, false);
			File->WriteLine("NO=", false);
			std::ostringstream temp;
			for (long i=0; i< GVBNumOpenShells; i++) {
				if (i != 0) temp << ",";
				if (i < GVBOpenShellDeg.size()) {
					temp << GVBOpenShellDeg[i];
				} else {
					temp << "0";	//default value
				}
			}
			temp << " ";
			File->WriteLine(temp.str().c_str(), false);
		}
	}

	File->WriteLine("$END", true);
}
void SCFGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOSCFGroupElement));
	if (GetConvergance()) {
		snprintf(line, kMaxLineLength, "%d", GetConvergance());
		Ele->addChildElement(CML_convert(MMP_IOSGConvCriteria), line);
	}
	Ele->addBoolChildElement(CML_convert(MMP_IOSGDirectSCF), GetDirectSCF());
	Ele->addBoolChildElement(CML_convert(MMP_IOSGFockDiff), GetFockDiff());
	if (GetUHFNO()) Ele->addChildElement(CML_convert(MMP_IOSGUHFNauralOrbitals), trueXML);
	if (GetExtrapolation()) Ele->addChildElement(CML_convert(MMP_IOSGExtrap), trueXML);
	if (GetDamp()) Ele->addChildElement(CML_convert(MMP_IOSGDamp), trueXML);
	if (GetShift()) Ele->addChildElement(CML_convert(MMP_IOSGShift), trueXML);
	if (GetRestriction()) Ele->addChildElement(CML_convert(MMP_IOSGRestriction), trueXML);
	if (GetDIIS()) Ele->addChildElement(CML_convert(MMP_IOSGDIIS), trueXML);
	if (GetSOSCF()) Ele->addChildElement(CML_convert(MMP_IOSGSOSCF), trueXML);
	if (GetDEM()) Ele->addChildElement(CML_convert(MMP_IOSGDEM), trueXML);
	if (GetGVBNumCoreOrbs()>0) {
		snprintf(line, kMaxLineLength, "%d", GetGVBNumCoreOrbs());
		Ele->addChildElement(CML_convert(MMP_IOSGGVBNumCoreOrbs), line);
	}
	if (GetGVBNumPairs()>0) {
		snprintf(line, kMaxLineLength, "%d", GetGVBNumPairs());
		Ele->addChildElement(CML_convert(MMP_IOSGGVBNumPairs), line);
	}
	if (GetGVBNumOpenShells()>0) {
		snprintf(line, kMaxLineLength, "%d", GetGVBNumOpenShells());
		Ele->addChildElement(CML_convert(MMP_IOSGGVBNumOpenShells), line);
		Ele->AddLongArray(GVBOpenShellDeg, CML_convert(MMP_IOSCFArrayElement), CML_convert(MMP_IOSGGVBOpenShellDeg));
	}
}
void SCFGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOSCFGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				long temp;
				switch (item) {
					case MMP_IOSGConvCriteria:
					{
						if (child->getLongValue(temp)) {
							SetConvergance(temp);
						}
					}
						break;
					case MMP_IOSGDirectSCF:
						if (child->getBoolValue(tb))
							SetDirectSCF(tb);
						break;
					case MMP_IOSGFockDiff:
						if (child->getBoolValue(tb))
							SetFockDiff(tb);
						break;
					case MMP_IOSGUHFNauralOrbitals:
						if (child->getBoolValue(tb))
							SetUHFNO(tb);
						break;
					case MMP_IOSGExtrap:
						if (child->getBoolValue(tb))
							SetExtrapolation(tb);
						break;
					case MMP_IOSGDamp:
						if (child->getBoolValue(tb))
							SetDamp(tb);
						break;
					case MMP_IOSGShift:
						if (child->getBoolValue(tb))
							SetShift(tb);
						break;
					case MMP_IOSGRestriction:
						if (child->getBoolValue(tb))
							SetRestriction(tb);
						break;
					case MMP_IOSGDIIS:
						if (child->getBoolValue(tb))
							SetDIIS(tb);
						break;
					case MMP_IOSGSOSCF:
						if (child->getBoolValue(tb))
							SetSOSCF(tb);
						break;
					case MMP_IOSGDEM:
						if (child->getBoolValue(tb))
							SetDEM(tb);
						break;
					case MMP_IOSGGVBNumCoreOrbs:
						if (child->getLongValue(temp))
							SetGVBNumCoreOrbs(temp);
						break;
					case MMP_IOSGGVBNumPairs:
						if (child->getLongValue(temp))
							SetGVBNumPairs(temp);
						break;
					case MMP_IOSGGVBNumOpenShells:
						if (child->getLongValue(temp))
							SetGVBNumOpenShells(temp);
						break;
					case MMP_IOSCFArrayElement:
						//Check the title attribute, but there should be only one array
						CML_convert(child->getAttributeValue(CML_convert(titleAttr)),item);
						if (item == MMP_IOSGGVBOpenShellDeg) {
							GVBOpenShellDeg.clear();
							//This implies that this item must follow the NumOpenShells item.
							child->getLongArray(GVBNumOpenShells, GVBOpenShellDeg);
						}
						break;
					default:
					{
						wxString msg;
						msg.Printf(_("Skipping unknown XML Element: %s"), child->getName());
						wxLogMessage(msg);
					}
				}
			}
		}
		delete children;
	}
}
void SCFGroup::SetGVBNumOpenShells(const long &no) {
	if (no >= 0) {
		GVBNumOpenShells = no;
		for (int i=GVBOpenShellDeg.size(); i<GVBNumOpenShells; i++) {
			GVBOpenShellDeg.push_back(0);	//The GAMESS default is 0, maybe 1 is a better default?
		}
	}
}
void SCFGroup::SetGVBNODegValue(int index, long value) {
	if ((index >= 0)&&(value >= 0)&&(index < GVBOpenShellDeg.size())) {
		GVBOpenShellDeg[index] = value;
	}
}
#pragma mark MP2Group
MP2Group::MP2Group(void) {
	InitData();
}
MP2Group::MP2Group(MP2Group *Copy) {
	*this = *Copy;
}
void MP2Group::InitData(void) {
	CutOff = 0.0;
	NumCoreElectrons = -1;
	Memory = 0;
	Method = AOInts = LMOMP2 = 0;
	MP2Prop = false;
}
double MP2Group::SetIntCutoff(double NewCutoff) {
	if (NewCutoff > 0.0) CutOff = NewCutoff;
	return CutOff;
}
long MP2Group::SetNumCoreElectrons(long NewNum) {
	if (NewNum>=0) NumCoreElectrons = NewNum;
	return NumCoreElectrons;
}
long MP2Group::SetMemory(long NewMem) {
	if (NewMem >= 0) Memory = NewMem;
	return Memory;
}
char MP2Group::SetMethod(char NewMethod) {
	if ((NewMethod==2)||(NewMethod==3)) Method = NewMethod;
	return Method;
}
const char * MP2Group::GetAOIntMethodText(void) const {
	if (AOInts == 0) return NULL;
	if (AOInts == 1) return "DUP";
	return "DIST";
}
void MP2Group::SetAOIntMethod(const char * t) {
	if (!t) return;
	if (!strcasecmp(t, "DUP")) AOInts = 1;
	else if (!strcasecmp(t, "DIST")) AOInts = 2;
}
char MP2Group::SetAOIntMethod(char NewMethod) {
	if ((NewMethod == 1)||(NewMethod == 2)) AOInts = NewMethod;
	return AOInts;
}
bool MP2Group::GetLMOMP2(void) const {
	if (LMOMP2) return true;
	return false;
}
bool MP2Group::SetLMOMP2(bool State) {
	if (State) LMOMP2 = true;
	else LMOMP2 = false;
	return LMOMP2;
}
void MP2Group::WriteToFile(BufferFile *File, InputData *IData) {
	long	test=false;
	char	Out[133];

		//first determine wether or not the MP2 group needs to be punched
	if (IData->Control->GetMPLevel() != 2) return;	//Don't punch if MP2 isn't active
	if ((NumCoreElectrons>=0)||Memory||Method||AOInts) test = true;
	if (GetLMOMP2()) test = true;
	if (CutOff > 0.0) test = true;
	
	if (!test) return;

		//Punch the group label
	File->WriteLine(" $MP2 ", false);
		//core electrons
	if (NumCoreElectrons >= 0) {
		sprintf(Out,"NACORE=%ld ", NumCoreElectrons);
		File->WriteLine(Out, false);
		if (IData->Control->GetSCFType() == GAMESS_UHF) {
			sprintf(Out,"NBCORE=%ld ", NumCoreElectrons);
			File->WriteLine(Out, false);
		}
	}
	//MP2Prop
	if ((IData->Control->GetRunType() <= Energy) && GetMP2Prop()) {
		sprintf(Out, "MP2PRP=.TRUE. ");
		File->WriteLine(Out, false);
	}
		//LMOMP2
	if (GetLMOMP2()) {
		sprintf(Out, "LMOMP2=.TRUE. ");
		File->WriteLine(Out, false);
	}	//Memory
	if (Memory) {
		sprintf(Out, "NWORD=%ld ",Memory);
		File->WriteLine(Out, false);
	}	//CutOff
	if (CutOff > 0.0) {
		sprintf(Out, "CUTOFF=%.2e ", CutOff);
		File->WriteLine(Out, false);
	}	//Method
	if (Method) {
		sprintf(Out, "METHOD=%d ", Method);
		File->WriteLine(Out, false);
	}	//AO storage
	if (AOInts) {
		sprintf(Out, "AOINTS=%s ", GetAOIntMethodText());
		File->WriteLine(Out, false);
	}

	File->WriteLine("$END", true);
}
void MP2Group::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOMP2GroupElement));
	if (NumCoreElectrons) {
		snprintf(line, kMaxLineLength, "%ld", NumCoreElectrons);
		Ele->addChildElement(CML_convert(MMP_IOMGNumCoreElectrons), line);
	}
	if (CutOff > 0.0) {
		snprintf(line, kMaxLineLength, "%lf", CutOff);
		Ele->addChildElement(CML_convert(MMP_IOMGCutOff), line);
	}
	if (Memory) {
		snprintf(line, kMaxLineLength, "%ld", Memory);
		Ele->addChildElement(CML_convert(MMP_IOMGMemory), line);
	}
	if (Method) {
		snprintf(line, kMaxLineLength, "%d", Method);
		Ele->addChildElement(CML_convert(MMP_IOMGTransMethod), line);
	}
	if (AOInts) Ele->addChildElement(CML_convert(MMP_IOMGAOInts), GetAOIntMethodText());
	if (GetLMOMP2()) Ele->addChildElement(CML_convert(MMP_IOMGLMOMP2), trueXML);
	if (GetMP2Prop()) Ele->addChildElement(CML_convert(MMP_IOMP2MP2PRP), trueXML);
}
void MP2Group::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOMP2GroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				switch (item) {
					case MMP_IOMGNumCoreElectrons:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetNumCoreElectrons(temp);
						}
					}
						break;
					case MMP_IOMGCutOff:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetIntCutoff(temp);
						}
					}
						break;
					case MMP_IOMGMemory:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetMemory(temp);
						}
					}
						break;
					case MMP_IOMGTransMethod:
					{
						long temp;
						if (child->getLongValue(temp)) {
							Method = temp;
						}
					}
						break;
					case MMP_IOMGAOInts:
					{
						const char * v = child->getValue();
						if (v) {
							SetAOIntMethod(v);
						}
					}
						break;
					case MMP_IOMGLMOMP2:
					{
						bool tb;
						if (child->getBoolValue(tb))
							SetLMOMP2(tb);
					}
						break;
					case MMP_IOMP2MP2PRP:
					{
						bool tb;
						if (child->getBoolValue(tb))
							SetMP2Prop(tb);
					}
						break;
				}
			}
		}
		delete children;
	}
}
#pragma mark HessianGroup
void HessianGroup::InitData(void) {
	DisplacementSize = 0.01f;
	FrequencyScaleFactor = 1.0f;
	BitOptions = 17;	//bit 1 + bit 5
}
void HessianGroup::WriteToFile(BufferFile *File, InputData *IData) {
	Boolean	method=false;
	char	Out[133];

		//Only output a force group if it's relevant to the overall run options
	if (! IsHessianGroupNeeded(IData)) return;

	bool AnalyticPoss = IsAnalyticHessianPossible(IData);

	method = GetAnalyticMethod() && AnalyticPoss;
		//Punch the group label
	File->WriteLine(" $FORCE ", false);
		//Method
	if (method) File->WriteLine("METHOD=ANALYTIC ", false);
	else File->WriteLine("METHOD=SEMINUM ", false);
	if (!method) {
			//NVIB
		if (GetDoubleDiff()) {
			File->WriteLine("NVIB=2 ", false);
		}	//Vib Size
		if (DisplacementSize != 0.01) {
			sprintf(Out, "VIBSIZ=%f ", DisplacementSize);
			File->WriteLine(Out, false);
		}
	}	//Purify
	if (GetPurify()) {
		File->WriteLine("PURIFY=.TRUE. ", false);
	}	//Print internal FC's
	if (GetPrintFC()) {
		File->WriteLine("PRTIFC=.TRUE. ", false);
	}	//vib analysis
	if (GetVibAnalysis()) {
		File->WriteLine("VIBANL=.TRUE. ", false);
		if (FrequencyScaleFactor != 1.0) {
			sprintf(Out, "SCLFAC=%f ", FrequencyScaleFactor);
			File->WriteLine(Out, false);
		}
	} else File->WriteLine("VIBANL=.FALSE. ", false);

	File->WriteLine("$END", true);
}
void HessianGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOHessianGroupElement));
	if (DisplacementSize > 0.0) {
		snprintf(line, kMaxLineLength, "%f", DisplacementSize);
		Ele->addChildElement(CML_convert(MMP_IOHGDisplacementSize), line);
	}
	if (FrequencyScaleFactor > 0.0) {
		snprintf(line, kMaxLineLength, "%lf", FrequencyScaleFactor);
		Ele->addChildElement(CML_convert(MMP_IOHGFrequencyScaleFactor), line);
	}
	if (GetAnalyticMethod()) Ele->addChildElement(CML_convert(MMP_IOHGMethod), "analytic");
	else Ele->addChildElement(CML_convert(MMP_IOHGMethod), "numeric");
	if (GetPurify()) Ele->addChildElement(CML_convert(MMP_IOHGPurify), trueXML);
	if (GetPrintFC()) Ele->addChildElement(CML_convert(MMP_IOHGInternalFC), trueXML);
	if (GetVibAnalysis()) Ele->addChildElement(CML_convert(MMP_IOHGVibAnalysis), trueXML);
}
void HessianGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOHessGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOHGDisplacementSize:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetDisplacementSize(temp);
						}
					}
						break;
					case MMP_IOHGFrequencyScaleFactor:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetFreqScale(temp);
						}
					}
						break;
					case MMP_IOHGMethod:
					{
						const char * v = child->getValue();
						if (v) {
							if (!strcasecmp(v, "analytic")) SetAnalyticMethod(true);
							else if (!strcasecmp(v, "numeric")) SetAnalyticMethod(false);
						}
					}
						break;
					case MMP_IOHGPurify:
						if (child->getBoolValue(tb))
							SetPurify(tb);
						break;
					case MMP_IOHGInternalFC:
						if (child->getBoolValue(tb))
							SetPrintFC(tb);
						break;
					case MMP_IOHGVibAnalysis:
						if (child->getBoolValue(tb))
							SetVibAnalysis(tb);
						break;
				}
			}
		}
		delete children;
	}
}
bool HessianGroup::IsAnalyticHessianPossible(const InputData * IData) {
	//Check based on the SCF type
	//This is only part of the current rules. It probably covers the common cases though.
	//ToDo: Match this test up more closely with the state of GAMESS.
	bool result = (((IData->Control->GetSCFType() == 1)||(IData->Control->GetSCFType() == 3)||
					(IData->Control->GetSCFType() == 4)||(IData->Control->GetSCFType() == 0))&&
				   (IData->Control->GetMPLevel() <= 0)&&(!IData->Control->UseDFT()));
	//Analytic hessians are not available for semi-emperical basis sets
	if ((IData->Basis->GetBasis() == GAMESS_BS_MNDO)||(IData->Basis->GetBasis() == GAMESS_BS_AM1)||
		(IData->Basis->GetBasis() == GAMESS_BS_PM3)) return false;
	
	return result;
}
bool HessianGroup::IsHessianGroupNeeded(const InputData * IData) {
	bool result = false;
	//Hessians are computed with runtyp of hessian, optimize or sadpoint
	//optimize/sadpoint runs using Hess=Calc or hssend=.t.
	if ((IData->Control->GetRunType() == HessianRun)||
		(IData->Control->GetRunType() == G3MP2)) result = true;
	else if ((IData->Control->GetRunType() == OptimizeRun)||(IData->Control->GetRunType() == SadPointRun)) {
		if (IData->StatPt) {
			if (IData->StatPt->GetHessMethod() == 3) result = true;
			if (IData->StatPt->GetHessEndFlag()) result = true;
		}
	}
	return result;
}
#pragma mark DFTGroup
void DFTGroup::InitData(void) {
	GridSwitch = 3.0e-4;
	Threshold = 1.0e-4;
	Functional = 0;
/*	NumRadialGrids = 96;
	NumThetaGrids = 12;
	NumPhiGrids = 24;
	NumRadialGridsInit = 24;
	NumThetaGridsInit = 8;
	NumPhiGridsInit = 16;
 */
	BitFlags = 0;
	SetAuxFunctions(true);
	SetMethodGrid(true);
}
void DFTGroup::WriteToFile(BufferFile *File, InputData *IData) {
	char	Out[kMaxLineLength];

	short SCFType = IData->Control->GetSCFType();
		//first determine wether or not the DFT group needs to be punched
	if ((SCFType > 3)|| !IData->Control->UseDFT()) return;//only punch for HF runtypes (RHF, ROHF, UHF)
	if (MethodGrid()) return; //Only need this group for gridfree method currently
		//Punch the group label
	File->WriteLine(" $DFT ", false);
		//Write out the funtional, and any other optional parameters
		//Method
	if (!MethodGrid()) {	//punch method if it needs to be grid-free
		sprintf(Out, "METHOD=GRIDFREE ");
		File->WriteLine(Out, false);
	}

	File->WriteLine("$END", true);
}
const char * DFTGroup::GetDFTGridFuncText(DFTFunctionalsGrid type) {
	switch (type) {
		case DFT_Grid_Slater:
			return "SLATER";
		case DFT_Grid_Becke:
			return "BECKE";
		case DFT_Grid_GILL:
			return "GILL";
		case DFT_Grid_OPTX:
			return "OPTX";
		case DFT_Grid_PW91X:
			return "PW91X";
		case DFT_Grid_PBEX:
			return "PBEX";
		case DFT_Grid_VWN:
			return "VWN";
		case DFT_Grid_VWN1:
			return "VWN1";
		case DFT_Grid_PZ81:
			return "PZ81";
		case DFT_Grid_P86:
			return "P86";
		case DFT_Grid_LYP:
			return "LYP";
		case DFT_Grid_PW91C:
			return "PW91C";
		case DFT_Grid_PBEC:
			return "PBEC";
		case DFT_Grid_OP:
			return "OP";
		case DFT_Grid_SVWN:
			return "SVWN";
		case DFT_Grid_BLYP:
			return "BLYP";
		case DFT_Grid_BOP:
			return "BOP";
		case DFT_Grid_BP86:
			return "BP86";
		case DFT_Grid_GVWN:
			return "GVWN";
		case DFT_Grid_GPW91:
			return "GPW91";
		case DFT_Grid_PBEVWN:
			return "PBEVWN";
		case DFT_Grid_PBEOP:
			return "PBEOP";
		case DFT_Grid_OLYP:
			return "OLYP";
		case DFT_Grid_PW91:
			return "PW91";
		case DFT_Grid_PBE:
			return "PBE";
		case DFT_Grid_revPBE:
			return "REVPBE";
		case DFT_Grid_RPBE:
			return "RPBE";
		case DFT_Grid_PBEsol:
			return "PBESOL";
		case DFT_Grid_EDF1:
			return "EDF1";
		case DFT_Grid_HCTH93:
			return "HCTH93";
		case DFT_Grid_HCTH120:
			return "HCTH120";
		case DFT_Grid_HCTH147:
			return "HCTH147";
		case DFT_Grid_HCTH407:
			return "HCTH407";
		case DFT_Grid_SOGGA:
			return "SOGGA";
		case DFT_Grid_MOHLYP:
			return "MOHLYP";
		case DFT_Grid_B97_D:
			return "B97-D";
		case DFT_Grid_BHHLYP:
			return "BHHLYP";
		case DFT_Grid_B3PW91:
			return "B3PW91";
		case DFT_Grid_B3LYP:
			return "B3LYP";
		case DFT_Grid_B3LYP1:
			return "B3LYP1";
		case DFT_Grid_B97:
			return "B97";
		case DFT_Grid_B97_1:
			return "B97-1";
		case DFT_Grid_B97_2:
			return "B97-2";
		case DFT_Grid_B97_3:
			return "B97-3";
		case DFT_Grid_B97_K:
			return "B97-K";
		case DFT_Grid_B98:
			return "B98";
		case DFT_Grid_PBE0:
			return "PBE0";
		case DFT_Grid_X3LYP:
			return "X3LYP";
		case DFT_Grid_CAMB3LYP:
			return "CAMB3LYP";
		case DFT_Grid_wB97:
			return "WB97";
		case DFT_Grid_wB97X:
			return "WB97X";
		case DFT_Grid_wB97X_D:
			return "WB97X-D";
		case DFT_Grid_B2PLYP:
			return "B2PLYP";
		case DFT_Grid_xB97X_2:
			return "XB97X-2";
		case DFT_Grid_xB97X_2L:
			return "XB97X-2L";
		case DFT_Grid_VS98:
			return "VS98";
		case DFT_Grid_PKZB:
			return "PKZB";
		case DFT_Grid_tHCTH:
			return "tHCTH";
		case DFT_Grid_tHCTHhyb:
			return "tHCTHhyb";
		case DFT_Grid_BMK:
			return "BMK";
		case DFT_Grid_TPSS:
			return "TPSS";
		case DFT_Grid_TPSSh:
			return "TPSSh";
		case DFT_Grid_TPSSm:
			return "TPSSm";
		case DFT_Grid_revTPSS:
			return "REVTPSS";
		case DFT_Grid_M05:
			return "M05";
		case DFT_Grid_M05_2X:
			return "M05-2X";
		case DFT_Grid_M06:
			return "M06";
		case DFT_Grid_M06_L:
			return "M06-L";
		case DFT_Grid_M06_2X:
			return "M06-2X";
		case DFT_Grid_M06_HF:
			return "M06-HF";
		case DFT_Grid_M08_HX:
			return "M08-HX";
		case DFT_Grid_M08_SO:
			return "M08-SO";
	}
	return "invalid";
}
const char * DFTGroup::GetDFTGridFreeFuncText(DFTFunctionalsGridFree type) {
	switch (type) {
		case DFT_GridFree_XALPHA:
			return "XALPHA";
		case DFT_GridFree_Slater:
			return "SLATER";
		case DFT_GridFree_Becke:
			return "BECKE";
		case DFT_GridFree_Depristo:
			return "DEPRISTO";
		case DFT_GridFree_CAMA:
			return "CAMA";
		case DFT_GridFree_HALF:
			return "HALF";
		case DFT_GridFree_VWN:
			return "VWN";
		case DFT_GridFree_PWLOC:
			return "PWLOC";
		case DFT_GridFree_LYP:
			return "LYP";
		case DFT_GridFree_BVWN:
			return "BVWN";
		case DFT_GridFree_BLYP:
			return "BLYP";
		case DFT_GridFree_BPWLOC:
			return "BPWLOC";
		case DFT_GridFree_B3LYP:
			return "B3LYP";
		case DFT_GridFree_CAMB:
			return "CAMB";
		case DFT_GridFree_XVWN:
			return "XVWN";
		case DFT_GridFree_XPWLOC:
			return "XPWLOC";
		case DFT_GridFree_SVWN:
			return "SVWN";
		case DFT_GridFree_SPWLOC:
			return "SPWLOC";
		case DFT_GridFree_WIGNER:
			return "WIGNER";
		case DFT_GridFree_WS:
			return "WS";
		case DFT_GridFree_WIGEXP:
			return "WIGEXP";
	}
	return "invalid";
}

const char * DFTGroup::GetFunctionalText(void) const {
	short temp = Functional;
	if (temp <= 0) temp = 1;
	if (MethodGrid()) {
		return GetDFTGridFuncText((DFTFunctionalsGrid) temp);
	} else {	//Grid-free functional list is fairly different
		return GetDFTGridFreeFuncText((DFTFunctionalsGridFree) temp);
	}
	return NULL;
}
short DFTGroup::SetFunctional(const char * DFT_Type) {
	if (MethodGrid()) {
		for (int i=0; i<NumberGRIDDFTFuncs; i++) {
			if (!strcasecmp(DFT_Type, GetDFTGridFuncText((DFTFunctionalsGrid) i))) {
				SetFunctional((DFTFunctionalsGrid) i);
				break;
			}
		}
	} else {
		for (int i=0; i<NumberDFTGridFreeFuncs; i++) {
			if (!strcasecmp(DFT_Type, GetDFTGridFreeFuncText((DFTFunctionalsGridFree) i))) {
				SetFunctional((DFTFunctionalsGridFree) i);
				break;
			}
		}
	}
	return GetFunctional();
}
short DFTGroup::SetFunctional(short newvalue) {
	if (MethodGrid()) {
		if ((newvalue > 0)&&(newvalue < NumberGRIDDFTFuncs))
			Functional = newvalue;
	} else
		if ((newvalue > 0)&&(newvalue < NumberDFTGridFreeFuncs))
			Functional = newvalue;
	return Functional;
}
void DFTGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IODFTGroupElement));
	Ele->addBoolAttribute(CML_convert(MMP_IODFTGridMethod), MethodGrid());
	snprintf(line, kMaxLineLength, "%f", GridSwitch);
	Ele->addChildElement(CML_convert(MMP_IODFTGGridSwitch), line);
	snprintf(line, kMaxLineLength, "%f", Threshold);
	Ele->addChildElement(CML_convert(MMP_IODFTThreshold), line);
	Ele->addChildElement(CML_convert(MMP_IODFTFunctional), GetFunctionalText());
/*	snprintf(line, kMaxLineLength, "%d", NumRadialGrids);
	Ele->addChildElement(CML_convert(MMP_IODFTNumRadialGrids), line);
	snprintf(line, kMaxLineLength, "%d", NumThetaGrids);
	Ele->addChildElement(CML_convert(MMP_IODFTNumThetaGrids), line);
	snprintf(line, kMaxLineLength, "%d", NumPhiGrids);
	Ele->addChildElement(CML_convert(MMP_IODFTNumPhiGrids), line);
	snprintf(line, kMaxLineLength, "%d", NumRadialGridsInit);
	Ele->addChildElement(CML_convert(MMP_IODFTNumRadialGridsInit), line);
	snprintf(line, kMaxLineLength, "%d", NumThetaGridsInit);
	Ele->addChildElement(CML_convert(MMP_IODFTNumThetaGridsInit), line);
	snprintf(line, kMaxLineLength, "%d", NumPhiGridsInit);
	Ele->addChildElement(CML_convert(MMP_IODFTNumPhiGridsInit), line);
 */
	if (GetAuxFunctions()) Ele->addChildElement(CML_convert(MMP_IODFTGetAuxFunctions), trueXML);
	if (GetThree()) Ele->addChildElement(CML_convert(MMP_IODFTThree), trueXML);
}
void DFTGroup::ReadXML(XMLElement * parent) {
	bool boolResult;
	if (parent->getAttributeValue(CML_convert(MMP_IODFTGridMethod), boolResult))
		SetMethodGrid(boolResult);
	XMLElementList * children = parent->getChildren(); 
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IODFTGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IODFTGGridSwitch:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							GridSwitch = temp;
						}
					}
						break;
					case MMP_IODFTThreshold:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							Threshold = temp;
						}
					}
						break;
					case MMP_IODFTFunctional:
					{
						const char * v = child->getValue();
						if (v)
							SetFunctional(v);
					}
						break;
/*					case MMP_IODFTNumRadialGrids:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumRadialGrids = temp;
						}
					}
						break;
					case MMP_IODFTNumThetaGrids:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumThetaGrids = temp;
						}
					}
						break;
					case MMP_IODFTNumPhiGrids:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumPhiGrids = temp;
						}
					}
						break;
					case MMP_IODFTNumRadialGridsInit:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumRadialGridsInit = temp;
						}
					}
						break;
					case MMP_IODFTNumThetaGridsInit:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumThetaGridsInit = temp;
						}
					}
						break;
					case MMP_IODFTNumPhiGridsInit:
					{
						long temp;
						if (child->getLongValue(temp)) {
							NumPhiGridsInit = temp;
						}
					}
						break;
 */
					case MMP_IODFTGetAuxFunctions:
						if (child->getBoolValue(tb))
							SetAuxFunctions(tb);
						break;
					case MMP_IODFTThree:
						if (child->getBoolValue(tb))
							SetThree(tb);
						break;
				}
			}
		}
		delete children;
	}
}
#pragma mark EffectiveFragmentsGroup
const char * EffectiveFragmentsGroup::ConvertCoordToText(int t) {
	switch (t) {
		case 0:
			return "CART";
		case 1:
			return "INT";
	}
	return "invalid";
}
const char * EffectiveFragmentsGroup::ConvertPolMethodToText(EFRAG_PolMethods t) {
	switch (t) {
		case FRGSCF_PolMethod:
			return "FRGSCF";
		case SCF_PolMethod:
			return "SCF";
	}
	return "invalid";
}
const char * EffectiveFragmentsGroup::ConvertPositionMethodToText(EFRAG_PositionTypes t) {
	switch (t) {
		case Optimize_Position:
			return "OPTIMIZE";
		case Fixed_Position:
			return "FIXED";
		case EFOPT_Position:
			return "EFOPT";
	}
	return "invalid";
}
bool EffectiveFragmentsGroup::SetCoordinatesType(const char * tag) {
	if (!strcasecmp(tag, ConvertCoordToText(0))) {
		UseCartesianCoordinates(true);
		return true;
	} else if (!strcasecmp(tag, ConvertCoordToText(1))) {
		UseInternalCoordinates(true);
		return true;
	}
	return false;
}
bool EffectiveFragmentsGroup::SetPolMethod(const char * tag) {
	for (int i=FRGSCF_PolMethod; i<NumEFragPolMethods; i++) {
		if (!strcasecmp(tag, ConvertPolMethodToText((EFRAG_PolMethods) i))) {
			PolMethod((EFRAG_PolMethods) i);
			return true;
		}
	}
	return false;
}
bool EffectiveFragmentsGroup::SetPositionType(const char * tag) {
	for (int i=Optimize_Position; i<NumEFragPositionTypes; i++) {
		if (!strcasecmp(tag, ConvertPositionMethodToText((EFRAG_PositionTypes) i))) {
			PositionMethod((EFRAG_PositionTypes) i);
			return true;
		}
	}
	return false;
}
void EffectiveFragmentsGroup::WriteXML(XMLElement * parent) const {
	//This group is only needed if there are non-default values
	if (UseInternalCoordinates() || !PolMethodIsDefault() || !PositionIsDefault() ||
		(MaxMOs >= 0) || (NumBufferMOs >= 0) || (MaxBasisFuncs>=0)) {

		XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOEFPGroupElement));
		if (UseInternalCoordinates())
			Ele->addAttribute(CML_convert(MMP_IOEFPCoordType), GetGAMESSCoordText());
		if (!PolMethodIsDefault())
			Ele->addAttribute(CML_convert(MMP_IOEFPPolMethod), ConvertPolMethodToText(PolMethod()));
		if (!PositionIsDefault())
			Ele->addAttribute(CML_convert(MMP_IOEFPPosition), ConvertPositionMethodToText(PositionMethod()));
		if (MaxMOs >= 0)
			Ele->addAttribute(CML_convert(MMP_IOEFPMaxMOs), MaxMOs);
		if (MaxBasisFuncs >= 0)
			Ele->addAttribute(CML_convert(MMP_IOEFPMaxBasisFuncs), MaxBasisFuncs);
		if (NumBufferMOs >= 0)
			Ele->addAttribute(CML_convert(MMP_IOEFPNumBufferMOs), NumBufferMOs);
	}
}
void EffectiveFragmentsGroup::ReadXML(XMLElement * parent) {
	const char * u = parent->getAttributeValue(CML_convert(MMP_IOEFPCoordType));
	if (u) {
		SetCoordinatesType(u);
	}
	u = parent->getAttributeValue(CML_convert(MMP_IOEFPPolMethod));
	if (u) {
		SetPolMethod(u);
	}
	u = parent->getAttributeValue(CML_convert(MMP_IOEFPPosition));
	if (u) {
		SetPositionType(u);
	}
	parent->getAttributeValue(CML_convert(MMP_IOEFPMaxMOs), MaxMOs);
	parent->getAttributeValue(CML_convert(MMP_IOEFPMaxBasisFuncs), MaxBasisFuncs);
	parent->getAttributeValue(CML_convert(MMP_IOEFPNumBufferMOs), NumBufferMOs);
}

#pragma mark FMOGroup
void FMOGroup::WriteToFile(BufferFile *File, MoleculeData * MainData) {
	char	Out[kMaxLineLength];
	
	if (!IsFMOActive()) return;
	
	//Punch the group label
	File->WriteLine("!!!! Warning: FMO input is incomplete - you must complete by hand.", true);
	File->WriteLine(" $FMO ", false);

	sprintf(Out, "NFRAG=%ld ", GetNumberFragments());
	File->WriteLine(Out, false);
	
	//INDAT array
	MainData->WriteINDAT(File);
	
	File->WriteLine(" $END", true);
}
void MoleculeData::WriteINDAT(BufferFile * Buffer) {
	if (FMOFragmentIds.size() > 0) {
		char	Out[kMaxLineLength];
		Buffer->WriteLine("INDAT(1)=", false);
		if (InputOptions->FMO.UseFragmentINDAT()) { //group by fragment
			for (long ifrag=1; ifrag<=InputOptions->FMO.GetNumberFragments(); ifrag++) {
				Buffer->WriteLine(" 0,",true);
				Buffer->WriteLine("        ", false);
				for (long iatom=0; iatom<FMOFragmentIds.size(); iatom++) {
					if (FMOFragmentIds[iatom] == ifrag) {
						sprintf(Out, " %ld,", iatom+1);
						Buffer->WriteLine(Out, false);
						//Now see how many atoms in a row are assigned to the same fragment
						long jatom=0;
						for (; (jatom+iatom)<FMOFragmentIds.size(); jatom++) {
							if (FMOFragmentIds[iatom+jatom+1] != ifrag) break;
						}
						if (jatom>1) {
							sprintf(Out, " -%ld,", iatom+jatom+1);
							Buffer->WriteLine(Out, false);
							iatom+=jatom;
						}
					}
				}
			}
			Buffer->WriteLine(" 0",true);
		} else {	//ordered by atom number
			for (long iatom=0; iatom<(FMOFragmentIds.size() - 1); iatom++) {
				sprintf(Out, " %ld,", FMOFragmentIds[iatom]);
				Buffer->WriteLine(Out, false);
			}
			sprintf(Out, " %ld", FMOFragmentIds[FMOFragmentIds.size()-1]);
			Buffer->WriteLine(Out, true);
		}
	}
}
void FMOGroup::WriteXML(XMLElement * parent, MoleculeData * MainData) const {
	//This group is only needed if there are non-default values
	if (IsFMOActive() || (GetNumberFragments()>1)) {
		
		XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOFMOGroupElement));
		Ele->addBoolAttribute(CML_convert(MMP_IOFMOActiveFlag), IsFMOActive());
		Ele->addBoolAttribute(CML_convert(MMP_IOFMOOutputStyleFlag), UseFragmentINDAT());
		char line[kMaxLineLength];
		snprintf(line, kMaxLineLength, "%ld", GetNumberFragments());
		Ele->addChildElement(CML_convert(MMP_IOFMONumFragments), line);
	//	snprintf(line, kMaxLineLength, "%ld", Newfunction());
	//	Ele->addChildElement(CML_convert(MMP_IOFMONBODY), line);
		
		MainData->WriteFMOIdsToXML(Ele);
	}
}
void FMOGroup::ReadXML(XMLElement * parent, MoleculeData * MainData) {
	bool tb;
	if (parent->getAttributeValue(CML_convert(MMP_IOFMOActiveFlag), tb)) FMOActive(tb);
	if (parent->getAttributeValue(CML_convert(MMP_IOFMOOutputStyleFlag), tb)) UseFragmentINDAT(tb);
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOFMOGroupNS item;
			CML_Element cmlitem;
			if (child && CML_convert(child->getName(), item)) {
				switch (item) {
					case MMP_IOFMONumFragments:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetNumberFragments(temp);
						}
					}
						break;
				}
			} else if (child && CML_convert(child->getName(), cmlitem)) {
				MainData->ReadFMOIdsFromXML(child);
			}
		}
		delete children;
	}
}
void MoleculeData::WriteFMOIdsToXML(XMLElement * parent) {
	if (FMOFragmentIds.size() > 0) {
		std::ostringstream buf;
		for (unsigned int i=0; i<FMOFragmentIds.size(); i++) buf << FMOFragmentIds[i] << " ";
		XMLElement * map = parent->addChildElement(CML_convert(ArrayElement),
												   buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_IOFMOFramentArray));
		map->addAttribute(CML_convert(sizeAttr), FMOFragmentIds.size());
	}
}
void MoleculeData::ReadFMOIdsFromXML(XMLElement * item) {
	const char * attr = item->getAttributeValue(CML_convert(titleAttr));
	MMP_IOFMOGroupNS title;
	if (CML_convert(attr, title)) {
		if (title == MMP_IOFMOFramentArray) {
			long size;
			if (item->getAttributeValue(CML_convert(sizeAttr), size)) {
				FMOFragmentIds.reserve(size);
				int nchar, pos=0;
				const char * val = item->getValue();
				for (int i=0; i<size; i++) {
					long is;
					if (sscanf(&(val[pos]), "%ld%n", &is, &nchar) != 1) throw DataError();
					FMOFragmentIds.push_back(is);
					pos += nchar;
				}
			}
		}
	}
}

#pragma mark StatPtGroup
void StatPtGroup::InitData(void) {
	OptConvergance = 0.0001f;
	InitTrustRadius = 0.0f;
	MaxTrustRadius = 0.0f;
	MinTrustRadius = 0.05f;
	StatJumpSize = 0.01f;
	ModeFollow = 1;
	BitOptions = 0;
	method = 3;
	MaxSteps = 20;
	nRecalcHess = 0;
	SetRadiusUpdate(true);
}
const char * StatPtGroup::GetMethodText(OptMethod type) {
	switch (type) {
		case StatPt_OptMethod_NR:
			return "NR";
		case StatPt_OptMethod_RFO:
			return "RFO";
		case StatPt_OptMethod_QA:
			return "QA";
		case StatPt_OptMethod_Schlegel:
			return "SCHLEGEL";
		case StatPt_OptMethod_ConOpt:
			return "CONOPT";
	}
	return "invalid";
}
bool StatPtGroup::SetMethod(const char * text) {
	for (int i=StatPt_OptMethod_NR; i<NumberStatPtOptMethods; i++) {
		if (!strcasecmp(text, GetMethodText((OptMethod) i))) {
			SetMethod(i);
			return true;
		}
	}
	return false;
}
const char * StatPtGroup::GetHessUpdateMethodText(HessUpdateMethod type) {
	switch (type) {
		case StatPt_HessUpdateMethod_Guess:
			return "GUESS";
		case StatPt_HessUpdateMethod_Read:
			return "READ";
		case StatPt_HessUpdateMethod_Calc:
			return "CALC";
	}
	return "invalid";
}
bool StatPtGroup::SetHessMethod(const char * text) {
	for (int i=StatPt_HessUpdateMethod_Guess; i<NumberStatPtHessUpdateMethods; i++) {
		if (!strcasecmp(text, GetHessUpdateMethodText((HessUpdateMethod) i))) {
			SetHessMethod(i);
			return true;
		}
	}
	return false;
}
void StatPtGroup::WriteToFile(BufferFile *File, InputData *IData) {
	char	Out[133];

	short runType = IData->Control->GetRunType();
		//first determine wether or not the statpt group needs to be punched
	if ((runType != OptimizeRun)&&(runType != SadPointRun)&&
		(runType != G3MP2)) return;	//only punch for optimize and sadpoint runs

		//Punch the group label
	File->WriteLine(" $STATPT ", false);
		//write out the convergance criteria and number of steps by default, just
		//to remind the user of their values. Everything else is optional.
	sprintf(Out, "OPTTOL=%g ", GetOptConvergance());
	File->WriteLine(Out, false);
	sprintf(Out, "NSTEP=%d ", GetMaxSteps());
	File->WriteLine(Out, false);
		//Method
	if (GetMethod() != 3) {
		sprintf(Out, "Method=%s ", GetMethodText((OptMethod) GetMethod()));
		File->WriteLine(Out, false);
	}	//DXMAX if non-default and method is not NR
	if ((GetInitRadius() != 0.0)&&(GetMethod() !=1)) {
		sprintf(Out, "DXMAX=%g ", GetInitRadius());
		File->WriteLine(Out, false);
	}
	if ((GetMethod()==2)||(GetMethod()==3)) {
		if (!GetRadiusUpdate()) File->WriteLine("TRUPD=.FALSE. ", false);
		if (GetMaxRadius() != 0.0) {
			sprintf(Out, "TRMAX=%g ", GetMaxRadius());
			File->WriteLine(Out, false);
		}
		if (fabs(GetMinRadius() - 0.05)>1e-5) {
			sprintf(Out, "TRMIN=%g ", GetMinRadius());
			File->WriteLine(Out, false);
		}
	}
	if ((runType == 6)&&(GetModeFollow() != 1)) {
		sprintf(Out, "IFOLOW=%ld ", GetModeFollow());
		File->WriteLine(Out, false);
	}
	if (GetStatPoint()) {
		File->WriteLine("STPT=.TRUE. ", false);
		if (fabs(GetStatJump() - 0.01)>1e-5) {
			sprintf(Out, "STSTEP=%g ", GetStatJump());
			File->WriteLine(Out, false);
		}
	}
	if (GetHessMethod()) {
		sprintf(Out, "HESS=%s ", GetHessUpdateMethodText((HessUpdateMethod) GetHessMethod()));
		File->WriteLine(Out, false);
	}
	if (GetHessRecalcInterval()) {
		sprintf(Out, "IHREP=%d ", GetHessRecalcInterval());
		File->WriteLine(Out, false);
	}
	if (AlwaysPrintOrbs()) {
		File->WriteLine("NPRT=1 ", false);
	}
	if (GetHessEndFlag())
		File->WriteLine("HSSEND=.t. ", false);

	File->WriteLine("$END", true);
	//When HESS=READ emit a warning so the user knows they must manually add the $HESS group
	if ((GetHessMethod() == StatPt_HessUpdateMethod_Read)||
		((GetHessMethod()==0)&&(runType == SadPointRun))) {
		MessageAlert("You must manually add a $HESS group to the input file!");
		File->WriteLine("! A $HESS group must be manually added to this file!", true);
	}
}
void StatPtGroup::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * Ele = parent->addChildElement(CML_convert(MMP_IOStatPtGroupElement));
	snprintf(line, kMaxLineLength, "%f", OptConvergance);
	Ele->addChildElement(CML_convert(MMP_IOSPGOptConvergance), line);
	snprintf(line, kMaxLineLength, "%f", InitTrustRadius);
	Ele->addChildElement(CML_convert(MMP_IOSPGInitTrustRadius), line);
	snprintf(line, kMaxLineLength, "%f", MaxTrustRadius);
	Ele->addChildElement(CML_convert(MMP_IOSPGMaxTrustRadius), line);
	snprintf(line, kMaxLineLength, "%f", MinTrustRadius);
	Ele->addChildElement(CML_convert(MMP_IOSPGMinTrustRadius), line);
	snprintf(line, kMaxLineLength, "%f", StatJumpSize);
	Ele->addChildElement(CML_convert(MMP_IOSPGStatJumpSize), line);
	snprintf(line, kMaxLineLength, "%ld", ModeFollow);
	Ele->addChildElement(CML_convert(MMP_IOSPGModeFollow), line);
	if (GetRadiusUpdate()) Ele->addChildElement(CML_convert(MMP_IOSPGRadiusUpdate), trueXML);
	if (GetStatPoint()) Ele->addChildElement(CML_convert(MMP_IOSPGStatPoint), trueXML);
	snprintf(line, kMaxLineLength, "%d", GetHessMethod());
	Ele->addChildElement(CML_convert(MMP_IOSPGHessMethod), line);
	snprintf(line, kMaxLineLength, "%d", method);
	Ele->addChildElement(CML_convert(MMP_IOSPGMethod), line);
	snprintf(line, kMaxLineLength, "%d", MaxSteps);
	Ele->addChildElement(CML_convert(MMP_IOSPGMaxSteps), line);
	snprintf(line, kMaxLineLength, "%d", nRecalcHess);
	Ele->addChildElement(CML_convert(MMP_IOSPGnRecalcHess), line);
	if (AlwaysPrintOrbs()) Ele->addChildElement(CML_convert(MMP_IOSPGAlwaysPrintOrbs), trueXML);
	if (GetHessEndFlag()) Ele->addChildElement(CML_convert(MMP_IOSPGHessEnd), trueXML);
}
void StatPtGroup::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			MMP_IOStatPtGroupNS item;
			if (child && CML_convert(child->getName(), item)) {
				bool tb;
				switch (item) {
					case MMP_IOSPGOptConvergance:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetOptConvergance(temp);
						}
					}
						break;
					case MMP_IOSPGInitTrustRadius:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetInitRadius(temp);
						}
					}
						break;
					case MMP_IOSPGMaxTrustRadius:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetMaxRadius(temp);
						}
					}
						break;
					case MMP_IOSPGMinTrustRadius:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetMinRadius(temp);
						}
					}
						break;
					case MMP_IOSPGStatJumpSize:
					{
						double temp;
						if (child->getDoubleValue(temp)) {
							SetStatJump(temp);
						}
					}
						break;
					case MMP_IOSPGModeFollow:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetModeFollow(temp);
						}
					}
						break;
					case MMP_IOSPGRadiusUpdate:
						if (child->getBoolValue(tb))
							SetRadiusUpdate(tb);
						break;
					case MMP_IOSPGStatPoint:
						if (child->getBoolValue(tb))
							SetStatPoint(tb);
						break;
					case MMP_IOSPGHessMethod:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetHessMethod(temp);
						}
					}
						break;
					case MMP_IOSPGMethod:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetMethod(temp);
						}
					}
						break;
					case MMP_IOSPGMaxSteps:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetMaxSteps(temp);
						}
					}
						break;
					case MMP_IOSPGnRecalcHess:
					{
						long temp;
						if (child->getLongValue(temp)) {
							SetHessRecalcInterval(temp);
						}
					}
						break;
					case MMP_IOSPGAlwaysPrintOrbs:
						if (child->getBoolValue(tb))
							SetAlwaysPrintOrbs(tb);
						break;
					case MMP_IOSPGHessEnd:
						if (child->getBoolValue(tb))
							SetHessFlag(tb);
						break;
				}
			}
		}
		delete children;
	}
}

long MoleculeData::GetNumElectrons(void) const {
	long result=cFrame->GetNumElectrons();
	if (InputOptions && InputOptions->Control) result -= InputOptions->Control->GetCharge();
	return result;
}
short MoleculeData::GetMultiplicity(void) const {
	short result=1;
	if (InputOptions && InputOptions->Control) result = InputOptions->Control->GetMultiplicity();
	return result;
}
InputData * MoleculeData::GetInputData(void) {
	if (!InputOptions) InputOptions = new InputData;
	if (!InputOptions) throw MemoryError();
	return InputOptions;
}
InputData * MoleculeData::SetInputData(InputData * NewData) {
	if (InputOptions) delete InputOptions;
	InputOptions = new InputData(NewData);
	return InputOptions;
}
void MoleculeData::WriteInputFile(MolDisplayWin * owner) {
	if (InputOptions) {
		InputOptions->WriteInputFile(this, owner);
	}
}
void MOPacInternals::WriteZMATToFile(BufferFile * File) {
	char	Out[133];

	File->WriteLine(" $ZMAT IZMAT(1)=", false);
	for (long i=3; i<Count; i+=3) {
		if (i>9) File->WriteLine(", ", false);
		sprintf(Out, "1,%ld,%ld, ", (i+3)/3, ConnectionAtoms[i]+1);
		File->WriteLine(Out, false);
		if (i>3) {
			sprintf(Out, "2,%ld,%ld,%ld, ", (i+3)/3, ConnectionAtoms[i]+1, ConnectionAtoms[i+1]+1);
			File->WriteLine(Out, false);
			if (i>6) {
				sprintf(Out, "3,%ld,%ld,%ld,%ld", (i+3)/3, ConnectionAtoms[i]+1,
					ConnectionAtoms[i+1]+1, ConnectionAtoms[i+2]+1);
				File->WriteLine(Out, false);
			}
		}
	}
	File->WriteLine(" $END", true);
}
void MOPacInternals::WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) {
	UpdateAtoms(MainData);	//First make sure the connectivity and values are up to date
	CartesiansToInternals(MainData);
	float unitConversion = 1.0f;
	InputData * lOptions = MainData->GetInputData();
	if (lOptions && lOptions->Data->GetUnits()) unitConversion = kAng2BohrConversion;
		char	Out[133];
		Str255	AtomLabel;
		Frame *	cFrame = MainData->GetCurrentFramePtr();

	for (int iatom=0; iatom<cFrame->NumAtoms; iatom++) {
		Prefs->GetAtomLabel(cFrame->Atoms[iatom].GetType()-1, AtomLabel);
		AtomLabel[AtomLabel[0]+1] = 0;
		if (iatom==0) sprintf(Out, "%s", (char *) &(AtomLabel[1]));
		else if (iatom == 1)
			sprintf(Out, "%s  %ld %10.5f", (char *) &(AtomLabel[1]),
				ConnectionAtoms[3*iatom]+1, Values[3*iatom]*unitConversion);
		else if (iatom == 2)
			sprintf(Out, "%s   %ld %10.5f  %ld %8.4f",
				(char *) &(AtomLabel[1]), ConnectionAtoms[3*iatom]+1, Values[3*iatom]*unitConversion, 
				ConnectionAtoms[3*iatom+1]+1, Values[3*iatom+1]);
		else
			sprintf(Out, "%s   %ld %10.5f  %ld %8.4f  %ld %8.4f",
				(char *) &(AtomLabel[1]), ConnectionAtoms[3*iatom]+1, Values[3*iatom]*unitConversion, 
				ConnectionAtoms[3*iatom+1]+1, Values[3*iatom+1],
				ConnectionAtoms[3*iatom+2]+1, Values[3*iatom+2]);
		File->WriteLine(Out, true);
	}
}
//This if very similar to the prevous function, but the format is a little different
void MOPacInternals::WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) {
	UpdateAtoms(MainData);	//First make sure the connectivity and values are up to date
	CartesiansToInternals(MainData);
	float unitConversion = 1.0f;
	InputData * lOptions = MainData->GetInputData();
	if (lOptions && lOptions->Data->GetUnits()) unitConversion = kAng2BohrConversion;
	char	Out[133];
	Str255	AtomLabel;
	Frame *	cFrame = MainData->GetCurrentFramePtr();
	
	for (int iatom=0; iatom<cFrame->NumAtoms; iatom++) {
		Prefs->GetAtomLabel(cFrame->Atoms[iatom].GetType()-1, AtomLabel);
		AtomLabel[AtomLabel[0]+1] = 0;
		if (iatom==0) sprintf(Out, "%s", (char *) &(AtomLabel[1]));
		else if (iatom == 1)
			sprintf(Out, "%s   %10.5f", (char *) &(AtomLabel[1]),
					Values[3*iatom]*unitConversion);
		else if (iatom == 2)
			sprintf(Out, "%s   %10.5f 0 %8.4f 0 %ld %ld",
					(char *) &(AtomLabel[1]), Values[3*iatom]*unitConversion, 
					Values[3*iatom+1], ConnectionAtoms[3*iatom]+1, ConnectionAtoms[3*iatom+1]+1);
		else
			sprintf(Out, "%s   %10.5f 0 %8.4f 0 %8.4f 0 %ld %ld %ld",
					(char *) &(AtomLabel[1]), Values[3*iatom]*unitConversion, Values[3*iatom+1],
					Values[3*iatom+2], ConnectionAtoms[3*iatom]+1,
					ConnectionAtoms[3*iatom+1]+1, ConnectionAtoms[3*iatom+2]+1);
		File->WriteLine(Out, true);
	}
}
void OrbitalRec::WriteVecGroup(BufferFile * File, const long & NumBasisFuncs, const long & OrbCount) const {
//First check for and write out the vec label
	if (Label) {	//The label should include any necessary linefeeds
		File->WriteLine(Label, true);
	}
//Punch the Group title
	File->WriteLine(" $VEC", true);
//write out the vectors using the GAMESS format (I2,I3,5E15.8)
#ifdef _MSC_VER
	//Visual studio defaults to using 3 digit exponent output which doesn't fit in the field
	//This function changes that behavior to 2 digits.
	_set_output_format(_TWO_DIGIT_EXPONENT);
#endif
		long iline, nVec, nn, nOrbs, pOrb;
	if ((OrbCount > 0) && (OrbCount <= NumAlphaOrbs)) nOrbs = OrbCount;
	else nOrbs = NumAlphaOrbs;
		char	Line[kMaxLineLength];
		float * Vector=Vectors;
	for (int ipass=0; ipass<2; ipass++) {
		if (!Vector) {
			File->WriteLine("Error in Vectors request!", true);
			return;
		}
		nn = 0;	pOrb = 0;
		for (long i=0; i<nOrbs; i++) {
			iline = 1;	nVec = 0;
			pOrb++;
			if (pOrb>=100) pOrb -= 100;
			sprintf(Line, "%2ld%3ld", pOrb, iline);
			File->WriteLine(Line, false);
			for (long ivec=0; ivec<NumBasisFuncs; ivec++) {
				sprintf(Line, "%15.8E", Vector[nn]);
				nn++;
				File->WriteLine(Line, false);
				nVec++;
				if ((nVec>=5)&&(ivec+1<NumBasisFuncs)) {//wrap line and start the next line
					File->WriteLine("", true);
					iline ++;
					sprintf(Line, "%2ld%3ld", pOrb, iline);
					File->WriteLine(Line, false);
					nVec = 0;
				}
			}
			File->WriteLine("", true);
		}
		if (BaseWavefunction == UHF) {	//Repeat for beta set of orbitals for UHF wavefunctions
			Vector = VectorsB;
			if ((OrbCount > 0) && (OrbCount <= NumBetaOrbs)) nOrbs = OrbCount;
			else nOrbs = NumBetaOrbs;
		} else ipass++;
	}
//finish off the group
	File->WriteLine(" $END", true);
}
