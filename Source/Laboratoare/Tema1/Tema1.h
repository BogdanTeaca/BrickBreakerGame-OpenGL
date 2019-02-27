#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

// TEACA BOGDAN

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	// enum pentru starea jocului cand jucatorul nu a apasat pe
	// butonul de la mouse, iar bila se misca odata cu platforma (ON_PADDLE)
	// si starea normala a jocului dupa ce jucatorul a apasat pe butonul
	// de la mouse, iar bila se afla in miscare (MOVING)
	enum Ball_State { ON_PADDLE, MOVING };

	// structura pentru proprietatile unei caramizi
	typedef struct Brick {
		float x; // coordonata x a coltului din stanga jos a caramizii
		float y; // coordonata y a coltului din stanga jos a caramizii
		bool alive; // daca aceasta caramida este in viata (nu este distrusa)
		float dead_time; // cat timp a trecut de cand caramida a fost distrusa
		int lives; // numarul de vieti ale caramizii
	} Brick;

	// structura pentru proprietatile unei bile
	typedef struct Ball {
		float x; // coordonata x a coltului din stanga jos a bilei
		float y; // coordonata y a coltului din stanga jos a bilei
		float vel_x; // componenta x a vitezei bilei
		float vel_y; // componenta y a vitezei bilei
	} Ball;

	// structura pentru proprietatile unui powerup care se afla in cadere
	typedef struct Powerup {
		float x; // coordonata x a coltului din stanga jos a powerup-ului
		float y; // coordonata y a coltului din stanga jos a powerup-ului
		float falling_time; // cat timp a trecut de cand powerup-ul se afla in cadere
	} Powerup;

	Brick ** bricks; // matrice pentru caramizile din joc
	Ball_State ball_state; // starea bilei (ON_PADDLE sau MOVING)

	std::list<Ball> ball_list; // lista bilelor din scena
	std::list<Powerup> powerup_list; // lista powerup-urilor care se afla in cadere in scena

	float screen_width = 1280; // latimea ecranului
	float screen_height = 720; // inaltimea ecranului
	float max_ball_speed = 10; // viteza maxima admisa pentru o bila
	float min_ball_speed = 5; // viteza minima admisa pentru o bila

	int player_lives = 3; // numarul initial de vieti ale jucatorului
	int level = 1; // nivelul initial de unde incepe jocul

	float brick_width = 62; // latimea unei caramizi
	float brick_height = 35; // inaltimea unei caramizi
	float brick_gap = 10; // spatiul liber dintre doua linii sau coloane de caramizi
	float brick_offset_x = 33; // offset-ul orizontal de unde incepe pattern-ul de caramizi
	float brick_offset_y = 250; // offeset-ul vertical de unde incepe pattern-ul de caramizi
	int brick_columns = 17; // numarul de coloane de caramizi
	int brick_rows = 7; // numarul de linii de caramizi

	float wall_thickness = 30; // grosimea zidurilor de pe marginea scenei
	float wall_offset = 45; // offset-ul zidurilor laterale fata de partea de jos a scenei

	float powerup_radius = 16; // latimea unui powerup in cadere
	float powerup_duration = 60; // durata pentru care un powerup este activ
	float powerup_falling_speed = 3; // viteza cu care cade un powerup
	int powerup_rarity = 5; // cat de rar este ca la distrugerea unei caramizi sa cada un powerup

	int lastMousePosX = 0; // variabila in care retin pozitia x a cursorului
	                       // pentru a fi folosita in functia Update()

	float long_paddle_duration = 0; // variabila in care retin cat a ramas din durata powerup-ului "long paddle"
	float super_ball_duration = 0; // variabila in care retin cat a ramas din durata powerup-ului "super ball"

	float paddle_x = 300; // coordonata x a coltului din stanga jos a platformei
	float paddle_y = 20; // coordonata y a coltului din stanga jos a platformei
	float paddle_width = 200; // latimea platformei
	float paddle_height = 20; // inaltimea platformei

	float collision_const = 7; // constanta cu care imi dau seama din ce parte a caramizii a lovit bila
	float ball_radius = 8; // raza bilei
	float ball_accel = 0.001f; // constanta cu care creste viteza bilelor in timp

	void Init() override;

private:
	int Tema1::CircleRectangleCollision(float circ_diameter, float circ_corner_x, float circ_corner_y, float rect_width, float rect_height, float rect_x, float rect_y);
	void Tema1::InitLevel();
	void Tema1::Update_Walls();
	void Tema1::Update_Lives_Icons();
	void Tema1::Update_Bricks(float deltaTimeSeconds);
	void Tema1::Update_Paddle(float deltaTimeSeconds);
	void Tema1::Update_Powerups(float deltaTimeSeconds);
	void Tema1::Update_Balls(float deltaTimeSeconds);
	void Tema1::Update_Level_Clear_Check();

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float translateX, translateY;
	float scaleX, scaleY;
	float angularStep;
};
