#include "FBO.h"

FBO::FBO(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize)
{
    init(pScreenWidth, pScreenHeight, pPos, pSize);
    initShader();
}

FBO::~FBO()
{
    glDeleteFramebuffers(1, &mFBO);
    mFBO = 0;
    mRenderBufferID = 0;
    mTexture.destroyTexture();
}

void FBO::init(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize)
{
    mScreenQuad.init(pScreenWidth, pScreenHeight, pPos, pSize);
    mScreenWidth = pScreenWidth;
    mScreenHeight = pScreenHeight;

    // frame buffer
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    // color texture
    mTexture.initEmpty(pSize.x, pSize.y);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.getID(), 0);

    // depth and stencil
    glGenRenderbuffers(1, &mRenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pSize.x, pSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

    // completeness 
    int32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << std::format("Frame buffer wasnt completed! Status: {}\n", status);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::start()
{
    FBO::bind();
    glViewport(0, 0, static_cast<GLsizei>(mScreenQuad.getSize().x), static_cast<GLsizei>(mScreenQuad.getSize().y));
    FBO::clearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::stop()
{
    FBO::unbind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(static_cast<GLint>(mScreenQuad.getPos().x), static_cast<GLint>(mScreenQuad.getPos().y),
               static_cast<GLsizei>(mScreenQuad.getSize().x), static_cast<GLsizei>(mScreenQuad.getSize().y));
}

void FBO::stopAndRender()
{
    FBO::stop();
    FBO::render();
}

void FBO::render()
{
    mShader.bind();
    mShader.setUniform1i("uTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture.getID());
    mScreenQuad.render();
    
    glViewport(0, 0, mScreenWidth, mScreenHeight);
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FBO::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::clearColor()
{
    float r{ mClearColors.x };
    float g{ mClearColors.y };
    float b{ mClearColors.z };
    float a{ mClearColors.w };
    glClearColor(r, g, b, a);
}

uint32_t FBO::getFBO() const noexcept
{
    return mFBO;
}

Texture2& FBO::getTexture() noexcept
{
    return mTexture;
}

uint32_t FBO::getRenderBufferID() const noexcept
{
    return mRenderBufferID;
}

void FBO::setClearColors(const glm::vec4& pClearColors)
{
    mClearColors = pClearColors;
}

glm::vec4 FBO::getClearColors() const noexcept
{
    return mClearColors;
}

void FBO::initShader()
{
    std::string resourcePath = RESOURCES_PATH;
    mShader.init(resourcePath + "Shaders/vertFrameBuffer.glsl", resourcePath + "Shaders/fragFrameBuffer.glsl");
}

glm::vec2 FBO::getSize() const noexcept
{
    return mScreenQuad.getSize();
}

ScreenQuad& FBO::getScreenQuad() noexcept
{
    return mScreenQuad;
}

Shader& FBO::getShader() noexcept
{
    return mShader;
}

ScreenQuad::ScreenQuad(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize)
{
    init(pScreenWidth, pScreenHeight, pPos, pSize);
}

void ScreenQuad::init(uint32_t pScreenWidth, uint32_t pScreenHeight, glm::vec2 pPos, glm::vec2 pSize)
{
    mSize = pSize;
    mPos = pPos;

    if (pPos.x == 0.0f)
        pPos.x = 0.1f;
    if (pPos.y == 0.0f)
        pPos.y = 0.1f;
    if (pSize.x == 0.0f)
        pSize.x = 0.1f;
    if (pSize.y == 0.1f)
        pSize.y = 0.1f;

    glm::vec2 ndc;
    ndc.x = (pPos.x / pScreenWidth) * 2.0f - 1.0f;
    ndc.y = 1.0f - (pPos.y / pScreenHeight) * 2.0f; 

    glm::vec2 ndcSize;
    ndcSize.x = (pSize.x / pScreenWidth) * 2.0f;
    ndcSize.y = (pSize.y / pScreenHeight) * 2.0f;

    float vertices[] =
    {
        ndc.x,             ndc.y,              0.0f, 1.0f,  
        ndc.x,             ndc.y - ndcSize.y,  0.0f, 0.0f,  
        ndc.x + ndcSize.x, ndc.y - ndcSize.y,  1.0f, 0.0f,  

        ndc.x,             ndc.y,              0.0f, 1.0f,  
        ndc.x + ndcSize.x, ndc.y - ndcSize.y,  1.0f, 0.0f,  
        ndc.x + ndcSize.x, ndc.y,              1.0f, 1.0f   
    };

    mVAO.bind();
    mVBO.init(vertices, sizeof(vertices), GL_STATIC_DRAW);
    mVBOLayout.pushLayout(GL_FLOAT, 2);
    mVBOLayout.pushLayout(GL_FLOAT, 2);
    mVAO.addBuffer(mVBO, mVBOLayout);
}

glm::vec2 ScreenQuad::getSize() const noexcept
{
    return mSize;
}

glm::vec2 ScreenQuad::getPos() const noexcept
{
    return mPos;
}

void ScreenQuad::render()
{
    mVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}