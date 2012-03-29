#ifndef _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_
#define _CABBAGE_SCENE_CRENDERABLE_H_INCLUDED_

#include <map>
#include <set>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"
#include "ISceneObject.h"

#include "CMaterial.h"
#include "SAttribute.h"
#include "SUniform.h"


/*!
 * A CRenderable is some collection of buffer objects which can be drawn by a single OpenGL draw call.
 */
class CRenderable
{

protected:

    // Implicit shader variables
    glm::mat4 ModelMatrix, NormalMatrix, InverseModelMatrix;
	SUniform<glm::mat4> BindModelMatrix, BindNormalMatrix, BindInverseModelMatrix;

	// Local shader variables
    std::map<std::string, IAttribute const *> Attributes;
    std::map<std::string, IUniform const *> Uniforms;

	// Loaded shader variables
	std::map<GLint, IAttribute const *> LoadedAttributes;
    std::map<GLint, IUniform const *> LoadedUniforms;

	// Required data for drawing
	CShader * Shader[ERP_COUNT];
    CBufferObject<GLushort> * IndexBufferObject;

	// Material attribute used by phong light, etc.
    CMaterial Material;
	STransformation3 Transformation;

    static CShader * NormalColorShader;
    CRenderable * NormalObject;

    GLenum DrawType;

    void loadShaderVariables(CShader const * const shader, CScene const * const scene);
    CScene const * LastLoadedScene;
    CShader const * LastLoadedShader;

	ISceneObject * Parent;

public:

	int Size;
    CRenderable(ISceneObject * parent);

    CMaterial & getMaterial();
    CMaterial const & getMaterial() const;

    CBufferObject<GLushort> * getIndexBufferObject();
    void setIndexBufferObject(CBufferObject<GLushort> * indexBufferObject);

	CShader * getShader(ERenderPass const Pass);
	void setShader(ERenderPass const Pass, CShader * shader);

    GLenum const getDrawType() const;
    void setDrawType(GLenum const drawType);

    virtual void draw(CScene const * const scene, ERenderPass const Pass);
	virtual void drawNormals(CScene const * const scene);

    void addAttribute(std::string const & label, IAttribute const * const attribute);
    void addUniform(std::string const & label, IUniform const * const uniform);
    void removeAttribute(std::string const & label);
    void removeUniform(std::string const & label);

	void load(CScene const * const Scene, ERenderPass const Pass);
	CShader * updateShaderVariables(CScene const * const Scene, ERenderPass const Pass);

	IAttribute const * const getAttribute(std::string const & label);
	IUniform const * const getUniform(std::string const & label);
	
    CRenderable * & getDebuggingNormalObject();

	void reloadVariablesOnNextDraw();

};

#endif
