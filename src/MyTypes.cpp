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
	CPoint3D pt1, pt2;
	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);
	return (pt2 - pt1).Magnitude();
}

void AnnotationLength::setParam(Frame& frame, float value) {
	CPoint3D pt1, pt2, vec;
	frame.GetAtomPosition(atoms[0], pt1);
	frame.GetAtomPosition(atoms[1], pt2);
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
	len = vec2.Magnitude();

	Normalize3D(&vec1);

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

float AnnotationDihedral::getParam(const Frame& frame) const {
	return 0.0f;
}

void AnnotationDihedral::setParam(Frame& frame, float value) {
}
