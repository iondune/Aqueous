#include "CSceneManager.h"

#include <algorithm>
#include <sstream>

#include "CShaderLoader.h"
#include "CMeshLoader.h"
#include "CTextureLoader.h"


CLight const CScene::NullLight;


CScene::CScene()
   : BindProjMatrix(ProjMatrix), BindViewMatrix(ViewMatrix), BindLightCount(LightCount), UseCulling(true)
{
    ActiveCamera = & DefaultCamera;

    addUniform("uProjMatrix", & BindProjMatrix);
    addUniform("uViewMatrix", & BindViewMatrix);
    addUniform("uLightCount", & BindLightCount);
}

void CScene::addUniform(std::string const & label, IUniform const * uniform)
{
    Uniforms[label] = uniform;
}

void CScene::removeUniform(std::string const & label)
{
    std::map<std::string, IUniform const *>::iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        Uniforms.erase(it);
}

ICamera * const CScene::getActiveCamera()
{
    return ActiveCamera;
}

ICamera const * const CScene::getActiveCamera() const
{
    return ActiveCamera;
}

void CScene::setActiveCamera(ICamera * const activeCamera)
{
    ActiveCamera = activeCamera;
}

bool const CScene::isCullingEnabled() const
{
   return UseCulling;
}

void CScene::setCullingEnabled(bool const culling)
{
   UseCulling = culling;
}


unsigned int const digitCount(int n)
{
    int count = 1;
    if (n < 0)
    {
        n *= -1;
        ++ count;
    }

    while (n > 10)
    {
        ++ count;
        n /= 10;
    }

    return count;
}

IUniform const * CScene::getUniform(std::string const & label) const
{
    if (label.substr(0, 8) == "uLights[")
    {
        std::stringstream ss(label.substr(8));
        unsigned int index;
        ss >> index;
        std::string remaining = ss.str();
        remaining = remaining.substr(2 + digitCount(index));

        if (remaining == "Color")
        {
            if (index >= Lights.size())
            return & NullLight.BindColor;
            else
                return & Lights[index]->BindColor;
        }
        else if (remaining == "Position")
        {
            if (index >= Lights.size())
                return & NullLight.BindPosition;
            else
                return & Lights[index]->BindPosition;
        }
    }

    std::map<std::string, IUniform const *>::const_iterator it = Uniforms.find(label);

    if (it != Uniforms.end())
        return it->second;

    return 0;
}

void CScene::update()
{
	ActiveCamera->recalculateViewMatrix();
	ViewMatrix = ActiveCamera->getViewMatrix();
	ProjMatrix = ActiveCamera->getProjectionMatrix();

	if (LightCount != Lights.size())
	{
		SceneChanged = true;
		LightCount = Lights.size();
	}

	RootObject.updateAbsoluteTransformation();
	RootObject.update();
}



CSceneManager::CSceneManager(SPosition2 const & screenSize)
	: ScreenSize(screenSize)
{
    CurrentScene = this;
}

void CSceneManager::addSceneObject(ISceneObject * sceneObject)
{
   RootObject.addChild(sceneObject);
}

void CSceneManager::removeSceneObject(ISceneObject * sceneObject)
{
   RootObject.removeChild(sceneObject);
}

void CSceneManager::removeAllSceneObjects()
{
   RootObject.removeChildren();
}

void CSceneManager::drawAll()
{
    CurrentScene->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RootObject.draw(CurrentScene, ERP_DEFAULT);
	
	SceneChanged = false;
}


CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(ERP_DEFAULT, Shader);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(CMesh * Mesh, CShader * Shader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();
   Object->setMesh(Mesh);
   Object->setShader(ERP_DEFAULT, Shader);
   Object->setMaterial(Material);
   addSceneObject(Object);
   return Object;
}

CMeshSceneObject * CSceneManager::addMeshSceneObject(std::string const & Mesh, std::string const & Shader, CMaterial const & Material)
{
   CMeshSceneObject * Object = new CMeshSceneObject();

   if (Mesh.substr(Mesh.size() - 4, 4) == ".obj")
	   Object->setMesh(CMeshLoader::loadObjMesh(Mesh));
   else
	   Object->setMesh(CMeshLoader::load3dsMesh(Mesh));

   Object->setShader(ERP_DEFAULT, CShaderLoader::loadShader(Shader));
   Object->setMaterial(Material);
   addSceneObject(Object);
   return Object;
}


	
void CScene::enableDebugData(EDebugData::Domain const type)
{
	RootObject.enableDebugData(type);
}

void CScene::disableDebugData(EDebugData::Domain const type)
{
	RootObject.disableDebugData(type);
}

void CSceneManager::load()
{
	RootObject.load(this, ERP_DEFAULT);
}

SSize2 const & CSceneManager::getScreenSize() const
{
	return ScreenSize;
}
