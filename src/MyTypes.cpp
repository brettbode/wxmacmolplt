#include "MyTypes.h"
#include "Frame.h"

AnnotationLength::AnnotationLength(void) : Annotation() {
	atoms[0] = -1;
	atoms[1] = -1;
};

AnnotationLength::AnnotationLength(const long atom1_id, const long atom2_id) : Annotation() {
	atoms[0] = atom1_id;
	atoms[1] = atom2_id;
}

bool AnnotationLength::containsAtom(const int atom_id) const {
	return ((atoms[0] == atom_id)||(atoms[1] == atom_id));
}

void AnnotationLength::adjustIds(const int atom_id) {
	if (atoms[0] > atom_id) atoms[0]--;
	if (atoms[1] > atom_id) atoms[1]--;
}

bool AnnotationLength::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 2 &&
		   ((new_list[0] == atoms[0] && new_list[1] == atoms[1]) ||
			(new_list[1] == atoms[0] && new_list[0] == atoms[1]));
}

int AnnotationLength::getAtom(const int index) const {
	if (index < 0 || index > 1) return -1;
	return atoms[index];
}

int AnnotationLength::getType(void) const {
	return MP_ANNOTATION_LENGTH;
}

float AnnotationLength::getParam(const Frame& frame) const {

	// This function returns the distance between the two atoms of this
	// annotation.
	
	CPoint3D pt1, pt2;

	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);

	return (pt2 - pt1).Magnitude();
}

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

	if (!frame.GetAtomSelection(atoms[0])) {
		for (int i = 0; i < frame.GetNumAtoms(); i++) {
			if (frame.GetAtomSelection(i)) {
				frame.GetAtomPosition(i, pt1);
				frame.SetAtomPosition(i, pt1 + vec);
			}
		}
	}

}

AnnotationMarker::AnnotationMarker(void) : Annotation() {
	atom = -1;
};

AnnotationMarker::AnnotationMarker(const long atom_id) : Annotation() {
	atom = atom_id;
}

bool AnnotationMarker::containsAtom(const int atom_id) const {
	return atom == atom_id;
}

void AnnotationMarker::adjustIds(const int atom_id) {
	if (atom > atom_id) atom--;
}

bool AnnotationMarker::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 1 && new_list[0] == atom;
}

int AnnotationMarker::getAtom(const int index) const {
	if (index != 0) return -1;
	return atom;
}

int AnnotationMarker::getType(void) const {
	return MP_ANNOTATION_MARKER;
}

float AnnotationMarker::getParam(const Frame& frame) const {
	return 0.0f;
}

void AnnotationMarker::setParam(Frame& frame, float value) {
}

AnnotationAngle::AnnotationAngle(void) : Annotation() {
	atoms[0] = -1;
	atoms[1] = -1;
	atoms[2] = -1;
}

AnnotationAngle::AnnotationAngle(const long atom1_id, const long atom2_id,
	const long atom3_id) : Annotation() { 
	atoms[0] = atom1_id;
	atoms[1] = atom2_id;
	atoms[2] = atom3_id;
}
	
bool AnnotationAngle::containsAtom(const int atom_id) const {
	return ((atoms[0] == atom_id)||(atoms[1] == atom_id)||(atoms[2] == atom_id));
}

void AnnotationAngle::adjustIds(const int atom_id) {
	if (atoms[0] > atom_id) atoms[0] --;
	if (atoms[1] > atom_id) atoms[1] --;
	if (atoms[2] > atom_id) atoms[2] --;
}

bool AnnotationAngle::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 3 && new_list[1] == atoms[1] &&
		   ((new_list[0] == atoms[0] && new_list[2] == atoms[2]) ||
			(new_list[2] == atoms[0] && new_list[0] == atoms[2]));
}

int AnnotationAngle::getAtom(const int index) const {
	if (index < 0 || index > 2) return -1;
	return atoms[index];
}

int AnnotationAngle::getType(void) const {
	return MP_ANNOTATION_ANGLE;
}

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

	angle = acos(DotProduct3D(&vec1, &vec2)) * 180.0f / kPi;

	return angle;

}

