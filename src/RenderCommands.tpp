#pragma once

template<class Sprite>
void RenderSpriteCommand<Sprite>::render_sprite(SDL_Renderer *renderer)
{
	if (sprite->get_texture() == nullptr) { return; }
	camera == NULL ? _render_sprite_no_cam(renderer) : _render_sprite_cam(renderer);
}

template<class Sprite>
void RenderSpriteCommand<Sprite>::_render_sprite_cam(SDL_Renderer *renderer)
{
	float w_hat = sprite->get_pixel_width() * camera->rx;
	float h_hat = sprite->get_pixel_height() * camera->ry;
	Vec2 off = Vec2((glob::SCREEN_WIDTH - w_hat) / 2, (glob::SCREEN_HEIGHT - h_hat) / 2);
	if (!center) { px_pos += Vec2(sprite->get_pixel_width() / 2, sprite->get_pixel_height() / 2); }
	Vec2 lu = camera->get_transform() * (px_pos - camera->get_pixel_origin()) + off;
	SDL_Rect dest = { (int)lu.x, (int)lu.y, (int)w_hat, (int)h_hat };
	SDL_RenderCopyEx(renderer, sprite->get_texture(), clip, &dest, sprite->get_degrees() - camera->theta, NULL, sprite->get_flip());
}

template<class Sprite>
void RenderSpriteCommand<Sprite>::_render_sprite_no_cam(SDL_Renderer *renderer)
{
	float w_hat = sprite->get_pixel_width();
	float h_hat = sprite->get_pixel_height();
	Vec2 off = Vec2(0.0f, 0.0f);
	if (center) { off -= Vec2(w_hat / 2, h_hat / 2); }
	Vec2 lu = px_pos + off;
	SDL_Rect dest = { (int)lu.x, (int)lu.y, (int)w_hat, (int)h_hat };
	SDL_RenderCopyEx(renderer, sprite->get_texture(), clip, &dest, sprite->get_degrees(), NULL, sprite->get_flip());
}