/*
 *  (c) 2005 Iowa State University
 *      see the LICENSE file in the top level directory
 */
/* CML file parsing routines

	4/20/2003 - First try at CML parsing -- Troy
	1/2005	- Redone to use custom XML handling and CML 1 and 2 support -- Brett Bode
*/

#include "Globals.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BFiles.h"
#include "myFiles.h"
#include "Progress.h"
#include "Prefs.h"
#include "Gradient.h"
#include "BasisSet.h"
#include "GlobalExceptions.h"

#include <string>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <stdio.h>

#include "XML.hpp"
#include "CML.h"
#include "Internals.h"
#include "InputData.h"
#include "Math3D.h"
#include "MolDisplayWin.h"
#include "build_palette.h"
#if defined(WIN32)
#undef AddAtom
#endif

//notes on extending CML
//anything can go inside the CML tag.
//Molecule can be extended via scalar, array, matrix, list and metadata
//There appear to be types for atomic basis function, gradient and symmetry, but
//not sure they do what I need.

// --------------------------------------------------------------------------- 

long StructureGroup::WriteCMLFile(void) const {

	FILE * save_file = fopen(filename.mb_str(wxConvUTF8), "wb");
	if (save_file == NULL) {
		MessageAlert("Unable to access the file.");
		return 0;
	}
	
	BufferFile * Buffer = new BufferFile(save_file, true);
	
	XMLSetup();
	XMLDocument *xDoc = new XMLDocument(CML_convert(CMLElement), true, "CML uri");
	XMLElement *xmlRoot = xDoc->getDocumentRoot();
	xmlRoot->addAttribute("xmlns", "http://www.xml-cml.org/schema/schema24/schema.xsd");

	XMLElement *MetaDataListXML = xmlRoot->addChildElement(CML_convert(MetaDataListElement));
	XMLElement *el = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
	el->addAttribute(CML_convert(nameAttr), CML_convert(MMP_Structures));

	std::vector<Structure *>::const_iterator struc;
	for (struc = structures.begin(); struc != structures.end(); ++struc) {
		(*struc)->WriteXML(el);
	}

	std::ostringstream CMLtext;
	CMLtext << *xDoc;
	delete xDoc;
	XMLShutdown();
	Buffer->Write(CMLtext.str().c_str(), CMLtext.str().length()); 

	if (Buffer) {
		delete Buffer;
	}
	fclose(save_file);
	return CMLtext.str().length();
}

// --------------------------------------------------------------------------- 

long StructureGroup::ReadCMLFile(void) {

	// Open file of new structures.  If the file couldn't be opened, we
	// don't do anything.  (There used to be an error message here, but
	// there's a strong chance that the file might not exist, which is
	// a perfectly legal thing.)
	FILE * load_file = fopen(filename.mb_str(wxConvUTF8), "rb");
	if (load_file == NULL) {
		return false;
	}
	
	BufferFile *Buffer = NULL;
	try {
		Buffer = new BufferFile(load_file, false);
	} catch (std::bad_alloc) {
		MessageAlert("Insufficient memory to read in the file.");
	} catch (MemoryError) {
		MessageAlert("Insufficient memory to read in the file.");
	} catch (DataError) {
		MessageAlert("Invalid data encountered while reading the file.");
	} catch (FileError) {
		MessageAlert("File System error, read aborted.");
	}
	
	short errors = 0;
	long fsize = Buffer->GetFileSize();

	if (!fsize) {
		return 0;
	}
	char *xmlbuffer = new char[fsize + 1];

	Buffer->SetFilePos(0);
	Buffer->Read(xmlbuffer, fsize);
	XMLSetup();
	XMLDocument *xDoc = NULL;

	try {
		xDoc = new XMLDocument(xmlbuffer, fsize, true);
		if (xDoc->parse()) {
			XMLElement *root = xDoc->getDocumentRoot();
			CML_Element rootname;
			if (CML_convert(root->getName(), rootname)) {

				if (rootname != CMLElement) {
					throw 1;
				}

				XMLElement *child = root->getFirstChild();
				CML_Element elName;
				CML_convert(child->getName(), elName);

				if (elName != MetaDataListElement) {
					throw 1;
				}

				XMLElement * mdchild = child->getFirstChild();
				if (mdchild != NULL) {
					const char *name = mdchild->getAttributeValue(CML_convert(nameAttr));
					if (name) {
						MMP_MetadataNamespace attr;
						CML_convert(name, attr);
						if (attr != MMP_Structures) {
							throw 1;
						}

						XMLElement *struc_el = mdchild->getFirstChild();
						Structure *struc;
						while (struc_el) {
							if (struc_el->isName(kStructureXML)) {
								struc = new Structure;
								if (struc->ReadXML(struc_el)) {
									structures.push_back(struc);
								} else {
									delete struc;
								}
							}
							struc_el = struc_el->getNextChild();
						}
					}
				}

			} else {
				MessageAlert("Unable to parse XML.");
				errors++;
			}
		} else {
			errors++;
			MessageAlert("An error occurred in parsing the structures file.");
		}
		delete xDoc;
	} catch (...) {
		if (xDoc) delete xDoc;
		MessageAlert("XML Exception");
		errors++;
	}

	XMLShutdown();
	delete Buffer;
	fclose(load_file);

	return errors;

}

// --------------------------------------------------------------------------- 

long MoleculeData::WriteCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData *wData, 
								bool allFrames, bool AllData) {

	PruneUnusedFragments();

	XMLSetup();
		//The CML root is optional in the spec, but I will include here so that
		//I can wrapper multiple Molecule elements to represent different frames
	XMLDocument * xDoc = new XMLDocument(CML_convert(CMLElement), true, "CML uri");
	XMLElement * xmlRoot = xDoc->getDocumentRoot();
	xmlRoot->addAttribute("xmlns", "http://www.xml-cml.org/schema/cml2/core");
	if (Description != NULL)
		xmlRoot->addAttribute(CML_convert(titleAttr), Description);
	if (AllData) {	//write out non-CML base items
		//Need: IntCoords, Basis, CurrentFrame, WindowSize, MaxSize, RotationMatrix,
		//		InputOptions, DrawMode/DrawLabels flags, Additional window related info.
		{
			//Add rotation matrix
			std::ostringstream rotbuf;
			Matrix4D tmp;
			CopyMatrix(TotalRotation, tmp);
			tmp[3][0] = Centroid.x;
			tmp[3][1] = Centroid.y;
			tmp[3][2] = Centroid.z;
			for (int i=0; i<4; i++) {
				for (int j=0; j<4; j++) {
					if ((i>0)||(j>0))
						rotbuf << " ";
					rotbuf << tmp[j][i]; 
				}
			}
			XMLElement * rotmat = xmlRoot->addChildElement(CML_convert(MatrixElement),
														 rotbuf.str().c_str());
			rotmat->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
			rotmat->addAttribute(CML_convert(titleAttr), CML_convert(MMP_RotationMatrix));
			rotmat->addAttribute(CML_convert(columnsAttr), "4");
			rotmat->addAttribute(CML_convert(rowsAttr), "4");
		}
		XMLElement * MetaDataListXML = xmlRoot->addChildElement(CML_convert(MetaDataListElement));
		{	// MaxSize
			std::ostringstream rbuf;
			rbuf << MaxSize;
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_MaxSize));
			Ele->addAttribute(CML_convert(contentAttr), rbuf.str().c_str());
		}
		{	// Window Size
			std::ostringstream rbuf;
			rbuf << WindowSize;
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_WindowSize));
			Ele->addAttribute(CML_convert(contentAttr), rbuf.str().c_str());
		}
		{	// Current frame
			std::ostringstream rbuf;
			rbuf << CurrentFrame;
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_CurrentFrame));
			Ele->addAttribute(CML_convert(contentAttr), rbuf.str().c_str());
		}
		if (GetDrawMode()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_DrawNormalMode));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		if (GetSpecialAtomDrawMode()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_DrawSpecialAtoms));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		if (ShowAxis()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_ShowAxis));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		if (Prefs->ShowAtomicSymbolLabels()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_ShowAtomLabels));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		if (DrawHLabels()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_LabelHAtoms));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		if (Prefs->ShowAtomNumberLabels()) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_ShowAtomNumbers));
			Ele->addAttribute(CML_convert(contentAttr), "true");
		}
		//IntCoords
		if (IntCoords) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_InternalCoordinates));
			IntCoords->WriteXML(Ele);
		}
		//BasisSet
		if (Basis) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_BasisSet));
			Basis->WriteXML(Ele);
		}
		//List of Effective Fragment names
		if (FragmentNames.size() > 0) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_FragmentNameList));
			for (unsigned int i=0; i<FragmentNames.size(); i++) {
				/*XMLElement * FN = */ Ele->addChildElement(kFRAGNAMEXML, FragmentNames[i].c_str());
			}
		}

		// List of effective fragment definitions.
		if (FragmentNames.size() > 0) {
			XMLElement *Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_FragmentDefinitions));
			std::map<std::string, EFrag>::const_iterator efrag;
			for (efrag = efrags.begin(); efrag != efrags.end(); ++efrag) {
				XMLElement *FN = Ele->addChildElement(kFragmentDefXML, efrag->second.GetText().c_str());
				FN->addAttribute(CML_convert(nameAttr), efrag->first.c_str());
			}
		}

		//InputOptions
		if (InputOptions) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_InputOptions));
			InputOptions->WriteXML(Ele, this);
		}
		if (Annotations.size() > 0) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_Annotations));
			std::vector<Annotation *>::const_iterator anno;
			anno = Annotations.begin();
			for (anno = Annotations.begin(); anno != Annotations.end(); ) {
				(*anno)->WriteXML(Ele);
				anno++;
			}
		}
		//Prefs
		if (Prefs) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_Preferences));
			Prefs->WriteMMPPrefs(Ele);
		}
		//Window information, open windows and default window rects
		if (wData) {
			XMLElement * Ele = MetaDataListXML->addChildElement(CML_convert(MetaDataElement));
			Ele->addAttribute(CML_convert(nameAttr), CML_convert(MMP_WindowData));
			wData->WriteXML(Ele);
		}
	}
	if (allFrames) {
		Frame * lFrame;
		lFrame = Frames;
		while (lFrame != NULL) {
			lFrame->WriteCMLFrame(xmlRoot, AllData);
			lFrame = lFrame->NextFrame;
		}
	} else {
		cFrame->WriteCMLFrame(xmlRoot, AllData);
	}
	std::ostringstream CMLtext;
	CMLtext << *xDoc;
	delete xDoc;
	XMLShutdown();
	Buffer->Write(CMLtext.str().c_str(), CMLtext.str().length()); 
	return CMLtext.str().length();
}

void WindowData::WriteXML(XMLElement * parent) const {
	XMLElement * t = parent->addChildElement(CML_convert(MMP_WinDataMolWin));
	t->addwxRectAttribute(MolWinRect);
	if (BondsVis || (BondsWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataBondsWin));
		t->addwxRectAttribute(BondsWinRect);
		t->addBoolAttribute(VISIBLE_XML, BondsVis);
	}
	if (CoordsVis || (CoordsWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataCoordsWin));
		t->addwxRectAttribute(CoordsWinRect);
		t->addBoolAttribute(VISIBLE_XML, CoordsVis);
	}
	if (EnergyVis || (EnergyWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataEnergyWin));
		t->addwxRectAttribute(EnergyWinRect);
		t->addBoolAttribute(VISIBLE_XML, EnergyVis);
	}
	if (FreqVis || (FreqWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataFreqWin));
		t->addwxRectAttribute(FreqWinRect);
		t->addBoolAttribute(VISIBLE_XML, FreqVis);
	}
	if (SurfacesVis || (SurfacesWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataSurfacesWin));
		t->addwxRectAttribute(SurfacesWinRect);
		t->addBoolAttribute(VISIBLE_XML, SurfacesVis);
	}
	if (InputBVis || (InputBuilderRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataInputBWin));
		t->addwxRectAttribute(InputBuilderRect);
		t->addBoolAttribute(VISIBLE_XML, InputBVis);
	}
	if (PrefVis || (PreferenceWinRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataPrefWin));
		t->addwxRectAttribute(PreferenceWinRect);
		t->addBoolAttribute(VISIBLE_XML, PrefVis);
	}
	if (ZMatVis || (ZMatRect.x >= 0)) {
		t = parent->addChildElement(CML_convert(MMP_WinDataZMatWin));
		t->addwxRectAttribute(ZMatRect);
		t->addBoolAttribute(VISIBLE_XML, ZMatVis);
	}
}

void BasisSet::WriteXML(XMLElement * parent) const {
	XMLElement * t = parent->addChildElement(CML_convert(MMP_BasisSetElement));
	t->addAttribute(CML_convert(MMP_BSMapLength), MapLength);
	t->addAttribute(CML_convert(MMP_BSNumShells), NumShells);
	t->addAttribute(CML_convert(MMP_BSNumFunctions), NumFuncs);
	for (unsigned int i=0; i<Shells.size(); i++) {
		Shells[i].WriteXML(t);
	}
	if (BasisMap.size()) {
		std::ostringstream buf;
		for (unsigned int i=0; i<BasisMap.size(); i++) buf << BasisMap[i] << " ";
		XMLElement * map = t->addChildElement(CML_convert(ArrayElement),
														buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BSMap));
		map->addAttribute(CML_convert(sizeAttr), BasisMap.size());
	}
	if (NuclearCharge.size() && goodCharges) {
		std::ostringstream buf;
		for (unsigned int i=0; i<NuclearCharge.size(); i++) buf << NuclearCharge[i] << " ";
		XMLElement * map = t->addChildElement(CML_convert(ArrayElement),
											  buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BSNucCharge));
		map->addAttribute(CML_convert(sizeAttr), NuclearCharge.size());
	}
}
void BasisShell::WriteXML(XMLElement * parent) const {
	XMLElement * t = parent->addChildElement(CML_convert(MMP_BasisShell));
	t->addAttribute(CML_convert(MMP_BSShellType), ShellType);
	t->addAttribute(CML_convert(MMP_BSNumPrims), NumPrims);
	if (Exponent.size()) {
		std::ostringstream buf;
		for (unsigned int i=0; i<Exponent.size(); i++) buf << Exponent[i] << " ";
		XMLElement * map = t->addChildElement(CML_convert(ArrayElement),
											  buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BSExponent));
		map->addAttribute(CML_convert(sizeAttr), Exponent.size());
	}
	if (NormCoef.size()) {
		std::ostringstream buf;
		for (unsigned int i=0; i<NormCoef.size(); i++) buf << NormCoef[i] << " ";
		XMLElement * map = t->addChildElement(CML_convert(ArrayElement),
											  buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BSNormCoef));
		map->addAttribute(CML_convert(sizeAttr), NormCoef.size());
	}
	if (InputCoef.size()) {
		std::ostringstream buf;
		for (unsigned int i=0; i<InputCoef.size(); i++) buf << InputCoef[i] << " ";
		XMLElement * map = t->addChildElement(CML_convert(ArrayElement),
											  buf.str().c_str());
		map->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		map->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BSInputCoef));
		map->addAttribute(CML_convert(sizeAttr), InputCoef.size());
	}
}

