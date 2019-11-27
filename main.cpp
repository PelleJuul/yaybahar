#include <vector>
#include <functional>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include "plot.h"
#include "fileio.h"
#include "waveguide.h"

float dxf(std::vector<float> &u, int i, float rh)
{
    if (i == u.size() - 1)
    {
        // todo: Add option for other boundary conditions.
        return rh * (0 - u[i]);
    }
    else if (i >= u.size())
    {
        return 0;
    }
    else
    {
        return rh * (u[i + 1] - u[i]);
    }
}

float dxb(std::vector<float> &u, int i, float rh)
{
    if (i > 0 && i < u.size())
    {
        return rh * (u[i] - u[i - 1]);
    }
    else if (i == 0)
    {
        return rh * 2 * u[i];
    }
    else if (i == u.size())
    {
        return rh * (0 - u[i - 1]);
    }
    else
    {
        return 0;
    }
}

float dxx(std::vector<float> &u, int i, float rh)
{
    return rh * (dxb(u, i + 1, rh) - dxb(u, i, rh));
}

float dxxxx(std::vector<float> &u, int i, float rh)
{
    return rh * rh * (dxx(u, i + 1, rh) + dxx(u, i - 1, rh) - 2 * dxx(u, i, rh));
}

struct wavFileHeader
{
    char fileDescriptor[4];
    uint32_t fileSize;
    char fileTypeHeader[4];
    char formatChunkMarker[4];
    uint32_t formatLength;
    uint16_t formatType;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t bytesPerFrame;
    uint16_t bitsPerSample;
    char dataChunkHeader[4];
    uint32_t dataSize;
};

void writeFile(std::vector<std::vector<float>> &data, std::string path, float sampleRate)
{
    int numChannels = data.size();
    int numSamples = data[0].size();
    uint32_t byteRate = (uint32_t)(sampleRate * 16 * numChannels) / 8;
    uint16_t bytesPerFrame = (uint32_t)(16 * numChannels) / 8;
    uint32_t dataSize = 2 * numChannels * numSamples;
    uint32_t fileSize = sizeof(wavFileHeader) + dataSize - 8;

    wavFileHeader header =
    {
        {'R','I','F','F'},      // fileDescriptor
        fileSize,               // fileSize
        {'W','A','V','E'},      // file type header
        {'f','m','t',' '},      // formatChunkMarker
        16,                     // formatLength
        1,                      // formatType
        (uint8_t)numChannels,   // numChannels
        (uint32_t)sampleRate,   // sampleRate
        byteRate,               // byteRate
        bytesPerFrame,          // bytesPerFrame
        16,                     // bitsPerSample
        {'d','a','t','a'},      // dataChunkHeader
        dataSize                // dataSize
    };

    int16_t outData[numChannels * numSamples];

    for (int s = 0; s < numSamples; s++)
    {
        for (int c = 0; c < numChannels; c++)
        {
            float x = data[c][s];
            int16_t y = std::round(((1 << 15) - 1) * x);
            outData[s * numChannels + c] = y;
        }
    }

    std::ofstream file;
    file.open(path, std::ios::binary | std::ios::out);
    file.write((char*)&header, sizeof(wavFileHeader));
    file.write((char*)outData, dataSize);
    file.close();
}

void writeFile(std::vector<float> &data, std::string path, float sampleRate)
{
    std::vector<std::vector<float>> wrapper;
    wrapper.push_back(data);
    writeFile(wrapper, path, sampleRate);
}

int main(int argc, char **argv)
{
    // Load input
    std::vector<float> x;
    readWavFile("drumloop.wav", x, 0);
    writeWavFile("drumloop_rewritten.wav", x, 44100);

    Plot plot;

    float sampleRate = 44100.0f;
    int lengthInSamples = x.size() + 5 * sampleRate;
    float k = 1.0 / sampleRate;

    printf("Rendering %i samples (%fs)\n", lengthInSamples, lengthInSamples / sampleRate);

    float k2 = k * k;
    float kappa = 0.06;
    float s0 = 4.0;
    float s1 = 10e-8;

    float kappa2 = kappa * kappa;
    int numNodes = 200;

    printf("numNodes = %i\n", numNodes);

    std::vector<float> u0(numNodes, 0);
    std::vector<float> u1(numNodes, 0);
    std::vector<float> ut(numNodes, 0);
    std::vector<float> f(numNodes, 0);
    std::vector<float> y(lengthInSamples, 0);

    bool shouldPlot = true;

    for (int n = 0; n < lengthInSamples; n++)
    {
        float dry = 0;

        if (n < x.size())
        {
            f[0] = 50000 * x[n];
            dry = x[n];
        }

        for (int l = 0; l < numNodes; l++)
        {
            ut[l] = (1.0 / (1.0 + k * s0)) *
            (
                -kappa2 * k2 * dxxxx(u0, l, numNodes)
                + k * s0 * u1[l]
                + 2 * k * s1 * (dxx(u0, l, numNodes) - dxx(u1, l, numNodes))
                - u1[l]
                + 2 * u0[l]
                + k2 * f[l]
            );
        }

        for (int l = 0; l < numNodes; l++)
        {
            u1[l] = u0[l];
            u0[l] = ut[l];
            f[l] = 0;
        }

        plot.plot(u0, 0);

        y[n] = 0.8 * 50 * u0[numNodes - 4] + 0.2 * dry;
    }

    // plot.reset();
    // while (plot.plot(y)) {};

    writeWavFile("output.wav", y, sampleRate);

    return 0;
}