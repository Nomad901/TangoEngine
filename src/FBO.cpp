#include "FBO.h"

FBO::FBO(uint32_t pWidth, uint32_t pHeight)
{
    init(pWidth, pHeight);
}

FBO::~FBO()
{
    glDeleteFramebuffers(1, &mFBO);
    mFBO = 0;
    mRenderBufferID = 0;
    mTexture.destroyTexture();
}

void FBO::init(uint32_t pWidth, uint32_t pHeight)
{
    mWidth = pWidth;
    mHeight = pHeight;

    // frame buffer
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    
    // color texture
    mTexture.initEmpty(pWidth, pHeight);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.getID(), 0);

    // depth and stencil
    glGenRenderbuffers(1, &mRenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
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

void FBO::getSize(uint32_t& pWidht, uint32_t& pHeight)
{
    pWidht = mWidth;
    pHeight = mHeight;
}
