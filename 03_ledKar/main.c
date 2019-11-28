/**
 * Второй пример.
 * Включает светодиоды во время нажатия на различные клавиши.
 *
 */

// Device Startup
#include "MDR32F9Qx_config.h"
// Device Header
#include "MDR32Fx.h"
// стандартные библиотеки для чтения из консоли и записи в неё.
#include "stdlib.h"
#include "stdio.h"


// Определение констант для доступа к портам для работы с кнопками
#define RIGHT (1<<6) // PB6
#define LEFT (1<<3)  // PE3
#define SEL (1<<2)   // PC2
// Определение констант для доступа к выводам порта С,
// к которым подключены светодиоды
#define VD1 0x002    // PC1
#define VD0 0x001    // PC0

// Объявление функций
void frq_init_pll(void);
void joystick_init(void);
void led_on(unsigned short);
void led_off(unsigned short);
int Gorit;
int B=0;
int main(void)
{
  frq_init_pll();
  joystick_init();

	Gorit=0;
 while (1)
  {
    // Нажатие на RIGHT
    if ((!(MDR_PORTB->RXTX & RIGHT) || !(MDR_PORTE->RXTX & LEFT) || !(MDR_PORTC->RXTX & SEL)) && B==0)
        {
    		B=1;
    				if	(Gorit==0)
    					{
    					Gorit=1;
    					led_on(0);
    					led_off(1);
    					}
    				else
    					{
    					Gorit=0;
    					led_on(1);
    					led_off(0);
    					}
        }

    if	((MDR_PORTB->RXTX & RIGHT) && (MDR_PORTE->RXTX & LEFT) && (MDR_PORTC->RXTX & SEL))
	{B=0;}

}
}

/**
 * Функция включения светодиода
 *
 * @param num Индекс светодиода для включения
 */
void led_on(unsigned short num)
{
  if (num == 0)
  MDR_PORTC->RXTX |= VD0;
  else
  MDR_PORTC->RXTX |= VD1;
}
/**
 * Функция выключения светодиода
 *
 * @param num Индекс светодиода для отключения
 */
void led_off(unsigned short num)
{
  if (num == 0)
  MDR_PORTC->RXTX &= ~VD0;
  else
  MDR_PORTC->RXTX &= ~VD1;
}

/**
 * Функция инициализации тактовой частоты процессора
 */
void frq_init_pll(void)
{
  // включение внешнего генератора тактовых импульсов
  MDR_RST_CLK->HS_CONTROL = 0x1;
  // ожидание запуска внешнего генератора
  while (MDR_RST_CLK->CLOCK_STATUS == 0x00)
    __NOP();
  // коэффициент умножения базовой частоты 8 (частоста процессора 80МГгц)
  MDR_RST_CLK->PLL_CONTROL |= 0x700;
  // Включение коэффициента умножения для процессорного ядра
  MDR_RST_CLK->PLL_CONTROL |= 0x04;
  // ожидание применения коэффициента умножения ядра
  while (!(MDR_RST_CLK->CLOCK_STATUS & 0x02))
    __NOP();
  // Преключение на тактирование от внешнего генератора
  // с выбранным коэффициентом умножения
  MDR_RST_CLK->CPU_CLOCK = 0x106;
  // системная функция, обновляющая тактовую частоту
  SystemCoreClockUpdate();
}

/**
 * Функция инициализации портов для работы с кнопками
 */
void joystick_init(void)
{
  // включение тактирования нужных нам портов
  MDR_RST_CLK->PER_CLOCK |= (0x0B << 22);
  /* Порт C */
  // SEL (PC2)
  MDR_PORTC->OE &= ~SEL;
  MDR_PORTC->OE |= 0x003;
  MDR_PORTC->ANALOG |= (SEL | 0x003);
  MDR_PORTC->PWR &= ~(((2 + 1) << SEL) | 0x00A);
  MDR_PORTC->PWR |= ((2 << SEL) | 0x005);
  MDR_PORTC->RXTX = 0;
  /* Порт B */
  // RIGHT (PB6)
  MDR_PORTB->OE &= ~RIGHT;
  MDR_PORTB->ANALOG |= RIGHT;
  MDR_PORTB->PWR &= ~((6 + 1) << RIGHT);
  MDR_PORTB->PWR |= (6 << RIGHT);
  MDR_PORTB->RXTX = 0;
  /* Порт E */
  // LEFT (PE3)
  MDR_PORTE->OE &= ~LEFT;
  MDR_PORTE->ANALOG |= LEFT;
  MDR_PORTB->PWR &= ~((3 + 1) << LEFT);
  MDR_PORTB->PWR |= (3 << LEFT);
  MDR_PORTE->RXTX = 0;
}

/*
 * Реализация функции exit(). Требуется для корректной сборки программы,
 * использующей newlibс
 * Этот комментарий не является частью автоматически генерируемой документации,
 * так как не описывает значимых для программы функций.
 */
void exit(int code)
{
}
