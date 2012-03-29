#include "CSceneObject.h"

#include "CShaderLoader.h"
#include "CImageLoader.h"


CSceneObject::CSceneObject()
{}

void CSceneObject::addSceneObject(CRenderable * Renderable)
{
	Renderables.push_back(Renderable);
}

void CSceneObject::removeSceneObject(CRenderable * Renderable)
{
	Renderables.erase(std::remove(Renderables.begin(), Renderables.end(), Renderable), Renderables.end());
}

void CSceneObject::draw(CScene const * const scene, ERenderPass const Pass)
{
	if (! Visible)
		return;

	ISceneObject::draw(scene, Pass);

	switch (Pass)
	{
	case ERP_DEFAULT:
	case ERP_DEFERRED_OBJECTS:
		for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
			(* it)->draw(scene, Pass);
		break;

	case ERP_MODEL_NORMALS:
		for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
			(* it)->drawNormals(scene);
		break;

	case ERP_DEFERRED_LIGHTS:
		break;
	}
}

void CSceneObject::setShader(ERenderPass const Pass, CShader * shader)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->setShader(Pass, shader);
}

void CSceneObject::setShader(ERenderPass const Pass, std::string const & shader)
{
	setShader(Pass, CShaderLoader::loadShader(shader));
}

void CSceneObject::setMaterial(CMaterial const & material)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->getMaterial() = material;
}

void CSceneObject::setTexture(CTexture * texture)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->getMaterial().Texture = texture;
}

void CSceneObject::setTexture(std::string const & texture)
{
	setTexture(CImageLoader::loadTexture(texture));
}

void CSceneObject::setTexture(CTexture * texture, int const renderable)
{
	//Renderables[renderable].Texture = texture;
	std::vector<CRenderable *>::iterator it = Renderables.begin();
	for (int i = 0; i < renderable && it != Renderables.end(); ++ i, ++ it)
		;
	(* it)->getMaterial().Texture = texture;
}

void CSceneObject::setTexture(std::string const & texture, int const renderable)
{
	setTexture(CImageLoader::loadTexture(texture), renderable);
}

void CSceneObject::addAttribute(std::string const & label, IAttribute const * const attribute)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->addAttribute(label, attribute);
}

void CSceneObject::addUniform(std::string const & label, IUniform const * const uniform)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->addUniform(label, uniform);
}

void CSceneObject::removeAttribute(std::string const & label)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->removeAttribute(label);
}

void CSceneObject::removeUniform(std::string const & label)
{
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
		(* it)->removeUniform(label);
}

CShader const * const CSceneObject::getShader() const
{
	CShader const * Shader = 0;
	for (std::vector<CRenderable *>::const_iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(ERP_DEFAULT);
		else if (Shader != (* it)->getShader(ERP_DEFAULT))
			return 0;
	}
	return Shader;
}

CShader * CSceneObject::getShader()
{
	CShader * Shader = 0;
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(ERP_DEFAULT);
		else if (Shader != (* it)->getShader(ERP_DEFAULT))
			return 0;
	}
	return Shader;
}

CShader const * const CSceneObject::getShader(ERenderPass const Pass) const
{
	CShader const * Shader = 0;
	for (std::vector<CRenderable *>::const_iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(Pass);
		else if (Shader != (* it)->getShader(Pass))
			return 0;
	}
	return Shader;
}

CShader * CSceneObject::getShader(ERenderPass const Pass)
{
	CShader * Shader = 0;
	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		if (! Shader)
			Shader = (* it)->getShader(Pass);
		else if (Shader != (* it)->getShader(Pass))
			return 0;
	}
	return Shader;
}

void CSceneObject::load(CScene const * const Scene, ERenderPass const Pass)
{
	ISceneObject::load(Scene, Pass);

	for (std::vector<CRenderable *>::iterator it = Renderables.begin(); it != Renderables.end(); ++ it)
	{
		(* it)->load(Scene, Pass);
	}
}
