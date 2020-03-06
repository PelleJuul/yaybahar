#pragma once

#include <vector>
#include <string>

void writeWavFile(std::string path, std::vector<float> samples, float sampleRate);

void readWavFile(std::string path, std::vector<float> &samples, int channel, float *sampleRateOut = nullptr);