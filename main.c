

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "utils/ustdlib.h"
#include "Kentec320x240x16_ssd2119_8bit.h"
#include "touch.h"
#include "images.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


//*****************************************************************************
//
// The DMA control structure table.
//
//*****************************************************************************
#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable sDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(sDMAControlTable, 1024)
tDMAControlTable sDMAControlTable[64];
#else
tDMAControlTable sDMAControlTable[64] __attribute__ ((aligned(1024)));
#endif

//*****************************************************************************
//
// Forward declarations for the globals required to define the widgets at
// compile-time.
//
//*****************************************************************************
void OnPrevious(tWidget *pWidget);
void OnNext(tWidget *pWidget);
void OnIntroPaint(tWidget *pWidget, tContext *pContext);
void OnPrimitivePaint(tWidget *pWidget, tContext *pContext);
void OnCanvasPaint(tWidget *pWidget, tContext *pContext);
void OnCheckChange(tWidget *pWidget, uint32_t bSelected);
void OnButtonPress(tWidget *pWidget);
void OnRadioChange(tWidget *pWidget, uint32_t bSelected);
void OnSliderChange(tWidget *pWidget, int32_t lValue);
void Reset();
void ADC();
void ReadTemp();
void SPI();
void VU_meter();
//void SPI();

extern tCanvasWidget g_psPanels[];


//*****************************************************************************
//
// Potentiometer Global Variables
//
//*****************************************************************************
#define TONAL_CONTROL_RANGE 127
#define VOLUME_CONTROL_RANGE 127
int32_t TREBLE = 32;
int32_t MIDRANGE = 32;
int32_t BASS = 32;
int32_t VOLUME = 50;
int32_t BALANCE = 32;

//#define ContainerFillOn(psWidget)

//#define ContainerFillOff(psWidget)
#define NUM_PUSH_BUTTONS        (sizeof(g_psPushButtons) /   \
                                 sizeof(g_psPushButtons[0]))
uint32_t g_ulButtonState;




//screen 1
//*****************************************************************************
//
// Sliders for Potentiometer Control
//
//*****************************************************************************
Canvas(g_sSliderValueCanvas, g_psPanels, 0, 0,
       &g_sKentec320x240x16_SSD2119, 0, 0, 200, 200,
       CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_OPAQUE, ClrBlack, 0, ClrSilver,
       &g_sFontCm24, "", 0, 0);

