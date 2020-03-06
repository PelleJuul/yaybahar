#pragma once

#include <vector>

namespace pal
{
    /// Implementation of a basic delay line.
    struct Delay
    {
        public:
        /// Create a new delay object.
        /// @param  length  The length of the delay line in samples.
        Delay(int length);

        /// Get the number of samples in the delay line.
        int getLength() { return buffer.size() - 1; };

        /// Proceed the internal read and write pointers. Should be called after
        /// a sample has been processed.
        void proceed();

        /// Read the sample at the end of the delay line.
        /// @returns    The oldest value in the delay line.
        float read();

        /// Read a sample at a specified position.
        /// @param  n   The position to read at, zero being the most recent
        ///             sample, limited to the length of the delay line.
        /// @returns    The sample at position `n`.
        float readAt(int n);

        /// Read a sample at a decimal position using linear interpolation.
        /// @param n    The position to read at, zero being the most recent
        ///             sample, limited to the length of the delay line. May be
        ///             a decimal number.
        /// @returns    The sample at position `n`, computed using linear
        ///             interpolation if `n` is fractional.
        float readFractional(float n);

        /// Write a sample to the delay line.
        /// @param  x   The sample value to write into the delay line.
        void write(float x);

        /// Write a sample at a specified position in the delay line.
        /// @param  x   The sample value to write into the delay line.
        /// @param  n   The position to read at, zero being the beginning of the
        ///             delay line, limited to the length of the delay line.
        void writeAt(float x, int n);

#ifdef PAL_TEST
        static void test();
#endif

        private:
        int index = 0;
        std::vector<float> buffer;
    };
}