#include <iostream>
void AnnotationAngle::setParam(Frame& frame, float value) {

	// This function sets exactly the angle between the three atoms of this
	// annotation by rotating the third atom to appropriate position.  value
	// should be in degrees.

	CPoint3D pt1, pt2, pt3;
	CPoint3D vec1;
	CPoint3D vec2;
	CPoint3D normal;
	float len;
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

	// The axis of rotation is the normal of the plane formed by the three
	// atoms.
	CrossProduct3D(&vec1, &vec2, &normal);
	Normalize3D(&normal);

	CPoint3D new_vec;
	float angle_diff = value - getParam(frame);

	RotateAroundAxis(rotate, normal, angle_diff);

	if (!frame.GetAtomSelection(atoms[0])) {
		for (int i = 0; i < frame.GetNumAtoms(); i++) {
			if (frame.GetAtomSelection(i)) {
				frame.GetAtomPosition(i, pt3);
				vec2 = pt3 - pt2;
				Rotate3DPt(rotate, vec2, &new_vec);
				frame.SetAtomPosition(i, pt2 + new_vec);
			}
		}
	}

}

AnnotationDihedral::AnnotationDihedral(void) : Annotation() {
	atoms[0] = -1;
	atoms[1] = -1;
	atoms[2] = -1;
	atoms[3] = -1;
}

AnnotationDihedral::AnnotationDihedral(const long atom1_id, const long atom2_id, 
	const long atom3_id, const long atom4_id) : Annotation() { 
	atoms[0] = atom1_id;
	atoms[1] = atom2_id;
	atoms[2] = atom3_id;
	atoms[3] = atom4_id;
}

bool AnnotationDihedral::containsAtom(const int atom_id) const {
	return (atoms[0] == atom_id ||
			atoms[1] == atom_id || 
			atoms[2] == atom_id ||
			atoms[3] == atom_id);
}

void AnnotationDihedral::adjustIds(const int atom_id) {
	if (atoms[0] > atom_id) atoms[0] --;
	if (atoms[1] > atom_id) atoms[1] --;
	if (atoms[2] > atom_id) atoms[2] --;
	if (atoms[3] > atom_id) atoms[3] --;
}

bool AnnotationDihedral::isEquivalent(const int natoms, const int *new_list) const {
	return natoms == 4 &&
			new_list[2] == atoms[2] &&
			new_list[1] == atoms[1] &&
			((new_list[3] == atoms[3] && new_list[0] == atoms[0]) ||
			 (new_list[3] == atoms[0] && new_list[0] == atoms[3]));
}

int AnnotationDihedral::getAtom(const int index) const {
	if (index < 0 || index > 3) return -1;
	return atoms[index];
}

int AnnotationDihedral::getType(void) const {
	return MP_ANNOTATION_DIHEDRAL;
}

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
	CPoint3D cross;
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

	CrossProduct3D(&normal1, &normal2, &cross);
	Normalize3D(&cross);

	RotateAroundAxis(rotate, cross, getParam(frame) - value);

	// Rotate3DPt(rotate, vec2, &rotated_vec); 

	// Now we push the fourth atom away from the third along the rotated
	// form of the vector between them.
	// frame.SetAtomPosition(atoms[3], atom3_pos + rotated_vec); 

	CPoint3D new_vec;
	if (!frame.GetAtomSelection(atoms[0]) &&
		!frame.GetAtomSelection(atoms[1]) &&
		!frame.GetAtomSelection(atoms[2])) {
		for (int i = 0; i < frame.GetNumAtoms(); i++) {
			if (frame.GetAtomSelection(i)) {
				frame.GetAtomPosition(i, pt);
				vec2 = pt - atom3_pos;
				Rotate3DPt(rotate, vec2, &new_vec);
				frame.SetAtomPosition(i, atom3_pos + new_vec);
			}
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
void mpAtom::SetDefaultCoordinationNumber(void) {
	//Setup the default coordination based on the atom type
	coordinationNumber = 0;
	if (Type == 15) {
		coordinationNumber = 5;
		return;
	}
	if (Type == 16) {
		coordinationNumber = 6;
		return;
	}
	if (((Type>=6)&&(Type<=9))||((Type>=14)&&(Type<=17))||((Type>=32)&&(Type<=35))||
		((Type>=50)&&(Type<=53))||((Type>=82)&&(Type<=85))) {
		coordinationNumber = 4;
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
			coordinationNumber = 1;
			break;
		case 4:
		case 12:
		case 20:
		case 38:
		case 56:
		case 88:
			coordinationNumber = 2;
			break;
		case 5:
		case 13:
		case 31:
		case 49:
		case 81:
			coordinationNumber = 3;
			break;
	}
}

void mpAtom::SetDefaultLonePairCount(void) {
	LPCount = 0;
	switch (Type) {
		case 7:
	//	case 15:
		case 33:
		case 51:
		case 83:
			LPCount = 1;
			break;
		case 8:
	//	case 16:
		case 34:
		case 52:
		case 84:
			LPCount = 2;
			break;
		case 9:
		case 17:
		case 35:
		case 53:
		case 85:
			LPCount = 3;
			break;
	}
}
