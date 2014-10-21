#include "Bloom.h"

Bloom::Bloom(string name) : PostProcessingEffect(name) {

	/* Create Motion Blur Shader */
	this->bloomShader = new BloomShader(BLOOM_SHADER);
	this->bloomShader->createShaderProgram();
	this->bloomShader->bindAttributes();
	this->bloomShader->linkShaderProgram();
	this->bloomShader->bindUniforms();
}

Bloom::~Bloom() {

	delete this->bloomShader;
}

void Bloom::draw() {

	/* Bind the Shader Program */
	glBindVertexArray(this->arrayObjectID);

		glUseProgram(this->bloomShader->getProgramID());

			/* Activate Texture 0 */
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->frameBuffer->getFrameBufferTexture(0));
			glUniform1i(glGetUniformLocation(this->bloomShader->getProgramID(), COLOR_TEXTURE_UNIFORM), 0);

			Utility::checkOpenGLError("ERROR: MotionBlur Texture 0 Binding error.");

			/* Activate Texture 1 */
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->frameBuffer->getFrameBufferTexture(1));
			glUniform1i(glGetUniformLocation(this->bloomShader->getProgramID(), BRIGHTNESS_TEXTURE_UNIFORM), 1);

			Utility::checkOpenGLError("ERROR: MotionBlur Texture 1 Binding error.");

			/* Draw the Scene */
			glDisable(GL_DEPTH_TEST);

				glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glEnable(GL_DEPTH_TEST);

			/* Store the current screen content in Texture 1 */
			glBindTexture(GL_TEXTURE_2D, this->frameBuffer->getFrameBufferTexture(1));

				glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,this->frameBuffer->getWidth(), this->frameBuffer->getHeight());

			glBindTexture(GL_TEXTURE_2D, 0);

			Utility::checkOpenGLError("ERROR: Could not draw the post processing effect.");

		/* Unbind the Shader Program */
		glUseProgram(0);

	glBindVertexArray(0);
}

void Bloom::reshape(GLint width, GLint height) {

	/* Reshape the Motion Blur FrameBuffer */
	this->frameBuffer->reshape(width, height);

	/* Store the current screen content in Texture 1 */
	glBindTexture(GL_TEXTURE_2D, this->frameBuffer->getFrameBufferTexture(1));

		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frameBuffer->getFrameBufferObject());

			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,this->frameBuffer->getWidth(), this->frameBuffer->getHeight());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D,0);
}