#include "Camera.h"

Camera::Camera()
{
	GameObject::Init(0, 0, 0, 0, 0, 0);
	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);

	Camera::trans = trans;
}
void Camera::Destroy()
{
	al_identity_transform(&trans);
	al_use_transform(&trans);

	GameObject::Destroy();
}
void Camera::Update(int x, int y)
{
	Camera::x = x;
	Camera::y = y;
}
void Camera::Render()
{
	al_identity_transform(&trans);
	al_translate_transform(&trans, -x, -y);
//	al_scale_transform(&trans, 2, 2);
//	al_translate_transform(&trans, -WIDTH / 2, -HEIGHT / 2);
	al_use_transform(&trans);
}
void Camera::Collided()
{}