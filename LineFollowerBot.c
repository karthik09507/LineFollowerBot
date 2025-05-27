#include <stdint.h>
#include <util/delay.h>

#define DDRD_REG  (*(volatile uint8_t*)0x2A)
#define PORTD_REG (*(volatile uint8_t*)0x2B)
#define PIND_REG  (*(volatile uint8_t*)0x29)

#define DDRB_REG  (*(volatile uint8_t*)0x24)
#define PORTB_REG (*(volatile uint8_t*)0x25)

#define TCCR0A_REG (*(volatile uint8_t*)0x44)
#define TCCR0B_REG (*(volatile uint8_t*)0x45)
#define OCR0B_REG  (*(volatile uint8_t*)0x47)

#define TCCR2A_REG (*(volatile uint8_t*)0xB0)
#define TCCR2B_REG (*(volatile uint8_t*)0xB1)
#define OCR2B_REG  (*(volatile uint8_t*)0xB4)

#define MR1_BIT 0
#define MR2_BIT 1
#define ML1_BIT 2
#define ML2_BIT 3
#define ENR_BIT 3
#define ENL_BIT 5
#define SR_BIT  6
#define SL_BIT  7

#define LED_BIT 5

#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 1)

void analogWriteOC0B(uint8_t val) {
    OCR0B_REG = val;
}

void analogWriteOC2B(uint8_t val) {
    OCR2B_REG = val;
}

void setup(void) {
    SET_BIT(DDRD_REG, MR1_BIT);
    SET_BIT(DDRD_REG, MR2_BIT);
    SET_BIT(DDRD_REG, ML1_BIT);
    SET_BIT(DDRD_REG, ML2_BIT);

    CLEAR_BIT(DDRD_REG, SR_BIT);
    CLEAR_BIT(DDRD_REG, SL_BIT);

    SET_BIT(DDRB_REG, LED_BIT);

    TCCR0A_REG = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B_REG = (1 << CS01);

    TCCR2A_REG = (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
    TCCR2B_REG = (1 << CS21);
}

void forward(uint8_t speed) {
    SET_BIT(PORTD_REG, MR1_BIT);
    CLEAR_BIT(PORTD_REG, MR2_BIT);
    SET_BIT(PORTD_REG, ML1_BIT);
    CLEAR_BIT(PORTD_REG, ML2_BIT);
    analogWriteOC2B(speed);
    analogWriteOC0B(speed);
}

void backward(uint8_t speed) {
    CLEAR_BIT(PORTD_REG, MR1_BIT);
    SET_BIT(PORTD_REG, MR2_BIT);
    CLEAR_BIT(PORTD_REG, ML1_BIT);
    SET_BIT(PORTD_REG, ML2_BIT);
    analogWriteOC2B(speed);
    analogWriteOC0B(speed);
}

void right(uint8_t speed, uint16_t delay_ms) {
    CLEAR_BIT(PORTD_REG, MR1_BIT);
    SET_BIT(PORTD_REG, MR2_BIT);
    SET_BIT(PORTD_REG, ML1_BIT);
    CLEAR_BIT(PORTD_REG, ML2_BIT);
    analogWriteOC2B(speed);
    analogWriteOC0B(speed);
    _delay_ms(delay_ms);
}

void left(uint8_t speed, uint16_t delay_ms) {
    SET_BIT(PORTD_REG, MR1_BIT);
    CLEAR_BIT(PORTD_REG, MR2_BIT);
    CLEAR_BIT(PORTD_REG, ML1_BIT);
    SET_BIT(PORTD_REG, ML2_BIT);
    analogWriteOC2B(speed);
    analogWriteOC0B(speed);
    _delay_ms(delay_ms);
}

void stopRobot(void) {
    analogWriteOC2B(0);
    analogWriteOC0B(0);
    SET_BIT(PORTB_REG, LED_BIT);
}

int main(void) {
    setup();
    _delay_ms(5000);

    uint8_t vspeed = 100;
    uint8_t tspeed = 255;
    uint16_t tdelay = 20;

    while (1) {
        uint8_t svr = READ_BIT(PIND_REG, SR_BIT);
        uint8_t svl = READ_BIT(PIND_REG, SL_BIT);

        if (svl == 0 && svr == 0) {
            forward(vspeed);
        } else if (svl == 1 && svr == 0) {
            left(tspeed, tdelay);
        } else if (svl == 0 && svr == 1) {
            right(tspeed, tdelay);
        } else if (svl == 1 && svr == 1) {
            stopRobot();
        }
    }
    return 0;
}
