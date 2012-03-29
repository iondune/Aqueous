#ifndef _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_
#define _CABBAGE_SCENE_ISCENEOBJECT_H_INCLUDED_

#include <map>
#include <set>
#include <list>

#include "../CabbageCore/boost/shared_ptr.hpp"
#include "../CabbageCore/SVector3.h"
#include "../CabbageCore/SBoundingBox3.h"

#include "CBufferObject.h"
#include "CCamera.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderContext.h"


class EDebugData
{

public:

    enum Domain
    {
        None = 0,
        Normals = 1,
        NormalColors = 2
    };

};

enum ERenderPass
{
	ERP_DEFAULT,
	ERP_MODEL_NORMALS,
	ERP_DEFERRED_OBJECTS,
	ERP_DEFERRED_LIGHTS,
	ERP_COUNT
};

class CScene;

class ISceneObject
{

protected:

    // Model Transformation
    STransformation3 Transformation;
	glm::mat4 AbsoluteTransformation;
    glm::mat4 RotationMatrix;
    SBoundingBox3 BoundingBox;

	SVector3 Rotation, Translation, Scale;

    int DebugDataFlags;

    bool Visible;

	std::list<ISceneObject *> Children;
	ISceneObject * Parent;

	bool UseCulling;

public:

    ISceneObject();

	void updateAbsoluteTransformation();
	glm::mat4 const & getAbsoluteTransformation() const;

    void setTranslation(SVector3 const & translation);
    void setRotation(SVector3 const & rotation);
    void setRotation(glm::mat4 const & matrix);
    void setScale(SVector3 const & scale);

	virtual void update();
    virtual void draw(CScene const * const scene, ERenderPass const Pass);

    SBoundingBox3 const & getBoundingBox() const;
    SBoundingBox3 & getBoundingBox();
    void setBoundingBox(SBoundingBox3 const & boundingBox);

    bool const isDebugDataEnabled(EDebugData::Domain const type) const;
    void enableDebugData(EDebugData::Domain const type);
    void disableDebugData(EDebugData::Domain const type);

    bool const intersectsWithLine(SLine3 const & line) const;

    bool const isVisible() const;
    void setVisible(bool const isVisible);

	STransformation3 const & getTransformation() const;

	ISceneObject const * const getParent() const;
	std::list<ISceneObject *> const & getChildren() const;

	void removeChild(ISceneObject * child);
	void addChild(ISceneObject * child);
	void setParent(ISceneObject * parent);
	void removeChildren();

	SVector3 const & getRotation() const;
	SVector3 const & getTranslation() const;
	SVector3 const & getScale() const;

	bool const isCulled(CScene const * const Scene) const;
	bool const isCullingEnabled() const;
	void setCullingEnabled(bool const culling);

	virtual void load(CScene const * const Scene, ERenderPass const Pass);

};

#endif
