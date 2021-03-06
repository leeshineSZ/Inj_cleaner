#include "../inc/encoders.h"

#define __check_bit(N,I) GPIO_##I##_##N
#define _check_bit(N,I) __check_bit(N,I)
#define ENC1_CHECK_1 _check_bit(ENC1_PIN1, INP_REG)
#define ENC1_CHECK_2 _check_bit(ENC1_PIN2, INP_REG)
#define ENC2_CHECK_1 _check_bit(ENC2_PIN1, INP_REG)
#define ENC2_CHECK_2 _check_bit(ENC2_PIN2, INP_REG)

#define __moder_bit(N) GPIO_MODER_MODER##N
#define _moder_bit(N) __moder_bit(N)
#define ENC1_MODER_1 _moder_bit(ENC1_PIN1)
#define ENC1_MODER_2 _moder_bit(ENC1_PIN2)
#define ENC2_MODER_1 _moder_bit(ENC2_PIN1)
#define ENC2_MODER_2 _moder_bit(ENC2_PIN2)

#define __pupdr_bit0(N) GPIO_PUPDR_PUPDR##N##_0
#define _pupdr_bit0(N) __pupdr_bit0(N)
#define ENC1_PUPDR_1_0 _pupdr_bit0(ENC1_PIN1)
#define ENC1_PUPDR_2_0 _pupdr_bit0(ENC1_PIN2)
#define ENC2_PUPDR_1_0 _pupdr_bit0(ENC2_PIN1)
#define ENC2_PUPDR_2_0 _pupdr_bit0(ENC2_PIN2)

#define __pupdr_bit1(N) GPIO_PUPDR_PUPDR##N##_1
#define _pupdr_bit1(N) __pupdr_bit1(N)
#define ENC1_PUPDR_1_1 _pupdr_bit1(ENC1_PIN1)
#define ENC1_PUPDR_2_1 _pupdr_bit1(ENC1_PIN2)
#define ENC2_PUPDR_1_1 _pupdr_bit1(ENC2_PIN1)
#define ENC2_PUPDR_2_1 _pupdr_bit1(ENC2_PIN2)

enum{
    Stable = 0,
    Left,
    Right
};

void EncoderInit(void){
    PORT->MODER &= ~(ENC1_MODER_1 | ENC1_MODER_2);
    PORT->PUPDR |= (ENC1_PUPDR_1_0 | ENC1_PUPDR_2_0);
    PORT->PUPDR &= ~(ENC1_PUPDR_1_1 | ENC1_PUPDR_2_1);

    PORT->MODER &= ~(ENC2_MODER_1 | ENC2_MODER_2);
    PORT->PUPDR |= (ENC2_PUPDR_1_0 | ENC2_PUPDR_2_0);
    PORT->PUPDR &= ~(ENC2_PUPDR_1_1 | ENC2_PUPDR_2_1);
    /*
    GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); //Input
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);// Pull up
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1);

    GPIOC->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11); //Input
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0);// Pull up
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1);
    */
}

void EncoderScan(Encoder *enc_struct)
{
    uint16_t val, shift;

    if(enc_struct->pin1 > enc_struct->pin2)shift = enc_struct->pin2;
    else shift = enc_struct->pin1;

    val = ((PORT->INP_REG & (1<<enc_struct->pin1 | 1<<enc_struct->pin2))>>shift);

      switch(val)
    {
        case 2:
            if(enc_struct->old_state == 0) enc_struct->direction = Right;
            if(enc_struct->old_state == 3) enc_struct->old_state = 2;
            break;

        case 0:
            enc_struct->old_state = 0;
            break;

        case 1:
            if(enc_struct->old_state == 0) enc_struct->direction = Left;
            if(enc_struct->old_state == 3) enc_struct->old_state = 1;
            break;

        case 3:
            if(enc_struct->direction == Left){
                if(enc_struct->value >= enc_struct->step + enc_struct->min)enc_struct->value -= enc_struct->step;
            }
            if(enc_struct->direction == Right){
                if(enc_struct->value <= enc_struct->max - enc_struct->step)enc_struct->value += enc_struct->step;
            }
            enc_struct->direction = Stable;
            enc_struct->old_state = 3;
            break;
    }
}


