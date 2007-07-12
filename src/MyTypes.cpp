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
	pt2 = pt1 + vec * value;

	frame.SetAtomPosition(atoms[1], pt2);
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
	len = vec2.Magnitude();
	Normalize3D(&vec1);

	// The axis of rotation is the normal of the plane formed by the three
	// atoms.
	CrossProduct3D(&vec1, &vec2, &normal);
	Normalize3D(&normal);

	RotateAroundAxis(rotate, normal, value);

	Rotate3DPt(rotate, vec1, &vec2);
	vec2 = vec2 * len;

	frame.SetAtomPosition(atoms[2], pt2 + vec2);

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

#include <iostream>
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

	// The axis to rotate around is between the 2nd and 3rd atoms.
	axis = atom2_pos - atom3_pos;
	Normalize3D(&axis);

	// The vector we want to rotate around the axis is from the 3rd to the 4th
	// atoms.
	vec2 = atom4_pos - atom3_pos;

	// The direction that we rotate depends on how the angle is oriented.
	// The direction of rotation depends on the winding (CW or CCW) of the
	// atoms.  To determine this, we compare the crossproduct of the two
	// planes' normals with the shared axis.  These two vectors should either
	// point in the same direction or in opposite directions.
	vec1 = atom1_pos - atom3_pos;
	CrossProduct3D(&vec1, &axis, &normal1);
	Normalize3D(&normal1);

	CrossProduct3D(&axis, &vec2, &normal2);
	Normalize3D(&normal2);

	CrossProduct3D(&normal1, &normal2, &cross);
	Normalize3D(&cross);
	
	if (DotProduct3D(&cross, &axis) > 0.0f) {
		RotateAroundAxis(rotate, axis, getParam(frame) - value);
	} else {
		RotateAroundAxis(rotate, axis, value - getParam(frame));
	}

	Rotate3DPt(rotate, vec2, &rotated_vec);

	// Now we push the fourth atom away from the third along the rotated
	// form of the vector between them.
	frame.SetAtomPosition(atoms[3], atom3_pos + rotated_vec);
}