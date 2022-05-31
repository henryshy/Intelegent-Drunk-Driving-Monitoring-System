#ifndef __MELODY_ANALOGTEMPERATURE_H__
#define __MELODY_ANALOGTEMPERATURE_H__

class CMelodyLMT84
{
    private:
        uint8_t m_sampleport;
        float calcTemperature(uint16_t sample);
    public:
        CMelodyLMT84();
        CMelodyLMT84(uint8_t port);
        void begin(); 
        void begin(uint8_t port);
        float getTemperature();
};

extern CMelodyLMT84 MelodyLMT84;
#endif /*__MELODY_ANALOGTEMPERATURE_H__*/