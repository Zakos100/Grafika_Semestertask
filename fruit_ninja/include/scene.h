#ifndef SCENE_H
#define SCENE_H
#include "utils.h"
#include <obj/model.h>
#include "texture.h"
#include <stdlib.h>
#include "camera.h"

typedef struct Targets
{
	vec3 velocity;
	vec3 position;
	double size;
	int point;
	int texture_type;
	bool isenemy;
}Targets;

typedef struct Scene
{
	bool isrunning;
	int max_targets;
	int current_targets;
	int difficulty_increase;
	int points;
	double enemy_chance;
	Targets* targets;
	Model model;
	GLuint* texture;
	Model map_model;
	GLuint map_texture;
	GLuint f1_panel;
	GLuint middle_dot;
	Material material;
	float light;
	double player_speed;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Render the scene objects.
 * Return: true, if fruit fall down. false othervise
 */
bool render_scene(Scene* scene,Camera* camera);

/**
 * Check target hit.
 * Return: true, if it is a bomb. false othervise
 */
bool checktarget(Camera* camera, Scene* scene);

#endif /* SCENE_H */
