#pragma once
#include <kapusha/core.h>
#include <kapusha/window.h>

void kp__X11Init();
void kp__X11Run();
void kp__X11Close();

KPsize kp__X11OutputsSelect(KPuptr *selectors, KPoutput_o *outputs, KPsize max);
KPsize kp__X11InputsSelect(KPuptr *selectors, KPinput_o *inputs, KPsize max);

