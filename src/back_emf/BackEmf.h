#ifndef _BACK_EMF_H
#define _BACK_EMF_H

#include "../common/foc_utils.h"

class BLDCDriver;

class BackEMF {
public:

    BackEMF(float gain_a, float gain_b, float gain_c, int pinA, int pinB, int pinC);

    void init(BLDCDriver* driver, float drive_voltage);

    PhaseCurrent_s getBackEMF();    
    PhaseCurrent_s getBackEMFRaw();    

    void storeVoltageOffset(float Ua, float Ub, float Uc);

private:
    PhaseCurrent_s readVoltageInternal();

    void calibrateOffsets(BLDCDriver* driver, float drive_voltage);

    float gain_a;
    float gain_b;
    float gain_c;

    float offset_a;
    float offset_b;
    float offset_c;

    int pinA;
    int pinB;
    int pinC;

    PhaseCurrent_s last_raw_read;
    PhaseCurrent_s last_voltage_offsets;
    PhaseCurrent_s current_voltage_offsets;

};

#endif