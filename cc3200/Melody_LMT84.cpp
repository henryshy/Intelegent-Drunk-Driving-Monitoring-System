#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <Energia.h>
#include "Melody_LMT84.h"

static const int16_t MinTemperature = -50;         //芯片可测最低温度
static const int16_t MaxTemperature = 150;         //芯片可测最高温度
static const uint8_t DefalutPort = A1;         

CMelodyLMT84::CMelodyLMT84()
{
    m_sampleport = DefalutPort;
}

CMelodyLMT84::CMelodyLMT84(uint8_t port)
{
    m_sampleport = port;
}


float CMelodyLMT84::calcTemperature(uint16_t sample)
{
    uint16_t voltage = map(sample,0,4095,0,1460);
    float temperature = 0.0;
    temperature = (5.506 - sqrt(30.316036 + 0.00704 * (870.6 - voltage))) / (-0.00352) + 30.0;
    if(temperature > MaxTemperature)
    {
        temperature = MaxTemperature;
    }
    if(temperature < MinTemperature)
    {
        temperature = MinTemperature;
    }
    return temperature;
}

void CMelodyLMT84::begin()
{
    begin(m_sampleport);
}


void CMelodyLMT84::begin(uint8_t port)
{
    m_sampleport = port;
}

float CMelodyLMT84::getTemperature()
{
    uint16_t ADValue = analogRead(m_sampleport);
    return calcTemperature(ADValue);
}

CMelodyLMT84  MelodyLMT84;