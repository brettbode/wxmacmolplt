#include "MyTypes.h"
#include "Frame.h"
#include <iostream>

/* ------------------------------------------------------------------------- */

Annotation::Annotation(const unsigned int natoms,
					   const long atom1_id, const long atom2_id,
					   const long atom3_id, const long atom4_id)
	: natoms(natoms) {

	atoms[0] = atom1_id;
	atoms[1] = atom2_id;
	atoms[2] = atom3_id;
	atoms[3] = atom4_id;

	switch (natoms) {
		case 1:
			type = MP_ANNOTATION_MARKER;
			break;
		case 2:
			type = MP_ANNOTATION_LENGTH;
			break;
		case 3:
			type = MP_ANNOTATION_ANGLE;
			break;
		case 4:
			type = MP_ANNOTATION_DIHEDRAL;
			break;
		default:
			type = MP_ANNOTATION;
	}

}

/* ------------------------------------------------------------------------- */

bool Annotation::containsAtom(const int atom_id) const {

	unsigned int i;
	for (i = 0; i < natoms; i++) {
		if (atoms[i] == atom_id) {
			return true;
		}
	}
	return false;

}

/* ------------------------------------------------------------------------- */

void Annotation::adjustIds(const int atom_id, int offset) {

	unsigned int i;
	for (i = 0; i < natoms; i++) {
		if (atoms[i] > atom_id) atoms[i] += offset;
	}

}

/* ------------------------------------------------------------------------- */

int Annotation::getAtom(const int index) const {

	if (index < 0 || index >= natoms) return -1;
	return atoms[index];

}

/* ------------------------------------------------------------------------- */

int Annotation::getType() const {
	return type;
}

/* ------------------------------------------------------------------------- */

bool Annotation::containsFragment(const mpAtom *atom_list) const {

	unsigned int i;
	for (i = 0; i < natoms; i++) {
		if (atom_list[atoms[i]].IsEffectiveFragment()) {
			return true;
		}
	}
	return false;

}

/* ------------------------------------------------------------------------- */

AnnotationLength::AnnotationLength(const long atom1_id, const long atom2_id)
	: Annotation(2, atom1_id, atom2_id) {
}

/* ------------------------------------------------------------------------- */

bool AnnotationLength::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 2 &&
		   ((new_list[0] == atoms[0] && new_list[1] == atoms[1]) ||
			(new_list[1] == atoms[0] && new_list[0] == atoms[1]));
}

/* ------------------------------------------------------------------------- */

float AnnotationLength::getParam(const Frame& frame) const {

	// This function returns the distance between the two atoms of this
	// annotation.
	
	CPoint3D pt1, pt2;

	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);

	return (pt2 - pt1).Magnitude();
}

/* ------------------------------------------------------------------------- */

void AnnotationLength::setParam(Frame& frame, float value) {

	// This function exactly sets the distance between the two atoms of this
	// annotation by shifting the position of the second atom the appropriate
	// length.
	
	CPoint3D pt1, pt2, vec;

	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);

	// Find the vector from the fixed first atom to the second.  Then move
	// the second atom along this vector the appropriate length.
	vec = pt2 - pt1;
	Normalize3D(&vec);
	vec = (pt1 + vec * value) - pt2;

	// The first atom must stay put for the annotation adjustment to be of any
	// merit.  So, we deselect it explicitly, which may surprise the user.
	// I think this is what the user would really want, however.  We also
	// select the second atom explicitly, 'cuz this is the atom that should
	// move.
	frame.SetAtomSelection(atoms[0], false);
	frame.SetAtomSelection(atoms[1], true);

	for (int i = 0; i < frame.GetNumAtoms(); i++) {
		if (frame.GetAtomSelection(i)) {
			frame.GetAtomPosition(i, pt1);
			frame.SetAtomPosition(i, pt1 + vec);
		}
	}

}

/* ------------------------------------------------------------------------- */

AnnotationMarker::AnnotationMarker(const long atom_id)
	: Annotation(1, atom_id) {
}

/* ------------------------------------------------------------------------- */

bool AnnotationMarker::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 1 && new_list[0] == atoms[0];
}

/* ------------------------------------------------------------------------- */

float AnnotationMarker::getParam(const Frame& frame) const {
	return 0.0f;
}

/* ------------------------------------------------------------------------- */

void AnnotationMarker::setParam(Frame& frame, float value) {
}

/* ------------------------------------------------------------------------- */

AnnotationAngle::AnnotationAngle(const long atom1_id, const long atom2_id,
								 const long atom3_id)
	: Annotation(3, atom1_id, atom2_id, atom3_id) { 
}