tSliderWidget g_psSliders[] =
{
    SliderStruct(g_psPanels + 0, g_psSliders + 1, 0,
                 &g_sKentec320x240x16_SSD2119, 10, 40, 220, 22, 0, TONAL_CONTROL_RANGE, 32,
                 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
                  SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
                 ClrGray, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
                 &g_sFontCm20, "Treble", 0, 0, OnSliderChange),
SliderStruct(g_psPanels + 0, g_psSliders + 2, 0,
&g_sKentec320x240x16_SSD2119, 10, 70, 220, 20, 0, TONAL_CONTROL_RANGE, 32,
(SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
 SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
ClrGray, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
&g_sFontCm20, "Mid", 0, 0, OnSliderChange),
SliderStruct(g_psPanels + 0, g_psSliders + 3, 0,
&g_sKentec320x240x16_SSD2119, 10, 100, 220, 20, 0, TONAL_CONTROL_RANGE, 32,
(SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
 SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
ClrGray, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
&g_sFontCm20, "Bass", 0, 0, OnSliderChange),
SliderStruct(g_psPanels + 0, g_psSliders + 4, 0,
&g_sKentec320x240x16_SSD2119, 10, 130, 220, 20, 0, TONAL_CONTROL_RANGE, 32,
(SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
ClrGray, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
&g_sFontCm20, "Balance", 0, 0, OnSliderChange),
/*SliderStruct(g_psPanels + 0, g_psSliders + 4, 0,
&g_sKentec320x240x16_SSD2119, 260, 40, 30, 150, 0, 100, 25,
(SL_STYLE_IMG | SL_STYLE_BACKG_IMG | SL_STYLE_VERTICAL |
SL_STYLE_OUTLINE | SL_STYLE_LOCKED), 0, ClrBlack, ClrSilver, 0, 0, 0,
0, g_pucGettingHotter28x148, g_pucGettingHotter28x148Mono,
OnSliderChange),*/
SliderStruct(g_psPanels + 0, &g_sSliderValueCanvas, 0,
&g_sKentec320x240x16_SSD2119, 10, 160, 220, 20, 0, VOLUME_CONTROL_RANGE, 50,
(SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
 SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
 ClrPurple, ClrBlack, ClrSilver, ClrWhite, ClrWhite,
&g_sFontCm20, "Volume ", 0, 0, OnSliderChange),


};
#define SLIDER_CANVAS_VAL_INDEX 5

#define NUM_SLIDERS (sizeof(g_psSliders) / sizeof(g_psSliders[0]))
//screen 2
Canvas(g_sSliderValueCanvas2, g_psPanels + 1, 0, 0,
       &g_sKentec320x240x16_SSD2119, 0, 0, 200, 200,
       CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_OPAQUE, ClrBlack, 0, ClrSilver,
       &g_sFontCm24, "", 0, 0);




//VU Meter


Container(g_sContainer20, g_psPanels + 1, 0, 0,
          &g_sKentec320x240x16_SSD2119, 285, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");

Container(g_sContainer19, g_psPanels + 1, &g_sContainer20, 0,
          &g_sKentec320x240x16_SSD2119, 260, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer18, g_psPanels + 1, &g_sContainer19, 0,
          &g_sKentec320x240x16_SSD2119, 235, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer17, g_psPanels + 1, &g_sContainer18, 0,
          &g_sKentec320x240x16_SSD2119, 210, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer16, g_psPanels + 1, &g_sContainer17, 0,
          &g_sKentec320x240x16_SSD2119, 185, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer15, g_psPanels + 1, &g_sContainer16, 0,
          &g_sKentec320x240x16_SSD2119, 160, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer14, g_psPanels + 1, &g_sContainer15, 0,
          &g_sKentec320x240x16_SSD2119, 135, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer13, g_psPanels + 1, &g_sContainer14, 0,
          &g_sKentec320x240x16_SSD2119, 110, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer12, g_psPanels + 1, &g_sContainer13, 0,
          &g_sKentec320x240x16_SSD2119, 85, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer11, g_psPanels + 1, &g_sContainer12, 0,
          &g_sKentec320x240x16_SSD2119, 60, 130, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");

//******************************************************************

Container(g_sContainer10, g_psPanels + 1, &g_sContainer11, 0,
          &g_sKentec320x240x16_SSD2119, 285, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer9, g_psPanels + 1, &g_sContainer10, 0,
          &g_sKentec320x240x16_SSD2119, 260, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer8, g_psPanels + 1, &g_sContainer9, 0,
          &g_sKentec320x240x16_SSD2119, 235, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer7, g_psPanels + 1, &g_sContainer8, 0,
          &g_sKentec320x240x16_SSD2119, 210, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer6, g_psPanels + 1, &g_sContainer7, 0,
          &g_sKentec320x240x16_SSD2119, 185, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer5, g_psPanels + 1, &g_sContainer6, 0,
          &g_sKentec320x240x16_SSD2119, 160, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer4, g_psPanels + 1, &g_sContainer5, 0,
          &g_sKentec320x240x16_SSD2119, 135, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer3, g_psPanels + 1, &g_sContainer4, 0,
          &g_sKentec320x240x16_SSD2119, 110, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer2, g_psPanels + 1, &g_sContainer3, 0,
          &g_sKentec320x240x16_SSD2119, 85, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");
Container(g_sContainer1, g_psPanels + 1, &g_sContainer2, 0,
          &g_sKentec320x240x16_SSD2119, 60, 50, 20, 45,
           CTR_STYLE_FILL | CTR_STYLE_TEXT | CTR_STYLE_TEXT_OPAQUE | CTR_STYLE_TEXT_CENTER , ClrYellow,
          ClrGray, ClrSilver, &g_sFontCm22, "");




#define NUM_SLIDERS (sizeof(g_psSliders) / sizeof(g_psSliders[0]))

//*****************************************************************************
//
// An array of canvas widgets, one per panel.  Each canvas is filled with
// black, overwriting the contents of the previous panel.
//
//*****************************************************************************
tCanvasWidget g_psPanels[] =
{
CanvasStruct(0, 0, g_psSliders, &g_sKentec320x240x16_SSD2119, 0,
                24, 320, 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),

CanvasStruct(0, 0, &g_sContainer1, &g_sKentec320x240x16_SSD2119, 0, 24,
320, 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
};




//*****************************************************************************
//
// The number of panels.
//
//*****************************************************************************

#define NUM_PANELS              (sizeof(g_psPanels) / sizeof(g_psPanels[0]))






char *g_pcPanelNames[] =
{

    "     Controls    ",
    "     VU Meter    "

};

//*****************************************************************************
//
// The buttons and text across the bottom of the screen.
//
//*****************************************************************************
RectangularButton(g_sPrevious, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 210,
                  40, 30, PB_STYLE_FILL, ClrBlack, ClrBlack, 0, ClrSilver,
                  &g_sFontCm20, "-", g_pucBlue50x50, g_pucBlue50x50Press, 0, 0,
                  OnPrevious);

Canvas(g_sTitle, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 50, 190, 220, 50,
       CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_OPAQUE, 0, 0, ClrSilver,
       &g_sFontCm20, 0, 0, 0);

RectangularButton(g_sNext, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 270, 210,
                  40, 30, PB_STYLE_IMG | PB_STYLE_TEXT, ClrBlack, ClrBlack, 0,
                  ClrSilver, &g_sFontCm20, "+", g_pucBlue50x50,
                  g_pucBlue50x50Press, 0, 0, OnNext);




//*****************************************************************************
//
// Handles presses of the previous panel button.
//
//*****************************************************************************
uint32_t g_ulPanel;

void OnPrevious(tWidget *pWidget)
{
    if(g_ulPanel == 0)
    {
        return;
    }

    WidgetRemove((tWidget *)(g_psPanels + g_ulPanel));

    g_ulPanel--;

    WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ulPanel));
    WidgetPaint((tWidget *)(g_psPanels + g_ulPanel));

    CanvasTextSet(&g_sTitle, g_pcPanelNames[g_ulPanel]);
    WidgetPaint((tWidget *)&g_sTitle);

    if(g_ulPanel == 0)
    {
        PushButtonImageOff(&g_sPrevious);
        PushButtonTextOff(&g_sPrevious);
        PushButtonFillOn(&g_sPrevious);
        WidgetPaint((tWidget *)&g_sPrevious);
    }

    if(g_ulPanel == (NUM_PANELS - 2))
    {
        PushButtonImageOn(&g_sNext);
        PushButtonTextOn(&g_sNext);
        PushButtonFillOff(&g_sNext);
        WidgetPaint((tWidget *)&g_sNext);
    }

}

//*****************************************************************************
//
// Handles presses of the next panel button.
//
//*****************************************************************************
void OnNext(tWidget *pWidget)
{
    if(g_ulPanel == (NUM_PANELS - 1))
    {
        return;
    }

    WidgetRemove((tWidget *)(g_psPanels + g_ulPanel));

    g_ulPanel++;

    WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ulPanel));
   WidgetPaint((tWidget *)(g_psPanels + g_ulPanel));

    CanvasTextSet(&g_sTitle, g_pcPanelNames[g_ulPanel]);
    WidgetPaint((tWidget *)&g_sTitle);

    if(g_ulPanel == 1)
    {
        PushButtonImageOn(&g_sPrevious);
        PushButtonTextOn(&g_sPrevious);
        PushButtonFillOff(&g_sPrevious);
        WidgetPaint((tWidget *)&g_sPrevious);
    }

    if(g_ulPanel == (NUM_PANELS - 1))
    {
        PushButtonImageOff(&g_sNext);
        PushButtonTextOff(&g_sNext);
        PushButtonFillOn(&g_sNext);
        WidgetPaint((tWidget *)&g_sNext);
    }

}

//*****************************************************************************
//
// Handles press notifications for the push button widgets.
//
//*****************************************************************************
void OnButtonPress(tWidget *pWidget)
{

}

int temp;
int temp_vol;
int temp_mid;
int temp_bal;
int temp_treb;
//*****************************************************************************
//
// Handles notifications from the slider controls.
//
//*****************************************************************************
void OnSliderChange(tWidget *pWidget, int32_t lValue)
{


if(pWidget == (tWidget *)&g_psSliders[0]) // Treble
{

GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4, 0x00);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5, GPIO_PIN_5);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, GPIO_PIN_6);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, GPIO_PIN_7);
GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, GPIO_PIN_2);


temp_treb=(int)((float)(lValue*0.475));
SSIDataPut(SSI3_BASE, temp_treb);

TREBLE = lValue;
}

