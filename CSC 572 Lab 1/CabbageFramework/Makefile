DIRECTORIES = -I../CabbageCore -I../CabbageScene -I../CabbageFramework
OPENGL = -DGL_GLEXT_PROTOTYPES -lGL -lGLU -lSDL

CPP = g++ -O1 -s
CPPFLAGS = $(DIRECTORIES) $(OPENGL)

SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.h)
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(OBJS)

clean:
	rm -f $(OBJS) lab4 Makefile.bak

gedit:
	rm -f *~

cleanish: gedit
	rm -f $(OBJS) Makefile.bak

remake: clean depend all

depend:
	@echo Regenerating local dependencies.
	@makedepend -Y $(SRCS) $(HDRS) -s "# Dependencies" > /dev/null 2>&1
	@rm Makefile.bak

# Dependencies

CApplication.o: CApplication.h CEventManager.h CStateManager.h
CCameraControl.o: CCameraControl.h ../CabbageScene/CabbageScene.h
CCameraControl.o: ../CabbageScene/CMesh.h ../CabbageCore/SBoundingBox3.h
CCameraControl.o: ../CabbageCore/SVector3.h ../CabbageCore/glm/glm.hpp
CCameraControl.o: ../CabbageCore/glm/core/_fixes.hpp
CCameraControl.o: ../CabbageCore/glm/core/setup.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_detail.hpp
CCameraControl.o: ../CabbageCore/glm/./core/setup.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_half.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_half.inl
CCameraControl.o: ../CabbageCore/glm/./core/_detail.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_float.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_int.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_gentype.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_size.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec1.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle_func.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec1.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_trigonometric.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_trigonometric.inl
CCameraControl.o: ../CabbageCore/glm/./core/_vectorize.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_exponential.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_exponential.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_common.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_fixes.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_common.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_packing.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_packing.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_geometric.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_geometric.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_matrix.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_matrix.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_vector_relational.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_vector_relational.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_integer.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_integer.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_noise.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_noise.inl
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle.hpp
CCameraControl.o: ../CabbageCore/Utils.h ../CabbageCore/SVector2.h
CCameraControl.o: ../CabbageCore/SLine3.h ../CabbageScene/SVertex.h
CCameraControl.o: ../CabbageCore/SVector3.h ../CabbageCore/SVector2.h
CCameraControl.o: ../CabbageCore/SColor.h ../CabbageScene/CBufferObject.h
CCameraControl.o: ../CabbageScene/CMaterial.h ../CabbageScene/SUniform.h
CCameraControl.o: ../CabbageScene/CShaderContext.h ../CabbageScene/CShader.h
CCameraControl.o: ../CabbageScene/CTexture.h ../CabbageScene/CImage.h
CCameraControl.o: ../CabbageCore/STransformation3.h
CCameraControl.o: ../CabbageCore/glm/gtc/matrix_transform.hpp
CCameraControl.o: ../CabbageCore/glm/glm.hpp
CCameraControl.o: ../CabbageCore/glm/gtc/matrix_transform.inl
CCameraControl.o: ../CabbageCore/glm/glm.hpp ../CabbageScene/SAttribute.h
CCameraControl.o: ../CabbageScene/CMeshLoader.h ../CabbageScene/CRenderable.h
CCameraControl.o: ../CabbageCore/boost/shared_ptr.hpp
CCameraControl.o: ../CabbageScene/CCamera.h ../CabbageScene/ISceneObject.h
CCameraControl.o: ../CabbageScene/CMeshSceneObject.h
CCameraControl.o: ../CabbageScene/CSceneObject.h
CCameraControl.o: ../CabbageScene/CShaderLoader.h
CCameraControl.o: ../CabbageScene/CImageLoader.h
CCameraControl.o: ../CabbageScene/CSceneManager.h
CCameraControl.o: ../CabbageScene/CTextureLoader.h CEventManager.h
CCameraControl.o: CApplication.h
CEventManager.o: CEventManager.h CApplication.h
COrthoCamera.o: COrthoCamera.h ../CabbageCore/SVector3.h
COrthoCamera.o: ../CabbageCore/glm/glm.hpp ../CabbageCore/glm/core/_fixes.hpp
COrthoCamera.o: ../CabbageCore/glm/core/setup.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_detail.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/setup.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_half.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_half.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_detail.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_float.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_int.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_gentype.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_size.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec1.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle_func.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec1.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_trigonometric.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_trigonometric.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_vectorize.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_exponential.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_exponential.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_common.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_fixes.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_common.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_packing.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_packing.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_geometric.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_geometric.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_matrix.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_matrix.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_vector_relational.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_vector_relational.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_integer.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_integer.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_noise.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_noise.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle.hpp ../CabbageCore/Utils.h
COrthoCamera.o: ../CabbageCore/SVector2.h ../CabbageScene/CCamera.h
COrthoCamera.o: ../CabbageCore/glm/gtc/matrix_transform.hpp
COrthoCamera.o: ../CabbageCore/glm/glm.hpp
COrthoCamera.o: ../CabbageCore/glm/gtc/matrix_transform.inl
COrthoCamera.o: ../CabbageFramework/CApplication.h
CStateManager.o: CStateManager.h CEventManager.h CApplication.h
CabbageFramework.o: CApplication.h CEventManager.h CStateManager.h
CabbageFramework.o: CCameraControl.h ../CabbageScene/CabbageScene.h
CabbageFramework.o: ../CabbageScene/CMesh.h ../CabbageCore/SBoundingBox3.h
CabbageFramework.o: ../CabbageCore/SVector3.h ../CabbageCore/glm/glm.hpp
CabbageFramework.o: ../CabbageCore/glm/core/_fixes.hpp
CabbageFramework.o: ../CabbageCore/glm/core/setup.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/_detail.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/setup.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_half.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_half.inl
CabbageFramework.o: ../CabbageCore/glm/./core/_detail.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_float.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_int.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_gentype.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_size.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec1.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/_swizzle.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/_swizzle_func.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec1.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec2.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec2.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec3.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec3.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec4.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_vec4.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x2.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x2.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x3.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x3.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x4.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat2x4.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x2.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x2.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x3.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x3.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x4.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat3x4.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x2.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x2.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x3.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x3.inl
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x4.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/type_mat4x4.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_trigonometric.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_trigonometric.inl
CabbageFramework.o: ../CabbageCore/glm/./core/_vectorize.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_exponential.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_exponential.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_common.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/_fixes.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_common.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_packing.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_packing.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_geometric.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_geometric.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_matrix.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_matrix.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_vector_relational.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_vector_relational.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_integer.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_integer.inl
CabbageFramework.o: ../CabbageCore/glm/./core/func_noise.hpp
CabbageFramework.o: ../CabbageCore/glm/./core/func_noise.inl
CabbageFramework.o: ../CabbageCore/glm/./core/_swizzle.hpp
CabbageFramework.o: ../CabbageCore/Utils.h ../CabbageCore/SVector2.h
CabbageFramework.o: ../CabbageCore/SLine3.h ../CabbageScene/SVertex.h
CabbageFramework.o: ../CabbageCore/SVector3.h ../CabbageCore/SVector2.h
CabbageFramework.o: ../CabbageCore/SColor.h ../CabbageScene/CBufferObject.h
CabbageFramework.o: ../CabbageScene/CMaterial.h ../CabbageScene/SUniform.h
CabbageFramework.o: ../CabbageScene/CShaderContext.h
CabbageFramework.o: ../CabbageScene/CShader.h ../CabbageScene/CTexture.h
CabbageFramework.o: ../CabbageScene/CImage.h
CabbageFramework.o: ../CabbageCore/STransformation3.h
CabbageFramework.o: ../CabbageCore/glm/gtc/matrix_transform.hpp
CabbageFramework.o: ../CabbageCore/glm/glm.hpp
CabbageFramework.o: ../CabbageCore/glm/gtc/matrix_transform.inl
CabbageFramework.o: ../CabbageCore/glm/glm.hpp ../CabbageScene/SAttribute.h
CabbageFramework.o: ../CabbageScene/CMeshLoader.h
CabbageFramework.o: ../CabbageScene/CRenderable.h
CabbageFramework.o: ../CabbageCore/boost/shared_ptr.hpp
CabbageFramework.o: ../CabbageScene/CCamera.h ../CabbageScene/ISceneObject.h
CabbageFramework.o: ../CabbageScene/CMeshSceneObject.h
CabbageFramework.o: ../CabbageScene/CSceneObject.h
CabbageFramework.o: ../CabbageScene/CShaderLoader.h
CabbageFramework.o: ../CabbageScene/CImageLoader.h
CabbageFramework.o: ../CabbageScene/CSceneManager.h
CabbageFramework.o: ../CabbageScene/CTextureLoader.h
CCameraControl.o: ../CabbageCore/SVector3.h ../CabbageCore/glm/glm.hpp
CCameraControl.o: ../CabbageCore/glm/core/_fixes.hpp
CCameraControl.o: ../CabbageCore/glm/core/setup.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_detail.hpp
CCameraControl.o: ../CabbageCore/glm/./core/setup.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_half.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_half.inl
CCameraControl.o: ../CabbageCore/glm/./core/_detail.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_float.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_int.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_gentype.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_size.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec1.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle_func.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec1.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_vec4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_vec4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat2x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat3x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x2.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x2.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x3.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x3.inl
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x4.hpp
CCameraControl.o: ../CabbageCore/glm/./core/type_mat4x4.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_trigonometric.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_trigonometric.inl
CCameraControl.o: ../CabbageCore/glm/./core/_vectorize.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_exponential.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_exponential.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_common.hpp
CCameraControl.o: ../CabbageCore/glm/./core/_fixes.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_common.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_packing.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_packing.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_geometric.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_geometric.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_matrix.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_matrix.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_vector_relational.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_vector_relational.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_integer.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_integer.inl
CCameraControl.o: ../CabbageCore/glm/./core/func_noise.hpp
CCameraControl.o: ../CabbageCore/glm/./core/func_noise.inl
CCameraControl.o: ../CabbageCore/glm/./core/_swizzle.hpp
CCameraControl.o: ../CabbageCore/Utils.h ../CabbageCore/SVector2.h
CCameraControl.o: ../CabbageScene/CabbageScene.h ../CabbageScene/CMesh.h
CCameraControl.o: ../CabbageCore/SBoundingBox3.h ../CabbageCore/SLine3.h
CCameraControl.o: ../CabbageScene/SVertex.h ../CabbageCore/SVector3.h
CCameraControl.o: ../CabbageCore/SVector2.h ../CabbageCore/SColor.h
CCameraControl.o: ../CabbageScene/CBufferObject.h ../CabbageScene/CMaterial.h
CCameraControl.o: ../CabbageScene/SUniform.h ../CabbageScene/CShaderContext.h
CCameraControl.o: ../CabbageScene/CShader.h ../CabbageScene/CTexture.h
CCameraControl.o: ../CabbageScene/CImage.h ../CabbageCore/STransformation3.h
CCameraControl.o: ../CabbageCore/glm/gtc/matrix_transform.hpp
CCameraControl.o: ../CabbageCore/glm/glm.hpp
CCameraControl.o: ../CabbageCore/glm/gtc/matrix_transform.inl
CCameraControl.o: ../CabbageCore/glm/glm.hpp ../CabbageScene/SAttribute.h
CCameraControl.o: ../CabbageScene/CMeshLoader.h ../CabbageScene/CRenderable.h
CCameraControl.o: ../CabbageCore/boost/shared_ptr.hpp
CCameraControl.o: ../CabbageScene/CCamera.h ../CabbageScene/ISceneObject.h
CCameraControl.o: ../CabbageScene/CMeshSceneObject.h
CCameraControl.o: ../CabbageScene/CSceneObject.h
CCameraControl.o: ../CabbageScene/CShaderLoader.h
CCameraControl.o: ../CabbageScene/CImageLoader.h
CCameraControl.o: ../CabbageScene/CSceneManager.h
CCameraControl.o: ../CabbageScene/CTextureLoader.h CEventManager.h
CCameraControl.o: CApplication.h
CEventManager.o: ../CabbageCore/SVector2.h
COrthoCamera.o: ../CabbageCore/SVector3.h ../CabbageCore/glm/glm.hpp
COrthoCamera.o: ../CabbageCore/glm/core/_fixes.hpp
COrthoCamera.o: ../CabbageCore/glm/core/setup.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_detail.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/setup.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_half.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_half.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_detail.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_float.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_int.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_gentype.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_size.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec1.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle_func.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec1.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_vec4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat2x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat3x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x2.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x2.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x3.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x3.inl
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x4.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/type_mat4x4.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_trigonometric.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_trigonometric.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_vectorize.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_exponential.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_exponential.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_common.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/_fixes.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_common.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_packing.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_packing.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_geometric.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_geometric.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_matrix.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_matrix.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_vector_relational.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_vector_relational.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_integer.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_integer.inl
COrthoCamera.o: ../CabbageCore/glm/./core/func_noise.hpp
COrthoCamera.o: ../CabbageCore/glm/./core/func_noise.inl
COrthoCamera.o: ../CabbageCore/glm/./core/_swizzle.hpp ../CabbageCore/Utils.h
COrthoCamera.o: ../CabbageCore/SVector2.h ../CabbageScene/CCamera.h
CStateManager.o: CEventManager.h CApplication.h
