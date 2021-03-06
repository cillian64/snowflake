#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/iwdg.h>

#define LED1 GPIO0
#define LED2 GPIO1
#define LED3 GPIO2
#define LED4 GPIO3
#define LED5 GPIO4
#define LED6 GPIO6
#define LED7 GPIO7
#define LED8 GPIO1

#define LED1_PORT GPIOA
#define LED2_PORT GPIOA
#define LED3_PORT GPIOA
#define LED4_PORT GPIOA
#define LED5_PORT GPIOA
#define LED6_PORT GPIOA
#define LED7_PORT GPIOA
#define LED8_PORT GPIOB

static const uint16_t led_pins[] = {
    0,
    LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8
};

static const uint32_t led_ports[] = {
    0,
    LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT,
    LED5_PORT, LED6_PORT, LED7_PORT, LED8_PORT
};

#define TURNON(led) gpio_set(led_ports[led], led_pins[led])
#define TURNOFF(led) gpio_clear(led_ports[led], led_pins[led])

//#define RTC_BKP0R MMIO32(RTC_BASE + 0x50)
#define RTC_BKP0R MMIO32(0x40002860)

#define SCB_SCR MMIO32(SCB_BASE + 0x10)
#define SCB_SCR_SLEEPDEEP (1<<2)

static void gpio_setup(void)
{
    int led;

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    for(led=1; led<=8; led++) {
        gpio_clear(led_ports[led], led_pins[led]);
        gpio_mode_setup(
            led_ports[led], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, led_pins[led]);
        gpio_set_output_options(
            led_ports[led], GPIO_OTYPE_PP, GPIO_OSPEED_LOW, led_pins[led]);
    }
}

void tim14_isr(void)
{
    // Clear interrupt flag
    TIM_SR(TIM14) &= ~TIM_SR_UIF;
}

static void shortwait(void)
{
    // About 0.25ms
    timer_set_period(TIM14, 1);
    timer_enable_counter(TIM14);
    SCB_SCR &= ~SCB_SCR_SLEEPDEEP;
    __asm__("wfi");
    timer_disable_counter(TIM14);
}

static void standby(void)
{
    SCB_SCR |= SCB_SCR_SLEEPDEEP;
    pwr_set_standby_mode();
    __asm__("wfi");
}

int main(void)
{
    gpio_setup();

    // Setup TIM14 for sleep wakeups
    rcc_periph_clock_enable(RCC_TIM14);
    timer_reset(TIM2);
    timer_set_mode(TIM14, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    // PCLK is running at 8MHz from HSI
    // Prescaler: divide by 2000 for timer at ~4kHz
    timer_set_prescaler(TIM14, 1999u);
    nvic_enable_irq(NVIC_TIM14_IRQ);
    timer_enable_irq(TIM14, TIM_DIER_UIE);
    timer_enable_update_event(TIM14);
    timer_disable_counter(TIM14);

    rcc_periph_clock_enable(RCC_PWR);
    rcc_periph_clock_enable(RCC_RTC);
    pwr_disable_backup_domain_write_protect();

    iwdg_reset();

    if(RTC_BKP0R > 7)
        RTC_BKP0R = 0;

    switch(RTC_BKP0R)
    {
        case 0:
            TURNON(1);
            shortwait();
            TURNOFF(1);
            break;
        case 1:
            TURNON(4);
            shortwait();
            TURNOFF(4);
            break;
        case 2:
            TURNON(6);
            shortwait();
            TURNOFF(6);
            break;
        case 3:
            TURNON(2);
            shortwait();
            TURNOFF(2);
            break;
        case 4:
            TURNON(8);
            shortwait();
            TURNOFF(8);
            break;
        case 5:
            TURNON(5);
            shortwait();
            TURNOFF(5);
            break;
        case 6:
            TURNON(7);
            shortwait();
            TURNOFF(7);
            break;
        case 7:
            TURNON(3);
            shortwait();
            TURNOFF(3);
            break;
    }

    RTC_BKP0R += 1;
    if(RTC_BKP0R == 8)
        RTC_BKP0R = 0;

    iwdg_reset();
    iwdg_set_period_ms(100);
    iwdg_start();
    standby();

    return 0;
}
