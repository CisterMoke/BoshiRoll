#include "RenderCommands.h"
#include "Colliders.h"

void RenderSpriteCommand<BaseSprite>::execute(SDL_Renderer *renderer)
{
	render_sprite(renderer);
}

void RenderSpriteCommand<AnimSprite>::execute(SDL_Renderer *renderer)
{
	SDL_Rect frame_clip = sprite->get_frame_clip();
	clip = &frame_clip;
	render_sprite(renderer);
	sprite->sync();
}

void RenderSpriteCommand<FontSprite>::execute(SDL_Renderer *renderer)
{
	if (center) { px_pos -= Vec2(sprite->get_tot_width() / 2, sprite->get_tot_height() / 2); }
	Vec2 old_pos = px_pos;
	sprite->start_iter();
	px_pos = sprite->get_char_pos(center) + old_pos;
	render_sprite(renderer);
	while (sprite->next_iter())
	{ 
		px_pos = sprite->get_char_pos(center) + old_pos;
		render_sprite(renderer);
	}
}

RenderParallaxCommand::RenderParallaxCommand(BaseSprite *sprite, Camera *camera, const Vec2 &px_pos, float depth, bool center)
	: RenderSpriteCommand<BaseSprite>(sprite, camera, px_pos), depth(depth) { this->center = center; }

void RenderParallaxCommand::execute(SDL_Renderer *renderer)
{
	// For now ignore camera rotation. Use line rect collision when implementing.
	float off = glob::SCREEN_WIDTH/2/camera->rx;
	float w = sprite->get_pixel_width();
	float left = (px_pos.x - camera->get_pixel_origin().x) * depth + off;
	float right = left + w;
	int start = -ceilf( left / w);
	int stop = fmaxf(ceilf((glob::SCREEN_WIDTH/camera->rx - right) / w), start + 1);
	Vec2 pos_corr = px_pos * depth + camera->get_pixel_origin() * (1-depth); // Lazy correction to trick render_sprite function (too lazy to define new func)
	float exp = 2 - 2 / (1 + depth);
	float old_rx = camera->rx; float old_ry = camera->ry;
	camera->rx = std::powf(camera->rx, exp); camera->ry = std::powf(camera->ry, exp);
	for (int i = start; i <= stop; i++)
	{
		px_pos = pos_corr + Vec2(w, 0.0f) * i;
		render_sprite(renderer);
	}
	camera->rx = old_rx; camera->ry = old_ry;
}

void RenderColliderCommand<CircleCollider>::execute(SDL_Renderer *renderer)
{
	Mat22 T = camera->get_transform();
	Vec2 orig = camera->get_pixel_origin();
	Vec2 offset = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
	Vec2 center = T * (*coll->pos - orig) + offset;
	float zoom_x = (T * Vec2(1.0f, 0.0f)).norm();
	float zoom_y = (T * Vec2(0.0f, 1.0f)).norm();
	ellipseRGBA(renderer, center.x, center.y, coll->r * zoom_x, coll->r * zoom_y, color.r, color.g, color.b, 255);
}

void RenderColliderCommand<LineCollider>::execute(SDL_Renderer *renderer)
{
	Mat22 T = camera->get_transform();
	Vec2 orig = camera->get_pixel_origin();
	Vec2 offset = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
	Vec2 begin = T * (*coll->start - orig) + offset;
	Vec2 end = T * (*coll->stop - orig) + offset;
	lineRGBA(renderer, begin.x, begin.y, end.x, end.y, color.r, color.g, color.b, 255);
}

void RenderColliderCommand<RectCollider>::execute(SDL_Renderer *renderer)
{
	Mat22 T = camera->get_transform();
	Vec2 orig = camera->get_pixel_origin();
	Vec2 offset = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
	Vec2 center = T * (*coll->lu + Vec2(coll->w / 2, coll->h / 2) - orig) + offset;
	float w_hat = coll->w * (T * Vec2(1.0f, 0.0f)).norm();
	float h_hat = coll->h * (T * Vec2(0.0f, 1.0f)).norm();

	rectangleRGBA(renderer, center.x - w_hat / 2, center.y - h_hat / 2, center.x + w_hat / 2, center.y + h_hat / 2, color.r, color.g, color.b, 255);
}

void RenderColliderCommand<RampCollider>::execute(SDL_Renderer *renderer)
{
	RenderColliderCommand<CircleCollider>(coll->circ, camera, color).execute(renderer);
	RenderColliderCommand<RectCollider>(coll->rect, camera, color).execute(renderer);
	RenderColliderCommand<LineCollider>(coll->line, camera, color).execute(renderer);
	int* arr[] = {new int(1), new int(2), new int(3)};
}

std::deque<BaseRenderCommand*> render_command_pool = {};