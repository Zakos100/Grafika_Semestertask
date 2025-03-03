#include "scene.h"
#include <GL/gl.h>
#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>

void init_scene(Scene* scene)
{
	srand(time(0));
	scene->isrunning=false;
	scene->max_targets=3;
	scene->current_targets=1;
	scene->difficulty_increase=50;
	scene->points=0;
	scene->enemy_chance=0.0;
	load_model(&(scene->model), "assets/models/sphere.obj");
	scene->texture=(GLuint*)malloc(4*sizeof(GLuint));
	scene->targets=(Targets*)malloc(scene->max_targets*sizeof(Targets));
	scene->texture[1]=load_texture("assets/textures/type_1.jpg");
	scene->texture[2]=load_texture("assets/textures/type_2.jpg");
	scene->texture[3]=load_texture("assets/textures/type_3.jpg");
	scene->texture[0]=load_texture("assets/textures/enemy.jpg");
	for(int i=0;i<scene->max_targets;i++){
		scene->targets[i].texture_type=-1;
	}
	
	 load_model(&(scene->map_model), "assets/models/house.obj");
	 scene->map_texture = load_texture("assets/textures/house.jpg");
	
	scene->f1_panel= load_texture("assets/textures/help.jpg");
	scene->middle_dot= load_texture("assets/textures/middle_dot.png");
	scene->light=0.8f;
	scene->player_speed=6.0f;
    scene->material.ambient.red = 1.0;
    scene->material.ambient.green = 1.0;
    scene->material.ambient.blue = 1.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;
    scene->material.shininess = 0.0;
}


void set_lighting(Scene* scene)
{
    float ambient_light[] = { scene->light, scene->light, scene->light, 1.0f };
    float diffuse_light[] = { scene->light, scene->light, scene->light, 1.0f };
    float specular_light[] ={ scene->light, scene->light, scene->light, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}


bool render_scene(Scene* scene,Camera* camera)
{
	if(scene->isrunning)
	for(int i=0;i<scene->current_targets;i++){
		if(scene->targets[i].texture_type==-1){
			if(rand()%101<scene->enemy_chance) {
				scene->targets[i].isenemy=true;
				scene->targets[i].size=0.3;
				scene->targets[i].point=0;
				scene->targets[i].texture_type=0;
				scene->targets[i].position.x=rand()%5-2;
				scene->targets[i].position.y=8.0;
				scene->targets[i].position.z=-5.0;
				double speed=0.0;
				while(speed==0.0){
					speed=((double)(rand()%3-1))/50;
				}
				scene->targets[i].velocity.x=speed;
				scene->targets[i].velocity.y=0.0;
				speed=0.0;
				while(speed==0.0){
					speed=((double)(rand()%1+1))/12;
				}
				scene->targets[i].velocity.z=speed;
			}else{
				scene->targets[i].isenemy=false;
				scene->targets[i].texture_type=rand()%3+1;
				switch(scene->targets[i].texture_type){
					case 1:scene->targets[i].point=5;scene->targets[i].size=0.7;break;
					case 2:scene->targets[i].point=3;scene->targets[i].size=0.35;break;
					case 3:scene->targets[i].point=1;scene->targets[i].size=0.12;break;
				}
				scene->targets[i].position.x=rand()%5-2;
				scene->targets[i].position.y=8.0;
				scene->targets[i].position.z=-5.0;
				double speed=0.0;
				while(speed==0.0){
					speed=((double)(rand()%3-1))/50;
				}
				scene->targets[i].velocity.x=speed;
				scene->targets[i].velocity.y=0.0;
				speed=0.0;
				while(speed==0.0){
					speed=((double)(rand()%1+1))/12;
				}
				scene->targets[i].velocity.z=speed;
			}
		}else{
			scene->targets[i].position.x+=scene->targets[i].velocity.x/2;
			scene->targets[i].position.z+=scene->targets[i].velocity.z;
			scene->targets[i].velocity.z-=0.0003;
			if(scene->targets[i].position.z<-20.0 && scene->targets[i].texture_type!=0){
				printf("Elert pontjaid: %d",scene->points);
				return true;
			}
		}
	}
    set_material(&(scene->material));
    set_lighting(scene);
	for(int i=0;i<scene->current_targets;i++){
		glBindTexture(GL_TEXTURE_2D, scene->texture[scene->targets[i].texture_type]);
		glPushMatrix();
		glTranslatef(scene->targets[i].position.x,scene->targets[i].position.y,scene->targets[i].position.z);
		glScalef(scene->targets[i].size,scene->targets[i].size,scene->targets[i].size);
		draw_model(&(scene->model));
		glPopMatrix();
	}
	
	 glPushMatrix();
	 glBindTexture(GL_TEXTURE_2D, scene->map_texture);
	 glTranslatef(0.0,0.0,-8.0);
	 glRotatef(90.0,1.0,0.0,0.0);
	 glRotatef(275.0,0.0,1.0,0.0);
	 glTranslatef(15.0,0.0,0.0);
	 glScalef(3.0,3.0,3.0);
	 draw_model(&(scene->map_model));
	 glPopMatrix();
	return false;
}

bool checktarget(Camera* camera, Scene* scene){
	vec3 dir=camera_direction(camera);
	for(int i=0;i<scene->current_targets;i++){
		vec3 d = {camera->position.x + dir.x - camera->position.x, camera->position.y + dir.y - camera->position.y, camera->position.z + dir.z - camera->position.z};
		vec3 e = {camera->position.x - scene->targets[i].position.x, camera->position.y - scene->targets[i].position.y, camera->position.z - scene->targets[i].position.z};
		float a = d.x * d.x + d.y * d.y + d.z * d.z;
		float b = 2 * (d.x * e.x + d.y * e.y + d.z * e.z);
		float c = e.x * e.x + e.y * e.y + e.z * e.z - scene->targets[i].size * scene->targets[i].size;
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
			continue;
		if(scene->targets[i].isenemy) return true;
		scene->points+=scene->targets[i].point;
		scene->targets[i].texture_type=-1;
	}
	scene->current_targets=(scene->points/scene->difficulty_increase)+1;
	if(scene->current_targets>scene->max_targets) scene->current_targets=scene->max_targets;
	if(scene->points>scene->difficulty_increase) scene->enemy_chance=10.0;
	if(scene->points>scene->difficulty_increase*2) scene->enemy_chance=30.0;
	return false;
}