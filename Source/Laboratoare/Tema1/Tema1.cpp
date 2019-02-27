#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

// TEACA BOGDAN

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// alocam memorie pentru caramizile din joc
	bricks = (Brick **)malloc(brick_columns * sizeof(Brick *));

	for (int i = 0; i < brick_columns; i++) {
		bricks[i] = (Brick *)malloc(brick_rows * sizeof(Brick));
	}

	// cream meshe pentru:
	// - caramizi normala
	// - caramizi cu 2 vieti
	// - caramizi cu 3 vieti
	// - zid de sus
	// - zid lateral
	// - platforma jucatorului
	// - platforma lunga (daca powerup-ul "extend paddle" este activat)
	// - bila normala
	// - super bila (daca powerup-ul "super ball" este activat)
	// - powerup-uri
	// - vieti ale jucatorului

	Mesh * brick = Object2D::CreateRectangle("brick", corner, brick_width,
		                                     brick_height, glm::vec3(1, 0, 0), true);
	Mesh * hard_brick = Object2D::CreateRectangle("hard_brick", corner, brick_width,
		                                          brick_height, glm::vec3(0.5, 0.5, 1), true);
	Mesh * very_hard_brick = Object2D::CreateRectangle("very_hard_brick", corner, brick_width,
		                                               brick_height, glm::vec3(0, 0, 1), true);
	Mesh * top_wall = Object2D::CreateRectangle("top_wall", corner, screen_width,
		                                        wall_thickness, glm::vec3(1, 0, 0), true);
	Mesh * side_wall = Object2D::CreateRectangle("side_wall", corner, wall_thickness,
		                                         screen_height - wall_offset, glm::vec3(1, 0, 0), true);
	Mesh * paddle = Object2D::CreateRectangle("paddle", corner, paddle_width,
		                                      paddle_height, glm::vec3(1, 1, 1), true);
	Mesh * long_paddle = Object2D::CreateRectangle("long_paddle", corner, paddle_width + 200,
		                                           paddle_height, glm::vec3(1, 1, 1), true);
	Mesh * ball = Object2D::CreateCircle("ball", corner, ball_radius, glm::vec3(1, 1, 0), true);
	Mesh * super_ball = Object2D::CreateCircle("super_ball", corner, ball_radius,
		                                       glm::vec3(1, 0, 0), true);
	Mesh * powerup = Object2D::CreateRectangle("powerup", corner, powerup_radius,
		                                       powerup_radius, glm::vec3(0, 0.5, 1), true);
	Mesh * player_life = Object2D::CreateCircle("player_life", corner, ball_radius,
		                                        glm::vec3(0, 1, 0), true);

	AddMeshToList(brick);
	AddMeshToList(top_wall);
	AddMeshToList(side_wall);
	AddMeshToList(hard_brick);
	AddMeshToList(very_hard_brick);
	AddMeshToList(paddle);
	AddMeshToList(long_paddle);
	AddMeshToList(ball);
	AddMeshToList(super_ball);
	AddMeshToList(powerup);
	AddMeshToList(player_life);

	// initializam primul nivel
	InitLevel();
}

