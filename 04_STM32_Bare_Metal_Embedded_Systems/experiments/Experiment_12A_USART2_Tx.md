Project 04 – Bare-Metal GPIO Programming 

Experiment 12A: Transmitting Data over USART2 

1. Objective 

The objective of this experiment is to transmit data from the STM32 NUCLEO-F446RE microcontroller to a PC using asynchronous serial communication (UART). The USART2 peripheral will be configured directly through memory-mapped hardware registers without using STM32 HAL drivers. 

2. Implementation 

GPIOA pin PA2 is configured as USART2_TX. The transmitted data is received by the PC through the ST-LINK Virtual COM Port. The following configuration steps were performed: 

Enable the GPIOA clock: Set bit 0 of RCC_AHB1ENR to enable the GPIOA peripheral clock. 

Enable the USART2 clock: Set 17 of RCC_APB1ENR to enable the USART2 peripheral clock.  

Configure PA2 as an alternate-function pin: Set bits 5:4 of GPIOA_MODER to 10, selecting alternate-function mode. 

Select USART2_TX on PA2: Configure bits 11:8 of GPIOA_AFRL to 0111, selecting alternate function AF7. 

Configure oversampling: Clear bit 15 (OVER8) of USART2_CR1 to select oversampling by 16. 

Configure the baud rate: Set the desired baud rate to 115200 using the nominal USART2 peripheral clock of 16 MHz. 

The baud-rate divider is calculated using: 

[USARTDIV=\frac{f_{CK}}{16\times BaudRate}] 

[USARTDIV=\frac {16,000,000} {16\times115200} \approx8.6806] 

The mantissa is 8 and the rounded fractional field is 11. Therefore: 

USART2_BRR = (8u << 4) | 11u = 0x008B 

Enable USART2 transmission: Set the TE (bit 3) and UE (bit 13) fields in USART2_CR1 to enable the transmitter and USART peripheral. 

Implement nonblocking character transmission: Check bit 7 (TXE) of USART2_SR. If TXE = 1, write the character into USART2_DR and return true. Otherwise, return false without waiting. 

Initialize USART2 from main(): Call USART2_init() and attempt to transmit the character 'A' using USART2_tryTransmit(). 

Configure the PC serial terminal: Use PuTTY with the following communication parameters: 

COM port: COM7 

Baud rate: 115200 

Data bits: 8 

Stop bits: 1 

Parity: None 

Flow control: None 

Program:
//Enable GPIOA clock on the AHB1ENR register --> bit 0

static void enableUSART2Clock(void){
	RCC_APB1ENR |= (1u << 17);
}

//Configure PA2 as alternate output
static void setUSART2TxPin(void){
	//Clear bit 5:4 and set bit to 10, for alternate function mode
	GPIOA_MODER &= ~(3u << 4);
	GPIOA_MODER |= (2u << 4);
}

//Setting the PA2 for USART2_Tx
static void setUSART2_Tx(void){

	GPIOA_AFRL &= ~(15u << 8);//Clear bit 11:8
	GPIOA_AFRL |= (7u << 8);//Write 0111 into bit 11:8, select AF7
}

//Set USART2 Baud rate
static void setUSART2Baud(void){
	//APB1 = 16 MHz, 115200 Baud, oversampling by 16
	//USARTDIV = (fclk)/(oversampling * baud rate)
	//Get mantissa, multiply oversampling with fractional field and round it, get the hexadecimal number
	USART2_BRR = 0x008Bu;
	//USART2_BRR = (8u << 4) | 11u;
}

//Enable the USART2_CR1 register
static void enableUSART2Tx(void){
	//Set bit 3 and bit 13
	USART2_CR1 |= ((1u << 3) | (1u << 13));
}
//Transmitting a character
static bool USART2_tryTransmit(char character){

	//Check whether the transmission data register is ready
	if((USART2_SR & (1u << 7)) == 0u){
		return false;
	}
	// Load the character for transmission
	USART2_DR = (uint8_t) character;

	// Character accepted for transmission
	return true;

}

//USART2 initialization
static void USART2_init(void){
	enableUSART2Clock();
	// Disable USART2 before configuring its operating parameters
	USART2_CR1 &= ~(1u << 13);//disable
	setUSART2TxPin();
	setUSART2_Tx();
	//Select oversampling by 16
	USART2_CR1 &= ~ (1u << 15);
	setUSART2Baud();
	enableUSART2Tx();
}

3. Verification and Results 

The following statement was executed after USART2 initialization: 

bool accepted = USART2_tryTransmit('A'); 

The character 'A' was successfully transmitted
from USART2 to the PC through the ST-LINK
Virtual COM Port.

![USART2 transmission verified in PuTTY](C:\Users\zmmar\Desktop\Marsad-Embedded-Projects\04_STM32_Bare_Metal_Embedded_Systems\evidence\Experiment_12A_PuTTY.png)

This observation confirms successful end-to-end transmission from the STM32 USART2 peripheral to the PC for the tested character. 

 

4. Conclusion 

This experiment demonstrated successful asynchronous serial communication between the STM32 NUCLEO-F446RE microcontroller and a PC using direct register-level programming. 

The implementation required configuring peripheral clocks, GPIO alternate-function routing, USART baud-rate generation, transmitter enable controls, and the transmit data register. 

Developing the transmitter without HAL drivers provided practical experience with memory-mapped peripheral registers and the relationship between software configuration and physical hardware operation. 

The experiment also introduced nonblocking transmission through the TXE status flag. Future work will extend this implementation with a circular transmit buffer and integrate USART communication into the existing cooperative scheduler. 