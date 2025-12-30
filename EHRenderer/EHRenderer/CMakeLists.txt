# CMakeList.txt: EHRenderer에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
# 

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (EHRenderer WIN32
	"main.cpp" "EHRenderer.h" "SystemClass.cpp" "SimpleInputClass.cpp" "ApplicationClass.cpp" 
	"DirectX11/D3DClass.cpp" "DirectX11/CameraClass.cpp"
	
	"DirectX11/Data/SimpleModelClass.cpp" 
	"DirectX11/Data/ModelClass.cpp"
	
	"DirectX11/Shader/Color/ColorShaderClass.cpp"
	"DirectX11/Shader/Texture/TextureShaderClass.cpp"
	"DirectX11/Shader/Light/LightShaderClass.cpp"
	"DirectX11/Shader/Font/FontShaderClass.cpp"
	
	"DirectX11/Data/TextureClass.cpp"
	"DirectX11/Shader/LIghtClass.cpp" "DirectX11/Data/SpriteClass.cpp" "TimerClass.cpp"
	
	"DirectX11/Data/FontClass.cpp" "DirectX11/Data/TextClass.cpp"
	"DirectX11/Shader/ShaderBase.cpp" "DirectX11/InputClass.cpp"
	"DirectX11/Shader/MultiTexture/MultiTextureShaderClass.cpp"
	"DirectX11/Shader/LightMap/LightMapShaderClass.cpp"
	"DirectX11/Shader/AlphaMap/AlphaMapShaderClass.cpp"
	"DirectX11/Shader/NormalMap/NormalMapShaderClass.cpp"
	"DirectX11/Shader/SpecMap/SpecMapShaderClass.cpp")
	

# target_link_libraries (EHRenderer PUBLIC "wrl.lib")
target_link_libraries (EHRenderer PUBLIC "dxgi.lib")
target_link_libraries (EHRenderer PUBLIC "d3d11.lib")
target_link_libraries (EHRenderer PUBLIC "d3dcompiler.lib")
target_link_libraries (EHRenderer PUBLIC "dinput8.lib")
target_link_libraries (EHRenderer PUBLIC "dxguid.lib")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET EHRenderer PROPERTY CXX_STANDARD 20)
endif()

file (COPY "./DirectX11/Shader/HLSL/"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/HLSL")

file (COPY "../Assets/"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Assets")

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
#add_subdirectory ("DirectX11")