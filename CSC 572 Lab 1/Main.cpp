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

#include "perlin/Perlin3.h"

typedef float perlinreal;

class CPerlin
{

	static unsigned int const Size = 8;

	perlinreal Data[Size][Size][Size];

public:

	CPerlin(unsigned int const Seed = 2980345890)
	{
		for (unsigned int z = 0; z < Size; ++ z)
		for (unsigned int y = 0; y < Size; ++ y)
		for (unsigned int x = 0; x < Size; ++ x)
		{
			Data[x][y][z] = frand() * 2.f - 1.f;
		}
	}

	perlinreal noise(perlinreal x, perlinreal y, perlinreal z)
	{
		x = fmod(x, 1.f);
		y = fmod(y, 1.f);
		z = fmod(z, 1.f);
		x *= Size;
		y *= Size;
		z *= Size;
		perlinreal a = fmod(x, 1.f);
		perlinreal b = fmod(y, 1.f);
		perlinreal c = fmod(z, 1.f);

		auto lerp = [](perlinreal a, perlinreal b, perlinreal mu)
		{
			return a * (1 - mu) + b * mu;
		};

		unsigned int x0, x1, y0, y1, z0, z1;
		x0 = (unsigned int) x;
		x1 = x0 + 1;
		y0 = (unsigned int) y;
		y1 = y0 + 1;
		z0 = (unsigned int) z;
		z1 = z0 + 1;

		return 
			Data[x0][y0][z0]*(1-a)*(1-b)*(1-c) + 
			Data[x0][y0][z1]*(1-a)*(1-b)*  (c) + 
			Data[x0][y1][z0]*(1-a)*  (b)*(1-c) + 
			Data[x0][y1][z1]*(1-a)*  (b)*  (c) + 
			Data[x0][y0][z0]*  (a)*(1-b)*(1-c) + 
			Data[x1][y0][z1]*  (a)*(1-b)*  (c) + 
			Data[x1][y1][z0]*  (a)*  (b)*(1-c) + 
			Data[x1][y1][z1]*  (a)*  (b)*  (c);
	}
};

CPerlin Perlin;

class CVolumeData
{

public:

	static const int Resolution = 128;

	double Data[Resolution][Resolution][Resolution];

	CVolumeData()
	{
		std::cout << "Generating Volume Data..." << std::endl;
		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution; ++ x)
			Data[x][y][z] = 0.f;

		
		int TunnelSampleSize = 128;

		//Perlin * TunnelGen = new Perlin(TunnelSampleSize, 8, 0.2, 1.0, 205);

