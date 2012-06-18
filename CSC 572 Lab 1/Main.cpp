#include <CabbageCore.h>
#include <CabbageScene.h>
#include <CabbageFramework.h>

#ifdef __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freetype.lib")

#include <GL/glew.h>
#include <SDL/SDL.h>
#endif

#include "SciDataParser.h"

class CMainState : public CState<CMainState>
{

	CCameraControl * Camera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

	ISceneObject * SoupObject;
	ISceneObject * VoxelObject;

	SVector3 LightPosition;
	SUniform<SVector3> BindLightPosition;

	ISciTreeNode * DataTree;

public:

	CMainState()
		: Camera(0), Tyra(0), Scale(1), Mode(3), BindLightPosition(LightPosition)
	{}

	void begin()
	{
		SDL_WM_SetCaption("Dinosaurs in Space!", "");

        glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

		LightPosition = SVector3(0.2f, 0.4f, 0.2f);

		SceneManager.setActiveCamera(Camera = new CCameraControl(SVector3(1.f, 0.3f, 1.5f), -1.9f));

		// Load dino model and apply texture
		CMesh * TyraMesh = CMeshLoader::loadObjMesh("Tyra.obj");
		TyraMesh->centerMeshByExtents(SVector3());
		Tyra = SceneManager.addMeshSceneObject(TyraMesh, CShaderLoader::loadShader("NormalMap"), CMaterial());
		Tyra->setCullingEnabled(false);
		Tyra->addUniform("uLightPosition", & BindLightPosition);
		CTexture * Texture = CTextureLoader::loadTexture("TyraNormals.bmp");
		Tyra->setTexture(Texture);

		// Add space backdrop
		CMesh * Cube = CMeshLoader::createCubeMesh();
		CMeshSceneObject * SkyBox = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("DiffuseTexture"));
		SkyBox->setScale(SVector3(20.f));
		SkyBox->setTexture("Space.bmp");
		SkyBox->setCullingEnabled(false);
		SkyBox->setVisible(false);

