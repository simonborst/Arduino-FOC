#include "BackEmf.h"
#include <SimpleFOC.h>


float _readADCVoltageInline(const int pin);

BackEMF::BackEMF(float gain_a, float gain_b, float gain_c, int pinA, int pinB, int pinC)
    :   gain_a(gain_a)
    ,   gain_b(gain_b)
    ,   gain_c(gain_c)
    ,   pinA(pinA)
    ,   pinB(pinB)
    ,   pinC(pinC)
{

    offset_a = 0;
    offset_b = 0;
    offset_c = 0;
}

void BackEMF::init(BLDCDriver* driver, float amplitude) {
    // high duty cycle
    pinMode(A_GPIO_BEMF, OUTPUT);
    digitalWrite(A_GPIO_BEMF, 0);
    calibrateOffsets(driver, amplitude);

    // low duty cycle
    // pinMode(GPIO_BEMF, INPUT_FLOATING);
    // digitalWrite(GPIO_BEMF, 1);
    // pinMode(BEMF1, INPUT_ANALOG);
    // pinMode(BEMF2, INPUT_ANALOG);
    // pinMode(BEMF3, INPUT_ANALOG);
    // gain_a = 1;
    // gain_b = 1;
    // gain_c = 1;
    // offset_a = 0;
    // offset_b = 0;
    // offset_c = 0;

    return;

    // pinMode(GPIO_BEMF, INPUT_FLOATING);
    // digitalWrite(GPIO_BEMF, 1);
    // pinMode(BEMF1, INPUT_ANALOG);
    // pinMode(BEMF2, INPUT_ANALOG);
    // pinMode(BEMF3, INPUT_ANALOG);


    gain_a = 1000;
    gain_b = 1000;
    gain_c = 1000;
    offset_a = 0;
    offset_b = 0;
    offset_c = 0;


    // when measuring troughs with psu on, ADC always returns the maximum value(4095) regardless of pin state.
    // floating:
    // when psu voltage <= 5, some value seem to change.
    // with psu off, seems to sort of work, noisy on u.
    // output 0:
    // with psu voltage <= 10, seems to sort of work.
    // output 1:
    // when psu voltage <= 5, some value seem to change.

    // pinMode(PHASE_UH, INPUT_FLOATING);
    // pinMode(PHASE_UL, INPUT_FLOATING);
    

    // float center = driver->voltage_limit / 2;
    float center = 12;
    // read and print adc
    for (int i = 0; i < 10000; i++) {
        // if ((i % 3) == 0)
            driver->setPwm(center + amplitude, center - amplitude / 2, center - amplitude / 2);
        // if ((i % 3) == 1)
            // driver->setPwm(center - amplitude / 2, center + amplitude, center - amplitude / 2);
        // if ((i % 3) == 2)
            // driver->setPwm(center - amplitude / 2, center - amplitude / 2, center + amplitude);

        // driver->setPwm(center, center, center);


        // if (i % 3 == 0)
        //     driver->setPwm(amplitude, 0, 0);
        // if (i % 3 == 1)
        //     driver->setPwm(0, amplitude, 0);
        // if (i % 3 == 2)
        //     driver->setPwm(0, 0, amplitude);
    
        delay(200);

        PhaseCurrent_s emf = getBackEMF();
        Serial.printf("readings: %i %4.2f %4.2f %4.2f\r\n", i % 3, emf.a, emf.b, emf.c);

    }

    while (1) {
        pinMode(A_PHASE_UH, INPUT_FLOATING);
        pinMode(A_PHASE_UL, INPUT_FLOATING);
        pinMode(A_PHASE_VH, INPUT_FLOATING);
        pinMode(A_PHASE_VL, INPUT_FLOATING);
        pinMode(A_PHASE_WH, INPUT_FLOATING);
        pinMode(A_PHASE_WL, INPUT_FLOATING);
    }



    Serial.printf("stall\r\n");
    while(1) {
        delay(100);
    }
}

// Function finding zero offsets of the ADC
void BackEMF::calibrateOffsets(BLDCDriver* driver, float amplitude){
    float center = driver->voltage_limit / 2;
    const int calibration_rounds = 999;
    
    _delay(500);

    // find adc offset = zero current voltage
    offset_a = 0;
    offset_b = 0;
    offset_c = 0;
    // read the adc voltage 1000 times ( arbitrary number )
    for (int i = 0; i < calibration_rounds; i++) {
        // randomly set a voltage on some of the pwm pins, then measure the voltage
        if (i % 3 == 0)
            driver->setPwm(center + amplitude, center - amplitude / 2, center - amplitude / 2);
        if (i % 3 == 1)
            driver->setPwm(center - amplitude / 2, center + amplitude, center - amplitude / 2);
        if (i % 3 == 2)
            driver->setPwm(center - amplitude / 2, center - amplitude / 2, center + amplitude);

        _delay(1);
        offset_a += _readADCVoltageInline(pinA);
        offset_b += _readADCVoltageInline(pinB);
        if(_isset(pinC)) offset_c += _readADCVoltageInline(pinC);

        // Serial.printf("a b c %f %f %f\r\n",
        //     _readADCVoltageInline(pinA),
        //     _readADCVoltageInline(pinB),
        //     _readADCVoltageInline(pinC)
        // );
    }
    // // calculate the mean offsets
    // offset_a = offset_a / calibration_rounds;
    // offset_b = offset_b / calibration_rounds;
    // if(_isset(pinC)) offset_c = offset_c / calibration_rounds;

    driver->setPwm(center, center, center);

    // calculate gain
    gain_a = center / (offset_a / calibration_rounds); offset_a = 0;
    gain_b = center / (offset_b / calibration_rounds); offset_b = 0;
    gain_c = center / (offset_c / calibration_rounds); offset_c = 0;


    Serial.printf("backemf gain calibrated as: %f %f %f\r\n", gain_a, gain_b, gain_c);
}


PhaseCurrent_s BackEMF::getBackEMF() {
    PhaseCurrent_s new_read = readVoltageInternal();
    if (new_read == last_raw_read) {
        return last_raw_read - last_voltage_offsets;
    } else {
        return new_read - current_voltage_offsets;
    }
}

PhaseCurrent_s BackEMF::getBackEMFRaw() {
    PhaseCurrent_s new_read = readVoltageInternal();
    return new_read;
}

void BackEMF::storeVoltageOffset(float Ua, float Ub, float Uc) {
    last_raw_read = readVoltageInternal();
    last_voltage_offsets = current_voltage_offsets;
    current_voltage_offsets = PhaseCurrent_s{Ua, Ub, Uc};
}

PhaseCurrent_s BackEMF::readVoltageInternal() {
    PhaseCurrent_s current;
    current.a = (_readADCVoltageInline(pinA) - offset_a)*gain_a;// amps
    current.b = (_readADCVoltageInline(pinB) - offset_b)*gain_b;// amps
    current.c = (!_isset(pinC)) ? 0 : (_readADCVoltageInline(pinC) - offset_c)*gain_c; // amps
    return current;
}