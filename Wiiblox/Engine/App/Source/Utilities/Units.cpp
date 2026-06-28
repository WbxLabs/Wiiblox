#include "stdafx.h"

#include "Util/Units.h"

namespace WBX {



//////////////////////////////////////////////////////////////////////
//
// KMS to / from WBX



Vector3 Units::kmsVelocityToRbx(const Vector3& kmsVelocity)
{
	return kmsVelocity * WBXPerM();
}

Vector3 Units::kmsAccelerationToRbx(const Vector3& kmsAccel)
{
	return kmsAccel * WBXPerM();
}

float Units::kmsAccelerationToRbx(float kmsAccel)
{
	return kmsAccel * WBXPerM();
}

Vector3 Units::kmsForceToRbx(const Vector3& kmsForce)
{
	return kmsForce * WBXPerM();
}

float Units::kmsForceToRbx(float kmsForce)
{
	return kmsForce * WBXPerM();
}

Vector3 Units::kmsTorqueToRbx(const Vector3& kmsTorque)
{
	return kmsTorque * WBXPerM() * WBXPerM();
}

Vector3 Units::kmsKRotToRbx(const Vector3& kmsKRot)
{
	return kmsKRot * WBXPerM() * WBXPerM();
}


Vector3 Units::kmsKRotDampToRbx(const Vector3& kmsKRotDamp)
{
	return kmsKRotDamp * WBXPerM() * WBXPerM();
}


float Units::kmsDensityToRbx(float kmsDensity)
{
	return kmsDensity * WBXPerM() * WBXPerM() * WBXPerM();
}



} // namespace