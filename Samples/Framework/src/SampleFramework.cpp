#include "SamplesFramework.h"

int main(int /*argc*/, char* /*argv*/[])
{
    // Basic start-up for the sample browser application.
    // Will remain in run() until quitting

    SamplesFramework sampleFramework;
    return sampleFramework.run();
}


SamplesFramework::SamplesFramework()
{
}

SamplesFramework::~SamplesFramework()
{
}


int SamplesFramework::run()
{
    return 0;
}