compile:
	g++ \
	-D_CRT_SECURE_NO_WARNINGS \
	-Idependencies/glm98 \
	-Idependencies\glad-master\include \
	-Idependencies\assimp-3.1.1\include \
	-Idependencies\glfw-master\include \
	-Ldependencies\assimp-3.1.1\lib64 \
	-Ldependencies\glad-master\Debug \
	-Ldependencies\glfw-master\Debug\Lib \
	-lglad \
	-lglfw3 \
	-lopengl32 \
	-lglu32  \
	-g -Wall -std=c++17 \
	src/main.cpp src/Actor.cpp src/Model.cpp src/Mesh.cpp src/Shader.cpp \
	src/Render.cpp \
	-o bin/OpenglLearn