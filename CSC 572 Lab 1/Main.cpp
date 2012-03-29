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

	unsigned char Data[Resolution / 8][Resolution][Resolution];

	CVolumeData()
	{
		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution / 8; ++ x)
			Data[x][y][z] = 0x00;

		for (int z = 0; z < Resolution; ++ z)
		for (int y = 0; y < Resolution; ++ y)
		for (int x = 0; x < Resolution; ++ x)
			if (equals(pow(x - Resolution / 2.f, 2.f) + pow(y - Resolution / 2.f, 2.f) + pow(z - Resolution / 2.f, 2.f), pow(Resolution / 3.f, 2.f), 11.f))
				setVolumeData(x, y, z, true);
	}

	bool getVolumeData(int x, int y, int z)
	{
		return (Data[x/8][y][z] & (1 << (x % 8))) != 0;
	}

	void setVolumeData(int x, int y, int z, bool value)
	{
		if (value)
			Data[x/8][y][z] |= (1 << (x % 8));
		else
			Data[x/8][y][z] ^= (1 << (x % 8));
	}

};

class CMainState : public CState<CMainState>
{

	CCameraControl * Camera;
	CMeshSceneObject * Tyra;

	CMeshSceneObject * LightObject;

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

		for (int z = 0; z < VolumeData.Resolution; ++ z)
		for (int y = 0; y < VolumeData.Resolution; ++ y)
		for (int x = 0; x < VolumeData.Resolution; ++ x)
			if (VolumeData.getVolumeData(x, y, z))
			{
				CSceneObject * Object = SceneManager.addMeshSceneObject(Cube, Shader);
				Object->setScale(SVector3(1.f)/8.f);
				Object->setTranslation(SVector3((float) x, (float) y, (float) z)/4.f);
			}

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
		                glm::vec4 trans(difX*moveSpeed, difY*moveSpeed, 0, 0);
		                trans = ViewMatrix * trans;
		                Translation.X += trans.x;
		                Translation.Y -= trans.y;
		                Translation.Z -= trans.z;
	                }
	                else if (Mode == 2)
	                {
		                float const scaleSpeed = 0.01f;
                        Scale = SVector3(std::max(std::min(Scale.X + difX*scaleSpeed, 2.f), 0.1f));
	                }
					else if (Mode == 3)
	                {
		                float const moveSpeed = 0.01f;
		                glm::vec4 trans(difX*moveSpeed, difY*moveSpeed, 0, 0);
		                trans = ViewMatrix * trans;
						LightPosition.X += trans.x;
		                LightPosition.Y -= trans.y;
		                LightPosition.Z -= trans.z;
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
