#include "delay.h"
#include <vector>
#include <cmath>
#ifdef PAL_TEST
#include "test.h"
#endif

namespace pal
{
    Delay::Delay(int length) : buffer(length + 1, 0)
    {
        index = 0;
    }

    void Delay::proceed()
    {
        index = (index + 1) % buffer.size();
    }

    float Delay::read()
    {
        int i = index - getLength();

        // This is an efficient way of wrapping `i` around in case it is less
        // than zero.
        i = i + (i < 0) * getLength();

        return buffer[i];
    }

    float Delay::readAt(int n)
    {
        int i = index - n;

        // This is an efficient way of wrapping `i` around in case it is less
        // than zero.
        i = i + (i < 0) * getLength();

        return buffer[i];
    }

    float Delay::readFractional(float n)
    {
        int upper = ceil(n);
        int lower = floor(n);
        float a = n - lower;

        float x1 = readAt(lower);
        float x2 = readAt(upper);
        float y = x1 + a * (x2 - x1);
        return y;
    }

    void Delay::write(float x)
    {
        buffer[index] = x;
    }

    void Delay::writeAt(float x, int n)
    {
        int i = index - n;

        // This is an efficient way of wrapping `i` around in case it is less
        // than zero.
        i = i + (i < 0) * getLength();

        buffer[i] = x;
    }

#ifdef PAL_TEST
    void Delay::test()
    {
        printf("Testing Delay ... ");

        float input = 42.0;
        float output = 0;

        // Test single sample delay
        {
            Delay delay(1);
            input = 42.0;

            delay.write(input);
            delay.proceed();
            output = delay.read();
        }

        check(output == input);

        // Test 100 sample delay
        {
            Delay delay(100);
            delay.write(input);

            for (int i = 0; i < 100; i++)
            {
                delay.proceed();
            }

            output = delay.read();
        }

        check(output == input);

        // Test readAt
        {
            Delay delay(100);
            delay.write(input);

            for (int i = 0; i < 50; i++)
            {
                delay.proceed();
            }

            output = delay.readAt(50);
        }

        check(output == input);

        // Test readFractional
        {
            Delay delay(1);
            delay.write(input);
            delay.proceed();
            delay.write(0);

            output = delay.readFractional(0.2);
        }

        check(fabs(output - 0.2 * input) < 0.01);
    }
#endif
}