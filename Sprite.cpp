#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &spriteSize, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, spriteSize, spritesheet, program);

	return quad;
}

Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &spriteSize, Texture *spritesheet, ShaderProgram *program)
{
	glm::vec2 sizeInSpritesheet = spriteSize / spritesheet->getSize();

	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
						  quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
						  quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
						  0.f, 0.f, 0.f, 0.f, 
						  quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
						  0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
	size = quadSize;
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::render(glm::vec2 displacement)
{
	setPosition(this->position + displacement);
	render();
	setPosition(this->position - displacement);
}

//Renders a rotated sprite at @angle
void Sprite::render(float angle) const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	modelview = glm::translate(modelview, glm::vec3(size.x / 2, size.y / 2, 0.f));
	modelview = glm::rotate(modelview, angle, glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::translate(modelview, glm::vec3(-size.x/2, -size.y/2, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

/*
Renders a rotated sprite at @angle, and relative to point @rotationAxisRatio
@rotationAxisRatio represents the %s of the sprite that will serve as center
*/
void Sprite::render(float angle, const glm::vec2 rotationAxisRatio) const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	modelview = glm::translate(modelview, glm::vec3(size.x * rotationAxisRatio.x, size.y * rotationAxisRatio.y, 0.f));
	modelview = glm::rotate(modelview, angle, glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::translate(modelview, glm::vec3(-size.x * rotationAxisRatio.x, -size.y * rotationAxisRatio.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

//Values 0..1 (floats)
void Sprite::setTexturePosition(const glm::vec2 & displacement)
{
	texCoordDispl = displacement;
}

//In pixels
void Sprite::setSize(const glm::vec2 & size)
{
	this->size = size;
}



