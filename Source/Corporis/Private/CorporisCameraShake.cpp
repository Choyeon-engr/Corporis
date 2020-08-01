// Fill out your copyright notice in the Description page of Project Settings.


#include "CorporisCameraShake.h"

UCorporisCameraShake::UCorporisCameraShake()
{
    OscillationDuration = 0.1f;
    OscillationBlendInTime = 0.02f;
    OscillationBlendOutTime = 0.02f;
    
    RotOscillation.Pitch.Amplitude = 1.0f;
    RotOscillation.Pitch.Frequency = 10.0f;
}
