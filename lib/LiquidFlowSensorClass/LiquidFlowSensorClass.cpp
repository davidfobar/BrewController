#include "LiquidFlowSensorClass.h"

LiquidFlowSensorClass::LiquidFlowSensorClass(int inPin){
  prevPulseTime = 0;
  pulseCount = 0;
  flowRate = 0;
  pin = inPin;
  prevState = HIGH;

  pinMode(pin, INPUT_PULLUP);
}

void LiquidFlowSensorClass::updateFlowRate(){
	unsigned long curTime = millis();

  int curState = digitalRead(pin);
  if(prevState == HIGH && curState == LOW) pulseCount++;
  prevState = curState;

	if(pulseCount >= 3){ //accumulate at least three pulses for data averaging
		//flowRate is in units of gallon/min
		//sensor gives 450 pulses per liter, time is in milliseconds
		if(curTime!=prevPulseTime){ //cannot divide by zero
			double cur = pulseCount*35.22/(curTime-prevPulseTime);
      sum = sum + cur - flowRates[oldestVal];
      flowRates[oldestVal] = cur;
      flowRate = sum/NUM_TO_SUM;
      oldestVal++;
      if(oldestVal == NUM_TO_SUM) oldestVal = 0;

      if(integrating) pulsesInterated += pulseCount;
			pulseCount=0;
		}
		prevPulseTime=curTime;
	}
	//if too much time has passed, assume that flow has stopped
	//necessary to prevent scorching in the RIMS tube
	else if (curTime-prevPulseTime > PULSE_TIMEOUT) flowRate = 0;
}

double LiquidFlowSensorClass::getFlowRate(){
  return flowRate;
}

bool LiquidFlowSensorClass::isFlowing(){
  return flowRate > TOO_SLOW_FLOW_THRESHOLD;
}

int LiquidFlowSensorClass::checkFlowRate(){
  if(flowRate < TOO_SLOW_FLOW_THRESHOLD) return TOO_SLOW;
  if(flowRate > TOO_FAST_FLOW_THRESHOLD) return TOO_FAST;
  return JUST_RIGHT;
}

bool LiquidFlowSensorClass::collectionComplete(){
  if(pulsesInterated >= pulsesToCollect){
    integrating = false;
    pulsesToCollect = 0;
    pulsesInterated = 0;
    return true;
  }
  else return false;
}
void LiquidFlowSensorClass::collectWater(double volume){
  //450 pulses per liter, 0.264 gallons in a liter. 0.71 correction factor.
  pulsesToCollect = int(volume * 450.0 * 0.71 / 0.264);
  integrating = true;
}
