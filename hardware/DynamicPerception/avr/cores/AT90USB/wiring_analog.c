#include "wiring_private.h"
#include "pins_arduino.h"

uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
	analog_reference = mode;
}

uint8_t w_analog_reference = 0x40;

int analogRead(uint8_t pin)
{
	uint8_t low, high;

	if (pin >= 40 && pin <= 47) pin -= 40;
	if (pin < 8) {
		DIDR0 |= (1 << pin);
		//DDRF &= ~(1 << pin);
		//PORTF &= ~(1 << pin);
	}
	ADMUX = w_analog_reference | (pin & 0x1F);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)) ;
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

void _analogWrite(uint8_t pin, int val)
{
	pinMode(pin, OUTPUT);

	switch (pin) {
	case 24:
		if (val == 0) {
			PORTD &= ~_BV(PD0);
			cbi(TCCR0A, COM0B1);
		} else {
			OCR0B = val;
			sbi(TCCR0A, COM0B1);
		}
		break;
	case 13: //TIMER1A:
		OCR1A = val;
		sbi(TCCR1A, COM1A1);
		break;
	case 14: //TIMER1B:
		OCR1B = val;
		sbi(TCCR1A, COM1B1);
		break;
	case 15: //TIMER1C:
		OCR1C = val;
		sbi(TCCR1A, COM1C1);
		break;
	case 12: //TIMER2A:
		OCR2A = val;
		sbi(TCCR2A, COM2A1);
		break;
	case 25: //TIMER2B:
		OCR2B = val;
		sbi(TCCR2A, COM2B1);
		break;
	case 22: //TIMER3A:
		OCR3A = val;
		sbi(TCCR3A, COM3A1);
		break;
	case 21: //TIMER3B:
		OCR3B = val;
		sbi(TCCR3A, COM3B1);
		break;
	case 20: //TIMER3C:
		OCR3C = val;
		sbi(TCCR3A, COM3C1);
		break;
	default:
		if (val < 128) {
			digitalWrite(pin, LOW);
		} else {
			digitalWrite(pin, HIGH);
		}
	}
}