long Frame::WriteCMLFrame(XMLElement * parent, bool AllData) {
	XMLElement * molElement = parent->addChildElement(CML_convert(MoleculeElement));
	XMLElement * AArrayElement = molElement->addChildElement(CML_convert(AtomArrayElement));
	char line[kMaxLineLength];
	bool	AtomAttributes=false;
	for (int i=0; i<NumAtoms; i++) {
			//output CML2 style atom information of the form:
			//<atom id="a1" elementType="C" x3="-0.0167" y3="1.3781" z3="0.0096"/>
		XMLElement * atomEle = AArrayElement->addChildElement(CML_convert(AtomElement));
			//id's are used to reference the atoms later for bonding, I will generate
			//simple ids 'a' + index number (the CML spec discourages the use of all numeric ids)
		snprintf(line, kMaxLineLength, "a%d", i);
		atomEle->addAttribute(CML_convert(IdAttr), line);
		atomEle->addAttribute(CML_convert(elementTypeAttr), CML_TypetoName(Atoms[i].GetType()));
		snprintf(line, kMaxLineLength, "%f", Atoms[i].Position.x);
		atomEle->addAttribute(CML_convert(X3Attr), line);
		snprintf(line, kMaxLineLength, "%f", Atoms[i].Position.y);
		atomEle->addAttribute(CML_convert(Y3Attr), line);
		snprintf(line, kMaxLineLength, "%f", Atoms[i].Position.z);
		atomEle->addAttribute(CML_convert(Z3Attr), line);
		if (Atoms[i].GetSelectState() || Atoms[i].IsSIMOMMAtom() || Atoms[i].IsEffectiveFragment()
			|| Atoms[i].GetInvisibility() || Atoms[i].IsSymmetryUnique() ||
			Atoms[i].GetCoordinationNumber() || Atoms[i].GetLonePairCount())
			AtomAttributes = true;
	}
	bool	NonCMLBonds = false;
	//Add the bonds next
	if (NumBonds > 0) {
		XMLElement * bondArrayEle = molElement->addChildElement(CML_convert(BondArrayElement));
		for (int j=0; j<NumBonds; j++) {
				//<bond id="b1" atomRefs2="a1 a2" order="S"/>
				//CML only specifies single, double, triple, and aromatic bonds
				//Need to handle hydrogen bonds as an extension
			if ((Bonds[j].Order == kHydrogenBond)||(Bonds[j].Order > kAromaticBond)) {
				NonCMLBonds = true;
				continue;
			}
			XMLElement * bondEle = bondArrayEle->addChildElement(CML_convert(BondElement));
			snprintf(line, kMaxLineLength, "b%d", j);
			bondEle->addAttribute(CML_convert(IdAttr), line);
			snprintf(line, kMaxLineLength, "a%ld a%ld", Bonds[j].Atom1, Bonds[j].Atom2);
			bondEle->addAttribute(CML_convert(atomRefs2Attr), line);
			bondEle->addAttribute(CML_convert(orderAttr), CML_convert(Bonds[j].Order));
		}
	}
	if (AllData) { //Add in the extra stuff that CML doesn't appear to support yet
					//These are non-standard XML extensions to CML
		XMLElement * listElement = NULL;
		if ((Orbs.size() > 0)||(Energy != 0.0)||(Energies.size() > 0) ||
			(time != 0.0)||(IRCPt != 0)||Gradient||Vibs||SurfaceList||NonCMLBonds||
			 AtomAttributes) {
			listElement = molElement->addChildElement(CML_convert(ListElement));
		}
		if (AtomAttributes) {
			//Store non-standard attributes of atoms such as selection, effective fragment id, SIMOMM type
			XMLElement * AArrayElement = listElement->addChildElement(CML_convert(MMP_AtomAttrArrayElement));
			for (int i=0; i<NumAtoms; i++) {
				if (Atoms[i].GetSelectState() || Atoms[i].IsSIMOMMAtom() || Atoms[i].IsEffectiveFragment()
					|| Atoms[i].GetInvisibility() || Atoms[i].IsSymmetryUnique() ||
					Atoms[i].GetCoordinationNumber() || Atoms[i].GetLonePairCount()) {
					XMLElement * atomEle = AArrayElement->addChildElement(CML_convert(AtomElement));
					snprintf(line, kMaxLineLength, "a%d", i);
					atomEle->addAttribute(CML_convert(IdAttr), line);
					if (Atoms[i].GetSelectState())
						atomEle->addAttribute(CML_convert(selectedAttr), "true");
					if (Atoms[i].IsSIMOMMAtom())
						atomEle->addAttribute(CML_convert(SIMOMMFlagAttr), "true");
					if (Atoms[i].GetInvisibility())
						atomEle->addAttribute(CML_convert(invisibleAttr), "true");
					if (Atoms[i].IsSymmetryUnique())
						atomEle->addAttribute(CML_convert(symmetryUniqueAttr), "true");
					if (Atoms[i].IsEffectiveFragment()) {
						snprintf(line, kMaxLineLength, "%ld", Atoms[i].GetFragmentNumber());
						atomEle->addAttribute(CML_convert(effectiveFragmentIdAttr), line);
					}
					if (Atoms[i].GetCoordinationNumber()) {
						snprintf(line, kMaxLineLength, "%d", Atoms[i].GetCoordinationNumber());
						atomEle->addAttribute(CML_convert(coordinationNumberAttr), line);
					}
					if (Atoms[i].GetLonePairCount()) {
						snprintf(line, kMaxLineLength, "%d", Atoms[i].GetLonePairCount());
						atomEle->addAttribute(CML_convert(lonePairCountAttr), line);
					}
				}
			}
			
		}
		if (NonCMLBonds) {
			XMLElement * bondArrayEle = listElement->addChildElement(CML_convert(MMP_BondArrayElement));
			for (int j=0; j<NumBonds; j++) {
				//<bond id="b1" atomRefs2="a1 a2" order="S"/>
				//CML only specifies single, double, triple, and aromatic bonds
				//Need to handle hydrogen bonds as an extension
				if ((Bonds[j].Order >= kSingleBond)&&(Bonds[j].Order <= kTripleBond)) {
					continue;
				}
				XMLElement * bondEle = bondArrayEle->addChildElement(CML_convert(BondElement));
				snprintf(line, kMaxLineLength, "b%d", j);
				bondEle->addAttribute(CML_convert(IdAttr), line);
				snprintf(line, kMaxLineLength, "a%ld a%ld", Bonds[j].Atom1, Bonds[j].Atom2);
				bondEle->addAttribute(CML_convert(atomRefs2Attr), line);
				bondEle->addAttribute(CML_convert(orderAttr), CML_convert(Bonds[j].Order));
			}
		}
		//items: gradient, SpecialAtoms, KE, Energy, MP2Energy, IRCPt, time, Vibs,
		//       MOVectors, SurfaceList
		if (Energy != 0.0) {
			std::ostringstream rbuf;
			rbuf.setf(std::ios::scientific, std::ios::floatfield);
			rbuf.precision(9);
			rbuf <<Energy;
			XMLElement * Ele = listElement->addChildElement(CML_convert(ScalarElement),
														  rbuf.str().c_str());
			Ele->addAttribute(CML_convert(titleAttr), CML_convert(MMP_Energy));
		}
		std::vector<EnergyValue>::const_iterator it = Energies.begin();
		while (it != Energies.end()) {
			std::ostringstream rbuf;
			rbuf.clear();
			rbuf.setf(std::ios::scientific, std::ios::floatfield);
			rbuf.precision(9);
			rbuf << (*it).value;
			XMLElement * Ele = listElement->addChildElement(CML_convert(ScalarElement),
															rbuf.str().c_str());
			MMP_NameSpace tp;
			switch ((*it).type) {
				case PT2Energy:
					tp = MMP_MP2Energy;
					break;
				case KineticEnergy:
					tp = MMP_KineticEnergy;
					break;
				default:
					tp = MMP_Energy;
			}
			Ele->addAttribute(CML_convert(titleAttr), CML_convert(tp));
			++it;
		}
		if (time != 0.0) {
			std::ostringstream rbuf;
			rbuf << time;
			XMLElement * Ele = listElement->addChildElement(CML_convert(ScalarElement),
														   rbuf.str().c_str());
			Ele->addAttribute(CML_convert(titleAttr), CML_convert(MMP_Time));
		}
		if (IRCPt != 0) {
			std::ostringstream rbuf;
			rbuf << IRCPt;
			XMLElement * Ele = listElement->addChildElement(CML_convert(ScalarElement),
														   rbuf.str().c_str());
			Ele->addAttribute(CML_convert(titleAttr), CML_convert(MMP_ReactionCoord));
		}
		if (SpecialAtoms != NULL) {
/*			std::ostringstream atmbuf;
			char line[kMaxLineLength];
			for (int i=0; i<(NumModes*NumAtoms); i++) {
				if (i>0) atmbuf << " ";
				atmbuf << NormMode[i].x << " " 
					<< NormMode[i].y << " "
					<< NormMode[i].z;
			}
			XMLElement * modes = parent->addChildElement(CML_convert(MatrixElement),
														 atmbuf.str().c_str());
			modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
			modes->addAttribute(CML_convert(titleAttr), "Vibrational Frequencies");
			modes->addAttribute(CML_convert(columnsAttr), "3");
			snprintf(line, kMaxLineLength, "%d", (NumModes*NumAtoms));
			modes->addAttribute(CML_convert(rowsAttr), line);
			*/
		}
		if (Gradient) Gradient->WriteXML(listElement);
		if (Vibs) Vibs->WriteXML(listElement, NumAtoms);
		//MOVectors  put this in a list element
		if (Orbs.size() > 0) {
			std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs.begin();
			while (OrbSet != Orbs.end()) {
				(*OrbSet)->WriteXML(listElement);
				OrbSet++;
			}
		}
		//SurfaceList
		if (SurfaceList) {
			Surface * surf = SurfaceList;
			while (surf != NULL) {
				surf->WriteXML(listElement);
				surf = surf->GetNextSurface();
			}
		}
	}
	return 1;
}

void OrbitalRec::WriteXML(XMLElement * parent) const {
	char line[kMaxLineLength];
	XMLElement * orbElem = parent->addChildElement(CML_convert(MMP_OrbitalSet));
	orbElem->addAttribute(CML_convert(MMP_OrbitalTypeAttr), ConvertTypeOfOrbital(OrbitalType));
	orbElem->addAttribute(CML_convert(MMP_WavefunctionType), ConvertTypeOfWavefunction(BaseWavefunction));
	if (Label) orbElem->addChildElement(CML_convert(MMP_OrbitalLabel), Label);
	if (NumAlphaOrbs) {
		std::ostringstream rbuf;
		rbuf << NumAlphaOrbs;
		orbElem->addAttribute(CML_convert(MMP_NumAlphaOrbs), rbuf.str().c_str());
	}
	if (NumBetaOrbs) {
		std::ostringstream bbuf;
		bbuf << NumBetaOrbs;
		orbElem->addAttribute(CML_convert(MMP_NumBetaOrbs), bbuf.str().c_str());
	}
	if (NumOccupiedAlphaOrbs) {
		std::ostringstream rbuf;
		rbuf << NumOccupiedAlphaOrbs;
		orbElem->addAttribute(CML_convert(MMP_NumOccAlphaOrbs), rbuf.str().c_str());
	}
	if (NumOccupiedBetaOrbs) {
		std::ostringstream bbuf;
		bbuf << NumOccupiedBetaOrbs;
		orbElem->addAttribute(CML_convert(MMP_NumOccBetaOrbs), bbuf.str().c_str());
	}
	if (NumBasisFunctions) {
		std::ostringstream bbuf;
		bbuf << NumBasisFunctions;
		orbElem->addAttribute(CML_convert(MMP_NumBasisFunctions), bbuf.str().c_str());
	}
	if (Vectors) {
		std::ostringstream fbuf;
		for (int i=0; i<NumAlphaOrbs; i++) {
			for (int j=0; j<NumBasisFunctions; j++) {
				fbuf << Vectors[(i*NumBasisFunctions)+j] << " ";
			}
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(MatrixElement),
													 fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_OrbVectors));
		snprintf(line, kMaxLineLength, "%ld", NumAlphaOrbs);
		vectors->addAttribute(CML_convert(columnsAttr), line);
		snprintf(line, kMaxLineLength, "%ld", NumBasisFunctions);
		vectors->addAttribute(CML_convert(rowsAttr), line);
	}
	if (VectorsB) {
		std::ostringstream fbuf;
		for (int i=0; i<NumBetaOrbs; i++) {
			for (int j=0; j<NumBasisFunctions; j++) {
				fbuf << VectorsB[(i*NumBasisFunctions)+j] << " ";
			}
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(MatrixElement),
														fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BetaOrbVectors));
		snprintf(line, kMaxLineLength, "%ld", NumBetaOrbs);
		vectors->addAttribute(CML_convert(columnsAttr), line);
		snprintf(line, kMaxLineLength, "%ld", NumBasisFunctions);
		vectors->addAttribute(CML_convert(rowsAttr), line);
	}
	if (Energy) {
		std::ostringstream fbuf;
		for (int i=0; i<NumAlphaOrbs; i++) {
			fbuf << Energy[i] << " ";
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement),
														fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_OrbEnergies));
		snprintf(line, kMaxLineLength, "%ld", NumAlphaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
	if (EnergyB) {
		std::ostringstream fbuf;
		for (int i=0; i<NumBetaOrbs; i++) {
			fbuf << EnergyB[i] << " ";
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement),
														fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BetaOrbEnergies));
		snprintf(line, kMaxLineLength, "%ld", NumBetaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
	if (OrbOccupation) {
		std::ostringstream fbuf;
		for (int i=0; i<NumAlphaOrbs; i++) {
			fbuf << OrbOccupation[i] << " ";
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement),
														fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_OrbOccupations));
		snprintf(line, kMaxLineLength, "%ld", NumAlphaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
	if (OrbOccupationB) {
		std::ostringstream fbuf;
		for (int i=0; i<NumBetaOrbs; i++) {
			fbuf << OrbOccupationB[i] << " " ;
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement),
														fbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BetaOrbOccupations));
		snprintf(line, kMaxLineLength, "%ld", NumBetaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
	if (SymType) {
		std::ostringstream sbuf;
		long pos = 0;
		for (int i=0; i<NumAlphaOrbs; i++) {
			pos = 5*i;
			while (SymType[pos] != '\0') {
				sbuf << SymType[pos];
				pos++;
				if (pos >= (5*(i+1))) break;
			}
			sbuf << " ";
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement), sbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:string"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_OrbSyms));
		snprintf(line, kMaxLineLength, "%ld", NumAlphaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
	if (SymTypeB) {
		std::ostringstream sbuf;
		long pos = 0;
		for (int i=0; i<NumBetaOrbs; i++) {
			pos = 5*i;
			while (SymTypeB[pos] != '\0') {
				sbuf << SymTypeB[pos];
				pos++;
				if (pos >= (5*(i+1))) break;
			}
			sbuf << " ";
		}
		XMLElement * vectors = orbElem->addChildElement(CML_convert(ArrayElement), sbuf.str().c_str());
		vectors->addAttribute(CML_convert(dataTypeAttr), "xsd:string"); //required for the matrix XML element
		vectors->addAttribute(CML_convert(titleAttr), CML_convert(MMP_BetaOrbSyms));
		snprintf(line, kMaxLineLength, "%ld", NumBetaOrbs);
		vectors->addAttribute(CML_convert(sizeAttr), line);
	}
}

void VibRec::WriteXML(XMLElement * parent, long NumAtoms) const {
	//ok the main vibrational information can clearly be stored in matrices
	//Not sure where to put the labels or the CurrentMode
	XMLElement * vibElem = parent->addChildElement(CML_convert(MMP_NormalModes));
	vibElem->addAttribute(CML_convert(MMP_NormalModesCount), NumModes);
	std::ostringstream rbuf;
	rbuf << CurrentMode;
	XMLElement * Ele = vibElem->addChildElement(CML_convert(ScalarElement),
												  rbuf.str().c_str());
	Ele->addAttribute(CML_convert(titleAttr), CML_convert(MMP_CurrentMode));
	//Build a space separated string of the vibrational frequencies
	std::ostringstream fbuf;
	for (unsigned int i=0; i<Frequencies.size(); i++) {
		if (i>0) fbuf << "|";
		fbuf << Frequencies[i];
	}
	XMLElement * freqXML = vibElem->addChildElement(CML_convert(ArrayElement),
												 fbuf.str().c_str());
	char line[kMaxLineLength];
	freqXML->addAttribute(CML_convert(dataTypeAttr), "xsd:string");
	freqXML->addAttribute(CML_convert(titleAttr), CML_convert(MMP_VibFrequencies));
	snprintf(line, kMaxLineLength, "%ld", NumModes);
	freqXML->addAttribute(CML_convert(sizeAttr), line);
		//Add Normal Modes as a matrix
	std::ostringstream freqbuf;
	for (int i=0; i<(NumModes*NumAtoms); i++) {
		if (i>0) freqbuf << " ";
		freqbuf << NormMode[i].x << " " 
			<< NormMode[i].y << " "
			<< NormMode[i].z;
	}
	XMLElement * modes = vibElem->addChildElement(CML_convert(MatrixElement),
												freqbuf.str().c_str());
	modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
	modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_VibNormalModes));
	modes->addAttribute(CML_convert(columnsAttr), "3");
	snprintf(line, kMaxLineLength, "%ld", (NumModes*NumAtoms));
	modes->addAttribute(CML_convert(rowsAttr), line);
	
	if (!Symmetry.empty()) {
		std::ostringstream freqbuf;
		char line[kMaxLineLength];
		for (int i=0; i<NumModes; i++) {
			if (i>0) freqbuf << " ";
			freqbuf << Symmetry[i];
		}
		XMLElement * modes = vibElem->addChildElement(CML_convert(ArrayElement),
													  freqbuf.str().c_str());
		modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_Symmetry));
		snprintf(line, kMaxLineLength, "%ld", NumModes);
		modes->addAttribute(CML_convert(sizeAttr), line);
	}
	if (!Intensities.empty()) {
		std::ostringstream freqbuf;
		char line[kMaxLineLength];
		for (int i=0; i<NumModes; i++) {
			if (i>0) freqbuf << " ";
			freqbuf << Intensities[i];
		}
		XMLElement * modes = vibElem->addChildElement(CML_convert(ArrayElement),
													 freqbuf.str().c_str());
		modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_VibIntensity));
		snprintf(line, kMaxLineLength, "%ld", NumModes);
		modes->addAttribute(CML_convert(sizeAttr), line);
	}
	if (!ReducedMass.empty()) {
		std::ostringstream freqbuf;
		char line[kMaxLineLength];
		for (int i=0; i<NumModes; i++) {
			if (i>0) freqbuf << " ";
			freqbuf << ReducedMass[i];
		}
		XMLElement * modes = vibElem->addChildElement(CML_convert(ArrayElement),
													 freqbuf.str().c_str());
		modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_ReducedMass));
		snprintf(line, kMaxLineLength, "%ld", NumModes);
		modes->addAttribute(CML_convert(sizeAttr), line);
	}
	if (!RamanIntensity.empty()) {
		std::ostringstream freqbuf;
		char line[kMaxLineLength];
		for (int i=0; i<NumModes; i++) {
			if (i>0) freqbuf << " ";
			freqbuf << RamanIntensity[i];
		}
		XMLElement * modes = vibElem->addChildElement(CML_convert(ArrayElement),
													 freqbuf.str().c_str());
		modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_RamanIntensity));
		snprintf(line, kMaxLineLength, "%ld", NumModes);
		modes->addAttribute(CML_convert(sizeAttr), line);
	}
	if (!Depolarization.empty()) {
		std::ostringstream freqbuf;
		char line[kMaxLineLength];
		for (int i=0; i<NumModes; i++) {
			if (i>0) freqbuf << " ";
			freqbuf << Depolarization[i];
		}
		XMLElement * modes = vibElem->addChildElement(CML_convert(ArrayElement),
													 freqbuf.str().c_str());
		modes->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		modes->addAttribute(CML_convert(titleAttr), CML_convert(MMP_Depolarization));
		snprintf(line, kMaxLineLength, "%ld", NumModes);
		modes->addAttribute(CML_convert(sizeAttr), line);
	}
}
void Internals::WriteXML(XMLElement * parent) const {
	if (MOPacStyle) {
		MOPacStyle->WriteXML(parent);
	}
}
void MOPacInternals::WriteXML(XMLElement * parent) const {
		//count, ConnectionAtoms, Values, Type
	XMLElement * t = parent->addChildElement(CML_convert(MMP_MOPacStyleInternals));
	t->addAttribute(CML_convert(MMP_MOPacStyleCount), Count);
	if (ConnectionAtoms) {
		std::ostringstream buf;
		for (int i=0; i<Count; i++) buf << ConnectionAtoms[i] << " ";
		XMLElement * con = t->addChildElement(CML_convert(ArrayElement), buf.str().c_str());
		con->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");
		con->addAttribute(CML_convert(titleAttr), CML_convert(MMP_MOPacStyleConnections));
		con->addAttribute(CML_convert(sizeAttr), Count);
	}
	if (Values) {
		std::ostringstream buf;
		for (int i=0; i<Count; i++) buf << Values[i] << " ";
		XMLElement * con = t->addChildElement(CML_convert(ArrayElement), buf.str().c_str());
		con->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");
		con->addAttribute(CML_convert(titleAttr), CML_convert(MMP_MOPacStyleValues));
		con->addAttribute(CML_convert(sizeAttr), Count);
	}
	if (Type) {
		std::ostringstream buf;
		for (int i=0; i<Count; i++) buf << (int)Type[i] << " ";
		XMLElement * con = t->addChildElement(CML_convert(ArrayElement), buf.str().c_str());
		con->addAttribute(CML_convert(dataTypeAttr), "xsd:char");
		con->addAttribute(CML_convert(titleAttr), CML_convert(MMP_MOPacStyleTypes));
		con->addAttribute(CML_convert(sizeAttr), Count);
	}
}

