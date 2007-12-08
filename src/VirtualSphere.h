/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
  VirtualSphere.h

   Implements the Virtual Sphere algorithm for 3D rotation using a 2D input device.
   See paper "A Study in Interactive 3-D Rotation Using 2-D Control Devices" by
   Michael Chen, S. Joy Mountford and Abigail Sellen published in the ACM Siggraph '88
   proceedings (Volume 22, Number 4, August 1988) for more detail.  The code here
   provides a much simpler implementation than that described in the paper.

   Author: Michael Chen, Human Interface Group / ATG
   Copyright й 1987-93 Apple Computer, Inc.  All rights reserved.
   
   Modified slightly for MacMolPlt by Brett Bode 8-94

   Part of Virtual Sphere Sample Code Release v1.1
  еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/

#ifndef	__VIRTUALSPHERE__
#define __VIRTUALSPHERE__

#include "wx/wx.h"

void VirtualSphere (const wxPoint &	p,	
					const wxPoint &	q,
					const wxPoint &	cueCenter,
					long		cueRadius,
					Matrix4D rotationMatrix);

void VirtualSphereQD3D (const wxPoint &	p,	
						const wxPoint &	q,
						const wxPoint &	cueCenter,
						long	cueRadius,
						Matrix4D rotationMatrix,
						Matrix4D ModelRotation);
#endif /*__VIRTUALSPHERE__*/