		// Add cube to show light location
		LightObject = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("Simple"));
		LightObject->setScale(SVector3(0.09f));

		CShader * Shader = CShaderLoader::loadShader("Diffuse");

		VoxelObject = new ISceneObject();
		SceneManager.addSceneObject(VoxelObject);
		VoxelObject->setCullingEnabled(false);

		SoupObject = new ISceneObject();
		SoupObject->setVisible(false);
		SceneManager.addSceneObject(SoupObject);
		SoupObject->setCullingEnabled(false);

		float const ScaleFactor = 4.f * 12.f;

		SciDataParser p;
		p.parseFile("ForZoe.txt");

		int i = 0;

		for (auto it = p.m_data.begin(); it != p.m_data.end(); ++ it)
		{
			CMeshSceneObject * Object = new CMeshSceneObject();
			Object->setMesh(Cube);
			Object->setParent(VoxelObject);
			Object->setScale(SVector3(1.f) / 32.f);
			Object->setTranslation(SVector3((float) it->getLocation().X, (float) it->getLocation().Y, (float) it->getLocation().Z) / ScaleFactor);
			Object->addUniform("uLightPosition", & BindLightPosition);

			double o2_ratio = (it->getO2Concenration() - p.m_minO2) / (p.m_maxO2 - p.m_minO2);
			CMaterial mat;
			mat.DiffuseColor = SColor(1.f - (float) o2_ratio, (float) o2_ratio, 1.f - (float) o2_ratio);
			Object->setMaterial(mat);
			Object->setShader(ERenderPass::ERP_DEFAULT, Shader);
			Object->setCullingEnabled(false);

			i ++;
		}

		printf("Created %d points\n\n\n.", i);

		CSciTreeLeaf * Root = (CSciTreeLeaf *) (DataTree = new CSciTreeLeaf());

		for (auto it = p.m_data.begin(); it != p.m_data.end(); ++ it)
			Root->Datums.push_back(* it);
		Root->Extents = SBoundingBox3(p.m_minLoc, p.m_maxLoc);

		std::function<void(ISciTreeNode * & Node)> SubdivideNode;
		SubdivideNode = [&](ISciTreeNode * & Node)
		{
			if (! Node->isLeaf())
				return;

			CSciTreeLeaf * Root = (CSciTreeLeaf *) Node;

			CSciTreeNode * NewRoot = new CSciTreeNode();
			NewRoot->Extents = Root->Extents;

			for (int i = 0; i < EO_COUNT; ++ i)
			{
				NewRoot->Children[i] = new CSciTreeLeaf();

				switch (i)
				{
				case EO_TOP_UPPER_LEFT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
						SVector3(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
						);
					break;

				case EO_TOP_UPPER_RIGHT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z),
						SVector3(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.MaxCorner.Z)
						);
					break;

				case EO_TOP_LOWER_LEFT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
						SVector3(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
						);
					break;

				case EO_TOP_LOWER_RIGHT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.getCenter().X, Root->Extents.MinCorner.Y, Root->Extents.getCenter().Z),
						SVector3(Root->Extents.MaxCorner.X, Root->Extents.getCenter().Y, Root->Extents.MaxCorner.Z)
						);
					break;

				case EO_BOTTOM_UPPER_LEFT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.MinCorner.X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
						SVector3(Root->Extents.getCenter().X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
						);
					break;

				case EO_BOTTOM_UPPER_RIGHT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.MinCorner.Z),
						SVector3(Root->Extents.MaxCorner.X, Root->Extents.MaxCorner.Y, Root->Extents.getCenter().Z)
						);
					break;

				case EO_BOTTOM_LOWER_LEFT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.MinCorner.X, Root->Extents.MinCorner.Y, Root->Extents.MinCorner.Z),
						SVector3(Root->Extents.getCenter().X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z)
						);
					break;

				case EO_BOTTOM_LOWER_RIGHT:
					NewRoot->Children[i]->Extents = SBoundingBox3(
						SVector3(Root->Extents.getCenter().X, Root->Extents.MinCorner.Y, Root->Extents.MinCorner.Z),
						SVector3(Root->Extents.MaxCorner.X, Root->Extents.getCenter().Y, Root->Extents.getCenter().Z)
						);
					break;
				}
			}

			for (auto it = Root->Datums.begin(); it != Root->Datums.end(); ++ it)
			{
				for (int i = 0; i < EO_COUNT; ++ i)
				{
					if (NewRoot->Children[i]->Extents.isPointInside(it->getLocation()))
						((CSciTreeLeaf *)NewRoot->Children[i])->Datums.push_back(* it);
				}
			}

			delete Root;
			Node = NewRoot;

			for (int i = 0; i < EO_COUNT; ++ i)
			{
				if (((CSciTreeLeaf *)NewRoot->Children[i])->Datums.size() > 1)
					SubdivideNode(NewRoot->Children[i]);
				else if (((CSciTreeLeaf *)NewRoot->Children[i])->Datums.size() == 1)
				{
					CMeshSceneObject * Object = new CMeshSceneObject();
					Object->setMesh(Cube);
					Object->setParent(SoupObject);
					Object->setScale(NewRoot->Children[i]->Extents.getExtent() / ScaleFactor);
					Object->setTranslation(NewRoot->Children[i]->Extents.getCenter() / ScaleFactor);
					Object->addUniform("uLightPosition", & BindLightPosition);

					double o2_ratio = (((CSciTreeLeaf *)NewRoot->Children[i])->Datums[0].getO2Concenration() - p.m_minO2) / (p.m_maxO2 - p.m_minO2);
					CMaterial mat;
					mat.DiffuseColor = SColor(1.f - (float) o2_ratio, (float) o2_ratio, 1.f - (float) o2_ratio);
					Object->setMaterial(mat);
					Object->setShader(ERenderPass::ERP_DEFAULT, Shader);
					//Object->enableDebugData(EDebugData::Wireframe);
					Object->setCullingEnabled(false);
				}
			}
		};

		printf("Beginning subdivision.\n");
		unsigned int t0 = SDL_GetTicks(), t1;
		SubdivideNode(DataTree);
		t1 = SDL_GetTicks();
		printf("End subdivision. Took %d ms.\n", t1 - t0);

		CSciTreeNode * NewRoot = (CSciTreeNode *) DataTree;

#if 0
		for (int i = 0; i < EO_COUNT; ++ i)
		{
			CMeshSceneObject * Object = new CMeshSceneObject();
			Object->setMesh(Cube);
			Object->setParent(VoxelObject);
			Object->setScale(NewRoot->Children[i]->Extents.getExtent() / ScaleFactor);
			Object->setTranslation(NewRoot->Children[i]->Extents.getCenter() / ScaleFactor);
			Object->addUniform("uLightPosition", & BindLightPosition);

			CMaterial mat;
			mat.DiffuseColor = SColor(0.8f, 0.8f, 0.8f);
			Object->setMaterial(mat);
			Object->setShader(ERenderPass::ERP_DEFAULT, Shader);
			Object->enableDebugData(EDebugData::Wireframe);
			Object->setCullingEnabled(false);
		}