/* --------------------------------------------------------------------- */

bool AnnotationAngle::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 3 && new_list[1] == atoms[1] &&
		   ((new_list[0] == atoms[0] && new_list[2] == atoms[2]) ||
			(new_list[2] == atoms[0] && new_list[0] == atoms[2]));
}

/* ------------------------------------------------------------------------- */

float AnnotationAngle::getParam(const Frame& frame) const {

	// This function returns the angle in degrees between the three atoms
	// of this annotation.

	CPoint3D pt1, pt2, pt3;
	CPoint3D vec1;
	CPoint3D vec2;
	float angle;

	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);
	frame.GetAtomPosition(atoms[2], pt3);

	// First we get vectors representing the angle.
	vec1 = pt1 - pt2;
	vec2 = pt3 - pt2;

	Normalize3D(&vec1);
	Normalize3D(&vec2);

	float dot = DotProduct3D(&vec1, &vec2);
	if (fabs(dot) < 0.999999f) {
		angle = acos(dot);
	} else {
		angle = dot > 0.0f ? 0.0f : kPi;
	}

	return angle * 180.0f / kPi;

}

/* ------------------------------------------------------------------------- */

void AnnotationAngle::setParam(Frame& frame, float value) {

	// This function sets exactly the angle between the three atoms of this
	// annotation by rotating the third atom to appropriate position.  value
	// should be in degrees.

	CPoint3D pt1, pt2, pt3;
	CPoint3D vec1;
	CPoint3D vec2;
	CPoint3D normal;
	Matrix4D rotate;

	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);
	frame.GetAtomPosition(atoms[2], pt3);

	// First we get vectors representing the angle.
	vec1 = pt1 - pt2;
	vec2 = pt3 - pt2;

	// We rotate the third atom by rotating the fixed vector (from first atom
	// to vertex) of the angle the appropriate number of degrees and then
	// pushing the atom along it the same length that it was to begin with.
	// So, we don't even really consider the vector from the third atom to the
	// angle's vertex.
	Normalize3D(&vec1);
	Normalize3D(&vec2);

	// If the atoms aren't collinear, the axis of rotation is the normal of the
	// plane formed by the three atoms.
	if (fabs(DotProduct3D(&vec1, &vec2)) < 0.999999f) {
		CrossProduct3D(&vec1, &vec2, &normal);
		Normalize3D(&normal);
	}
	
	// Otherwise the atoms are collinear, and there are an infinite number of
	// planes whose normal we could use.  The rotation is ambiguous, so we pick
	// an arbitrary normal.
	else {
		OrthoVector(vec1, normal);
	}

	CPoint3D new_vec;
	float angle_diff = value - getParam(frame);

	RotateAroundAxis(rotate, normal, angle_diff);

	// The first atom must stay put for the annotation adjustment to be of any
	// merit.  So, we deselect it explicitly, which may surprise the user.
	// I think this is what the user would really want, however.  We also
	// select the third atom explicitly, 'cuz this is the atom that should
	// move.  The second atom, the vertex of the angle, doesn't need to be
	// selected though it shouldn't hurt if it is since it is the origin of
	// the rotation.
	frame.SetAtomSelection(atoms[0], false);
	frame.SetAtomSelection(atoms[1], false);
	frame.SetAtomSelection(atoms[2], true);

	for (int i = 0; i < frame.GetNumAtoms(); i++) {
		if (frame.GetAtomSelection(i)) {
			frame.GetAtomPosition(i, pt3);
			vec2 = pt3 - pt2;
			Rotate3DPt(rotate, vec2, &new_vec);
			frame.SetAtomPosition(i, pt2 + new_vec);
		}
	}

}

/* ------------------------------------------------------------------------- */

AnnotationDihedral::AnnotationDihedral(const long atom1_id,
									   const long atom2_id,
									   const long atom3_id,
									   const long atom4_id)
	: Annotation(4, atom1_id, atom2_id, atom3_id, atom4_id) { 
}

/* ------------------------------------------------------------------------- */

bool AnnotationDihedral::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 4 &&
			new_list[2] == atoms[2] &&
			new_list[1] == atoms[1] &&
			((new_list[3] == atoms[3] && new_list[0] == atoms[0]) ||
			 (new_list[3] == atoms[0] && new_list[0] == atoms[3]));
}

/* ------------------------------------------------------------------------- */

