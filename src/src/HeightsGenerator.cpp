#include "HeightsGenerator.h"

HeightsGenerator::HeightsGenerator()
{
    mSeed = mGenerator(mGen);
}

float HeightsGenerator::generateHeight(float pX, float pZ)
{
    float total = 0.0f;
    float d = static_cast<float>(std::pow(2, mOctaves - 1));
    for (int32_t i = 0; i < mOctaves; ++i)
    {
        float freq = static_cast<float>(std::pow(2, i) / d);
        float amp = static_cast<float>(std::pow(mRoughness, i) * mAmplitude);
        total += getInterpolatedNoise(pX * freq, pZ * freq) * amp;
    }
    return total;
}

float HeightsGenerator::getInterpolatedNoise(float pX, float pZ)
{
    int32_t intX = static_cast<int32_t>(pX);
    int32_t intZ = static_cast<int32_t>(pZ);
    float fracX = pX - intX;
    float fracZ = pZ - intZ;

    float v1 = getSmoothNoise(intX, intZ);
    float v2 = getSmoothNoise(intX + 1, intZ);
    float v3 = getSmoothNoise(intX, intZ + 1);
    float v4 = getSmoothNoise(intX + 1, intZ + 1);
    
    float i1 = interpolate(v1, v2, fracX);
    float i2 = interpolate(v3, v4, fracX);

    return interpolate(i1, i2, fracZ);
}

float HeightsGenerator::interpolate(float pFirst, float pSecond, float pBlend)
{
    float theta = pBlend * 3.141592f;
    float f = (float)(1.0f - std::cos(theta)) * 0.5f;
    return pFirst * (1.0f - f) + pSecond * f;
}

float HeightsGenerator::getSmoothNoise(float pX, float pZ)
{
    float corners = (getNoise(pX - 1.0f, pZ - 1.0f) + getNoise(pX + 1.0f, pZ - 1.0f) +
                     getNoise(pX - 1.0f, pZ + 1.0f) + getNoise(pX + 1.0f, pZ + 1.0f)) / 16.0f;
    float sides = (getNoise(pX - 1.0f, pZ) + getNoise(pX + 1.0f, pZ) +
                   getNoise(pX, pZ - 1.0f) + getNoise(pX, pZ + 1.0f)) / 8.0f;
    float center = getNoise(pX, pZ) / 4.0f;
    return corners + sides + center;
}

float HeightsGenerator::getNoise(float pX, float pZ)
{
    uint32_t hash = static_cast<uint32_t>(pX * 1619) +
                    static_cast<uint32_t>(pZ * 31337) +
                    mSeed % 0x7FFFFFFF;
    static std::mt19937 gen(hash);
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    gen.seed(hash);
    return dist(gen);
}
