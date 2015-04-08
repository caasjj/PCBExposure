
#include "panel.h"

#define PRESSED       0
#define HELD          1
#define RELEASED      2

int _selSwitchState       = RELEASED;
int _upButtonState        = RELEASED;
int _downButtonState      = RELEASED;
int _startStopButtonState = RELEASED;


unsigned char panelGetCommand(){

  char countMode;
  // If button is pressed
  //   then if button was not already pressed, respond with PRESSED.
  //        else, respond with HELD.
  // Else (i.e. button is not pressed)
  //   if button was not previously released, respond with RELEASED
  int startStopButton = _panelReadInput( PANEL_START_STOP_BUTTON );
  if ( startStopButton < ADC_SWITCH_LOW_THRESHOLD) {

    if (_startStopButtonState == RELEASED) {
      _startStopButtonState = PRESSED;
      return START_STOP_PRESSED;
    }

      _startStopButtonState = HELD;
      return START_STOP_HELD;
    
  } else if (_startStopButtonState != RELEASED) {
    _startStopButtonState = RELEASED;
    return START_STOP_RELEASED;
  }

  // if select switch is in neutral mode, count up/down buttons are disabled
  // return NULL_COMMAND
  int panelSelSwitch = _panelReadInput( PANEL_SEL_SWITCH );
  if (panelSelSwitch > ADC_SWITCH_HIGH_THRESHOLD) {
    countMode = COUNT_BUTTON_INTENSITY_OFFSET;
  } else if (panelSelSwitch < ADC_SWITCH_LOW_THRESHOLD) {
    countMode = COUNT_BUTTON_TIMER_OFFSET;
  } else {
    return NULL_COMMAND;
  }

  // if up counter button is pressed
  int upDownButton = _panelReadInput( PANEL_UP_DOWN_BUTTON );
  if (upDownButton > ADC_SWITCH_HIGH_THRESHOLD) {
    if (_upButtonState == RELEASED ) {
      _upButtonState = PRESSED;;
      return COUNT_UP_PRESSED(countMode);
    } else {
      _upButtonState = HELD;
      return COUNT_UP_HELD(countMode);
    }
  } else if (_upButtonState != RELEASED) {
    _upButtonState = RELEASED;
    return COUNT_UP_RELEASED(countMode);
  }

  // if down counter button is pressed
  if (upDownButton < ADC_SWITCH_LOW_THRESHOLD) {
    if (_downButtonState == RELEASED ) {
      _downButtonState = PRESSED;;
      return COUNT_DOWN_PRESSED(countMode);
    } else {
      _downButtonState = HELD;
      return COUNT_DOWN_HELD(countMode);
    }
  } else if (_downButtonState != RELEASED) {
    _downButtonState = RELEASED;
    return COUNT_DOWN_RELEASED(countMode);
  }

  // if select switch is set but neither button is pressed
  return NULL_COMMAND;
}


void panelInit() {
  
  MAKE_PORT_INPUT(PANEL_SEL_SWITCH_PORT, PANEL_SEL_SWITCH_PIN);
  MAKE_PORT_INPUT(PANEL_UP_DOWN_BUTTON_PORT, PANEL_UP_DOWN_BUTTON_PIN);
  MAKE_PORT_INPUT(PANEL_START_STOP_BUTTON_PORT, PANEL_START_STOP_BUTTON_PIN);
  MAKE_PORT_OUTPUT(PANEL_SYSTEM_ON_OFF_STATUS_LED_PORT, PANEL_SYSTEM_ON_OFF_STATUS_LED_PIN );
  MAKE_PORT_OUTPUT(PANEL_UV_ON_OFF_STATUS_LED_PORT, PANEL_UV_ON_OFF_STATUS_LED_PIN );

  unsigned char config=ADC_CONFIG;
  unsigned char config2 = ADC_CONFIG2;
  unsigned char port_config = ADC_PORT_CONFIG;

  OpenADC(config, config2, port_config);
}

void panelSetUiState(unsigned char state) {
  if (state == PANEL_UI_RUNNING) {
    panelSetUvOnOffStatusLed(1);
  } else {
    panelSetUvOnOffStatusLed(0);
  }
}

void panelSetSystemOnOffStatusLed(char value){
  PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH=value;
}

void panelToggleSystemOnOffStatusLed() {
  PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH=~PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH;
}

void panelSetUvOnOffStatusLed(char value){
  PANEL_UV_ON_OFF_STATUS_LED_LATCH=value;
}

void panelToggleUvOnOffStatusLed(){
  PANEL_UV_ON_OFF_STATUS_LED_LATCH=~PANEL_UV_ON_OFF_STATUS_LED_LATCH;
}

int _panelReadInput(unsigned char channel){
  int value;
  SelChanConvADC(channel);
  while(BusyADC());
  value =  ReadADC();
  
  return value;

}