if(pWidget == (tWidget *)&g_psSliders[1]) // Mid
{


GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4, GPIO_PIN_4);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5, 0x00);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, GPIO_PIN_6);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, GPIO_PIN_7);
GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, GPIO_PIN_2);

temp_mid=(int)((float)(lValue*0.75));
SSIDataPut(SSI3_BASE, temp_mid);
MIDRANGE = lValue;
}

if(pWidget == (tWidget *)&g_psSliders[2]) // Bass
{



temp=(int)((float)(lValue)*0.51);

GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4, GPIO_PIN_4);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5, GPIO_PIN_5);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, 0x00);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, GPIO_PIN_7);
GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, GPIO_PIN_2);

SSIDataPut(SSI3_BASE, temp);

BASS = lValue;

}

if(pWidget == (tWidget *)&g_psSliders[3]) // Balance
{

GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4, GPIO_PIN_4);

GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5, GPIO_PIN_5);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, GPIO_PIN_6);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, 0x00);
GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, GPIO_PIN_2);

temp_bal=(int)((float)(lValue*2));
SSIDataPut(SSI3_BASE, temp_bal);
BALANCE = lValue;

}


if(pWidget == (tWidget *)&g_psSliders[4]) // Volume
{


GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4, GPIO_PIN_4);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5, GPIO_PIN_5);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6, GPIO_PIN_6);
GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7, GPIO_PIN_7);
GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2, 0x00);