void GradientData::WriteXML(XMLElement * parent) {
	XMLElement * gradElem = parent->addChildElement(CML_convert(MMP_Gradient));
	if (RMS > 0.0) {
		std::ostringstream rbuf;
		rbuf << RMS;
		XMLElement * rmsEle = gradElem->addChildElement(CML_convert(ScalarElement),
														  rbuf.str().c_str());
		rmsEle->addAttribute(CML_convert(titleAttr), CML_convert(MMP_RMSGradient));
	}
	if (Maximum > 0.0) {
		std::ostringstream rbuf;
		rbuf << Maximum;
		XMLElement * maxEle = gradElem->addChildElement(CML_convert(ScalarElement),
														  rbuf.str().c_str());
		maxEle->addAttribute(CML_convert(titleAttr), CML_convert(MMP_MaxGradient));
	}
	if (CartAllocation) {
		//Add as a matrix
		std::ostringstream gradbuf;
		char line[kMaxLineLength];
		for (int i=0; i<CartAllocation; i++) {
			if (i>0) gradbuf << " ";
			gradbuf << CartesianGradient[i].x << " " 
				<< CartesianGradient[i].y << " "
				<< CartesianGradient[i].z;
		}
		XMLElement * grad = gradElem->addChildElement(CML_convert(MatrixElement),
														gradbuf.str().c_str());
		grad->addAttribute(CML_convert(dataTypeAttr), "xsd:decimal");//required for the matrix XML element
		grad->addAttribute(CML_convert(titleAttr), CML_convert(MMP_CartesianGradient));
		grad->addAttribute(CML_convert(columnsAttr), "3");
		snprintf(line, kMaxLineLength, "%ld", CartAllocation);
		grad->addAttribute(CML_convert(rowsAttr), line);
	}
}

void Structure::WriteXML(XMLElement *parent) const {

	int i;
	char line[kMaxLineLength];

	XMLElement *struc_el = parent->addChildElement(kStructureXML);

	struc_el->addAttribute(CML_convert(nameAttr),
						   std::string(name.ToAscii()).c_str());
	
	if (FragName.size() > 0) {
		struc_el->addAttribute(kFRAGNAMEXML, FragName.c_str());
	}

	XMLElement *frag_el;
	if (frag_def.size()) {
		frag_el = struc_el->addChildElement("FragmentDefinition", frag_def.c_str());
	}

	XMLElement *atoms_el = struc_el->addChildElement("atoms");
	atoms_el->addAttribute(CML_convert(sizeAttr), natoms);

	// Export all atoms.
	XMLElement *atom_el;
	for (i = 0; i < natoms; i++) {
		atom_el = atoms_el->addChildElement("atom");

		snprintf(line, kMaxLineLength, "a%d", i);
		atom_el->addAttribute(CML_convert(IdAttr), line);

		atom_el->addAttribute(CML_convert(elementTypeAttr),
							  CML_TypetoName(atoms[i].GetType()));

		snprintf(line, kMaxLineLength, "%f", atoms[i].Position.x);
		atom_el->addAttribute(CML_convert(X3Attr), line);
		snprintf(line, kMaxLineLength, "%f", atoms[i].Position.y);
		atom_el->addAttribute(CML_convert(Y3Attr), line);
		snprintf(line, kMaxLineLength, "%f", atoms[i].Position.z);
		atom_el->addAttribute(CML_convert(Z3Attr), line);

		snprintf(line, kMaxLineLength, "%d", atoms[i].coordinationNumber);
		atom_el->addAttribute(CML_convert(coordinationNumberAttr), line);

		snprintf(line, kMaxLineLength, "%d", atoms[i].LPCount);
		atom_el->addAttribute(CML_convert(lonePairCountAttr), line);
	}

	// Export all bonds.
	XMLElement *bonds_el = struc_el->addChildElement("bonds");
	bonds_el->addAttribute(CML_convert(sizeAttr), nbonds);

	XMLElement *bond_el;
	for (i = 0; i < nbonds; i++) {
		bond_el = bonds_el->addChildElement("bond");

		snprintf(line, kMaxLineLength, "b%d", i);
		bond_el->addAttribute(CML_convert(IdAttr), line);

		snprintf(line, kMaxLineLength, "a%ld a%ld", bonds[i].Atom1,
				 bonds[i].Atom2);
		bond_el->addAttribute(CML_convert(atomRefs2Attr), line);
		bond_el->addAttribute(CML_convert(orderAttr),
							  CML_convert(bonds[i].Order));
	}
}

void AnnotationMarker::WriteXML(XMLElement * parent) const {
	XMLElement * Elem = parent->addChildElement(kAnnotationXML);
	Elem->addAttribute(CML_convert(titleAttr), CML_convert(MMP_AnnotationMarker));
	Elem->addAttribute(kAnnAtom1XML, atoms[0]);
}

void AnnotationLength::WriteXML(XMLElement * parent) const {
	XMLElement * Elem = parent->addChildElement(kAnnotationXML);
	Elem->addAttribute(CML_convert(titleAttr), CML_convert(MMP_AnnotationLength));
	Elem->addAttribute(kAnnAtom1XML, atoms[0]);
	Elem->addAttribute(kAnnAtom2XML, atoms[1]);
}

void AnnotationAngle::WriteXML(XMLElement * parent) const {
	XMLElement * Elem = parent->addChildElement(kAnnotationXML);
	Elem->addAttribute(CML_convert(titleAttr), CML_convert(MMP_AnnotationAngle));
	Elem->addAttribute(kAnnAtom1XML, atoms[0]);
	Elem->addAttribute(kAnnAtom2XML, atoms[1]);
	Elem->addAttribute(kAnnAtom3XML, atoms[2]);
}

void AnnotationDihedral::WriteXML(XMLElement * parent) const {
	XMLElement * Elem = parent->addChildElement(kAnnotationXML);
	Elem->addAttribute(CML_convert(titleAttr), CML_convert(MMP_AnnotationDihedral));
	Elem->addAttribute(kAnnAtom1XML, atoms[0]);
	Elem->addAttribute(kAnnAtom2XML, atoms[1]);
	Elem->addAttribute(kAnnAtom3XML, atoms[2]);
	Elem->addAttribute(kAnnAtom4XML, atoms[3]);
}
XMLElement * XMLElement::AddLongArray(const std::vector<long> & array, const char * name, const char * title, long count) {
	XMLElement * result=NULL;
	long num = count;
	if (num>array.size()) num = array.size();
	if (num<0) num = array.size();
	if (num<1) return result; //If the array is empty there is nothing to add!
	std::ostringstream temp;
	for (long i=0; i< num; i++) {
		if (i != 0) temp << " ";
		if (i < array.size()) {
			temp << array[i];
		}
	}
	result = addChildElement(name, temp.str().c_str());
	result->addAttribute(CML_convert(dataTypeAttr), "xsd:integer"); //required for the array XML element
	result->addAttribute(CML_convert(titleAttr), title);
	temp.clear();
	temp << num;
	result->addAttribute(CML_convert(sizeAttr), num);
	return result;
}

#pragma mark -

/* Parse a CML1 or CML2 file */
long MoleculeData::OpenCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData * wData,
							   Progress * ProgressInd, bool readPrefs, bool UseAutoBond) {
	short errors = 0;
	long result = 0;
	long targetFrameNum = 1;

	Frame * lFrame = cFrame;
	bool firstFrame = true;
	
	wxFileOffset fsize = Buffer->GetFileSize();
	char * xmlbuffer = new char[fsize+1];
	Buffer->SetFilePos(0);
	Buffer->Read(xmlbuffer, fsize);

	XMLSetup();
	
	XMLDocument * xDoc = NULL;
	try{
		xDoc = new XMLDocument(xmlbuffer, fsize, true);
		if (ProgressInd) ProgressInd->ChangeText("Parsing CML...");
		if (xDoc->parse()) {
			XMLElement * root = xDoc->getDocumentRoot();
			//need to handle cases where <CML> and <Molecule> are both present
			CML_Element rootname;
			if (CML_convert(root->getName(), rootname)) {
				switch (rootname) {
					case CMLElement:
					{
						XMLElementList * children = root->getChildren();
						for (int i=0; i<children->length(); i++) {
							XMLElement * child = children->item(i);
							CML_Element elName;
							CML_convert(child->getName(), elName);
							if (ProgressInd) {
								ProgressInd->ChangeText("Reading CML elements...");
								if (!ProgressInd->UpdateProgress((100*i)/children->length())) { 
									delete xDoc;
									throw UserCancel();
								}
							}
							switch (elName) {
								case MoleculeElement:
									if (!firstFrame) {	//Add new frame
										lFrame = AddFrame(lFrame->NumAtoms,0);
									}
									firstFrame = false;
									lFrame->ReadCMLMolecule(child);
									if (UseAutoBond && (result < 10))	//setup bonds, if needed
										lFrame->SetBonds(Prefs, true, ProgressInd);
									break;
								case MatrixElement:
								{	//The only Matrix element we expect here is the rotation matrix
									const char * title = child->getAttributeValue(CML_convert(titleAttr));
									if (title) {
										if (!strcmp(title, CML_convert(MMP_RotationMatrix))) {
											long cols=0, rows=0;
											child->getAttributeValue(CML_convert(columnsAttr), cols);
											child->getAttributeValue(CML_convert(rowsAttr), rows);
											if ((cols==4)&&(rows==4)) {
												const char * val = child->getValue();
												if (val) {
													int pos=0, nchar;
													for (int i=0; i<4; i++) {
														for (int j=0; j<4; j++) {
															sscanf(&(val[pos]),"%f%n", &(TotalRotation[j][i]), &nchar);
															pos += nchar;
														}
													}
												}
												Centroid.x = TotalRotation[3][0];
												Centroid.y = TotalRotation[3][1];
												Centroid.z = TotalRotation[3][2];
												TotalRotation[3][0] = 0.0f;
												TotalRotation[3][1] = 0.0f;
												TotalRotation[3][2] = 0.0f;
												OrthogonalizeRotationMatrix(TotalRotation);
											}
										}
									}
								}
									break;
								case MetaDataListElement:
								{
									if (readPrefs) {
										XMLElement * mdchild = child->getFirstChild();
										while (mdchild != NULL) {
											const char * name = mdchild->getAttributeValue(CML_convert(nameAttr));
											if (name) {
												MMP_MetadataNamespace attr;
												if (CML_convert(name, attr)) {
													switch (attr) {
														case MMP_MaxSize:
														{
															float val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val)) {
																if (val > 0.0) MaxSize = val;
															}
														}
															break;
														case MMP_WindowSize:
														{
															float val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val)) {
																if (val > 0.0) WindowSize = val;
															}
														}
															break;
														case MMP_CurrentFrame:
														{
															long val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val)) {
																targetFrameNum = val;
															}
														}
															break;
														case MMP_DrawNormalMode:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																SetDrawMode(val);
														}
															break;
														case MMP_DrawSpecialAtoms:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																SetSpecialAtomDrawMode(val);
														}
															break;
														case MMP_ShowAxis:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																SetShowAxis(val);
														}
															break;
														case MMP_ShowAtomLabels:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																Prefs->ShowAtomicSymbolLabels(val);
														}
															break;
														case MMP_LabelHAtoms:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																SetHLabelMode(val);
														}
															break;
														case MMP_ShowAtomNumbers:
														{
															bool val;
															if (mdchild->getAttributeValue(CML_convert(contentAttr), val))
																Prefs->ShowAtomNumberLabels(val);
														}
															break;
														case MMP_InternalCoordinates:
														{	//There should be only one
															if (!(IntCoords)) {
																IntCoords = new Internals;
																IntCoords->ReadXML(mdchild);
															}
														}
															break;
														case MMP_BasisSet:
															if (!Basis) Basis = BasisSet::ReadXML(mdchild);
															break;
														case MMP_FragmentNameList:
														{
															XMLElement * frags = mdchild->getFirstChild();
															while (frags != NULL) {
																const char * name = frags->getName();
																if (name) {
																	if (!strcmp(name, "FRAGNAME")) {
																		FragmentNames.push_back(std::string(frags->getValue()));
																	}
																}
																frags = frags->getNextChild();
															}
														}
															break;
														case MMP_FragmentDefinitions:
															{
																XMLElement *frags = mdchild->getFirstChild();
																const char *fragname;
																while (frags != NULL) {
																	const char *name = frags->getName();
																	if (name) {
																		if (!strcmp(name, "Fragment")) {
																			fragname = frags->getAttributeValue(CML_convert(nameAttr));
																			efrags.insert(std::pair<std::string, EFrag>(fragname, EFrag(frags->getValue())));
																		}
																	}
																	frags = frags->getNextChild();
																}
															}
															break;
														case MMP_InputOptions:
														{
															if (InputOptions) delete InputOptions;
															InputOptions = new InputData;
															InputOptions->ReadXML(mdchild, this);
														}
															break;
														case MMP_Annotations:
														{
															XMLElement * AnnChild = mdchild->getFirstChild();
															while (AnnChild) {
																if (AnnChild->isName(kAnnotationXML)) {
																	const char * childtype = AnnChild->getAttributeValue(CML_convert(titleAttr));
																	if (childtype) {
																		MMP_AnnotationTypesNS attr;
																		if (CML_convert(childtype, attr)) {
																			switch (attr) {
																				case MMP_AnnotationMarker:
																				{
																					AnnotationMarker * t = new AnnotationMarker();
																					if (t->ReadXML(AnnChild))
																						Annotations.push_back(t);
																					else
																						delete t;
																				}
																					break;
																				case MMP_AnnotationLength:
																				{
																					AnnotationLength * t = new AnnotationLength();
																					if (t->ReadXML(AnnChild))
																						Annotations.push_back(t);
																					else
																						delete t;
																				}
																					break;
																				case MMP_AnnotationAngle:
																				{
																					AnnotationAngle * t = new AnnotationAngle();
																					if (t->ReadXML(AnnChild))
																						Annotations.push_back(t);
																					else
																						delete t;
																				}
																					break;
																				case MMP_AnnotationDihedral:
																				{
																					AnnotationDihedral * t = new AnnotationDihedral();
																					if (t->ReadXML(AnnChild))
																						Annotations.push_back(t);
																					else
																						delete t;
																				}
																					break;
																			}
																		}
																	}
																}
																AnnChild = AnnChild->getNextChild();
															}
														}
															break;
														case MMP_Preferences:
															if (readPrefs) {
																Prefs->ReadMMPPrefs(mdchild);
																result = 10;
															}
															break;
														case MMP_WindowData:
															if (wData) wData->ReadXML(mdchild);
															break;
													}
												}
											}
											mdchild = mdchild->getNextChild();
										}
									}
								}
									break;
							}
						}
						delete children;
						if (result <= 0) result = 1;
					}
					break;
					case MoleculeElement:
						lFrame->ReadCMLMolecule(root);
						if (UseAutoBond && (result<10))	//setup bonds, if needed
							lFrame->SetBonds(Prefs, true, ProgressInd);
						if (result <= 0) result = 1;
					break;
					default:
						MessageAlert("Unknown root element.");
						errors++;
				}
			} else {
				MessageAlert("Unable to parse XML.");
				errors++;
			}
		} else {
			errors++;
			MessageAlert("An error occured parsing cml file");
		}
		delete xDoc;
	}
    catch (UserCancel) {
		errors++;
    }
	catch (...) {
		if (xDoc != NULL) delete xDoc;
		MessageAlert("XML Exception");
		errors++;
	}
	
	XMLShutdown();
	SetCurrentFrame(targetFrameNum);
	if (errors > 0){
//		MessageAlert("A xerces error occured parseing cml file");
		result = 0;
	}
	return result;
}

