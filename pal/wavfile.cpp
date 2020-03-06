#include <vector>
#include <string>
#include <sndfile.h>

void writeWavFile(std::string path, std::vector<float> samples, float sampleRate)
{
    SF_INFO info;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    info.samplerate = sampleRate;
    info.channels = 1;

    SNDFILE *file = sf_open(path.c_str(), SFM_WRITE, &info);
    
    if (file == NULL)
    {
        std::string err(sf_strerror(file));
        throw std::runtime_error("Could not open file " + path + "for writing because: " + err);
    }

    int numWritten = sf_write_float(file, samples.data(), samples.size());

    if (numWritten != samples.size()) 
    {
        throw std::runtime_error("Did not write the expected number of samples");
    }

    sf_close(file);
}

void readWavFile(std::string path, std::vector<float> &samples, int channel, float *sampleRateOut)
{
        SF_INFO info;
        SNDFILE *file = sf_open(path.c_str(), SFM_READ, &info);

        if (file == NULL)
        {
            sf_close(file);
            printf("Error: could not load file %s: %s\n", path.c_str(), sf_strerror(file));
            return;
        }

        int numSamples = info.frames * info.channels;

        std::vector<float> temp(numSamples, 0);

        if (sf_read_float(file, temp.data(), numSamples) != numSamples)
        {
            printf("Error: did not read the correct number of samples from %s\n", path.c_str());
        }

        samples.clear();
        samples.reserve(info.frames);

        // Unscramble data
        for (int s = 0; s < info.frames; s++)
        {
            samples.push_back(temp[s * info.channels + channel]);
        }

        sf_close(file);
}