float AnnotationDihedral::getParam(const Frame& frame) const {

	// This function returns the dihedral angle between the planes of the four
	// atoms in this annotation.   The first three atoms (1, 2, 3) form one
	// plane.  The last three (2, 3, 4) form the second plane.  We want to find
	// the angle between those two planes -- i.e., the angle between the
	// plane's normals -- for that is the dihedral angle.  The angle is 
	// returned in degrees, not radians.

	CPoint3D atom1_pos;
	CPoint3D atom2_pos;
	CPoint3D atom3_pos;
	CPoint3D atom4_pos;
	CPoint3D vec1;
	CPoint3D vec2;
	CPoint3D vec3;
	CPoint3D normal1;
	CPoint3D normal2;

	frame.GetAtomPosition(atoms[0], atom1_pos);
	frame.GetAtomPosition(atoms[1], atom2_pos);
	frame.GetAtomPosition(atoms[2], atom3_pos);
	frame.GetAtomPosition(atoms[3], atom4_pos);
	
	// Find the first plane's normal by finding the vectors between its
	// points and calculating the normal from them.
	vec1 = atom1_pos - atom3_pos;
	vec2 = atom2_pos - atom3_pos;
	CrossProduct3D(&vec1, &vec2, &normal1);
	Normalize3D(&normal1);

	// Find the second plane.  The two planes share a vector between
	// atoms 2 and 3, so we just reuse that from the first plane.
	vec3 = atom4_pos - atom3_pos;
	CrossProduct3D(&vec3, &vec2, &normal2);
	Normalize3D(&normal2);

	// Sometimes the dot product can get out of range, making the acos function
	// very unhappy.  We must cap it.
	float dot = DotProduct3D(&normal1, &normal2);
	if (dot < -1.0f) {
		dot = -1.0f;
	} else if (dot > 1.0f) {
		dot = 1.0f;
	}

	return acos(dot) * 180.0f / kPi;

}

/* ------------------------------------------------------------------------- */

void AnnotationDihedral::setParam(Frame& frame, float value) {

	// This function sets the dihedral angle exactly by rotating the fourth
	// atom around the annotation planes' shared axis the appropriate number
	// of degrees.

	CPoint3D atom1_pos;
	CPoint3D atom2_pos;
	CPoint3D atom3_pos;
	CPoint3D atom4_pos;
	CPoint3D pt;
	CPoint3D axis;
	CPoint3D vec1;
	CPoint3D vec2;
	CPoint3D rotated_vec;
	CPoint3D normal1;
	CPoint3D normal2;
	Matrix4D rotate;

	frame.GetAtomPosition(atoms[0], atom1_pos);
	frame.GetAtomPosition(atoms[1], atom2_pos);
	frame.GetAtomPosition(atoms[2], atom3_pos);
	frame.GetAtomPosition(atoms[3], atom4_pos);

	// The angle's central axis is between the 2nd and 3rd atoms.
	axis = atom2_pos - atom3_pos;
	Normalize3D(&axis);

	// The vector we want to rotate is from the 3rd to the 4th atoms.
	vec2 = atom4_pos - atom3_pos;

	// The axis we want to rotate around is between the 2nd and 3rd atoms,
	// but we have to get the direction of the vector right.  So, instead
	// of using axis, we find the two planes' normals and use their cross
	// product as the axis of rotation.
	vec1 = atom1_pos - atom3_pos;

	CrossProduct3D(&vec1, &axis, &normal1);
	Normalize3D(&normal1);

	CrossProduct3D(&axis, &vec2, &normal2);
	Normalize3D(&normal2);

	if (fabs(DotProduct3D(&normal1, &normal2)) < 0.999999f) {
		CrossProduct3D(&normal1, &normal2, &axis);
		Normalize3D(&axis);
	}

	RotateAroundAxis(rotate, axis, getParam(frame) - value);

	// Rotate3DPt(rotate, vec2, &rotated_vec); 

	// Now we push the fourth atom away from the third along the rotated
	// form of the vector between them.
	// frame.SetAtomPosition(atoms[3], atom3_pos + rotated_vec); 

	frame.SetAtomSelection(atoms[0], false);
	frame.SetAtomSelection(atoms[1], false);
	frame.SetAtomSelection(atoms[2], false);
	frame.SetAtomSelection(atoms[3], true);

	CPoint3D new_vec;
	for (int i = 0; i < frame.GetNumAtoms(); i++) {
		if (frame.GetAtomSelection(i)) {
			frame.GetAtomPosition(i, pt);
			vec2 = pt - atom3_pos;
			Rotate3DPt(rotate, vec2, &new_vec);
			frame.SetAtomPosition(i, atom3_pos + new_vec);
		}
	}

}