temp_vol=(int)((float)(lValue*0.25));
SSIDataPut(SSI3_BASE, temp_vol);
VOLUME = lValue;


}
while(SSIBusy(SSI3_BASE))
{
}
SysCtlDelay(128);

}




void SPI()
{
SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
GPIOPinConfigure(GPIO_PD0_SSI3CLK);
GPIOPinConfigure(GPIO_PD3_SSI3TX );
GPIOPinConfigure(GPIO_PD1_SSI3FSS);//chip sel
GPIOPinTypeSSI(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_3 | GPIO_PIN_1 );
GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);
SSIEnable(SSI3_BASE);
}

uint32_t ADC_value = 0;
int scale = 0;
int scale1 = 0;
int test[5000];
int count = 0;
int timer_count = 0;

void ADC()
{
uint32_t ui32Value[4];
//
// Enable the ADC0 module.
//
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//
// Wait for the ADC0 module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
{
}
//
// Enable the first sample sequencer to capture the value of channel 0 when
// the processor trigger occurs.
//
GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH2);
ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH2);
ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH2);
ADCSequenceStepConfigure(ADC0_BASE, 1, 3,ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2);
ADCSequenceEnable(ADC0_BASE, 1);

//while(0)
//{






//
// Trigger the sample sequence.
//
ADCIntClear(ADC0_BASE, 0);
ADCProcessorTrigger(ADC0_BASE, 1);
//
// Wait until the sample sequence has completed.
//
while(!ADCIntStatus(ADC0_BASE, 1, false))
{
}
//
// Read the value from the ADC.
//
ADCSequenceDataGet(ADC0_BASE, 1, ui32Value);
ADC_value = (ui32Value[0] + ui32Value[1] + ui32Value[2] + ui32Value[3])/4 ;
//voltage_val = ADC_value * 0.008;
if (count < 5000)
{
test[count] = ADC_value;
count = count + 1;
} else {
count = 0;
test[count] = ADC_value;
}

scale = 20*(log10(ADC_value/300*2)-3); //(ADC_value*10)/128;
//ContainerFillOff(&g_sContainer20);


