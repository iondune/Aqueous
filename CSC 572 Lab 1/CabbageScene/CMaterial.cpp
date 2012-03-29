#include "CMaterial.h"

CMaterial::CMaterial()
    : Texture(0), BindAmbientColor(& AmbientColor), BindDiffuseColor(& DiffuseColor), BindShininess(& Shininess)
{
	loadDefaults();
}

CMaterial::CMaterial(CMaterial const & other)
{
	Texture = other.Texture;
	AmbientColor = other.AmbientColor;
	DiffuseColor = other.DiffuseColor;
	Shininess = other.Shininess;
}

CMaterial & CMaterial::operator = (CMaterial const & other)
{
	Texture = other.Texture;
	AmbientColor = other.AmbientColor;
	DiffuseColor = other.DiffuseColor;
	Shininess = other.Shininess;

	return * this;
}

void CMaterial::loadDefaults()
{
	Shininess = 100000000000.0f;
	AmbientColor = SColor(0.15f, 0.15f, 0.15f);
	DiffuseColor = SColor(0.8f, 0.8f, 0.8f);
}