void Tema1::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::InitLevel() {
	// resetez toate variabilele ce tin de initializarea unui nivel

	// setez bila sa stea pe platforma la inceputul nivelului
	ball_state = ON_PADDLE;

	// resetez lista de bile si powerup-uri
	ball_list.clear();
	powerup_list.clear();

	// resetez powerup-urile care au durata de timp
	long_paddle_duration = 0;
	super_ball_duration = 0;

	// adaug bila initiala in lista de bile
	ball_list.push_back({ 0, 0, 0, 4 });

	// initializez pozitiile si vietile caramizilor in functie de nivelul curent
	if (level == 1) {
		// caramizile din primul nivel

		for (int i = 3; i < brick_columns - 3; i++) {
			for (int j = 0; j < brick_rows; j++) {
				float brick_x = (float)(brick_offset_x + i * (brick_width + brick_gap));
				float brick_y = (float)(brick_offset_y + j * (brick_height + brick_gap));

				if (i < 5 || i > brick_columns - 6) {
					bricks[i][j] = { brick_x, brick_y, true, 0, 3 };
				}else if (i < 7 || i > brick_columns - 8) {
					bricks[i][j] = { brick_x, brick_y, true, 0, 2 };
				}else {
					bricks[i][j] = { brick_x, brick_y, true, 0, 1 };
				}
			}
		}
	}else if (level == 2) {
		// caramizile din al doilea nivel

		for (int i = 0; i < brick_columns; i++) {
			for (int j = 0; j < brick_rows; j += 2) {
				float brick_x = (float)(brick_offset_x + i * (brick_width + brick_gap));
				float brick_y = (float)(brick_offset_y + j * (brick_height + brick_gap));

				if (i < 3 || i > brick_columns - 4) {
					bricks[i][j] = { brick_x, brick_y, true, 0, 2 };
				}else if (i < 8 || i > brick_columns - 9) {
					bricks[i][j] = { brick_x, brick_y, true, 0, 1 };
				}else {
					bricks[i][j] = { brick_x, brick_y, true, 0, 3 };
				}
			}
		}
	}else if (level == 3) {
		// caramizile din al treilea nivel

		for (int i = 0; i < brick_columns; i++) {
			for (int j = 0; j < brick_rows; j++) {
				if ((i + j) % 2 == 0) {
					float brick_x = (float)(brick_offset_x + i * (brick_width + brick_gap));
					float brick_y = (float)(brick_offset_y + j * (brick_height + brick_gap));

					if (j < 4) {
						bricks[i][j] = { brick_x, brick_y, true, 0, 1 };
					}else if (j < 6) {
						bricks[i][j] = { brick_x, brick_y, true, 0, 2 };
					}else {
						bricks[i][j] = { brick_x, brick_y, true, 0, 3 };
					}
				}
			}
		}
	}
}

int Tema1::CircleRectangleCollision(float circ_diameter, float circ_corner_x, float circ_corner_y,
	                                float rect_width, float rect_height, float rect_x, float rect_y) {
	// functie ce verifica daca are loc o coliziune intre un cerc (de exemplu o bila) si un dreptunghi
	// (de exemplu o caramida sau platforma)

	// valoarea returnata inseamna:
	// 0 daca nu are loc nicio coliziune
	// 1 daca are loc o coliziune iar aceasta este in partea stanga sau dreapta a dreptunghiului
	// 2 daca are loc o coliziune iar aceasta este in partea de jos sau de sus a dreptunghiului
	// 3 daca are loc o coliziune iar aceasta este intr-un colt al dreptunghiului

	int result = 0;

	float circ_center_x = circ_corner_x + circ_diameter / 2;
	float circ_center_y = circ_corner_y + circ_diameter / 2;

	float deltaX = circ_center_x - glm::max(rect_x, glm::min(circ_center_x, rect_x + rect_width));
	float deltaY = circ_center_y - glm::max(rect_y, glm::min(circ_center_y, rect_y + rect_height));

	if ((deltaX * deltaX + deltaY * deltaY) < (circ_diameter * circ_diameter / 4)) {
		// are loc o coliziune dintre cerc si dreptunghi

		// verificam daca aceasta coliziune are loc in partea stanga sau
		// dreapta a dreptunghiului
		if (circ_corner_x + circ_diameter >= rect_x &&
			circ_corner_x + circ_diameter <= rect_x + collision_const) {
			result = 1;
		}else if (circ_corner_x <= rect_x + rect_width &&
			      circ_corner_x >= rect_x + rect_width - collision_const) {
			result = 1;
		}

		// verificam daca aceasta coliziune are loc in partea de sus sau jos
		// a dreptunghiului sau intr-un colt al dreptunghiului
		if (circ_corner_y + circ_diameter >= rect_y &&
			circ_corner_y + circ_diameter <= rect_y + collision_const) {
			if (result == 1) {
				result = 3;
			}else {
				result = 2;
			}
		}else if (circ_corner_y <= rect_y + rect_height &&
			      circ_corner_y >= rect_y + rect_height - collision_const) {
			if (result == 1) {
				result = 3;
			}else {
				result = 2;
			}
		}
	}

	return result;
}