void WindowData::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	bool temp;
	for (long i=0; i < children->length(); i++) {
		XMLElement * child = children->item(i);
		MMP_WindowDataNS childName;
		if (CML_convert(child->getName(), childName)) {
			switch (childName) {
				case MMP_WinDataMolWin:
					child->getwxRectAttribute(MolWinRect);
					Validate(MolWinRect);
					break;
				case MMP_WinDataBondsWin:
					child->getwxRectAttribute(BondsWinRect);
					Validate(BondsWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						BondsVis = temp;
					break;
				case MMP_WinDataCoordsWin:
					child->getwxRectAttribute(CoordsWinRect);
					Validate(CoordsWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						CoordsVis = temp;
						break;
				case MMP_WinDataEnergyWin:
					child->getwxRectAttribute(EnergyWinRect);
					Validate(EnergyWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						EnergyVis = temp;
						break;
				case MMP_WinDataFreqWin:
					child->getwxRectAttribute(FreqWinRect);
					Validate(FreqWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						FreqVis = temp;
						break;
				case MMP_WinDataSurfacesWin:
					child->getwxRectAttribute(SurfacesWinRect);
					Validate(SurfacesWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						SurfacesVis = temp;
						break;
				case MMP_WinDataInputBWin:
					child->getwxRectAttribute(InputBuilderRect);
					Validate(InputBuilderRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						InputBVis = temp;
						break;
				case MMP_WinDataPrefWin:
					child->getwxRectAttribute(PreferenceWinRect);
					Validate(PreferenceWinRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						PrefVis = temp;
						break;
				case MMP_WinDataZMatWin:
					child->getwxRectAttribute(ZMatRect);
					Validate(ZMatRect);
					if (child->getAttributeValue(VISIBLE_XML,temp))
						ZMatVis = temp;
						break;
			}
		}
	}
	delete children;
}
bool Frame::ReadCMLMolecule(XMLElement * mol) {
	//parse the contents of a Molecule element, each molecule is a different frame
	
	std::map<std::string, long> idList; //map of ids to atom index
	XMLElementList * children = mol->getChildren();
	for (long i=0; i < children->length(); i++) {
		XMLElement * child = children->item(i);
		CML_Element childname;
		if (CML_convert(child->getName(), childname)) {
			switch (childname) {
				case AtomArrayElement:	//This can contain atom defs in several different forms
					ParseAtomArray(child, idList);
					break;
				case AtomElement:	//Simple single atom element
					ParseAtomXML(child, idList);
					break;
				case BondArrayElement:
					ParseBondArrayXML(child, idList);
					break;
				case BondElement:
					ParseBondXML(child, idList);
					break;
				case ListElement:
				{
					XMLElementList * listChildren = child->getChildren();
					for (int j=0; j<listChildren->length(); j++) {
						XMLElement * listChild = listChildren->item(j);
						MMP_MolListNameSpace listChildName;
						if (CML_convert(listChild->getName(), listChildName)) {
							switch (listChildName) {
								case MMP_ScalarElement:
								{
									const char * title = listChild->getAttributeValue(CML_convert(titleAttr));
									if (title) {
										MMP_NameSpace mmpName;
										if (CML_convert(title, mmpName)) {
											switch (mmpName) {
												case MMP_Energy:
												{
													double temp;
													if (listChild->getDoubleValue(temp))
														Energy = temp;
												}
													break;
												case MMP_KineticEnergy:
												{
													double temp;
													if (listChild->getDoubleValue(temp))
														SetEnergy(temp, KineticEnergy);
												}
													break;
												case MMP_MP2Energy:
												{
													double temp;
													if (listChild->getDoubleValue(temp))
														SetMP2Energy(temp);
												}
													break;
												case MMP_Time:
												{
													double temp;
													if (listChild->getDoubleValue(temp))
														time = temp;
												}
													break;
												case MMP_ReactionCoord:
												{
													long temp;
													if (listChild->getLongValue(temp))
														IRCPt = temp;
												}
													break;
											}
										}
									}
								}
									break;
								case MMP_AtomAttrArrayElement:	//"extra" atom attributes
									ParseAtomAttributeArrayXML(listChild, idList);
									break;
								case MMP_BondArrayElement:
									ParseBondArrayXML(listChild, idList);
									break;
								case MMP_Gradient:
									if (!Gradient) Gradient = new GradientData();
									if (Gradient) Gradient->ReadXML(listChild);
									break;
								case MMP_NormalModes:
									if (!Vibs) Vibs = new VibRec;
									Vibs->ReadXML(listChild, NumAtoms);
									break;
								case MMP_OrbitalSet:
								{
									OrbitalRec * orbset = new OrbitalRec();
									if (orbset->ReadXML(listChild)) {
										Orbs.push_back(orbset);
									} else {
										delete orbset;
									}
								}
									break;
								case MMP_SurfaceDescription:
								{
									Surface * newSurf = Surface::ReadSurface(listChild);
									if (newSurf) AppendSurface(newSurf);
								}
									break;
							}
						}
					}
					delete listChildren;
				}
					break;
			}
		}
	}
	delete children;
	
	return true;
}
void Frame::ParseAtomArray(XMLElement * arrayXML, std::map<std::string, long> & idList) {
	//The atom information can be in the form of Atom subelements or (gross) string attributes.
	const char * etype = NULL, *xcoord = NULL, *ycoord = NULL, *zcoord=NULL, *idresult=NULL;
	//Parse the subelements
	XMLElementList * children = arrayXML->getChildren();
	for (int i=0; i<children->length(); i++) {
		XMLElement * child = children->item(i);
		if (child != NULL) {
			const char * nm = child->getName();
			if (nm != NULL) {
				CML_Element elem;
				if (CML_convert(nm, elem)) {
					switch (elem) {
						case AtomElement:	//Hopefully most stuff uses this path
							ParseAtomXML(child, idList);
							break;
						case StringArrayElement:
						{
							const char * attType = child->getAttributeValue(CML_convert(builtinAttr));
							if (attType) {
								CML_Attribute attr;
								if (CML_convert(attType, attr)) {
									switch (attr) {
										case elementTypeAttr:
											etype = child->getValue();
											break;
										case atomIdAttr:
											idresult = child->getValue();
											break;
									}
								}
							}
						}
							break;
						case FloatArrayElement:
						{
							const char * attType = child->getAttributeValue(CML_convert(builtinAttr));
							if (attType) {
								CML_Attribute attr;
								if (CML_convert(attType, attr)) {
									switch (attr) {
										case X3Attr:
											xcoord = child->getValue();
											break;
										case Y3Attr:
											ycoord = child->getValue();
											break;
										case Z3Attr:
											zcoord = child->getValue();
											break;
									}
								}
							}
						}
							break;
						case IntegerArrayElement:
							break;
					}
				}
			}
		}
	}
	delete children;
	
	//Look for ugly attribute based strings
	if (etype == NULL)
		etype = arrayXML->getAttributeValue(CML_convert(elementTypeAttr));
	if (etype != NULL) {
		if (xcoord == NULL)
			xcoord = arrayXML->getAttributeValue(CML_convert(X3Attr));
		if (ycoord == NULL)
			ycoord = arrayXML->getAttributeValue(CML_convert(Y3Attr));
		if (zcoord == NULL)
			zcoord = arrayXML->getAttributeValue(CML_convert(Z3Attr));
		if ((xcoord!=NULL)&&(ycoord!=NULL)&&(zcoord!=NULL)) {
			long len = strlen(xcoord);
			char * xcstr = new char[len+1];
			len = strlen(ycoord);
			char * ycstr = new char[len+1];
			len = strlen(zcoord);
			char * zcstr = new char[len+1];
			len = strlen(etype);
			char * etstr = new char[len+1];
			char * idstr=NULL;
			if (idresult == NULL)
				idresult = arrayXML->getAttributeValue("atomID");
			if (idresult != NULL) {
				len = strlen(idresult);
				idstr = new char[len+1];
			}
			
			int nchar=0;
			long xpos=0, ypos=0, zpos=0, etpos=0, idpos=0;

			bool done=false;
			long atomType=0;
			CPoint3D atomPos;
			while (!done) {
				int n1 = sscanf(&(xcoord[xpos]), "%s%n", xcstr, &nchar);
				xpos += nchar;
				int n2 = sscanf(&(ycoord[ypos]), "%s%n", ycstr, &nchar);
				ypos += nchar;
				int n3 = sscanf(&(zcoord[zpos]), "%s%n", zcstr, &nchar);
				zpos += nchar;
				int n4 = sscanf(&(etype[etpos]), "%s%n", etstr, &nchar);
				etpos += nchar;
				int n5=0;
				if (idresult != NULL) {
					n5 = sscanf(&(idresult[idpos]), "%s%n", idstr, &nchar);
					idpos += nchar;
				}
				if ((n1!=1)||(n2!=1)||(n3!=1)||(n4!=1)) break;
				atomType = ::SetAtomType((const unsigned char *)etstr);
				if (atomType > 0) {
					sscanf(xcstr, "%f", &(atomPos.x));
					sscanf(ycstr, "%f", &(atomPos.y));
					sscanf(zcstr, "%f", &(atomPos.z));
					AddAtom(atomType, atomPos);
					if (n5 == 1) {
						idList[idstr] = GetNumAtoms() - 1;
					} else {
						//This can't be matched in the future, but we have to add something to keep the numbering right
						std::ostringstream t;
						t << idList.size();
						idList[t.str()] = GetNumAtoms() - 1;
					}
				}
			}
			
			if (xcstr) delete [] xcstr;
			if (ycstr) delete [] ycstr;
			if (zcstr) delete [] zcstr;
			if (etstr) delete [] etstr;
			if (idstr) delete [] idstr;
		}
	}
}
void Frame::ParseAtomAttributeArrayXML(XMLElement * arrayXML, const std::map<std::string, long> & idList) {
	//The atom information can be in the form of Atom subelements or (gross) string attributes.
	//Parse the subelements
	XMLElementList * children = arrayXML->getChildren();
	for (int i=0; i<children->length(); i++) {
		XMLElement * child = children->item(i);
		if (child != NULL) {
			const char * nm = child->getName();
			if (nm != NULL) {
				CML_Element elem;
				if (CML_convert(nm, elem)) {
					if (elem == AtomElement) {
						const char * idresult = child->getAttributeValue(CML_convert(IdAttr));
						if (!idresult) continue;
						long atomId=-1;
						const std::map<std::string, long>::const_iterator it = idList.find(idresult);
						if (it != idList.end()) {
							atomId = it->second;
						}
						if ((atomId>=0)&&(atomId<NumAtoms)) {
							bool temp;
							if (child->getAttributeValue(CML_convert(selectedAttr),temp))
								SetAtomSelection(atomId, temp);
							if (child->getAttributeValue(CML_convert(invisibleAttr),temp))
								Atoms[atomId].SetInvisibility(temp);
							if (child->getAttributeValue(CML_convert(symmetryUniqueAttr),temp))
								Atoms[atomId].IsSymmetryUnique(temp);
							if (child->getAttributeValue(CML_convert(SIMOMMFlagAttr),temp))
								Atoms[atomId].IsSIMOMMAtom(temp);
							long templ;
							if (child->getAttributeValue(CML_convert(effectiveFragmentIdAttr),templ)) {
								if (templ>0) {
									Atoms[atomId].IsEffectiveFragment(true);
									Atoms[atomId].SetFragmentNumber(templ);
								}
							}
							if (child->getAttributeValue(CML_convert(coordinationNumberAttr),templ))
								Atoms[atomId].SetCoordinationNumber(templ);
							if (child->getAttributeValue(CML_convert(lonePairCountAttr),templ))
								Atoms[atomId].SetLonePairCount(templ);
						}
					}
				}
			}
		}
	}
	delete children;
}

bool Frame::ParseAtomXML(XMLElement * atomXML, std::map<std::string, long> & idList) {
	long atomType=0;
	CPoint3D atomPos;
	bool	good=false;
	
	//Look for CML2 stuff first, all attributes
	//<atom id="a1" elementType="C" x3="-0.0167" y3="1.3781" z3="0.0096"/>

	const char * et = atomXML->getAttributeValue(CML_convert(elementTypeAttr));
	if (et != NULL) {	//I think the presence of "elementType" as an attribute should signal CML2
		atomType = ::SetAtomType((const unsigned char *)et);
		if (atomType > 0) {//good element name
			const char * xcoord = atomXML->getAttributeValue(CML_convert(X3Attr));
			const char * ycoord = atomXML->getAttributeValue(CML_convert(Y3Attr));
			const char * zcoord = atomXML->getAttributeValue(CML_convert(Z3Attr));
			if ((xcoord!=NULL)&&(ycoord!=NULL)&&(zcoord!=NULL)) {
				sscanf(xcoord, "%f", &(atomPos.x));
				sscanf(ycoord, "%f", &(atomPos.y));
				sscanf(zcoord, "%f", &(atomPos.z));
				AddAtom(atomType, atomPos);
				good = true;
			}
		}
	} else {	//parse CML1 style like:
				//<string builtin="elementType">C</string>
				//<float builtin="x3">1.1709</float>
				//<float builtin="y3">2.0855</float>
				//<float builtin="z3">0.0021</float>
		XMLElementList * strList = atomXML->getElementsByName("string");
		if (strList->length() > 0) {
			for (int i=0; i< strList->length(); i++) {
				XMLElement * strElem = strList->item(i);
				const char * typ = strElem->getAttributeValue("builtin");
				if (typ != NULL) {
					if (! strcmp(typ, "elementType")) {
						et = strElem->getValue();
						if (et != NULL) {
							atomType = ::SetAtomType((const unsigned char *)et);
							if (atomType > 0) {
								const char * xcoord=NULL, * ycoord=NULL, *zcoord=NULL;
								XMLElementList * flList = atomXML->getElementsByName("float");
								for (int j=0; j<flList->length(); j++) {
									XMLElement * flElem = flList->item(j);
									typ = flElem->getAttributeValue("builtin");
									if (typ != NULL) {
										CML_Attribute attrType;
										if (CML_convert(typ, attrType)) {
											switch (attrType) {
												case X3Attr:
													xcoord = flElem->getValue();
													break;
												case Y3Attr:
													ycoord = flElem->getValue();
													break;
												case Z3Attr:
													zcoord = flElem->getValue();
													break;
										//		default:
											}
										}
									}
								}
								delete flList;
								//Did we find x, y and z coords?
								if ((xcoord!=NULL)&&(ycoord!=NULL)&&(zcoord!=NULL)) {
									sscanf(xcoord, "%f", &(atomPos.x));
									sscanf(ycoord, "%f", &(atomPos.y));
									sscanf(zcoord, "%f", &(atomPos.z));
									AddAtom(atomType, atomPos);
									good = true;
									break;	//break out of the string loop
								}
							}
						}
					}
				}
			}
		}
		delete strList;
	}
	if (good) {	//grab the atom id, if present
		const char * idresult = atomXML->getAttributeValue("id");
		if (idresult != NULL) {
			idList[idresult] = GetNumAtoms() - 1;
		} else {
			std::ostringstream t;
			t << idList.size();
			idList[t.str()] = GetNumAtoms() - 1;
		}
	}
	return good;
}
void Frame::ParseBondArrayXML(XMLElement * arrayXML, const std::map<std::string, long> & idList) {
	const char * ordrref=NULL, *atmref1=NULL, *atmref2=NULL;
	int nrefs=0;
	//Look for Bond subelements
	XMLElementList * children = arrayXML->getChildren();
	for (int i=0; i<children->length(); i++) {
		XMLElement * child = children->item(i);
		if (child != NULL) {
			const char * nm = child->getName();
			if (nm != NULL) {
				CML_Element elem;
				if (CML_convert(nm, elem)) {
					switch (elem) {
						case BondElement:	//Hopefully most stuff uses this path
							ParseBondXML(child, idList);
							break;
						case StringArrayElement:
						{
							const char * attType = child->getAttributeValue(CML_convert(builtinAttr));
							if (attType) {
								CML_Attribute attr;
								if (CML_convert(attType, attr)) {
									switch (attr) {
										case atomRefAttr:
											if (nrefs == 0) {
												atmref1 = child->getValue();
												nrefs++;
											} else if (nrefs == 1) {
												atmref2 = child->getValue();
												nrefs++;
											}
											break;
										case orderAttr:
											ordrref = child->getValue();
											break;
									}
								}
							}
						}
							break;
					}
				}
			}
		}
	}
	delete children;
	// Check for ugly attribute arrays
	if (atmref1 == NULL)
		atmref1 = arrayXML->getAttributeValue(CML_convert(atomRef1Attr));
	if (atmref2 == NULL)
		atmref2 = arrayXML->getAttributeValue(CML_convert(atomRef2Attr));
	if ((atmref1 != NULL)&&(atmref2 != NULL)) {
		if (ordrref == NULL)
			ordrref = arrayXML->getAttributeValue(CML_convert(orderAttr));
		long ar1pos=0, ar2pos=0, ordpos=0;
		int nchar;
		long totalLen = strlen(atmref1);
		char * atm1 = new char[totalLen+1];
		totalLen = strlen(atmref1);
		char * atm2 = new char[totalLen+1];
		char *ordr=NULL;
		if (ordrref != NULL) {
			totalLen = strlen(ordrref);
			ordr = new char[totalLen+1];
		}
		bool done = false;
		while (!done) {
			int n1 = sscanf(&(atmref1[ar1pos]), "%s%n", atm1, &nchar);
			ar1pos += nchar;
			int n2 = sscanf(&(atmref2[ar2pos]), "%s%n", atm2, &nchar);
			ar2pos += nchar;
			if ((n1!=1)||(n2!=1)) break;
			long id1=-1, id2=-1;
			std::map<std::string, long>::const_iterator it = idList.find(atm1);
			if (it != idList.end()) {
				id1 = it->second;
			}
			it = idList.find(atm2);
			if (it != idList.end()) {
				id2 = it->second;
			}
			int n3;
			BondOrder b = kSingleBond;
			if (ordrref) {
				n3 = sscanf(&(ordrref[ordpos]), "%s%n", ordr, &nchar);
				ordpos += nchar;
				if (! CML_convert(ordr, b)) {
					if (b == kUnknownBond) continue;
				}
			}
			if (BondExists(id1, id2) < 0) {	//avoid duplicate bonds
				AddBond(id1, id2, b);
			}
		}
		if (atm1) delete [] atm1;
		if (atm2) delete [] atm2;
		if (ordr) delete [] ordr;
	}
}

bool Frame::ParseBondXML(XMLElement * bondXML, const std::map<std::string, long> & idList) {
	//<bond id="a1" atomRefs2="a1 a2" order="S"/>
	
	const char * et = bondXML->getAttributeValue(CML_convert(atomRefs2Attr));
	if (et != NULL) {
		int len = strlen(et);
		if (len > 0) {
			const char * order = bondXML->getAttributeValue(CML_convert(orderAttr));
			BondOrder b = kSingleBond;
			CML_convert(order, b);
			if (b != kUnknownBond) {
				char * atm1 = new char [len+1];
				char * atm2 = new char [len+1];
				sscanf(et, "%s %s", atm1, atm2);
				if ((strlen(atm1) > 0)&&(strlen(atm2) > 0)) {
					long id1=-1, id2=-1;
					std::map<std::string, long>::const_iterator it = idList.find(atm1);
					if (it != idList.end()) {
						id1 = it->second;
					}
					it = idList.find(atm2);
					if (it != idList.end()) {
						id2 = it->second;
					}
					if (BondExists(id1, id2) < 0) {	//avoid duplicate bonds
						AddBond(id1, id2, b); 
					}
				}
				delete [] atm1;
				delete [] atm2;
			}
		}
	} else { // CML1 style bond info
			//<bond>
			//<string builtin="atomRef">a1</string>
			//<string builtin="atomRef">a2</string>
			//<string builtin="order">1</string>
			//</bond>
		XMLElementList * strList = bondXML->getElementsByName("string");
		if (strList->length() > 0) {
			long id1, id2, nrefs=0;
			BondOrder b=kSingleBond;
			for (int i=0; i<strList->length(); i++) {
				XMLElement * child = strList->item(i);
				if (child != NULL) {
					const char * attvalue = child->getAttributeValue(CML_convert(builtinAttr));
					CML_Attribute att;
					if (CML_convert(attvalue, att)) {
						const char * value = child->getValue();
						switch (att) {
							case atomRefAttr: {
								std::map<std::string, long>::const_iterator it = idList.find(value);
								if (it != idList.end()) {
									id1 = it->second;
									if (nrefs==0) {
										id1 = it->second;
										nrefs++;
									} else if (nrefs==1) {
										id2 = it->second;
										nrefs++;
									}
								}
							}
								/* for (unsigned int j=0; j<idList.size(); j++) {
									if (!strcmp(value, idList[j])) {
										if (nrefs==0) {
											id1 = j;
											nrefs++;
										} else if (nrefs==1) {
											id2 = j;
											nrefs++;
										}
										break;
									}
								}
								 */
								break;
							case orderAttr:
								CML_convert(value, b);
								break;
						}
					}
				}
			}
			if (nrefs >= 2) {	//technically more than 2 means invalid XML
				if (BondExists(id1, id2) < 0) {	//avoid duplicate bonds
					AddBond(id1, id2, b); 
				}
			}
		}
		delete strList;
	}
	return true;
}
void Internals::ReadXML(XMLElement * parent) {
	XMLElementList * children = parent->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			const char * name = child->getName();
			if (!strcmp(name, CML_convert(MMP_MOPacStyleInternals))) {
				if (!MOPacStyle) {
					MOPacStyle = new MOPacInternals(child);
				}
			}
		}
		delete children;
	}
}
MOPacInternals::MOPacInternals(XMLElement * parent) {
	ConnectionAtoms = NULL;
	Values = NULL;
	Type = NULL;
	Allocation = Count = 0;
	if (parent->getAttributeValue(CML_convert(MMP_MOPacStyleCount), Count)) {
		ConnectionAtoms = new long[Count];
		Values = new float[Count];
		Type = new char[Count];
		if (!ConnectionAtoms || !Values || !Type) throw MemoryError();
		Allocation = Count;
		XMLElementList * children = parent->getChildren();
		if (children) {
			for (int i=0; i<children->length(); i++) {
				XMLElement * child = children->item(i);
				const char * title = child->getAttributeValue(CML_convert(titleAttr));
				MMP_InternalCoordNS attr;
				if (CML_convert(title, attr)) {
					const char * val = child->getValue();
					int nchar=0, pos=0;
					switch (attr) {
						case MMP_MOPacStyleConnections:
						{
							for (int j=0; j<Count; j++) {
								sscanf(&(val[pos]), "%ld%n", &(ConnectionAtoms[j]), &nchar);
								pos += nchar;
							}
						}
							break;
						case MMP_MOPacStyleValues:
						{
							for (int j=0; j<Count; j++) {
								sscanf(&(val[pos]), "%f%n", &(Values[j]), &nchar);
								pos += nchar;
							}
						}
							break;
						case MMP_MOPacStyleTypes:
						{
							for (int j=0; j<Count; j++) {
								int temp;
								sscanf(&(val[pos]), "%d%n", &temp, &nchar);
								Type[j] = temp;
								pos += nchar;
							}
						}
							break;
					}
				}
			}
			delete children;
		}
	}
}

BasisSet * BasisSet::ReadXML(XMLElement * parent) {
	long mlength=0, nshells=0, nfuncs=0, shellCount=0;
	BasisSet * target = NULL;
	XMLElement * BSXML = parent->getFirstChild();
	if (BSXML) {
		if (!strcmp(BSXML->getName(), CML_convert(MMP_BasisSetElement))) {
			BSXML->getAttributeValue(CML_convert(MMP_BSMapLength), mlength);
			BSXML->getAttributeValue(CML_convert(MMP_BSNumShells), nshells);
			BSXML->getAttributeValue(CML_convert(MMP_BSNumFunctions), nfuncs);
			try {
				if ((mlength>0)&&(nshells>0)) {
					target = new BasisSet(mlength, nshells);
					target->NumFuncs = nfuncs;
					target->NumShells = nshells;
					target->MapLength = mlength;
					XMLElementList * children = BSXML->getChildren();
					if (children) {
						for (int i=0; i<children->length(); i++) {
							XMLElement * child = children->item(i);
							if (!strcmp(child->getName(), CML_convert(MMP_BasisShell))) {
								BasisShell temp;
								target->Shells.push_back(temp);
								target->Shells[shellCount].ReadXML(child);
								shellCount++;
							} else if (!strcmp(child->getName(), CML_convert(ArrayElement))) {
								const char * title = child->getAttributeValue(CML_convert(titleAttr));
								MMP_BasisSetNS attr;
								if (CML_convert(title, attr)) {
									switch (attr) {
										case MMP_BSMap:
										{
											int nchar, pos=0;
											const char * val = child->getValue();
											for (int i=0; i<mlength; i++) {
												long is, ie;
												if (sscanf(&(val[pos]), "%ld %ld%n", &is, &ie, &nchar) != 2) throw DataError();
												target->BasisMap[2*i] = is;
												target->BasisMap[2*i+1] = ie;
												pos += nchar;
											}
										}
											break;
										case MMP_BSNucCharge:
										{
											int nchar, pos=0;
											const char * val = child->getValue();
											for (int i=0; i<mlength; i++) {
												long ic;
												if (sscanf(&(val[pos]), "%ld%n", &ic, &nchar) != 1) throw DataError();
												target->NuclearCharge[i] = ic;
												pos += nchar;
											}
											target->goodCharges = true;
										}
											break;
									}
								}
							}
						}
						delete children;
					}
				}
			}
			catch (DataError error) {
				if (target) {
					delete target;
					target = NULL;
				}
			}
		}
	}
	return target;
}
void BasisShell::ReadXML(XMLElement * parent) {
	long temp;
	if (parent->getAttributeValue(CML_convert(MMP_BSShellType), temp)) ShellType = temp;
	else throw DataError();
	if (parent->getAttributeValue(CML_convert(MMP_BSNumPrims), temp)) {
		if (temp <= 0) throw DataError();
		NumPrims = temp;
		XMLElementList * children = parent->getChildren();
		if (children) {
			for (int i=0; i<children->length(); i++) {
				XMLElement * child = children->item(i);
				if (!strcmp(child->getName(), CML_convert(ArrayElement))) {
					const char * attrnm = child->getAttributeValue(CML_convert(titleAttr));
					if (attrnm) {
						MMP_BasisSetNS attr;
						if (CML_convert(attrnm, attr)) {
							const char * val = child->getValue();
							int pos=0, nchar;
							switch (attr) {
								case MMP_BSExponent:
								{
									float temp;
									for (int j=0; j<NumPrims; j++) {
										if (sscanf(&(val[pos]), "%f%n", &temp, &nchar) != 1) throw DataError();
										Exponent.push_back(temp);
										pos += nchar;
									}
								}
									break;
								case MMP_BSNormCoef:
								{
									int length = NumPrims;
									if (ShellType < 0) length *= 2;
									float temp;
									for (int j=0; j<length; j++) {
										if (sscanf(&(val[pos]), "%f%n", &temp, &nchar) != 1) throw DataError();
										NormCoef.push_back(temp);
										pos += nchar;
									}
								}
									break;
								case MMP_BSInputCoef:
								{
									int length = NumPrims;
									if (ShellType < 0) length *= 2;
									float temp;
									for (int j=0; j<length; j++) {
										if (sscanf(&(val[pos]), "%f%n", &temp, &nchar) != 1) throw DataError();
										InputCoef.push_back(temp);
										pos += nchar;
									}
								}
									break;
							}
						}
					}
				}
			}
			delete children;
		}
	} else throw DataError();
}
void GradientData::ReadXML(XMLElement * grad) {
	XMLElementList * children = grad->getChildren();
	if (children) {
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			CML_Element childtype;
			if (CML_convert(child->getName(), childtype)) {
				const char * title = child->getAttributeValue(CML_convert(titleAttr));
				if (title) {
					MMP_NameSpace titlename;
					if (CML_convert(title, titlename)) {
						switch (childtype) {
							case ScalarElement:
							{
								switch (titlename) {
									double temp;
									case MMP_RMSGradient:
										if (child->getDoubleValue(temp)) RMS = temp;
										break;
									case MMP_MaxGradient:
										if (child->getDoubleValue(temp)) Maximum = temp;
										break;
								}
							}
								break;
							case MatrixElement:
								if (titlename == MMP_CartesianGradient) {
									long cols, rows;
									if (child->getAttributeValue(CML_convert(columnsAttr), cols)) {
										if (cols == 3) {	//should always be exactly 3 cols
											if (child->getAttributeValue(CML_convert(rowsAttr), rows)) {
												if (rows > 0) {
													CartesianGradient = new CPoint3D[rows];
													int pos = 0, nchar;
													const char * value = child->getValue();
													for (int j=0; j<rows; j++) {
														int count = sscanf(&(value[pos]), "%f %f %f%n",
																		   &(CartesianGradient[j].x),
																		   &(CartesianGradient[j].y),
																		   &(CartesianGradient[j].z), &nchar);
														pos += nchar;
														if (count != 3) {
															delete [] CartesianGradient;
															CartesianGradient = NULL;
															break;
														}
													}
												}
											}
										}
									}
								}
								break;
						}
					}
				}
			}
		}
		delete children;
	}
}
void VibRec::ReadXML(XMLElement * vibs, const long & NumAtoms) {
	XMLElementList * children = vibs->getChildren();
	if (children) {
		if (vibs->getAttributeValue(CML_convert(MMP_NormalModesCount), NumModes)) {
			for (int i=0; i<children->length(); i++) {
				XMLElement * child = children->item(i);
				CML_Element childtype;
				if (CML_convert(child->getName(), childtype)) {
					const char * title = child->getAttributeValue(CML_convert(titleAttr));
					if (title) {
						MMP_VibRecNameSpace titlename;
						if (CML_convert(title, titlename)) {
							switch (childtype) {
								case ScalarElement:
								{
									switch (titlename) {
										long temp;
										case MMP_CurrentMode:
											if (child->getLongValue(temp)) CurrentMode = temp;
											break;
									}
								}
									break;
								case MatrixElement:
									if (titlename == MMP_VibNormalModes) {
										long cols, rows;
										if (child->getAttributeValue(CML_convert(columnsAttr), cols)) {
											if (cols == 3) {	//should always be exactly 3 cols
												if (child->getAttributeValue(CML_convert(rowsAttr), rows)) {
													if (rows > 0) {
														if (NumModes == (rows/NumAtoms)) {
															NormMode.reserve(rows);
															int pos = 0, nchar;
															const char * value = child->getValue();
															CPoint3D temp;
															for (int j=0; j<rows; j++) {
																int count = sscanf(&(value[pos]), "%f %f %f%n",
																				   &(temp.x),
																				   &(temp.y),
																				   &(temp.z), &nchar);
																NormMode.push_back(temp);
																pos += nchar;
																if (count != 3) {
																	NormMode.resize(0);
																	break;
																}
															}
														}
													}
												}
											}
										}
									}
									break;
								case ArrayElement:
								{
									switch (titlename) {
										case MMP_VibFrequencies:
										{	//The text labels
											const char * v = child->getValue();
											if (v) {
												long len = strlen(v);
												if (Frequencies.empty()) {
													Frequencies.reserve(NumModes);
													int mode=0;
													std::ostringstream buf;
													for (int j=0; j<len; j++) {
														if (v[j] != '|') {
															buf << v[j];
														} else {
															Frequencies.push_back(buf.str());
															mode ++;
															buf.str("");
														}
													}
													if (!buf.str().empty()) {
														Frequencies.push_back(buf.str());
														mode ++;
													}
												}
											}
										}
											break;
										case MMP_Symmetry:
										{
											long temp;
											if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
												//failing the next test indicates a corrupt record.
												if (temp !=NumModes) continue;
												const char * v = child->getValue();
												if (Symmetry.empty() && v) {
													long len = strlen(v);
													Symmetry.reserve(NumModes);
													int mode=0;
													std::ostringstream buf;
													for (int j=0; j<len; j++) {
														if (v[j] != ' ') {
															buf << v[j];
														} else {
															Frequencies.push_back(buf.str());
															mode ++;
															buf.str("");
														}
													}
													if (!buf.str().empty()) {
														Frequencies.push_back(buf.str());
														mode ++;
													}
												}
											}
										}
											break;
										case MMP_VibIntensity:
										{
											long temp;
											if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
												//failing the next test indicates a corrupt record.
												if (temp !=NumModes) continue;
												if (Intensities.empty()) {
													Intensities.reserve(NumModes);
													temp = child->getFloatArray(NumModes, Intensities);
													if (temp != NumModes) {
														Intensities.resize(0);
													}
												}
											}
										}
											break;
										case MMP_ReducedMass:
										{
											long temp;
											if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
												//failing the next test indicates a corrupt record.
												if (temp !=NumModes) continue;
												if (ReducedMass.empty()) {
													ReducedMass.reserve(NumModes);
													temp = child->getFloatArray(NumModes, ReducedMass);
													if (temp != NumModes) {
														ReducedMass.resize(0);
													}
												}
											}
										}
											break;
										case MMP_RamanIntensity:
										{
											long temp;
											if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
												//failing the next test indicates a corrupt record.
												if (temp !=NumModes) continue;
												if (RamanIntensity.empty()) {
													RamanIntensity.reserve(NumModes);
													temp = child->getFloatArray(NumModes, RamanIntensity);
													if (temp != NumModes) {
														RamanIntensity.resize(0);
													}
												}
											}
										}
											break;
										case MMP_Depolarization:
										{
											long temp;
											if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
													//failing the next test indicates a corrupt record.
												if (temp !=NumModes) continue;
												if (Depolarization.empty()) {
													Depolarization.reserve(NumModes);
													temp = child->getFloatArray(NumModes, Depolarization);
													if (temp != NumModes) {
														Depolarization.resize(0);
													}
												}
											}
										}
											break;
									}
								}
									break;
							}
						}
					}
				}
			}
		}
		delete children;
	}
}
bool OrbitalRec::ReadXML(XMLElement * orbset) {
	bool result = false;	//return true only if we get a good, complete element
	const char * otyp = orbset->getAttributeValue(CML_convert(MMP_OrbitalTypeAttr));
	if (otyp) ConvertTypeOfOrbital(otyp, OrbitalType);
	const char * bw = orbset->getAttributeValue(CML_convert(MMP_WavefunctionType));
	if (bw) ConvertTypeOfWavefunction(bw, BaseWavefunction);
	XMLElementList * children = orbset->getChildren();
	if (children) {
			//grab the attributes first which are required to dimension the arrays
		orbset->getAttributeValue(CML_convert(MMP_NumAlphaOrbs), NumAlphaOrbs);
		orbset->getAttributeValue(CML_convert(MMP_NumBetaOrbs), NumBetaOrbs);
		orbset->getAttributeValue(CML_convert(MMP_NumOccAlphaOrbs), NumOccupiedAlphaOrbs);
		orbset->getAttributeValue(CML_convert(MMP_NumOccBetaOrbs), NumOccupiedBetaOrbs);
		orbset->getAttributeValue(CML_convert(MMP_NumBasisFunctions), NumBasisFunctions);
		for (int i=0; i<children->length(); i++) {
			XMLElement * child = children->item(i);
			CML_Element childtype;
			if (!strcasecmp(child->getName(), CML_convert(MMP_OrbitalLabel))) {
				setLabel(child->getValue());
			} else if (CML_convert(child->getName(), childtype)) {
				const char * title = child->getAttributeValue(CML_convert(titleAttr));
				if (title) {	//This can be for the Arrays and matrix
					MMP_OrbitalSetNS titlename;
					if (CML_convert(title, titlename)) {
						switch (titlename) {
							case MMP_OrbVectors:
							{
								long na, nb;
								if (child->getAttributeValue(CML_convert(columnsAttr), na)) {
									if (child->getAttributeValue(CML_convert(rowsAttr), nb)) {
										if ((na == NumAlphaOrbs)&&(nb == NumBasisFunctions)) {
											if (Vectors) {
												delete [] Vectors;
												Vectors = NULL;
											}
											Vectors = new float[NumAlphaOrbs*NumBasisFunctions];
											long test = child->getFloatArray(na*nb, Vectors);
											if (test != na*nb) {
												delete [] Vectors;
												Vectors = NULL;
											}
										}
									}
								}
							}
								break;
							case MMP_BetaOrbVectors:
							{
								long nb, nbf;
								if (child->getAttributeValue(CML_convert(columnsAttr), nb)) {
									if (child->getAttributeValue(CML_convert(rowsAttr), nbf)) {
										if ((nb == NumBetaOrbs)&&(nbf == NumBasisFunctions)) {
											if (VectorsB) {
												delete [] VectorsB;
												VectorsB = NULL;
											}
											VectorsB = new float[NumBetaOrbs*NumBasisFunctions];
											long test = child->getFloatArray(nb*nbf, VectorsB);
											if (test != nb*nbf) {
												delete [] VectorsB;
												VectorsB = NULL;
											}
										}
									}
								}
							}
								break;
							case MMP_OrbEnergies:
							{
								long na;
								if (child->getAttributeValue(CML_convert(sizeAttr), na)) {
									if (na == NumAlphaOrbs) {
										if (Energy) {
											delete [] Energy;
											Energy = NULL;
										}
										Energy = new float[NumAlphaOrbs];
										long test = child->getFloatArray(na, Energy);
										if (test != na) {
											delete [] Energy;
											Energy = NULL;
										}
									}
								}
							}
								break;
							case MMP_BetaOrbEnergies:
							{
								long nb;
								if (child->getAttributeValue(CML_convert(sizeAttr), nb)) {
									if (nb == NumBetaOrbs) {
										if (EnergyB) {
											delete [] EnergyB;
											EnergyB = NULL;
										}
										EnergyB = new float[NumBetaOrbs];
										long test = child->getFloatArray(nb, EnergyB);
										if (test != nb) {
											delete [] EnergyB;
											EnergyB = NULL;
										}
									}
								}
							}
								break;
							case MMP_OrbOccupations:
							{
								long na;
								if (child->getAttributeValue(CML_convert(sizeAttr), na)) {
									if (na == NumAlphaOrbs) {
										if (OrbOccupation) {
											delete [] OrbOccupation;
											OrbOccupation = NULL;
										}
										OrbOccupation = new float[NumAlphaOrbs];
										long test = child->getFloatArray(na, OrbOccupation);
										if (test != na) {
											delete [] OrbOccupation;
											OrbOccupation = NULL;
										}
									}
								}
							}
								break;
							case MMP_BetaOrbOccupations:
							{
								long nb;
								if (child->getAttributeValue(CML_convert(sizeAttr), nb)) {
									if (nb == NumBetaOrbs) {
										if (OrbOccupationB) {
											delete [] OrbOccupationB;
											OrbOccupationB = NULL;
										}
										OrbOccupationB = new float[NumBetaOrbs];
										long test = child->getFloatArray(nb, OrbOccupationB);
										if (test != nb) {
											delete [] OrbOccupationB;
											OrbOccupationB = NULL;
										}
									}
								}
							}
								break;
							case MMP_OrbSyms:
							{
								long temp;
								if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
									if (temp == NumAlphaOrbs) {
										const char * sym = child->getValue();
										if (sym) {
											if (SymType) {
												delete [] SymType;
												SymType = NULL;
											}
											int len = strlen(sym);
											SymType = new char[5*NumAlphaOrbs];
											long pos = 0;
											for (int i=0; i<NumAlphaOrbs; i++) {
												int n=0;
												while (sym[pos] != ' ') {
													SymType[n+5*i] = sym[pos];
													pos++;
													n++;
													if (pos >= len) {
														i = NumAlphaOrbs;
														break;
													}
												}
												SymType[n+5*i] = '\0';
												while (sym[pos]==' ') pos++;
											}
										}
									}
								}
							}
								break;
							case MMP_BetaOrbSyms:
							{
								long temp;
								if (child->getAttributeValue(CML_convert(sizeAttr), temp)) {
									if (temp == NumBetaOrbs) {
										const char * sym = child->getValue();
										if (sym) {
											if (SymTypeB) {
												delete [] SymTypeB;
												SymTypeB = NULL;
											}
											int len = strlen(sym);
											SymTypeB = new char[5*NumBetaOrbs];
											long pos = 0;
											for (int i=0; i<NumBetaOrbs; i++) {
												int n=0;
												while (sym[pos] != ' ') {
													SymTypeB[n+5*i] = sym[pos];
													pos++;
													n++;
													if (pos >= len) {
														i = NumBetaOrbs;
														break;
													}
												}
												SymTypeB[n+5*i] = '\0';
												while (sym[pos]==' ') pos++;
											}
										}
									}
								}
							}
								break;
						}
					}
				}
			}
		}
		delete children;
	}
	if (Vectors!= NULL) result = true;	//Probably should do some thing more...
	return result;
}
bool AnnotationMarker::ReadXML(XMLElement * Annotation) {
	bool result = Annotation->getAttributeValue(kAnnAtom1XML, atoms[0]);

	return result;
}
bool AnnotationLength::ReadXML(XMLElement * Annotation) {
	bool result = Annotation->getAttributeValue(kAnnAtom1XML, atoms[0]);
	result = result && Annotation->getAttributeValue(kAnnAtom2XML, atoms[1]);

	return result;
}

