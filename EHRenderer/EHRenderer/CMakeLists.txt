# CMakeList.txt: EHRenderer에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#d

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (EHRenderer WIN32
	"main.cpp" "EHRenderer.h" "SystemClass.cpp" "InputClass.cpp" "ApplicationClass.cpp" 
	"DirectX11/D3DClass.cpp" "DirectX11/CameraClass.cpp"
	
	"DirectX11/Model/SimpleModelClass.cpp" 
	"DirectX11/Model/ModelClass.cpp"
	
	"DirectX11/Shader/Color/ColorShaderClass.cpp"
	"DirectX11/Shader/Texture/TextureshaderClass.cpp"
	"DirectX11/Shader/Light/LightShaderClass.cpp"
	"DirectX11/Shader/Font/FontShaderClass.cpp"
	
	"DirectX11/Shader/TextureClass.cpp"
	"DirectX11/Shader/LIghtClass.cpp" "DirectX11/Sprite/SpriteClass.cpp" "TimerClass.cpp"
	
	"DirectX11/Font/FontClass.cpp" "DirectX11/Text/TextClass.cpp")
	

# target_link_libraries (EHRenderer PUBLIC "wrl.lib")
target_link_libraries (EHRenderer PUBLIC "dxgi.lib")
target_link_libraries (EHRenderer PUBLIC "d3d11.lib")
target_link_libraries (EHRenderer PUBLIC "d3dcompiler.lib")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET EHRenderer PROPERTY CXX_STANDARD 20)
endif()

file (COPY "./DirectX11/Shader/HLSL/"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/HLSL")

file (COPY "../Assets/"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Assets")

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
#add_subdirectory ("DirectX11")