/*void mpAtom::SetDefaultHybridization(void) {
	//Setup the default orbital hybridization based on the atom type
	hybridization = OH_NONE;
	if (Type == 15) {
		hybridization = OH_SP3D;
		return;
	}
	if (Type == 16) {
		hybridization = OH_SP3D2;
		return;
	}
	if (((Type>=6)&&(Type<=9))||((Type>=14)&&(Type<=17))||((Type>=32)&&(Type<=35))||
		((Type>=50)&&(Type<=53))||((Type>=82)&&(Type<=85))) {
		hybridization = OH_SP3;
		return;
	}

	switch (Type) {
		case 1:
		case 3:
		case 11:
		case 19:
		case 37:
		case 55:
		case 87:
			hybridization = OH_S;
			break;
		case 4:
		case 12:
		case 20:
		case 38:
		case 56:
		case 88:
			hybridization = OH_SP;
			break;
		case 5:
		case 13:
		case 31:
		case 49:
		case 81:
			hybridization = OH_SP2;
			break;
	}
}
*/
static short gCoordination[] = {1,                                0,
								1,2,                    3,4,3,2,1,0,
								1,2,                    3,4,5,6,1,0,
								1,2,3,2,5,3,2,3,2,2,2,2,3,4,3,4,1,0,
								1,2,3,4,5,6,6,3,3,2,1,2,3,4,3,4,1,0,
								1,2,
									3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
									  4,5,6,6,4,4,4,3,2,1,2,3,2,1,0,
								1,2,
									3,4,5,6,5,4,3,3,3,3,3,3,3,3,3,
									  4,0,6,0,3,0,0,0,0};

void mpAtom::SetDefaultCoordinationNumber(void) {
	//Setup the default coordination based on the atom type
	coordinationNumber = 0;
	if ((Type>0)&&(Type<=112)) coordinationNumber = gCoordination[Type-1];
}

void mpAtom::SetDefaultLonePairCount(void) {
	LPCount = 0;
	switch (Type) {
		case 7:
		case 22:
		case 33:
		case 51:
		case 83:
			LPCount = 1;
			break;
		case 8:
		case 24:
		case 28:
		case 29:
		case 30:
		case 46:
		case 47:
		case 48:
		case 84:
			LPCount = 2;
			break;
		case 9:
		case 17:
		case 25:
		case 26:
		case 27:
		case 35:
		case 44:
		case 45:
		case 53:
		case 85:
			LPCount = 3;
			break;
		case 10:
		case 18:
		case 36:
		case 54:
		case 86:
			LPCount = 4;
			break;
	}
}

// --------------------------------------------------------------------------- 

std::ostream& operator<<(std::ostream& stream, const mpAtom& a) {
	
	stream << "Atomic number: " << a.Type << ", Position: " << a.Position;
	return stream;

}

// --------------------------------------------------------------------------- 

std::ostream& operator<<(std::ostream& stream, const Bond& b) {
	
	stream << "Atom1: " << b.Atom1 << ", Atom2: " << b.Atom2 <<
			  ", Order: " << b.Order;
	return stream;

}

/* ------------------------------------------------------------------------- */

EFrag::EFrag(const std::string& text)
	: raw_text(text) {

	ParseText();

}

/* ------------------------------------------------------------------------- */

void EFrag::ParseText() {

	CPoint3D pos;
	char label[kMaxLineLength];
	char line[kMaxLineLength];
	int atomic_number;
	float tmp1;
	float tmp2;

	char *buf_input = new char[raw_text.length()];
	memcpy(buf_input, raw_text.c_str(), sizeof(char) * raw_text.length());
	BufferFile buf(buf_input, raw_text.length());

	// Skip group name, title, and COORDINATES.
	buf.SkipnLines(3);

	// Now read lines that start with A.  Stop after the first one
	// that doesn't start with A.
	do {
		buf.GetLine(line);
		sscanf(line, "%s %f %f %f %f %f", label,
			   &pos.x, &pos.y, &pos.z, &tmp1, &tmp2);
		atomic_number = (int) tmp2;
		if (atomic_number) {
			// Input is in atomic units; move to angstroms.
			pos *= kBohr2AngConversion;
			labeled_atoms.push_back(EFragAtom(label, pos, atomic_number));
		}
	} while (atomic_number);

	delete[] buf_input;

}

/* ------------------------------------------------------------------------- */

const std::vector<EFragAtom>& EFrag::GetAtoms() const {

	return labeled_atoms;

}

/* ------------------------------------------------------------------------- */

const std::string& EFrag::GetText() const {

	return raw_text;

}

/* ------------------------------------------------------------------------- */

