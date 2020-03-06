#pragma once

/// The ADSR class allow you attack-decay-sustain-release envelopes.

enum AdsrState
{
    AdsrAttack,
    AdsrDecay,
    AdsrSustain,
    AdsrRelease
};

class Adsr
{
    public:
    float fs = 44100;

    float attack = 0.5;
    float decay = 0.5;
    float sustain = 0.5;
    float rel = 1.0;
    /// Are the variable controlling the shape of the envelope.

    AdsrState state = AdsrState::AdsrRelease;

    float value = 0;
    /// Is the current value of the envelope.

    Adsr(float a, float d, float s, float r, float fs = 44100);

    void draw();
    /// Draw a control UI for the envelope;

    float next();
    /// Get the next sample of the envelope.

    void release();
    /// Release the envelope.

    void trigger();
    /// Trigger the envelope.
};