bool AnnotationAngle::ReadXML(XMLElement * Annotation) {

	bool result = Annotation->getAttributeValue(kAnnAtom1XML, atoms[0]);
	result = result && Annotation->getAttributeValue(kAnnAtom2XML, atoms[1]);
	result = result && Annotation->getAttributeValue(kAnnAtom3XML, atoms[2]);
	
	return result;
}
bool AnnotationDihedral::ReadXML(XMLElement * Annotation) {
	bool result = Annotation->getAttributeValue(kAnnAtom1XML, atoms[0]);
	result = result && Annotation->getAttributeValue(kAnnAtom2XML, atoms[1]);
	result = result && Annotation->getAttributeValue(kAnnAtom3XML, atoms[2]);
	result = result && Annotation->getAttributeValue(kAnnAtom4XML, atoms[3]);
	
	return result;
}

bool Structure::ReadXML(XMLElement *struc_el) {

	const char *attr;
	XMLElement *el;
	XMLElement *atom_el;
	XMLElement *bond_el;
	int i;

	// First we get structure's name.
	attr = struc_el->getAttributeValue(CML_convert(nameAttr));
	if (!attr) return false;

	name = wxString(attr, wxConvUTF8);

	attr = struc_el->getAttributeValue(kFRAGNAMEXML);
	if (attr != NULL) {
		FragName = std::string(attr);
	}

	el = struc_el->getFirstChild();
	if (!el) return false;

	// See if fragment definition exists.
	if (strcmp(el->getName(), "FragmentDefinition") == 0) {
		frag_def = el->getValue();
		el = el->getNextChild();
	}

	// Then we get an atoms element which contains some number of
	// atom elements.

	attr = el->getAttributeValue(CML_convert(sizeAttr));
	if (!attr) return false;
	sscanf(attr, "%d", &natoms);
	if (!natoms) return true;

	atoms = new mpAtom[natoms];

	i = 0;
	atom_el = el->getFirstChild();

	while (atom_el) {
		if (i >= natoms) {
			std::cerr << "Malformed structure!" << std::endl;
			return false;
		}

		attr = atom_el->getAttributeValue(CML_convert(elementTypeAttr));
		if (!attr) return false;

		atoms[i].Type = ::SetAtomType((const unsigned char *) attr);
		if (atoms[i].Type <= 0) return false;

		attr = atom_el->getAttributeValue(CML_convert(X3Attr));
		if (!attr) return false;
		sscanf(attr, "%f", &atoms[i].Position.x);

		attr = atom_el->getAttributeValue(CML_convert(Y3Attr));
		if (!attr) return false;
		sscanf(attr, "%f", &atoms[i].Position.y);

		attr = atom_el->getAttributeValue(CML_convert(Z3Attr));
		if (!attr) return false;
		sscanf(attr, "%f", &atoms[i].Position.z);

		attr = atom_el->getAttributeValue(CML_convert(coordinationNumberAttr));
		if (!attr) return false;
		sscanf(attr, "%hd", &atoms[i].coordinationNumber);

		attr = atom_el->getAttributeValue(CML_convert(lonePairCountAttr));
		if (!attr) return false;
		sscanf(attr, "%hd", &atoms[i].LPCount);

		atom_el = atom_el->getNextChild();
		i++;
	}

	// Then we get a bonds element which contains some number of
	// bond elements.
	el = el->getNextChild();
	if (!el) return false;

	attr = el->getAttributeValue(CML_convert(sizeAttr));
	if (!attr) return false;
	sscanf(attr, "%d", &nbonds);

	// Nothing else to do if there aren't any bonds.
	if (!nbonds) return true;

	bonds = new Bond[nbonds];
	i = 0;
	bond_el = el->getFirstChild();
	while (bond_el) {
		if (i >= nbonds) {
			std::cerr << "Malformed structure!" << std::endl;
			return false;
		}

		attr = bond_el->getAttributeValue(CML_convert(atomRefs2Attr));
		if (!attr) return false;
		sscanf(attr, "a%ld a%ld", &bonds[i].Atom1, &bonds[i].Atom2);

		attr = bond_el->getAttributeValue(CML_convert(orderAttr));
		if (!attr) return false;
		CML_convert(attr, bonds[i].Order);

		bond_el = bond_el->getNextChild();
		i++;
	}

	return true;
}