//Put to VU
//scale = -30;
if (ADC_value<1)//scale is some value
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOff(&g_sContainer15);
ContainerFillOff(&g_sContainer14);
ContainerFillOff(&g_sContainer13);
ContainerFillOff(&g_sContainer12);
ContainerFillOff(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOff(&g_sContainer3);
ContainerFillOff(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}


else if (ADC_value<(38))//scale is some value
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOff(&g_sContainer15);
ContainerFillOff(&g_sContainer14);
ContainerFillOff(&g_sContainer13);
ContainerFillOff(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOff(&g_sContainer3);
ContainerFillOff(&g_sContainer2);
ContainerFillOff(&g_sContainer1);
}

else if(ADC_value <=(63))
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOff(&g_sContainer15);
ContainerFillOff(&g_sContainer14);
ContainerFillOff(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOff(&g_sContainer3);
ContainerFillOff(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value<(120))
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOff(&g_sContainer15);
ContainerFillOff(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOff(&g_sContainer3);
ContainerFillOff(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}
else if(ADC_value <(160))
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOff(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOff(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value <(200))
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOff(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOff(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if( ADC_value <(235) )
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOff(&g_sContainer17);
ContainerFillOn(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOff(&g_sContainer5);
ContainerFillOn(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value <(320) )
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOff(&g_sContainer18);
ContainerFillOn(&g_sContainer17);
ContainerFillOn(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOff(&g_sContainer6);
ContainerFillOn(&g_sContainer5);
ContainerFillOn(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value <=(350))
{
ContainerFillOff(&g_sContainer20);
ContainerFillOff(&g_sContainer19);
ContainerFillOn(&g_sContainer18);
ContainerFillOn(&g_sContainer17);
ContainerFillOn(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);

ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOff(&g_sContainer7);
ContainerFillOn(&g_sContainer6);
ContainerFillOn(&g_sContainer5);
ContainerFillOn(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value < 450)
{
ContainerFillOff(&g_sContainer20);
ContainerFillOn(&g_sContainer19);
ContainerFillOn(&g_sContainer18);
ContainerFillOn(&g_sContainer17);
ContainerFillOn(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);


ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOff(&g_sContainer8);
ContainerFillOn(&g_sContainer7);
ContainerFillOn(&g_sContainer6);
ContainerFillOn(&g_sContainer5);
ContainerFillOn(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

else if(ADC_value >= 450)
{
ContainerFillOn(&g_sContainer20);
ContainerFillOn(&g_sContainer19);
ContainerFillOn(&g_sContainer18);
ContainerFillOn(&g_sContainer17);
ContainerFillOn(&g_sContainer16);
ContainerFillOn(&g_sContainer15);
ContainerFillOn(&g_sContainer14);
ContainerFillOn(&g_sContainer13);
ContainerFillOn(&g_sContainer12);
ContainerFillOn(&g_sContainer11);


ContainerFillOff(&g_sContainer10);
ContainerFillOff(&g_sContainer9);
ContainerFillOn(&g_sContainer8);
ContainerFillOn(&g_sContainer7);
ContainerFillOn(&g_sContainer6);
ContainerFillOn(&g_sContainer5);
ContainerFillOn(&g_sContainer4);
ContainerFillOn(&g_sContainer3);
ContainerFillOn(&g_sContainer2);
ContainerFillOn(&g_sContainer1);
}

}







int main(void)
{
    tContext sContext;
    tRectangle sRect;

#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    uint32_t ui32SysClock;
#endif
    //
    // Set the clocking to run at 20 MHz (200 MHz / 10) using the PLL.  When
    // using the ADC, you must either use the PLL or supply a 16 MHz clock
    // source.
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 20000000);
#else
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#endif

    FPUEnable();
    FPULazyStackingEnable();

    // Set the clock to 40Mhz derived from the PLL and the external oscillator
    //SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Reset Button Bindings
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);

    // Initialize the display driver.
    Kentec320x240x16_SSD2119Init();

    // Initialize the graphics context.
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);

    // Fill the top 24 rows of the screen with blue to create the banner.
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 23;
    GrContextForegroundSet(&sContext, ClrPurple);
    GrRectFill(&sContext, &sRect);

    // Put a white box around the banner.
    GrContextForegroundSet(&sContext, ClrPink);
    GrRectDraw(&sContext, &sRect);

    // Put the application name in the middle of the banner.
    GrContextFontSet(&sContext, &g_sFontCm20);
    GrStringDrawCentered(&sContext, "VU GUI", -1, GrContextDpyWidthGet(&sContext) / 2, 8, 0);

    // Configure and enable uDMA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    uDMAControlBaseSet(&sDMAControlTable[0]);
    uDMAEnable();
    // Initialize the touch screen driver and have it route its messages to the widget tree.
    TouchScreenInit();
    TouchScreenCallbackSet(WidgetPointerMessage);
    // Add the title block and the previous and next buttons to the widget tree.
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sPrevious);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sTitle);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sNext);

    // Add the first panel to the widget tree.
    g_ulPanel = 0;
    WidgetAdd(WIDGET_ROOT, (tWidget *)g_psPanels);
    CanvasTextSet(&g_sTitle, g_pcPanelNames[0]);

    // Issue the initial paint request to the widgets.
    WidgetPaint(WIDGET_ROOT);


    SPI();
    int ui32Loop = 0;

    while(1)
    {
    ui32Loop++;
           //if(ui32Loop == 2000000)//ADC updated 2000000
           //{
        ADC();//void VU_meter//Call VU.
    ui32Loop = 0;
    //}



        WidgetMessageQueueProcess();

        if(g_ulPanel == (NUM_PANELS - 1))
        {
        if (timer_count == 1000)
        {
        WidgetRemove((tWidget *)(g_psPanels + g_ulPanel));

        //g_ulPanel++;

        WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ulPanel));
        WidgetPaint((tWidget *)(g_psPanels + g_ulPanel));
        timer_count = 0;
        } else {
        timer_count++;
        }
        }
    }
}




