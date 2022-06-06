#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <glad/glad.h>
#include <vector>
#include <ctime>

#include "Matrix3.hpp"
#include "Mesh.hpp"
#include "Ball.hpp"

static const char* const VertexShaderSource = R"(

attribute vec2 aPosition;
		
uniform mat3 uMP;

void main() {
	gl_Position.xyz = uMP * vec3(aPosition, 1.0);
	gl_Position.w   = 1.0;
}

)";

static const char* const FragmentShaderSource = R"(

void main() {
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
	
)";

GLuint CompileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		return shader;
	}

	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		char* message = new char[length];
		glGetShaderInfoLog(shader, length, &length, message);
		SDL_Log("Shader compilation failed: %s", message);
		delete[] message;
	}
	return 0;
}

bool LinkProgram(GLuint program) {
	glLinkProgram(program);

	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_TRUE) {
		return true;
	}

	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		char* message = new char[length];
		glGetProgramInfoLog(program, length, &length, message);
		SDL_Log("Linking program failed: %s", message);
		delete[] message;
	}
	return false;
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_DisplayMode mode;
	if (SDL_GetDisplayMode(0, 0, &mode) < 0) {
		SDL_Log("Failed to retrieve display modes: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* window;
	window = SDL_CreateWindow("Pong", 0, 0, mode.w, mode.h, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

	SDL_GLContext context;
	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (SDL_GL_MakeCurrent(window, context) < 0) {
		SDL_Log("Failed to make OpenGL context current: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to load OpenGL ES functions");
		return EXIT_FAILURE;
	}

	int w, h;
	SDL_GL_GetDrawableSize(window, &w, &h);

	glViewport(0, 0, w, h);
	glClearColor(0.F, 0.F, 0.F, 1.F);

	SDL_GL_SetSwapInterval(1);

	//
	// Shaders
	//

	GLuint shaderProgram = glCreateProgram();
	{
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, VertexShaderSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, FragmentShaderSource);

		if (vertexShader == 0 || fragmentShader == 0) {
			return EXIT_FAILURE;
		}

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glBindAttribLocation(shaderProgram, 0, "aPosition");

		if (!LinkProgram(shaderProgram)) {
			return EXIT_FAILURE;
		}

		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	GLint uMP = glGetUniformLocation(shaderProgram, "uMP");
	if (uMP < 0) {
		SDL_Log("Cannot obtain uniform location!");
		return EXIT_FAILURE;
	}

	//
	// Projection
	//

	float ratio = float(w) / float(h);
	float pHorizontal = 5.F * ratio;
	float pVertical = 5.F;

	Matrix3 projection = Matrix3::Ortho(pHorizontal, pVertical);

	//
	// Random
	//

	std::srand(std::time(nullptr));

	auto RandomAngle = []() -> float {
		int sector = rand() & 0x3;
		int angle = rand() % 45;

		if (sector == 3) {
			angle = 360 - angle;
		}
		else if (sector == 2) {
			angle = 180 + angle;
		}
		else if (sector == 1) {
			angle = 180 - angle;
		}

		return (float(angle) / 180.F) * M_PI;
	};

	//
	// Meshes
	//

	Ball ball;
	ball.SetRadius(0.25F);
	ball.Reset(RandomAngle());

	const Vector2 ResetScalePaddle = { 0.33F, 1.5F };
	const Vector2 ResetPosPaddle1 = { -pHorizontal + 0.5F, 0.F };
	const Vector2 ResetPosPaddle2 = { pHorizontal - 0.5F, 0.F };

	Paddle paddle1;
	Paddle paddle2;
	paddle1.Reset(ResetPosPaddle1, ResetScalePaddle);
	paddle2.Reset(ResetPosPaddle2, ResetScalePaddle);

	//
	// Loop
	//

	int numKeys;
	const Uint8* keystate = SDL_GetKeyboardState(&numKeys);

	SDL_Event e;
	bool isRunning = true;
	Uint32 lastTime = 0;
	while (isRunning) {

		//
		// Compute delta time
		//

		Uint32 thisTime = SDL_GetTicks();
		Uint32 diffTime = thisTime - lastTime;
		lastTime = thisTime;

		float delta = float(diffTime) / 1000.F;

		//
		// Process input
		//

		if (keystate[SDL_SCANCODE_UP]) {
			paddle2.Move(delta, Paddle::Up, pVertical);
		}
		else if (keystate[SDL_SCANCODE_DOWN]) {
			paddle2.Move(delta, Paddle::Down, pVertical);
		}

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}

		//
		// Update
		//

		float dY = ball.GetPosition().y - paddle1.GetPosition().y;
		if (dY > 0.2F) {
			paddle1.Move(delta, Paddle::Up, pVertical);
		}
		else if (dY < -0.2F) {
			paddle1.Move(delta, Paddle::Down, pVertical);
		}

		ball.Update(delta);

		if (ball.CheckCollision(paddle1) || ball.CheckCollision(paddle2) || ball.CheckBorderCollision(pVertical)) {
			// napr. prehraj zvuk
		}

		int paddleCheck = ball.CheckPaddleBorderCollision(pHorizontal);
		if (paddleCheck != 0) {
			if (paddleCheck < 0) {
				// ľavý
			}
			else {
				// pravý
			}
			paddle1.Reset(ResetPosPaddle1, ResetScalePaddle);
			paddle2.Reset(ResetPosPaddle2, ResetScalePaddle);
			ball.Reset(RandomAngle());
		}

		//
		// Render
		//

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		Matrix3 mp;

		int count = 11;
		float pHeight = pVertical * 2.F;
		float step = pHeight / float(count);
		float y = step * 0.5F - pVertical;
		Matrix3 m;
		m.SetScale({ 0.25F, 0.45F });
		for (int i = 0; i < count; ++i) {
			m.SetPosition({ 0.F, y + step * i });
			mp = m * projection;
			glUniformMatrix3fv(uMP, 1, GL_FALSE, mp.GetPtr());
			Mesh::GetMesh(Mesh::BoxID).Render();
		}

		mp = ball.GetMatrix() * projection;
		glUniformMatrix3fv(uMP, 1, GL_FALSE, mp.GetPtr());

		ball.Render();

		mp = paddle1.GetMatrix() * projection;
		glUniformMatrix3fv(uMP, 1, GL_FALSE, mp.GetPtr());

		paddle1.Render();

		mp = paddle2.GetMatrix() * projection;
		glUniformMatrix3fv(uMP, 1, GL_FALSE, mp.GetPtr());

		paddle2.Render();

		SDL_GL_SwapWindow(window);
	}

	glDeleteProgram(shaderProgram);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