#pragma mark -
#pragma mark CML_Element
const char * CML_convert(CML_Element t)
{       
    switch(t)
    {   
        case CMLElement:
            return "cml";
        case MoleculeElement:
            return "molecule";
        case AtomArrayElement:
            return "atomArray";
        case AtomElement:
            return "atom";
		case BondArrayElement:
            return "bondArray";
        case BondElement:
            return "bond";
        case StringArrayElement:
            return "stringArray";
		case FloatArrayElement:
            return "floatArray";
        case IntegerArrayElement:
            return "integerArray";
		case ScalarElement:
			return "scalar";
		case MatrixElement:
			return "matrix";
		case MetaDataElement:
			return "metadata";
		case MetaDataListElement:
			return "metadataList";
        case ArrayElement:
            return "array";
		case ListElement:
			return "list";
		default:
            return "invalid";
    }
}
#pragma mark CML_Attribute
const char * CML_convert(CML_Attribute t) {

    switch(t)
    {   
        case IdAttr:
            return "id";
        case atomIdAttr:
            return "atomID";
        case titleAttr:
            return "title";
        case elementTypeAttr:
            return "elementType";
        case X3Attr:
            return "x3";
        case Y3Attr:
            return "y3";
        case Z3Attr:
            return "z3";
        case xyz3Attr:
            return "xyz3";
        case orderAttr:
            return "order";
        case atomRefs2Attr:
            return "atomRefs2";
        case bondRefsAttr:
            return "bondRefs";
        case atomRef1Attr:
            return "atomRef1";
        case atomRef2Attr:
            return "atomRef2";
        case atomRefAttr:
            return "atomRef";
        case builtinAttr:
            return "builtin";
        case dataTypeAttr:
            return "dataType";
        case rowsAttr:
            return "rows";
        case columnsAttr:
            return "columns";
        case sizeAttr:
            return "size";
		case nameAttr:
			return "name";
		case contentAttr:
			return "content";
		case selectedAttr:
			return "selected";
		case invisibleAttr:
			return "invisible";
		case symmetryUniqueAttr:
			return "symmetryUnique";
		case effectiveFragmentIdAttr:
			return "effectiveFragmentId";
		case SIMOMMFlagAttr:
			return "IsSIMOMMAtom";
		case coordinationNumberAttr:
			return "coordinationNumber";
		case lonePairCountAttr:
			return "lonePairCount";
        default:
            return "invalid";
    }
}  
const char * CML_convert(BondOrder t)
{       
    switch(t)
    {   //CML only maps to the three basic bond types
        case kHydrogenBond:
            return "H";
        case kSingleBond:
            return "S";
        case kDoubleBond:
            return "D";
        case kTripleBond:
            return "T";
        case kAromaticBond:
            return "A";
        case kMixedBonds:
            return "M";
		default:
            return "invalid";
    }
}
#pragma mark MMP_NameSpace
const char * CML_convert(MMP_NameSpace t)
{       
    switch(t)
    {   //Extra MacMolPlt specific XML elements...
        case MMP_RotationMatrix:
            return "Rotation Matrix";
		case MMP_Energy:
			return "Energy";
		case MMP_KineticEnergy:
			return "Kinetic Energy";
		case MMP_MP2Energy:
			return "MP2 Energy";
		case MMP_Time:
			return "Time";
		case MMP_ReactionCoord:
			return "Reaction Coordinate";
		case MMP_RMSGradient:
			return "RMS Gradient";
		case MMP_MaxGradient:
			return "Maximum Gradient";
		case MMP_CartesianGradient:
			return "Cartesian Gradient";
		default:
            return "invalid";
    }
}
#pragma mark MMP_VibRecNameSpace
const char * CML_convert(MMP_VibRecNameSpace t)
{       
    switch(t)
    {   //Extra MacMolPlt specific XML elements...
		case MMP_CurrentMode:
			return "Current Mode";
		case MMP_VibIntensity:
			return "Intensity";
		case MMP_VibFrequencies:
			return "Frequencies";
		case MMP_NormalModesCount:
			return "Number Modes";
		case MMP_VibNormalModes:
			return "Normal Modes";
		case MMP_ReducedMass:
			return "Reduced Mass";
		case MMP_RamanIntensity:
			return "Raman Intensity";
		case MMP_Depolarization:
			return "Depolarization";
		default:
            return "invalid";
    }
}
#pragma mark MMP_MetadataNamespace
const char * CML_convert(MMP_MetadataNamespace t)
{       
    switch(t)
    {   //Extra MacMolPlt specific XML elements...
        case MMP_MaxSize:
            return "Maximum Size";
        case MMP_WindowSize:
            return "Window Size";
		case MMP_CurrentFrame:
			return "Current Frame";
		case MMP_DrawNormalMode:
			return "Draw Normal Mode";
		case MMP_DrawSpecialAtoms:
			return "Draw Special Atoms";
		case MMP_ShowAxis:
			return "Show Axis";
		case MMP_ShowAtomLabels:
			return "Show Atom Labels";
		case MMP_LabelHAtoms:
			return "Label Hydrogen Atoms";
		case MMP_ShowAtomNumbers:
			return "Show Atom Numbers";
		case MMP_InternalCoordinates:
			return "InternalCoordinates";
		case MMP_BasisSet:
			return "BasisSet";
		case MMP_FragmentNameList:
			return "FRAGNAMEList";
		case MMP_FragmentDefinitions:
			return "FragmentDefinitions";
		case MMP_InputOptions:
			return "InputOptions";
		case MMP_Annotations:
			return "Annotations";
		case MMP_Structures:
			return "Structures";
		case MMP_Preferences:
			return "Preferences";
		case MMP_WindowData:
			return "WindowInformation";
		case MMP_Use3DMode:
			return "Use3DDrawing";
		default:
            return "invalid";
    }
}
#pragma mark MMP_MolListNameSpace
const char * CML_convert(MMP_MolListNameSpace t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
		case MMP_ScalarElement:
			return "scalar";
		case MMP_BondArrayElement:
            return "bondArray";
		case MMP_Gradient:
			return "Gradient";
		case MMP_NormalModes:
			return "NormalModes";
        case MMP_OrbitalSet:
            return "OrbitalSet";
        case MMP_SurfaceDescription:
            return "Surface";
        case MMP_AtomAttrArrayElement:
            return "atomArray";
		default:
            return "invalid";
    }
}
#pragma mark MMP_SurfaceNameSpace
const char * CML_convert(MMP_SurfaceNameSpace t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
		case MMP_BaseSurface:
			return "SurfaceCore";
        case MMP_OrbSurfBase:
            return "OrbitalSurfaceBase";
        case MMP_1DSurface:
            return "Surface1D";
        case MMP_2DSurface:
            return "Surface2D";
        case MMP_3DSurface:
            return "Surface3D";
        case MMP_VisibleAttr:
            return "visible";
        case MMP_SurfaceId:
            return "SurfaceId";
        case MMP_SurfaceLabel:
            return "Label";
        case MMP_OrbSurfPlotOrb:
            return "SelectedOrbital";
        case MMP_OrbSurfTargetSet:
            return "SelectedOrbitalSet";
        case MMP_OrbSurfUseAOs:
            return "SelectAOs";
        case MMP_OrbSurfUseBetaOrbs:
            return "SelectBetaOrbitals";
        case MMP_OrbSurfPhaseChage:
            return "InvertPhase";
        case MMP_OrbSurfSpherical:
            return "UseSphericalHarmonics";
        case MMP_OrbSurfOccNums:
            return "ShowOccupationNumbers";
        case MMP_SurfOrigin:
            return "SurfaceOrigin";
        case MMP_SurfXInc:
            return "SurfaceXIncrement";
        case MMP_SurfYInc:
            return "SurfaceYIncrement";
        case MMP_SurfZInc:
            return "SurfaceZIncrement";
        case MMP_SurfGridMax:
            return "MaxGridValue";
		case MMP_SurfGridMin:
			return "MinGridValue";
        case MMP_SurfNumGridPoints:
            return "NumberGridPoints";
        case MMP_SurfPosColor:
            return "PositiveColor";
        case MMP_SurfNegColor:
            return "NegativeColor";
        case MMP_SurfTransColor:
            return "TransparentColor";
        case MMP_SurfNumContours:
            return "NumberContours";
        case MMP_SurfMaxContourValue:
            return "MaximumContourValue";
        case MMP_SurfShowZeroContour:
            return "ShowZeroContour";
        case MMP_SurfScreenPlane:
            return "UseScreenPlane";
        case MMP_SurfDashedContours:
            return "UseDashedContours";
        case MMP_SurfPosNegContours:
            return "ShowPositiveNegativeContours";
		case MMP_DisplayPlane:
			return "DisplayPlane";
        case MMP_SurfMaxMEPValue:
            return "MaximumSurfaceValue";
        case MMP_SurfGrid:
            return "GridValues";
        case MMP_SurfNumXGridPoints:
            return "NumberXGridPoints";
        case MMP_SurfNumYGridPoints:
            return "NumberYGridPoints";
        case MMP_SurfNumZGridPoints:
            return "NumberZGridPoints";
        case MMP_SurfContourValue:
            return "ContourValue";
        case MMP_SurfGridSize:
            return "GridSize";
        case MMP_SurfSolidSurface:
            return "SolidSurface";
        case MMP_SurfColorizeSurface:
            return "ColorizeSurfaceByValue";
        case MMP_SurfUseSurfaceNormals:
            return "UseNormals";
        case MMP_SurfUseRGBColor:
            return "UseRGBColorization";
        case MMP_SurfInvertRGBColor:
            return "InvertRGBColorization";
        case MMP_SurfFixedGrid:
            return "FixedGrid";
        case MMP_SurfContourPosNeg:
            return "ContourPositiveNegative";
        case MMP_SurfNumVertices:
            return "NumberVertices";
        case MMP_SurfNumPosTris:
            return "NumberPositiveTriangles";
        case MMP_SurfNumNegTris:
            return "NumberNegativeTriangles";
        case MMP_SurfVertices:
            return "Vertices";
        case MMP_SurfVerticesList:
            return "TriangleList";
        case MMP_SurfNormals:
            return "Normals";
        case MMP_SurfSurfaceValues:
            return "SurfaceValues";
        case MMP_SurfEndpoint1:
            return "Endpoint1";
        case MMP_SurfEndpoint2:
            return "Endpoint2";
		default:
            return "invalid";
    }
}
#pragma mark MMP_OrbitalSetNS
const char * CML_convert(MMP_OrbitalSetNS t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
        case MMP_OrbitalTypeAttr:
            return "Type";
		case MMP_WavefunctionType:
			return "wavefunction";
        case MMP_OrbitalLabel:
            return "Label";
        case MMP_NumAlphaOrbs:
            return "NumberAlpha";
        case MMP_NumBetaOrbs:
            return "NumberBeta";
        case MMP_NumOccAlphaOrbs:
            return "AlphaOccupied";
        case MMP_NumOccBetaOrbs:
            return "BetaOccupied";
        case MMP_NumBasisFunctions:
            return "NumberBasisFunctions";
		case MMP_OrbVectors:
			return "Vectors";
		case MMP_BetaOrbVectors:
			return "BetaVectors";
		case MMP_OrbEnergies:
			return "Energies";
		case MMP_BetaOrbEnergies:
			return "BetaEnergies";
		case MMP_OrbOccupations:
			return "Occupations";
		case MMP_BetaOrbOccupations:
			return "BetaOccupations";
		case MMP_OrbSyms:
			return "Symmetries";
		case MMP_BetaOrbSyms:
			return "BetaSymmetries";
		default:
            return "invalid";
    }
}
#pragma mark MMP_MetaWindowItems
const char * CML_convert(MMP_MetaWindowItems t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
        case MMP_MainWinRect:
            return "MainWindowRectangle";
		case MMP_CoordWinRect:
			return "CoordinateWindowRectangle";
        case MMP_BondsWinRect:
            return "BondsWindowRectangle";
		case MMP_EPlotWinRect:
			return "EnergyWindowRectangle";
		case MMP_FreqWinRect:
			return "FrequencyWindowRectangle";
		case MMP_SurfaceWinRect:
			return "SurfaceWindowRectangle";
		case MMP_PrefsWinRect:
			return "PreferencesWindowRectangle";
		default:
            return "invalid";
    }
}
#pragma mark MMP_InternalCoordNS
const char * CML_convert(MMP_InternalCoordNS t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
        case MMP_MOPacStyleInternals:
            return "MOPacInternalCoordinates";
		case MMP_MOPacStyleCount:
			return "count";
        case MMP_MOPacStyleConnections:
            return "ConnectionList";
		case MMP_MOPacStyleValues:
			return "Values";
		case MMP_MOPacStyleTypes:
			return "Types";
		default:
            return "invalid";
    }
}
#pragma mark MMP_BasisSetNS
const char * CML_convert(MMP_BasisSetNS t)
{       
    switch(t)
    {   //Items for inside the List item in Molecule elements
        case MMP_BasisSetElement:
            return "BasisSet";
		case MMP_BSMapLength:
			return "MapLength";
        case MMP_BSNumShells:
            return "NumberBasisShells";
		case MMP_BSNumFunctions:
			return "NumberBasisFunctions";
		case MMP_BasisShell:
			return "BasisShell";
		case MMP_BSMap:
			return "Map";
		case MMP_BSNucCharge:
			return "NuclearCharge";
		case MMP_BSShellType:
			return "ShellType";
		case MMP_BSNumPrims:
			return "NumberPrimitives";
		case MMP_BSExponent:
			return "Exponent";
		case MMP_BSNormCoef:
			return "NormalizedCoefficient";
		case MMP_BSInputCoef:
			return "InputCoefficient";
		default:
            return "invalid";
    }
}
#pragma mark MMP_InputOptionsNS
const char * CML_convert(MMP_InputOptionsNS t)
{       
    switch(t) {
        case MMP_InputOptionsElement:
            return "InputOptions";
		case MMP_IOControlGroupElement:
			return "ControlGroup";
        case MMP_IOSystemGroupElement:
            return "SystemGroup";
		case MMP_IOBasisGroupElement:
			return "BasisGroup";
		case MMP_IODataGroupElement:
			return "DataGroup";
		case MMP_IOGuessGroupElement:
			return "GuessGroup";
		case MMP_IOSCFGroupElement:
			return "SCFGroup";
		case MMP_IOMP2GroupElement:
			return "MP2Group";
		case MMP_IOHessianGroupElement:
			return "HessianGroup";
		case MMP_IOStatPtGroupElement:
			return "StatPtGroup";
		case MMP_IODFTGroupElement:
			return "DFTGroup";
		case MMP_IOEFPGroupElement:
			return "EFRAGGroup";
		case MMP_IOFMOGroupElement:
			return "FMOGroup";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOControlGroupNS
const char * CML_convert(MMP_IOControlGroupNS t)
{       
    switch(t) {
        case MMP_IOCGSCFType:
            return "SCFType";
		case MMP_IOCGRunType:
			return "RunType";
        case MMP_IOCGExeType:
            return "ExeType";
		case MMP_IOCGMPLevel:
			return "MPLevel";
		case MMP_IOCGCIType:
			return "CIType";
		case MMP_IOCGCCType:
			return "CCType";
		case MMP_IOCGMaxIterations:
			return "MaxIterations";
		case MMP_IOCGCharge:
			return "Charge";
		case MMP_IOCGMultiplicity:
			return "Multiplicity";
		case MMP_IOCGLocalization:
			return "Localization";
		case MMP_IOCGFriend:
			return "Friend";
		case MMP_IOCGPrintOption:
			return "PrintOption";
		case MMP_IOCGTolerance:
			return "IntegralTolerance";
		case MMP_IOCGCutoff:
			return "IntegralCutoff";
		case MMP_IOCGMolPlt:
			return "MolPlt";
		case MMP_IOCGPlotOrb:
			return "PlotOrb";
		case MMP_IOCGAIMPac:
			return "AIMPac";
		case MMP_IOCGRPac:
			return "RPac";
		case MMP_IOCGDFTActive:
			return "DFTActive";
		case MMP_IOCGSphericalHarm:
			return "SphericalHarmonics";
		case MMP_IOCGNormF:
			return "NormalizeF";
		case MMP_IOCGNormP:
			return "NormalizeP";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOSystemGroupNS
const char * CML_convert(MMP_IOSystemGroupNS t)
{       
    switch(t) {
        case MMP_IOSGTimeLimit:
            return "TimeLimit";
		case MMP_IOSGMemory:
			return "Memory";
        case MMP_IOSGTimeUnits:
            return "TimeUnit";
		case MMP_IOSGMemoryUnits:
			return "MemoryUnit";
		case MMP_IOSGKDiag:
			return "KDiag";
		case MMP_IOSGCoreFlag:
			return "Core";
		case MMP_IOSGBalanceType:
			return "LoadBalance";
		case MMP_IOSGXDR:
			return "XDR";
		case MMP_IOSGMemDDI:
			return "DDIMemory";
		case MMP_IOSGParallel:
			return "Parallel";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOBasisGroupNS
const char * CML_convert(MMP_IOBasisGroupNS t)
{       
    switch(t) {
        case MMP_IOBGBasisSet:
            return "BasisSet";
		case MMP_IOBGNumGauss:
			return "NumGauss";
        case MMP_IOBGNumDFuncs:
            return "NumDFunctions";
		case MMP_IOBGNumFFuncs:
			return "NumFFunctions";
		case MMP_IOBGNumPFuncs:
			return "NumPFunctions";
		case MMP_IOBGPolar:
			return "Polarization";
		case MMP_IOBGECPPotential:
			return "ECPotential";
		case MMP_IOBGDiffuseSP:
			return "DiffuseSP";
		case MMP_IOBGDiffuseS:
			return "DiffuseS";
		case MMP_IOBGDisableBS:
			return "ExternalBasis";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IODataGroupNS
const char * CML_convert(MMP_IODataGroupNS t)
{       
    switch(t) {
        case MMP_IODGTitle:
            return "Title";
		case MMP_IODGCoordType:
			return "CoordType";
        case MMP_IODGNumZVars:
            return "NumZVars";
		case MMP_IODGPointGroup:
			return "PointGroup";
		case MMP_IODGPointGroupOrder:
			return "PointGroupOrder";
		case MMP_IODGUnits:
			return "Units";
		case MMP_IODGNoSymFlag:
			return "NoSym";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOGuessGroupNS
const char * CML_convert(MMP_IOGuessGroupNS t)
{       
    switch(t) {
        case MMP_IOGGGuessType:
            return "GuessType";
		case MMP_IOGGNumOrbs:
			return "OrbitalCount";
        case MMP_IOGGPrintMO:
            return "PrintInitialMOs";
		case MMP_IOGGVecSource:
			return "VectorSource";
		case MMP_IOGGOrbReorder:
			return "ReorderOrbitals";
		case MMP_IOGGOrbMix:
			return "MixOrbitals";
		case MMP_IOGGMOTolZ:
			return "MOTolZ";
		case MMP_IOGGMOTolEquil:
			return "MOTolEquil";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOSCFGroupNS
const char * CML_convert(MMP_IOSCFGroupNS t)
{       
    switch(t) {
        case MMP_IOSGDirectSCF:
            return "DirectSCF";
		case MMP_IOSGFockDiff:
			return "FockDiff";
        case MMP_IOSGUHFNauralOrbitals:
            return "UHFNaturalOrbitals";
		case MMP_IOSGConvCriteria:
			return "ConverganceCriteria";
		case MMP_IOSGExtrap:
			return "Extrap";
		case MMP_IOSGDamp:
			return "Damp";
		case MMP_IOSGShift:
			return "Shift";
		case MMP_IOSGRestriction:
			return "Restrict";
		case MMP_IOSGDIIS:
			return "DIIS";
		case MMP_IOSGSOSCF:
			return "SOSCF";
		case MMP_IOSGDEM:
			return "DEM";
		case MMP_IOSGGVBNumCoreOrbs:
			return "GVBCoreObitalCount";
		case MMP_IOSGGVBNumPairs:
			return "GVBPairCount";
		case MMP_IOSGGVBNumOpenShells:
			return "GVBOpenShellCount";
		case MMP_IOSGGVBOpenShellDeg:
			return "GVBOpenShellDegeneracy";
		case MMP_IOSCFArrayElement:
			return "array";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOMP2GroupNS
const char * CML_convert(MMP_IOMP2GroupNS t)
{       
    switch(t) {
        case MMP_IOMGCutOff:
            return "IntegralCutOff";
		case MMP_IOMGNumCoreElectrons:
			return "NumCoreElectrons";
        case MMP_IOMGMemory:
            return "Memory";
		case MMP_IOMGTransMethod:
			return "TransformationMethod";
		case MMP_IOMGAOInts:
			return "AOIntStorage";
		case MMP_IOMGLMOMP2:
			return "UseLocalizedOrbitals";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOHessGroupNS
const char * CML_convert(MMP_IOHessGroupNS t)
{       
    switch(t) {
        case MMP_IOHGDisplacementSize:
            return "DisplacementSize";
		case MMP_IOHGFrequencyScaleFactor:
			return "FrequencyScaleFactor";
        case MMP_IOHGMethod:
            return "Method";
		case MMP_IOHGDoubleDiff:
			return "DoubleDifference";
		case MMP_IOHGPurify:
			return "Purify";
		case MMP_IOHGInternalFC:
			return "PrintInternalFC";
		case MMP_IOHGVibAnalysis:
			return "VibrationalAnalysis";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IODFTGroupNS
const char * CML_convert(MMP_IODFTGroupNS t)
{       
    switch(t) {
        case MMP_IODFTGGridSwitch:
            return "GridSwitch";
		case MMP_IODFTThreshold:
			return "Threshold";
        case MMP_IODFTFunctional:
            return "Functional";
		case MMP_IODFTNumRadialGrids:
			return "NumRadialGrids";
		case MMP_IODFTNumThetaGrids:
			return "NumThetaGrids";
		case MMP_IODFTNumPhiGrids:
			return "NumPhiGrids";
		case MMP_IODFTNumRadialGridsInit:
			return "NumRadialGridsInit";
		case MMP_IODFTNumThetaGridsInit:
			return "NumThetaGridsInit";
		case MMP_IODFTNumPhiGridsInit:
			return "NumPhiGridsInit";
		case MMP_IODFTGridMethod:
			return "GridBased";
		case MMP_IODFTGetAuxFunctions:
			return "GetAuxFunctions";
		case MMP_IODFTThree:
			return "Three";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOStatPtGroupNS
const char * CML_convert(MMP_IOStatPtGroupNS t)
{       
    switch(t) {
        case MMP_IOSPGOptConvergance:
            return "ConverganceCriteria";
		case MMP_IOSPGInitTrustRadius:
			return "InitialTrustRadius";
        case MMP_IOSPGMaxTrustRadius:
            return "MaxTrustRadius";
		case MMP_IOSPGMinTrustRadius:
			return "MinTrustRadius";
		case MMP_IOSPGStatJumpSize:
			return "JumpSize";
		case MMP_IOSPGModeFollow:
			return "ModeToFollow";
		case MMP_IOSPGRadiusUpdate:
			return "RadiusUpdate";
		case MMP_IOSPGStatPoint:
			return "StationaryPoint";
		case MMP_IOSPGHessMethod:
			return "HessianMethod";
		case MMP_IOSPGMethod:
			return "Method";
		case MMP_IOSPGMaxSteps:
			return "MaximumSteps";
		case MMP_IOSPGnRecalcHess:
			return "RecalcHessianSteps";
		case MMP_IOSPGAlwaysPrintOrbs:
			return "AlwaysPrintOrbitals";
		case MMP_IOSPGHessEnd:
			return "HessianAtEnd";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOEFPGroupNS
const char * CML_convert(MMP_IOEFPGroupNS t)
{       
    switch(t) {
        case MMP_IOEFPCoordType:
            return "CoordType";
		case MMP_IOEFPPolMethod:
			return "PolMethod";
        case MMP_IOEFPPosition:
            return "Position";
		case MMP_IOEFPMaxBasisFuncs:
			return "MaxBasisFuncs";
		case MMP_IOEFPMaxMOs:
			return "MaxMOs";
		case MMP_IOEFPNumBufferMOs:
			return "NumBufferMOs";
		default:
            return "invalid";
    }
}
#pragma mark MMP_IOFMOGroupNS
const char * CML_convert(MMP_IOFMOGroupNS t)
{       
    switch(t) {
        case MMP_IOFMOActiveFlag:
            return "FMOActive";
		case MMP_IOFMONumFragments:
			return "NumberFMOFragments";
		case MMP_IOFMONBODY:
			return "NBody";
		case MMP_IOFMOOutputStyleFlag:
			return "INDATStyle";
		case MMP_IOFMOFramentArray:
			return "FragmentIdArray";
		case MMP_IOFMOFragmentNames:
			return "FragmentNames";
		case MMP_IOFMOFragmentCharges:
			return "FragmentCharges";
		case MMP_IOFMOFragmentMultArray:
			return "FragmentMultiplicities";
		case MMP_IOFMOFragmentSCFArray:
			return "FragmentSCF";
		case MMP_IOFMOLayerCount:
			return "LayerCount";
		case MMP_IOFMOMPArray:
			return "LayerMPn";
		case MMP_IOFMODFTArray:
			return "LayerDFT";
		case MMP_IOFMOSCFArray:
			return "LayerSCF";
		case MMP_IOFMOCCArray:
			return "LayerCC";
		case MMP_IOFMOTDArray:
			return "LayerTDDFT";
		default:
            return "invalid";
    }
}
#pragma mark MMP_WindowDataNS
const char * CML_convert(MMP_WindowDataNS t)
{       
    switch(t) {
        case MMP_WinDataMolWin:
            return "MoleculeDisplayWindow";
		case MMP_WinDataBondsWin:
			return "BondsWindow";
        case MMP_WinDataCoordsWin:
            return "CoordinatesWindow";
		case MMP_WinDataEnergyWin:
			return "EnergyPlotWindow";
		case MMP_WinDataFreqWin:
			return "FrequencyWindow";
		case MMP_WinDataSurfacesWin:
			return "SurfacesWindow";
		case MMP_WinDataInputBWin:
			return "InputBuilderWindow";
		case MMP_WinDataPrefWin:
			return "PreferencesWindow";
		case MMP_WinDataZMatWin:
			return "ZMatrixWindow";
		default:
            return "invalid";
    }
}
#pragma mark MMP_AnnotationTypesNS
const char * CML_convert(MMP_AnnotationTypesNS t)
{       
    switch(t) {
        case MMP_AnnotationMarker:
            return "Marker";
        case MMP_AnnotationLength:
            return "Length";
		case MMP_AnnotationAngle:
			return "Angle";
        case MMP_AnnotationDihedral:
            return "Dihedral";
		default:
            return "invalid";
    }
}
const char * SurfaceTypeToText(const SurfaceType & s)
{       
    switch(s)
    {   //Items for inside the List item in Molecule elements
		case kOrb3DType:
			return "Orbital3D";
        case kOrb2DType:
            return "Orbital2D";
        case kTotalDensity3D:
            return "TotalElectronDensity3D";
        case kTotalDensity2D:
            return "TotalElectronDensity2D";
		case kMEP2D:
			return "MolecularElectrostaticPotential2D";
		case kMEP3D:
			return "MolecularElectrostaticPotential3D";
		case kSolventType3D:
			return "Solvent3D";
		case kGeneral2DSurface:
			return "General2D";
		case kGeneral3DSurface:
			return "General3D";
        case kOrb1DType:
            return "Orbital1D";
        case kTotalDensity1D:
            return "TotalElectronDensity1D";
		case kMEP1D:
			return "MolecularElectrostaticPotential1D";
		case kGeneral1D:
			return "General1D";
		default:
            return "invalid";
    }
}  

#define MATCH(s, t, j, k, type)                                            \
if (!s || !*s) return false;                                             \
for (int i = (int) j; i != (int) k; ++i)                                 \
{                                                                        \
    if (strcmp(s, CML_convert((type) i)) == 0)                             \
    {                                                                      \
		t = (type) i;                                                        \
			return true;                                                     \
    }                                                                      \
}                                                                        \
return false;                                                            \

bool CML_convert(const char * s, CML_Element & t)
{
    MATCH(s, t, 0, NumberCMLElements, CML_Element)
}
bool CML_convert(const char * s, CML_Attribute & t)
{
    MATCH(s, t, 0, NumberCMLAttributes, CML_Attribute)
}
bool CML_convert(const char * s, MMP_NameSpace & t)
{
    MATCH(s, t, 0, NumberMMPNames, MMP_NameSpace)
}
bool CML_convert(const char * s, MMP_VibRecNameSpace & t)
{
    MATCH(s, t, 0, NumberMMPVibRecItems, MMP_VibRecNameSpace)
}
bool CML_convert(const char * s, MMP_MetadataNamespace & t)
{
    MATCH(s, t, 0, NumberMMPMetadataNames, MMP_MetadataNamespace)
}
bool CML_convert(const char * s, MMP_MolListNameSpace & t)
{
    MATCH(s, t, 0, NumberMMPMolListItems, MMP_MolListNameSpace)
}
bool CML_convert(const char * s, MMP_SurfaceNameSpace & t)
{
    MATCH(s, t, 0, NumberMMPSurfaceItems, MMP_SurfaceNameSpace)
}
bool CML_convert(const char * s, MMP_OrbitalSetNS & t)
{
    MATCH(s, t, 0, NumberMMPOrbitalSetNSItems, MMP_OrbitalSetNS)
}
bool CML_convert(const char * s, MMP_MetaWindowItems & t)
{
    MATCH(s, t, 0, NumberMMPMetaWindowItems, MMP_MetaWindowItems)
}
bool CML_convert(const char * s, MMP_InternalCoordNS & t)
{
    MATCH(s, t, 0, NumberMMPInternalCoordItems, MMP_InternalCoordNS)
}
bool CML_convert(const char * s, MMP_BasisSetNS & t)
{
    MATCH(s, t, 0, NumberMMPBasisSetItems, MMP_BasisSetNS)
}
bool CML_convert(const char * s, MMP_InputOptionsNS & t)
{
    MATCH(s, t, 0, NumberMMPInputOptions, MMP_InputOptionsNS)
}
bool CML_convert(const char * s, MMP_IOControlGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOControlGroupItems, MMP_IOControlGroupNS)
}
bool CML_convert(const char * s, MMP_IOSystemGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOSystemGroupItems, MMP_IOSystemGroupNS)
}
bool CML_convert(const char * s, MMP_IOBasisGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOBasisGroupItems, MMP_IOBasisGroupNS)
}
bool CML_convert(const char * s, MMP_IODataGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIODataGroupItems, MMP_IODataGroupNS)
}
bool CML_convert(const char * s, MMP_IOGuessGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOGuessGroupItems, MMP_IOGuessGroupNS)
}
bool CML_convert(const char * s, MMP_IOSCFGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOSCFGroupItems, MMP_IOSCFGroupNS)
}
bool CML_convert(const char * s, MMP_IOMP2GroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOMP2GroupItems, MMP_IOMP2GroupNS)
}
bool CML_convert(const char * s, MMP_IOHessGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOHessGroupItems, MMP_IOHessGroupNS)
}
bool CML_convert(const char * s, MMP_IODFTGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIODFTGroupItems, MMP_IODFTGroupNS)
}
bool CML_convert(const char * s, MMP_IOStatPtGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOStatPtGroupItems, MMP_IOStatPtGroupNS)
}
bool CML_convert(const char * s, MMP_IOEFPGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOEFPGroupItems, MMP_IOEFPGroupNS)
}
bool CML_convert(const char * s, MMP_IOFMOGroupNS & t)
{
    MATCH(s, t, 0, NumberMMPIOFMOGroupItems, MMP_IOFMOGroupNS)
}
bool CML_convert(const char * s, MMP_WindowDataNS & t)
{
    MATCH(s, t, 0, NumberMMPWindowDataItems, MMP_WindowDataNS)
}
bool CML_convert(const char * s, MMP_AnnotationTypesNS & t)
{
    MATCH(s, t, 0, NumberMMPAnnotationTypes, MMP_AnnotationTypesNS)
}
bool TextToSurfaceType(const char * t, SurfaceType & s) {
	if (!t || !*t) return false;
	for (int i = 0; i != NumSurfaceTypes; ++i)
	{
		if (strcmp(t, SurfaceTypeToText((SurfaceType) i)) == 0)
		{
			s = (SurfaceType) i;
			return true;
		}
	}
	return false;
}
#pragma mark -
bool CML_convert(const char * s, BondOrder & b) {
	if (!s) return false;
	switch (s[0]) {
		case '1':
		case 'S':
			b = kSingleBond;
			break;
		case '2':
		case 'D':
			b = kDoubleBond;
			break;
		case '3':
		case 'T':
			b = kTripleBond;
			break;
		case '\0':
			b = kUnknownBond;
			break;
		case 'H':
			b = kHydrogenBond;
			break;
		case 'A':
			b = kAromaticBond;
			break;
		case 'M':
			b = kMixedBonds;
			break;
		default:
			b = kUnknownBond;
			return false;
	}
	return true;
}

const char * CML_TypetoName(short t) {
	switch (t) {
		case 1:
			return "H";
		case 2:
			return "He";
		case 3:
			return "Li";
		case 4:
			return "Be";
		case 5:
			return "B";
		case 6:
			return "C";
		case 7:
			return "N";
		case 8:
			return "O";
		case 9:
			return "F";
		case 10:
			return "Ne";
		case 11:
			return "Na";
		case 12:
			return "Mg";
		case 13:
			return "Al";
		case 14:
			return "Si";
		case 15:
			return "P";
		case 16:
			return "S";
		case 17:
			return "Cl";
		case 18:
			return "Ar";
		case 19:
			return "K";
		case 20:
			return "Ca";
		case 21:
			return "Sc";
		case 22:
			return "Ti";
		case 23:
			return "V";
		case 24:
			return "Cr";
		case 25:
			return "Mn";
		case 26:
			return "Fe";
		case 27:
			return "Co";
		case 28:
			return "Ni";
		case 29:
			return "Cu";
		case 30:
			return "Zn";
		case 31:
			return "Ga";
		case 32:
			return "Ge";
		case 33:
			return "As";
		case 34:
			return "Se";
		case 35:
			return "Br";
		case 36:
			return "Kr";
		case 37:
			return "Rb";
		case 38:
			return "Sr";
		case 39:
			return "Y";
		case 40:
			return "Zr";
		case 41:
			return "Nb";
		case 42:
			return "Mo";
		case 43:
			return "Tc";
		case 44:
			return "Ru";
		case 45:
			return "Rh";
		case 46:
			return "Pd";
		case 47:
			return "Ag";
		case 48:
			return "Cd";
		case 49:
			return "In";
		case 50:
			return "Sn";
		case 51:
			return "Sb";
		case 52:
			return "Te";
		case 53:
			return "I";
		case 54:
			return "Xe";
		case 55:
			return "Cs";
		case 56:
			return "Ba";
		case 57:
			return "La";
		case 58:
			return "Ce";
		case 59:
			return "Pr";
		case 60:
			return "Nd";
		case 61:
			return "Pm";
		case 62:
			return "Sm";
		case 63:
			return "Eu";
		case 64:
			return "Gd";
		case 65:
			return "Tb";
		case 66:
			return "Dy";
		case 67:
			return "Ho";
		case 68:
			return "Er";
		case 69:
			return "Tm";
		case 70:
			return "Yb";
		case 71:
			return "Lu";
		case 72:
			return "Hf";
		case 73:
			return "Ta";
		case 74:
			return "W";
		case 75:
			return "Re";
		case 76:
			return "Os";
		case 77:
			return "Ir";
		case 78:
			return "Pt";
		case 79:
			return "Au";
		case 80:
			return "Hg";
		case 81:
			return "Tl";
		case 82:
			return "Pb";
		case 83:
			return "Bi";
		case 84:
			return "Po";
		case 85:
			return "At";
		case 86:
			return "Rn";
		case 87:
			return "Fr";
		case 88:
			return "Ra";
		case 89:
			return "Ac";
		case 90:
			return "Th";
		case 91:
			return "Pa";
		case 92:
			return "U";
		case 93:
			return "Np";
		case 94:
			return "Pu";
		case 95:
			return "Am";
		case 96:
			return "Cm";
		case 97:
			return "Bk";
		case 98:
			return "Cf";
		case 99:
			return "Es";
		case 100:
			return "Fm";
		case 101:
			return "Md";
		case 102:
			return "No";
		case 103:
			return "Lr";
		case 104:
			return "Rf";
		case 105:
			return "Db";
		case 106:
			return "Sg";
		case 107:
			return "Bh";
		case 108:
			return "Hs";
		case 109:
			return "Mt";
		case 110:
			return "Ds";
		case 111:
			return "Rg";
		case 112:
			return "Cn";
		case 113:
			return "Uut";
		case 114:
			return "Fl";
		case 115:
			return "Uup";
		case 116:
			return "Lv";
		case 117:
			return "Uus";
		case 118:
			return "Uuo";
	//Need to handle Dummy and group attachments
	}
	return "";
}
