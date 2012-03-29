#ifndef _CABBAGE_SCENE_CSCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_CSCENEOBJECT_H_INCLUDED_

#include "ISceneObject.h"
#include "CRenderable.h"

/*!
 * Standard implementation of ISceneObject, renders a set of CRenderables
 */
class CSceneObject : public ISceneObject
{

protected:

	std::vector<CRenderable *> Renderables;

public:

	CSceneObject();

	void addSceneObject(CRenderable * Renderable);
	void removeSceneObject(CRenderable * Renderable);

    void draw(CScene const * const scene, ERenderPass const Pass);

	void setShader(ERenderPass const Pass, CShader * shader);
	void setShader(ERenderPass const Pass, std::string const & shader);
	void setMaterial(CMaterial const & material);
	void setTexture(CTexture * texture);
	void setTexture(CTexture * texture, int const renderable);
	void setTexture(std::string const & texture);
	void setTexture(std::string const & texture, int const renderable);

	void addAttribute(std::string const & label, IAttribute const * const attribute);
    void addUniform(std::string const & label, IUniform const * const uniform);
    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

	CShader const * const getShader() const;
	CShader * getShader();

	CShader const * const getShader(ERenderPass const Pass) const;
	CShader * getShader(ERenderPass const Pass);

	virtual void load(CScene const * const Scene, ERenderPass const Pass);

};

#endif
