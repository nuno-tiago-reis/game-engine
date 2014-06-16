#include "MotionBlur.h"

MotionBlur::MotionBlur(string name) : PostProcessingEffect(name) {

	/* Create Motion Blur Shader */
	_motionBlurShader = new MotionBlurShader(MOTION_BLUR_SHADER);
	_motionBlurShader->createShaderProgram();
	_motionBlurShader->bindAttributes();
	_motionBlurShader->linkShaderProgram();
	_motionBlurShader->bindUniforms();
}

MotionBlur::~MotionBlur() {

	delete _motionBlurShader;
}

void MotionBlur::draw() {

	/* Bind the Shader Program */
	glBindVertexArray(_vertexArrayObjectID);

		glUseProgram(_motionBlurShader->getProgramID());

			/* Activate Texture 0 */
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _frameBuffer->getFrameBufferTexture(0));
			glUniform1i(glGetUniformLocation(_motionBlurShader->getProgramID(), TEXTURE_0_UNIFORM), 0);

			Utility::checkOpenGLError("ERROR: MotionBlur Texture 0 Binding error.");

			/* Activate Texture 1 */
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _frameBuffer->getFrameBufferTexture(1));
			glUniform1i(glGetUniformLocation(_motionBlurShader->getProgramID(), TEXTURE_1_UNIFORM), 1);

			Utility::checkOpenGLError("ERROR: MotionBlur Texture 1 Binding error.");

			/* Draw the Scene */
			glDisable(GL_DEPTH_TEST);

				glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glEnable(GL_DEPTH_TEST);

			/* Store the current screen content in Texture 1 */
			glBindTexture(GL_TEXTURE_2D, _frameBuffer->getFrameBufferTexture(1));

				glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,_frameBuffer->getWidth(), _frameBuffer->getHeight());

			glBindTexture(GL_TEXTURE_2D, 0);

			Utility::checkOpenGLError("ERROR: Could not draw the post processing effect.");

		/* Unbind the Shader Program */
		glUseProgram(0);

	glBindVertexArray(0);
}

void MotionBlur::reshape(GLint width, GLint height) {

	/* Reshape the Motion Blur FrameBuffer */
	_frameBuffer->reshape(width, height);

	/* Store the current screen content in Texture 1 */
	glBindTexture(GL_TEXTURE_2D, _frameBuffer->getFrameBufferTexture(1));

		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer->getFrameBufferObject());

			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,_frameBuffer->getWidth(), _frameBuffer->getHeight());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D,0);
}