#include <imgui\imgui.h>
#include <time.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
constexpr auto NUM_PARTICLES = 5000;

namespace Box {
	void drawCube();
}
namespace Axis {
	void drawAxis();
}

namespace Sphere {
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
namespace Capsule {
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius = 1.f);
	extern void drawCapsule();
}
namespace Particles {
	struct ParticleSystem{
		float particlePosition[NUM_PARTICLES * 3];
		float particleVelocity[NUM_PARTICLES * 3];

		void Init() {
			srand(time(NULL)); //todo Esto no se sabe si esta bien
			for (int i = 0; i < NUM_PARTICLES; i++) {
				particlePosition[i * 3] = rand() % 1001 / 100.0f - 5.0f; //X positions
				particlePosition[i * 3 + 1] = rand() % 501 / 100.0f + 5.0f; //Y positions
				particlePosition[i * 3 + 2] = rand() % 1001 / 100.0f - 5.0f; //Z positions
			}
			for (int i = 0; i < NUM_PARTICLES; i++) {
				particleVelocity[i * 3] = rand() % 101 / 10.0f - 5.0f; //X velocity
				particleVelocity[i * 3 + 1] = rand() % 101 / 10.0f - 5.0f; //Y velocity
				particleVelocity[i * 3 + 2] = rand() % 101 / 10.0f - 5.0f; //Z velocity
			}
		}

		void Update(float dt) {
			for (int i = 0; i < NUM_PARTICLES; i++) {
				particlePosition[i * 3] += particleVelocity[i * 3] * dt;
				particlePosition[i * 3 + 1] += particleVelocity[i * 3 + 1] * dt;
				particlePosition[i * 3 + 1] += particleVelocity[i * 3 + 2] * dt;
			}
		}
	};
	ParticleSystem ps;

	extern const int maxParticles;
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
	extern void cleanupParticles();

	struct ForceActuator { 
		virtual glm::vec3 computeForce(float mass, const glm::vec3& position) = 0; 
	};

	struct Collider {
		virtual bool checkCollision(const glm::vec3& prev_pos, const glm::vec3& next_pos) = 0;
		virtual void getPlane(glm::vec3& normal, float& d) = 0;
		void computeCollision(const glm::vec3& old_pos, const glm::vec3& old_vel, glm::vec3& new_pos, glm::vec3& new_vel) {
			//...
		}
	};
	
	struct PlaneCol : Collider {
		//...
	};
	struct SphereCol : Collider 
	{//...
	};
	struct CapsuleCol : Collider {
		//...
	};

	void euler(float dt, ParticleSystem& particles, const std::vector<Collider*>& colliders, const std::vector<ForceActuator*>& force_acts) {

	}
}
namespace Mesh {
	extern const int numCols;
	extern const int numRows;
	extern void updateMesh(float* array_data);
	extern void drawMesh();
}
namespace Fiber {
extern const int numVerts;
	extern void updateFiber(float* array_data);
	extern void drawFiber();
}
namespace Cube {
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
}



// Boolean variables allow to show/hide the primitives
bool renderSphere = false;
bool renderCapsule = false;
bool renderParticles = true;
bool renderMesh = false;
bool renderFiber = false;
bool renderCube = false;

//You may have to change this code
void renderPrims() {
	Box::drawCube();
	Axis::drawAxis();


	if (renderSphere)
		Sphere::drawSphere();
	if (renderCapsule)
		Capsule::drawCapsule();

	if (renderParticles) {
		int startDrawingFromParticle = 0;
		int numParticlesToDraw = Particles::maxParticles;
		Particles::drawParticles(startDrawingFromParticle, numParticlesToDraw);
	}

	if (renderMesh)
		Mesh::drawMesh();
	if (renderFiber)
		Fiber::drawFiber();

	if (renderCube)
		Cube::drawCube();
}


void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	// Do your GUI code here....
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		
	}
	// .........................
	
	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	bool show_test_window = false;
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	Particles::ps.Init();
}

void PhysicsUpdate(float dt) {
	Particles::ps.Update(dt);
	Particles::updateParticles(0, NUM_PARTICLES, Particles::ps.particlePosition);
	Particles::drawParticles(0, NUM_PARTICLES);
}

void PhysicsCleanup() {
	Particles::cleanupParticles();
}