#endif
	}

	void OnRenderStart(float const Elapsed)
	{
		Camera->update(Elapsed);

		Tyra->setTranslation(Translation);
        Tyra->setScale(Scale);
        Tyra->setRotation(Rotation);

		LightPosition = Camera->getPosition() + SVector3(0, 0, 0);

		LightObject->setTranslation(LightPosition);


		SceneManager.drawAll();
        SDL_GL_SwapBuffers();
	}

	
    void OnKeyboardEvent(SKeyboardEvent const & Event)
    {
        switch (Event.Key)
        {

		case SDLK_x:

			if (! Event.Pressed)
			{
				SoupObject->setVisible(! SoupObject->isVisible());
			}

			break;

		case SDLK_z:

			if (! Event.Pressed)
			{
				VoxelObject->setVisible(! VoxelObject->isVisible());
			}

			break;

        case SDLK_n:

			if (! Event.Pressed)
			{
				if (Tyra->isDebugDataEnabled(EDebugData::Normals))
					Tyra->disableDebugData(EDebugData::Normals);
				else
					Tyra->enableDebugData(EDebugData::Normals);
			}

            break;

        case SDLK_m:

            if (! Event.Pressed)
			{
				if (Tyra->isDebugDataEnabled(EDebugData::NormalColors))
					Tyra->disableDebugData(EDebugData::NormalColors);
				else
					Tyra->enableDebugData(EDebugData::NormalColors);
			}

            break;

        case SDLK_r:
            Mode = 0;
            break;

        case SDLK_t:
            Mode = 1;
            break;

        case SDLK_e:
            Mode = 2;
            break;

		case SDLK_y:
			Mode = 3;
			break;

        }
    }

    glm::vec3 LastVec;

    glm::vec3 makeSphereVec(int x, int y)
    {
        float ix = 2*(x / (float) CApplication::get().getWindowSize().X) - 1.f;
        float iy = 1.f - 2*(y / (float) CApplication::get().getWindowSize().X);

        float length = sqrt(ix*ix + iy*iy);

        if (length > 1.f)
        {
            return glm::vec3(ix / length, iy / length, 0);
        }
        else
        {
            return glm::vec3(ix, iy, sqrt(1 - ix*ix - iy*iy));
        }
    }

    int Mode;
	SVector3 Translation, Scale;
    glm::mat4 Rotation;

    void OnMouseEvent(SMouseEvent const & Event)
    {
        switch (Event.Type.Value)
        {

        case SMouseEvent::EType::Click:

            LastVec = makeSphereVec(Event.Location.X, Event.Location.Y);

            break;

        case SMouseEvent::EType::Move:
            {
                float difX = (float) Event.Movement.X;
                float difY = (float) Event.Movement.Y;
                glm::vec3 NewVec = makeSphereVec(Event.Location.X, Event.Location.Y);
                glm::mat4 ViewMatrix = CApplication::get().getSceneManager().getActiveCamera()->getViewMatrix();

                if (CApplication::get().getEventManager().IsMouseDown[Event.Button.Left])
                {
	                if (Mode == 0)
	                {
		                glm::vec3 cross = glm::cross(LastVec, NewVec);
		                glm::vec4 axis(cross.x, cross.y, cross.z, 0);
		                float dot = 180.f / 3.1415f * acos(glm::dot(LastVec, NewVec));

		                if (cross.x == cross.x && cross.y == cross.y && cross.z == cross.z && dot == dot && (cross.x != 0 || cross.y != 0 || cross.z != 0))
		                {
			
                            glm::mat4 inverse = glm::inverse(ViewMatrix);
			                axis = inverse * axis;

			                glm::vec3 rotAxis(axis.x, axis.y, axis.z);
			                Rotation = glm::rotate(glm::mat4(1.f), dot, rotAxis)*Rotation;
		                }
	                }
	                else if (Mode == 1)
	                {
		                float const moveSpeed = 0.01f;
		                glm::vec4 trans(difX*moveSpeed, -difY*moveSpeed, 0, 0);
		                trans = glm::inverse(ViewMatrix) * trans;
		                Translation.X += trans.x;
		                Translation.Y += trans.y;
		                Translation.Z += trans.z;
	                }
	                else if (Mode == 2)
	                {
		                float const scaleSpeed = 0.01f;
                        Scale = SVector3(std::max(std::min(Scale.X + difX*scaleSpeed, 2.f), 0.1f));
	                }
					else if (Mode == 3)
	                {
		                float const moveSpeed = 0.01f;
		                glm::vec4 trans(difX*moveSpeed, -difY*moveSpeed, 0, 0);
		                trans = glm::inverse(ViewMatrix) * trans;
						LightPosition.X += trans.x;
		                LightPosition.Y += trans.y;
		                LightPosition.Z += trans.z;
	                }
                }

                LastVec = NewVec;

                break;
            }
        }
    }

};

int main(int argc, char * argv[])
{
	CTextureLoader::ImageDirectory = "Media/";
	CMeshLoader::MeshDirectory = "Media/";
	CShaderLoader::ShaderDirectory = "Shaders/";

	CApplication & Application = CApplication::get();
	Application.init(SPosition2(1600, 900));

	Application.getStateManager().setState(& CMainState::get());

	Application.run();

	return 0;
}
