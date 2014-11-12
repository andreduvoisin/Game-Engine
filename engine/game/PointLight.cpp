#include "PointLight.h"
#include "../core/dbg_assert.h"

namespace ITP485
{

PointLight::PointLight()
{

}

bool PointLight::Spawn(std::string sObjectName, minIni& iniReader)
{
	// Only spawn if its class is PointLight.
	std::string sInput = iniReader.gets(sObjectName, "Class");
	Dbg_Assert(sInput == "PointLight", "Cannot spawn a PointLight if the object's class is not PointLight.");
	if (sInput == "PointLight")
	{
		// Read in all the properties of a PointLight.

		sInput = iniReader.gets(sObjectName, "DiffuseColor");
		if (sInput != "")
		{
			sscanf_s(sInput.c_str(), "(%f,%f,%f,%f)",
				&m_DiffuseColor.x, &m_DiffuseColor.y, &m_DiffuseColor.z, &m_DiffuseColor.w);
		}

		sInput = iniReader.gets(sObjectName, "SpecularColor");
		if (sInput != "")
		{
			sscanf_s(sInput.c_str(), "(%f,%f,%f,%f)",
				&m_SpecularColor.x, &m_SpecularColor.y, &m_SpecularColor.z, &m_SpecularColor.w);
		}

		sInput = iniReader.gets(sObjectName, "Position");
		if (sInput != "")
		{
			float x, y, z;
			sscanf_s(sInput.c_str(), "(%f,%f,%f)", &x, &y, &z);
			m_Position.Set(x, y, z);
		}

		float fInput = iniReader.getf(sObjectName, "SpecularPower");
		if (fInput != 0)
		{
			m_SpecularPower = fInput;
		}

		fInput = iniReader.getf(sObjectName, "InnerRadius");
		if (fInput != 0)
		{
			m_InnerRadius = fInput;
		}

		fInput = iniReader.getf(sObjectName, "OuterRadius");
		if (fInput != 0)
		{
			m_OuterRadius = fInput;
		}
	}

	return true;
}

}