void Tema1::Update_Walls() {
	// translatam cele 3 ziduri pe marginea din stanga, dreapta si sus a scenei
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, wall_offset);
	RenderMesh2D(meshes["side_wall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(screen_width - wall_thickness, wall_offset);
	RenderMesh2D(meshes["side_wall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, screen_height - wall_thickness);
	RenderMesh2D(meshes["top_wall"], shaders["VertexColor"], modelMatrix);
}

void Tema1::Update_Lives_Icons() {
	// translatam mesh-ele cu vietile jucatorului in scena in coltul din stanga jos
	for (int i = 0; i < player_lives; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(20 + (float)i * 30, 20);
		RenderMesh2D(meshes["player_life"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::Update_Bricks(float deltaTimeSeconds) {
	// translatam caramizile care au ramas in nivel in scena la pozitiile lor
	// si afisam animatia de micsorare a caramizilor care au fost distruse
	for (int i = 0; i < brick_columns; i++) {
		for (int j = 0; j < brick_rows; j++) {
			if (bricks[i][j].alive) {
				if (bricks[i][j].lives == 1) {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(bricks[i][j].x, bricks[i][j].y);

					// daca aceasta caramida mai are o singura viata atunci o desenam rosie
					RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
				}else if (bricks[i][j].lives == 2) {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(bricks[i][j].x, bricks[i][j].y);

					// daca aceasta caramida mai are 2 vieti atunci o desenam albastru deschis
					RenderMesh2D(meshes["hard_brick"], shaders["VertexColor"], modelMatrix);
				}else if (bricks[i][j].lives == 3) {
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(bricks[i][j].x, bricks[i][j].y);

					// daca aceasta caramida mai are 3 vieti atunci o desenam albastra
					RenderMesh2D(meshes["very_hard_brick"], shaders["VertexColor"], modelMatrix);
				}
			}else {
				// daca aceasta caramida nu mai exista in joc atunci pentru un scurt timp
				// afisam o animatie in care aceasta se micsoreaza pana dispare

				// incepem de la scalare de 1.0f (adica dimensiunea initiala a caramizii), iar la fiecare frame,
				// aceasta scalare se micsoreaza pana ajunge la 0 (scalare = 0 inseamna ca aceasta caramida a dispararut)
				float scale_factor = 1.0f - 2.0f * bricks[i][j].dead_time;

				if (scale_factor > 0) {
					// daca timpul de animatie de disparitie a caramizii nu s-a incheiat atunci inca
					// facem caramida sa se micsoreze (continuam animatia)
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(bricks[i][j].x + brick_width / 2, bricks[i][j].y + brick_height / 2);
					modelMatrix *= Transform2D::Scale(scale_factor, scale_factor);
					modelMatrix *= Transform2D::Translate(-brick_width / 2, -brick_height / 2);
					RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
				}

				// incrementam timpul caramizii de cand a fost distrusa, timp care este folosit in animatia de micsorare a caramizii
				bricks[i][j].dead_time += deltaTimeSeconds;
			}
		}
	}
}

void Tema1::Update_Paddle(float deltaTimeSeconds) {
	// translatam platforma la pozitia sa pe scena
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(paddle_x, paddle_y);

	if (long_paddle_duration > 0) {
		// daca powerup-ul pentru "long paddle" este activat, atunci
		// afisam platforma de doua ori mai mare
		long_paddle_duration -= deltaTimeSeconds;
		paddle_x = lastMousePosX - paddle_width / 2;

		// randam platforma extinsa
		RenderMesh2D(meshes["long_paddle"], shaders["VertexColor"], modelMatrix);
	}else {
		// daca powerup-ul pentru "long paddle" este dezactivat, atunci
		// afisam platforma cu lungimea normala (adica 200)
		paddle_width = 200;
		paddle_x = lastMousePosX - paddle_width / 2;

		// randam platforma de lungime normala
		RenderMesh2D(meshes["paddle"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::Update_Powerups(float deltaTimeSeconds) {
	// translatam si rotim powerup-urile in cadere la pozitia lor in scena si ne
	// gestionam logica acestora atunci cand cad sub scena sau sunt prinse de platforma

	// trecem prin toate powerup-urile din lista
	list<Powerup>::iterator current_powerup;
	for (current_powerup = powerup_list.begin(); current_powerup != powerup_list.end(); current_powerup++) {
		// translatam si rotim powerup-ul curent
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate((*current_powerup).x + ball_radius, (*current_powerup).y + ball_radius);
		modelMatrix *= Transform2D::Rotate((*current_powerup).falling_time);
		modelMatrix *= Transform2D::Translate(-ball_radius, -ball_radius);
		RenderMesh2D(meshes["powerup"], shaders["VertexColor"], modelMatrix);

		// updatam durata de cand este powerup-ul curent pe ecran
		(*current_powerup).falling_time += deltaTimeSeconds;

		// updatam coordonata y a powerup-ului curent
		(*current_powerup).y -= powerup_falling_speed;

		// daca powerup-ul a cazut mai jos de ecran si nu a fost prinsa de platforma
		// atunci o eliminam din lista de powerup-uri
		if ((*current_powerup).y < 0) {
			current_powerup = powerup_list.erase(current_powerup);

			if (current_powerup != powerup_list.end()) {
				continue;
			}else {
				break;
			}
		}

		// daca powerup-ul a fost prins de platforma atunci activam un powerup aleator
		if (CircleRectangleCollision(2 * ball_radius, (*current_powerup).x, (*current_powerup).y,
			paddle_width, paddle_height, paddle_x, paddle_y)) {
			int random = rand() % 5;

			if (random == 0) {
				// powerup-ul este "split ball" care imparte fiecare bila in doua

				// cream o lista noua in care adaugam noile bile dublate
				std::list<Ball> new_ball_list;

				list<Ball>::iterator current_ball;
				for (current_ball = ball_list.begin(); current_ball != ball_list.end(); current_ball++) {
					// noua bila va avea viteza in directia opusa fata de viteza bilei din care provine
					new_ball_list.push_back({ (*current_ball).x, (*current_ball).y,
											 -(*current_ball).vel_x, (*current_ball).vel_y });
				}

				// inseram noile bile in lista propriu-zisa a bilelor
				ball_list.insert(ball_list.end(), new_ball_list.begin(), new_ball_list.end());
			}else if (random == 1) {
				// powerup-ul este "extend paddle" care dubleaza lungimea platformei pentru o durata de timp

				long_paddle_duration = powerup_duration;
				paddle_width = 400;
			}else if (random == 2) {
				// powerup-ul este "slow ball" care incetineste de doua ori viteza fiecarei bile, insa numai
				// daca aceasta nu se misca deja prea incet

				list<Ball>::iterator current_ball;
				for (current_ball = ball_list.begin(); current_ball != ball_list.end(); current_ball++) {
					if (sqrt((*current_ball).vel_x * (*current_ball).vel_x +
						     (*current_ball).vel_y * (*current_ball).vel_y) > min_ball_speed) {
						(*current_ball).vel_x = 0.5f * (*current_ball).vel_x;
						(*current_ball).vel_y = 0.5f * (*current_ball).vel_y;
					}
				}
			}else if (random == 3){
				// powerup-ul este "super ball" care face bilele sa treaca prin caramizi
				// fara sa se produca reflectii

				super_ball_duration = powerup_duration;
			}else if (random == 4) {
				// powerup-ul este "extra life" care da jucatorului inca o viata

				player_lives++;
			}

			// powerup-ul fiind prins de platforma, il eliminam din lista de powerup-uri
			current_powerup = powerup_list.erase(current_powerup);

			if (current_powerup != powerup_list.end()) {
				continue;
			}else {
				break;
			}
		}
	}
}

void Tema1::Update_Balls(float deltaTimeSeconds) {
	// translatam bilele din joc la pozitia lor pe ecran si gestionam coliziunile acestora cu
	// platforma, caramizile si zidurile din joc

	// iteram prin fiecare bila din joc
	list<Ball>::iterator current_ball;
	for (current_ball = ball_list.begin(); current_ball != ball_list.end(); current_ball++) {
		modelMatrix = glm::mat3(1);

		if (ball_state == ON_PADDLE) {
			// daca jucatorul inca nu a apasat pe butonul de la mouse pentru
			// a lansa bila, atunci translatam bila odata cu platforma
			modelMatrix *= Transform2D::Translate(paddle_x + paddle_width / 2, paddle_y + paddle_height);
		}else {
			// daca bila este in miscare (jucatorul a apasat deja pe butonul de la mouse), atunci verificam daca
			// bila a lovit vreun perete, iar daca da, atunci reflectam componenta corespunzatoare a vitezei bilei

			if ((*current_ball).x < wall_thickness) {
				(*current_ball).x = wall_thickness;
				(*current_ball).vel_x = -(*current_ball).vel_x;
			}

			if ((*current_ball).x + 2 * ball_radius > screen_width - wall_thickness) {
				(*current_ball).x = screen_width - wall_thickness - 2 * ball_radius;
				(*current_ball).vel_x = -(*current_ball).vel_x;
			}

			if ((*current_ball).y + 2 * ball_radius > screen_height - wall_thickness) {
				(*current_ball).vel_y = -(*current_ball).vel_y;
			}

			// daca bila a cazut sub scena, atunci o eliminam din lista de bile
			if ((*current_ball).y < 0) {
				current_ball = ball_list.erase(current_ball);

				if (current_ball != ball_list.end()) {
					continue;
				}else {
					break;
				}
			}

			// verificam daca bila curenta a lovit platforma, iar daca da o reflectam folosind
			// functia arccos, astfel incat daca bila loveste platforma in mijloc, atunci este reflectata
			// vertical, daca loveste platforma pe coltul platformei, bila este reflectata orizontal, iar
			// daca loveste platforma in puncte intermediare, atunci este reflectata in directii intermediare
			// celor mentionate
			if (CircleRectangleCollision(2 * ball_radius, (*current_ball).x, (*current_ball).y,
				paddle_width, paddle_height, paddle_x, paddle_y)) {

				// speed_modul este viteza in modul a bilei (viteza efectiva a bilei)
				float speed_modul = sqrt((*current_ball).vel_x * (*current_ball).vel_x + (*current_ball).vel_y * (*current_ball).vel_y);

				// ne asiguram ca valoarea care este data ca parametru functiei arccos este intre -1 si 1 deoarece functia
				// arccos primeste numai valori din acest interval
				float acos_param = ((*current_ball).x + ball_radius / 2 - paddle_x - paddle_width / 2) / (paddle_width / 2);

				if (acos_param < -1) {
					acos_param = -1;
				}else if (acos_param > 1) {
					acos_param = 1;
				}

				// updatam vitezele bilei in functie de unde a lovit platforma
				(*current_ball).vel_x = speed_modul * glm::cos(glm::acos(acos_param));
				(*current_ball).vel_y = speed_modul * glm::sin(glm::acos(acos_param));
			}

			// verificam daca bila curenta loveste vreo caramida, iar daca da, atunci sa reduca o unitate
			// din viata caramizii respective, iar daca acea caramida nu mai are vieti atunci o setam ca fiind
			// distrusa (not alive), in urma coliziunii cu acea caramida, reflectam directia de miscare
			// a bilei in functie de locul unde aceasta a lovit caramida
			int collision = 0;
			for (int i = 0; i < brick_columns; i++) {
				for (int j = 0; j < brick_rows; j++) {
					if (bricks[i][j].alive) {
						collision = CircleRectangleCollision(2 * ball_radius, (*current_ball).x, (*current_ball).y,
							brick_width, brick_height, bricks[i][j].x, bricks[i][j].y);

						// reflectam directia vitezei bilei numai daca powerup-ul de "super ball" este
						// dezactivat ("super ball" face ca bila sa treaca prin caramizi fara sa ii se
						// reflecte directia vitezei)
						if (super_ball_duration <= 0) {
							if (collision == 1) {
								// daca bila a lovit caramida in partea stanga sau dreapta
								// atunci reflectam componenta x a vitezei bilei
								(*current_ball).vel_x = -(*current_ball).vel_x;
							}else if (collision == 2) {
								// daca bila a lovit caramida in partea de sus sau jos
								// atunci reflectam componenta y a vitezei bilei
								(*current_ball).vel_y = -(*current_ball).vel_y;
							}else if (collision == 3) {
								// daca bila a lovit caramida intr-un colt
								// atunci reflectam ambele componente a vitezei bilei
								(*current_ball).vel_x = -(*current_ball).vel_x;
								(*current_ball).vel_y = -(*current_ball).vel_y;
							}
						}

						// daca s-a produs o coliziune cu aceasta caramida, atunci ii reducem o viata caramizii
						// iar daca aceasta a ramas fara vieti atunci o setam ca fiind "not alive" si ii dam o
						// sansa sa lase in urma un powerup care sa cada
						if (collision != 0) {
							bricks[i][j].lives--;

							if (bricks[i][j].lives == 0) {
								int random = rand() % powerup_rarity;

								if (random == 0) {
									powerup_list.push_back({ bricks[i][j].x + brick_width / 2 - powerup_radius, bricks[i][j].y, 0 });
								}

								bricks[i][j].alive = false;
								bricks[i][j].dead_time = 0;
							}
						}
					}

					if (collision != 0) {
						break;
					}

				}
				if (collision != 0) {
					break;
				}
			}

			// limitam viteza bilei curente la viteza maxima admisa de bile
			float speed_modul = sqrt((*current_ball).vel_x * (*current_ball).vel_x + (*current_ball).vel_y * (*current_ball).vel_y);

			if (speed_modul < max_ball_speed) {
				(*current_ball).vel_x += ball_accel * (*current_ball).vel_x / speed_modul;
				(*current_ball).vel_y += ball_accel * (*current_ball).vel_y / speed_modul;
			}

			// updatam viteza bilei curente in functie de viteza acesteia
			(*current_ball).x += (*current_ball).vel_x;
			(*current_ball).y += (*current_ball).vel_y;

			// translatam bila la pozitia acesteia in scena
			modelMatrix *= Transform2D::Translate((*current_ball).x, (*current_ball).y);
		}

		// randam o bila rosie sau una normala daca powerup-ul "super ball" este
		// activat sau respectiv este dezactivat
		if (super_ball_duration > 0) {
			RenderMesh2D(meshes["super_ball"], shaders["VertexColor"], modelMatrix);
		}else {
			RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		}
	}

	if (super_ball_duration > 0) {
		super_ball_duration -= deltaTimeSeconds;
	}

	// daca nu au mai ramas bile in scena, atunci inseamna ca nicio bila nu a putut
	// fi prinsa de platforma jucatorului, si deci decrementam o viata jucatorului
	// iar daca acesta a ramas fara vieti atunci restartam jocul
	if (ball_list.empty()) {
		player_lives--;

		if (player_lives == 0) {
			player_lives = 3;

			level = 1;

			for (int i = 0; i < brick_columns; i++) {
				free(bricks[i]);
			}

			free(bricks);

			bricks = (Brick **)malloc(brick_columns * sizeof(Brick *));

			for (int i = 0; i < brick_columns; i++) {
				bricks[i] = (Brick *)malloc(brick_rows * sizeof(Brick));
			}

			InitLevel();
		}

		ball_state = ON_PADDLE;

		ball_list.push_back({ 0, 0, 0, 4 });
	}
}

void Tema1::Update_Level_Clear_Check() {
	// verificam daca au mai ramas caramizi, daca nu au
	// mai ramas atunci trecem la nivelul urmator
	bool found_brick_alive = false;
	for (int i = 0; i < brick_columns; i++) {
		for (int j = 0; j < brick_rows; j++) {
			if (bricks[i][j].alive == true) {
				// am gasit o caramida in joc, deci inca
				// nu trecem la nivelul urmator

				found_brick_alive = true;

				break;
			}
		}

		if (found_brick_alive == true) {
			break;
		}
	}

	// daca am variabila "found_brick_alive" nu a fost modificata inseamna
	// ca nu au fost gasite caramizi, deci trecem la nivelul urmator
	if (found_brick_alive == false) {
		level++;

		if (level == 4) {
			// daca am terminat ultimul nivel din joc, atunci restarteaza
			// jocul (numarul de vieti este 3 si nivelul este 1)

			player_lives = 3;
			level = 1;
		}

		// eliberam si alocam memoria caramizilor pentru urmatorul nivel

		for (int i = 0; i < brick_columns; i++) {
			free(bricks[i]);
		}

		free(bricks);

		bricks = (Brick **)malloc(brick_columns * sizeof(Brick *));

		for (int i = 0; i < brick_columns; i++) {
			bricks[i] = (Brick *)malloc(brick_rows * sizeof(Brick));
		}

		// initializam urmatorul nivel
		InitLevel();
	}
}

void Tema1::Update(float deltaTimeSeconds) {
	// translatam cele 3 ziduri pe marginea din stanga, dreapta si sus a scenei
	Update_Walls();

	// translatam mesh-ele cu vietile jucatorului in scena in coltul din stanga jos
	Update_Lives_Icons();

	// translatam caramizile care au ramas in nivel pe ecran la pozitiile lor
	// si afisam animatia de micsorare a caramizilor care au fost distruse
	Update_Bricks(deltaTimeSeconds);

	// translatam platforma la pozitia sa pe ecran
	Update_Paddle(deltaTimeSeconds);

	// translatam si rotim powerup-urile in cadere la pozitia lor pe ecran si ne
	// gestionam logica acestora atunci cand cad sub scena sau sunt prinse de platforma
	Update_Powerups(deltaTimeSeconds);

	// translatam bilele din joc la pozitia lor pe ecran si gestionam coliziunile acestora cu
	// platforma, caramizile si zidurile din joc
	Update_Balls(deltaTimeSeconds);

	// verificam daca au mai ramas caramizi, daca nu au
	// mai ramas atunci trecem la nivelul urmator
	Update_Level_Clear_Check();
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods) {

}

void Tema1::OnKeyPress(int key, int mods) {

}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){
	// miscam platforma jucatorului in functie de pozitia cursorului

	paddle_x = mouseX - paddle_width / 2;
	lastMousePosX = mouseX;

	// limitam miscarea platformei de mouse astfel incat sa eliminam cazuri
	// limita in care jucatorul ar lansa bila direct intr-unul din zidurile
	// de pe margine
	if (mouseX < wall_thickness + 2) {
		paddle_x = (float)(wall_thickness + 2 - paddle_width / 2);
		lastMousePosX = (int)wall_thickness + 2;
	}

	if (mouseX > screen_width - wall_thickness - 2 - 2 * ball_radius) {
		paddle_x = (float)(screen_width - wall_thickness - 2 - paddle_width / 2 - 2 * ball_radius);
		lastMousePosX = (int)(screen_width - wall_thickness - 2 - 2 * ball_radius);
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){
	// daca jucatorul apasa pe butonul de la mouse atunci lansam bila in sus
	if (ball_state == ON_PADDLE) {
		list<Ball>::iterator current_ball;
		for (current_ball = ball_list.begin(); current_ball != ball_list.end(); current_ball++) {
			(*current_ball).x = paddle_x + paddle_width / 2;
			(*current_ball).y = paddle_y + paddle_height;
		}

		ball_state = MOVING;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