		TunnelSampleSize = 512;

		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution; ++ x)/*
			setVolumeData(x, y, z, (float) (TunnelGen->Get(
			x / (double) Resolution * TunnelSampleSize, 
			y / (double) Resolution * TunnelSampleSize, 
			z / (double) Resolution * TunnelSampleSize, false) < 0.0 ? -1.0 : 1.0));*/
		{
			if (x == 0 || y == 0 || z == 0 || x == Resolution - 1 || y == Resolution - 1 || z == Resolution - 1)
				setVolumeData(x, y, z, 1.0);
			else
				setVolumeData(x, y, z, (double) Perlin.noise(x / (double) Resolution, y / (double) Resolution, z / (double) Resolution));/*(TunnelGen->Get(
				x / (double) Resolution * TunnelSampleSize, 
				y / (double) Resolution * TunnelSampleSize, 
				z / (double) Resolution * TunnelSampleSize, false)));*/
		}
		//pow(x - Resolution / 2.f, 2.f) + pow(y - Resolution / 2.f, 2.f) + pow(z - Resolution / 2.f, 2.f) - pow(Resolution / 3.f, 2.f));
	}

	double const getVolumeData(int const x, int const y, int const z) const
	{
		return Data[x][y][z];
	}

	void setVolumeData(int const x, int const y, int const z, double const value)
	{
		Data[x][y][z] = value;
		//printf("%f\n", value);
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

		float const ScaleFactor = 4.f * VolumeData.Resolution / 32.f;

#if 0
		for (int z = 0; z < VolumeData.Resolution; ++ z)
		for (int y = 0; y < VolumeData.Resolution; ++ y)
		for (int x = 0; x < VolumeData.Resolution; ++ x)
		{
			static float const Range = 11.f;
			if (equals(VolumeData.getVolumeData(x, y, z), 0.f, Range))
			{
				CSceneObject * Object = SceneManager.addMeshSceneObject(Cube, Shader);
				Object->setParent(VoxelObject);
				Object->setScale(SVector3(1.f) / 2.f / ScaleFactor);
				Object->setTranslation(SVector3((float) x, (float) y, (float) z) / ScaleFactor);
				Object->addUniform("uLightPosition", & BindLightPosition);/*
				CSceneObject * Object = SceneManager.addMeshSceneObject(TyraMesh, CShaderLoader::loadShader("NormalMap"), CMaterial());
				Object->setCullingEnabled(false);
				Object->addUniform("uLightPosition", & BindLightPosition);
				CTexture * Texture = CTextureLoader::loadTexture("TyraNormals.bmp");
				Object->setTexture(Texture);
				Object->setParent(VoxelObject);
				Object->setScale(SVector3(1.f) / 2.f / ScaleFactor);
				Object->setTranslation(SVector3((float) x, (float) y, (float) z) / ScaleFactor);*/
			}
		} // x - y - z
#endif

		CMesh * Mesh = new CMesh();
		int CurrentBuffer = 0;
		Mesh->MeshBuffers.push_back(new CMesh::SMeshBuffer());

		std::cout << "Creating polygonal mesh..." << std::endl;

		for (int z = 0; z < VolumeData.Resolution - 1; ++ z)
		for (int y = 0; y < VolumeData.Resolution - 1; ++ y)
		for (int x = 0; x < VolumeData.Resolution - 1; ++ x)
		{
			int lookup = 0;

			if ((VolumeData.getVolumeData(x, y, z)<0.f)) lookup |= 128;
			if ((VolumeData.getVolumeData(x+1, y, z)< 0.0)) lookup |= 64;
			if ((VolumeData.getVolumeData(x+1, y+1, z)< 0.0)) lookup |= 4;
			if ((VolumeData.getVolumeData(x, y+1, z)< 0.0)) lookup |= 8;
			if ((VolumeData.getVolumeData(x, y, z+1)< 0.0)) lookup |= 16;
			if ((VolumeData.getVolumeData(x+1, y, z+1)< 0.0)) lookup |= 32;
			if ((VolumeData.getVolumeData(x+1, y+1, z+1)< 0.0)) lookup |= 2;
			if ((VolumeData.getVolumeData(x, y+1, z+1)< 0.0)) lookup |= 1;

			SVertex verts[12];

			auto interpolate = [& ScaleFactor, this](int const v1x, int const v1y, int const v1z, int const v2x, int const v2y, int const v2z) -> SVertex
			{
				SVertex v;
				double const d1 = VolumeData.getVolumeData(v1x, v1y, v1z);
				double const d2 = VolumeData.getVolumeData(v2x, v2y, v2z);
				double dratio = d1 / (d2 - d1);
				if (dratio < 0.0)
					dratio += 1.0;
				float ratio = (float) dratio;
				v.Position = (SVector3((float) v1x, (float) v1y, (float) v1z) * (float) (ratio) + SVector3((float) v2x, (float) v2y, (float) v2z) * (1.f - ratio)) / ScaleFactor;
				return v;
			};

			if ((lookup != 0) && (lookup != 255))
			{
				// 0 - 1
				if (edgeTable[lookup] & 1)
					verts[0] = interpolate(x, y+1, z+1, x+1, y+1, z+1);

				// 1 - 2
				if (edgeTable[lookup] & 2)
					verts[1] = interpolate(x+1, y+1, z+1, x+1, y+1, z);

				// 2 - 3
				if (edgeTable[lookup] & 4)
					verts[2] = interpolate(x+1, y+1, z, x, y+1, z);

				// 3 - 0
				if (edgeTable[lookup] & 8)
					verts[3] = interpolate(x, y+1, z, x, y+1, z+1);
			
				// 4 - 5
				if (edgeTable[lookup] & 16)
					verts[4] = interpolate(x, y, z+1, x+1, y, z+1);

				// 5 - 6
				if (edgeTable[lookup] & 32)
					verts[5] = interpolate(x+1, y, z+1, x+1, y, z);

				// 6 - 7
				if (edgeTable[lookup] & 64)
					verts[6] = interpolate(x+1, y, z, x, y, z);

				// 7 - 4
				if (edgeTable[lookup] & 128)
					verts[7] = interpolate(x, y, z, x, y, z+1);

				// 0 - 4
				if (edgeTable[lookup] & 256)
					verts[8] = interpolate(x, y+1, z+1, x, y, z+1);

				// 1 - 5
				if (edgeTable[lookup] & 512)
					verts[9] = interpolate(x+1, y+1, z+1, x+1, y, z+1);

				// 2 - 6
				if (edgeTable[lookup] & 1024)
					verts[10] = interpolate(x+1, y+1, z, x+1, y, z);

				// 3 - 7
				if (edgeTable[lookup] & 2048)
					verts[11] = interpolate(x, y+1, z, x, y, z);

				int i, j;
					
				for (i = 0; triTable[lookup][i] != -1; i+=3)
				{
					if (Mesh->MeshBuffers[CurrentBuffer]->Vertices.size() + 3 >= 1 << 16)
					{
						++ CurrentBuffer;
						Mesh->MeshBuffers.push_back(new CMesh::SMeshBuffer());
					}

					for (j = i; j < (i+3); j++)
					{
						Mesh->MeshBuffers[CurrentBuffer]->Vertices.push_back(verts[triTable[lookup][j]]);
					}
					CMesh::STriangle Tri;
					Tri.Indices[0] = Mesh->MeshBuffers[CurrentBuffer]->Vertices.size() - 3;
					Tri.Indices[1] = Mesh->MeshBuffers[CurrentBuffer]->Vertices.size() - 2;
					Tri.Indices[2] = Mesh->MeshBuffers[CurrentBuffer]->Vertices.size() - 1;
					Mesh->MeshBuffers[CurrentBuffer]->Triangles.push_back(Tri);
				}
			}
		} // x - y - z

		Mesh->calculateNormalsPerFace();
		SoupObject = SceneManager.addMeshSceneObject(Mesh, Shader);
		SoupObject->setVisible(true);
		SoupObject->setCullingEnabled(false);
		SoupObject->addUniform("uLightPosition", & BindLightPosition);
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
