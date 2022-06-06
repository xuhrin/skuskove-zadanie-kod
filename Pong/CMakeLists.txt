add_executable(Pong)

target_link_libraries(Pong
	PRIVATE
		SDL2::SDL2
		SDL2::SDL2main
		glad::glad
)

target_compile_features(Pong
	PUBLIC
		cxx_std_17
)

target_sources(Pong
	PRIVATE
		"Pong.cpp"
		"Vector2.hpp"
		"Matrix3.hpp"
		"Mesh.hpp"
		"Mesh.cpp"
		"Paddle.hpp"
		"Paddle.cpp"
		"Ball.hpp"
		"Ball.cpp"
)
