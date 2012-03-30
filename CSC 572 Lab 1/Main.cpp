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

class CVolumeData
{

public:

	static const int Resolution = 32;

	float Data[Resolution][Resolution][Resolution];

	CVolumeData()
	{
		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution; ++ x)
			Data[x][y][z] = 0.f;

		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution; ++ x)
			setVolumeData(x, y, z, pow(x - Resolution / 2.f, 2.f) + pow(y - Resolution / 2.f, 2.f) + pow(z - Resolution / 2.f, 2.f) - pow(Resolution / 3.f, 2.f));
	}

	float const getVolumeData(int const x, int const y, int const z) const
	{
		return Data[x][y][z];
	}

	void setVolumeData(int const x, int const y, int const z, float const value)
	{
		Data[x][y][z] = value;
	}

};

#include "LookupTables.h"

class CMainState : public CState<CMainState>
{

	CCameraControl * Camera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

	CSceneObject * SoupObject;
	ISceneObject * VoxelObject;

	SVector3 LightPosition;
	SUniform<SVector3> BindLightPosition;

	CVolumeData VolumeData;

public:

	CMainState()
		: Camera(0), Tyra(0), Scale(1), Mode(3), BindLightPosition(LightPosition)
	{}

	void begin()
	{
		SDL_WM_SetCaption("Dinosaurs in Space!", "");

        glClearColor(0.6f, 0.4f, 0.3f, 1.0f);
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

		// Add cube to show light location
		LightObject = SceneManager.addMeshSceneObject(Cube, CShaderLoader::loadShader("Simple"));
		LightObject->setScale(SVector3(0.09f));

		CShader * Shader = CShaderLoader::loadShader("Diffuse");

		VoxelObject = new ISceneObject();
		SceneManager.addSceneObject(VoxelObject);
		VoxelObject->setCullingEnabled(false);

		for (int z = 0; z < VolumeData.Resolution; ++ z)
		for (int y = 0; y < VolumeData.Resolution; ++ y)
		for (int x = 0; x < VolumeData.Resolution; ++ x)
		{
			if (equals(VolumeData.getVolumeData(x, y, z), 0.f, 11.f))
			{
				CSceneObject * Object = SceneManager.addMeshSceneObject(Cube, Shader);
				Object->setParent(VoxelObject);
				Object->setScale(SVector3(1.f)/8.f);
				Object->setTranslation(SVector3((float) x, (float) y, (float) z)/4.f);
				Object->addUniform("uLightPosition", & BindLightPosition);
			}
		} // x - y - z

		CMesh * Mesh = new CMesh();
		Mesh->MeshBuffers.push_back(new CMesh::SMeshBuffer());

		for (int z = 0; z < VolumeData.Resolution - 1; ++ z)
		for (int y = 0; y < VolumeData.Resolution - 1; ++ y)
		for (int x = 0; x < VolumeData.Resolution - 1; ++ x)
		{
			int lookup = 0;
			float const Range = 11.f;

			if ((VolumeData.getVolumeData(x, y, z)<0.f)) lookup |= 128;
			if ((VolumeData.getVolumeData(x+1, y, z)< 0.f)) lookup |= 64;
			if ((VolumeData.getVolumeData(x+1, y+1, z)< 0.f)) lookup |= 4;
			if ((VolumeData.getVolumeData(x, y+1, z)< 0.f)) lookup |= 8;
			if ((VolumeData.getVolumeData(x, y, z+1)< 0.f)) lookup |= 16;
			if ((VolumeData.getVolumeData(x+1, y, z+1)< 0.f)) lookup |= 32;
			if ((VolumeData.getVolumeData(x+1, y+1, z+1)< 0.f)) lookup |= 2;
			if ((VolumeData.getVolumeData(x, y+1, z+1)< 0.f)) lookup |= 1;

			SVertex verts[12];

			auto interpolate = [](SVector3 const v1, SVector3 const v2) -> SVertex
			{
				SVertex v;
				v.Position = (v1 + v2) / 2.f / 4.f;
				return v;
			};

			if ((lookup != 0) && (lookup != 255))
			{
				// 0 - 1
				if (edgeTable[lookup] & 1)
					verts[0] = interpolate(SVector3(x, y+1, z+1), SVector3(x+1, y+1, z+1));

				// 1 - 2
				if (edgeTable[lookup] & 2)
					verts[1] = interpolate(SVector3(x+1, y+1, z+1), SVector3(x+1, y+1, z));

				// 2 - 3
				if (edgeTable[lookup] & 4)
					verts[2] = interpolate(SVector3(x+1, y+1, z), SVector3(x, y+1, z));

				// 3 - 0
				if (edgeTable[lookup] & 8)
					verts[3] = interpolate(SVector3(x, y+1, z), SVector3(x, y+1, z+1));
			
				// 4 - 5
				if (edgeTable[lookup] & 16)
					verts[4] = interpolate(SVector3(x, y, z+1), SVector3(x+1, y, z+1));

				// 5 - 6
				if (edgeTable[lookup] & 32)
					verts[5] = interpolate(SVector3(x+1, y, z+1), SVector3(x+1, y, z));

				// 6 - 7
				if (edgeTable[lookup] & 64)
					verts[6] = interpolate(SVector3(x+1, y, z), SVector3(x, y, z));

				// 7 - 4
				if (edgeTable[lookup] & 128)
					verts[7] = interpolate(SVector3(x, y, z), SVector3(x, y, z+1));

				// 0 - 4
				if (edgeTable[lookup] & 256)
					verts[8] = interpolate(SVector3(x, y+1, z+1), SVector3(x, y, z+1));

				// 1 - 5
				if (edgeTable[lookup] & 512)
					verts[9] = interpolate(SVector3(x+1, y+1, z+1), SVector3(x+1, y, z+1));

				// 2 - 6
				if (edgeTable[lookup] & 1024)
					verts[10] = interpolate(SVector3(x+1, y+1, z), SVector3(x+1, y, z));

				// 3 - 7
				if (edgeTable[lookup] & 2048)
					verts[11] = interpolate(SVector3(x, y+1, z), SVector3(x, y, z));

				int i, j;
					
				for (i = 0; triTable[lookup][i] != -1; i+=3)
				{
					for (j = i; j < (i+3); j++)
					{
						Mesh->MeshBuffers[0]->Vertices.push_back(verts[triTable[lookup][j]]);
					}
					CMesh::STriangle Tri;
					Tri.Indices[0] = Mesh->MeshBuffers[0]->Vertices.size() - 3;
					Tri.Indices[1] = Mesh->MeshBuffers[0]->Vertices.size() - 2;
					Tri.Indices[2] = Mesh->MeshBuffers[0]->Vertices.size() - 1;
					Mesh->MeshBuffers[0]->Triangles.push_back(Tri);
				}
			}
		} // x - y - z

		Mesh->calculateNormalsPerFace();
		SoupObject = SceneManager.addMeshSceneObject(Mesh, Shader);
		SoupObject->setVisible(false);
		SoupObject->setCullingEnabled(false);
		SoupObject->addUniform("uLightPosition", & BindLightPosition);
	}

	void OnRenderStart(float const Elapsed)
	{
		Camera->update(Elapsed);

		Tyra->setTranslation(Translation);
        Tyra->setScale(Scale);
        Tyra->setRotation(Rotation);

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
	Application.init(SPosition2(1024, 768));

	Application.getStateManager().setState(& CMainState::get());

	Application.run();

	return 0;
}
