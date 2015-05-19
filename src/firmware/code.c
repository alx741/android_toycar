/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <pic16f628a.h>

/*****************************************************************************
                            PIC16F628A CONFIGURATION
         '#pragma config' directive not supported for pic14  SDCC port
*****************************************************************************/

// Use internal oscillator, I/O functions on all the pins
// Watch dog timer disabled
// Power timer disabled
// MCLR pin enabled
// Brown out detection disabled
// Low level programming disabled
// Memory protection disabled
static __code char __at(_CONFIG) CONFIG = _INTOSC_OSC_NOCLKOUT & _WDT_OFF &
                                   _PWRTE_OFF & _MCLRE_ON & _BOREN_OFF &
                                   _LVP_OFF & DATA_CP_OFF & _CP_OFF;

/*****************************************************************************
*****************************************************************************/



/*
 * Init and configure uart hardware
 */
void uart_init(void)
{
	// TRISB 1,2 set as inputs for USART USE
	TRISB1 = 1;
	TRISB2 = 1;

	// BRGH - High Baud Rate Select Bit
    // TXSTAbits.BRGH
	BRGH = 1; // high speed
	// SPBRG - Baud Rate Generator Register
	SPBRG = 25; // 4MHz => 9600 baud (BRGH = 1)

	// SPEN - Serial Port Enable Bit
    // RCSTAbits.SPEN
	SPEN = 1; // Enable USART

	// TXIE - USART Transmit Interupt Enable Bit
    // PIE1bits.TXIE
	TXIE = 0; // Transmission interrupt disabled

	// TX9 - 9-bit Transmit Enable Bit
    // TXSTAbits.TX9
	TX9 = 0; // 8-bit transmission

	// TXEN - Transmit Enable Bit
    // TXSTAbists.TXEN
	TXEN = 1; // Transmission enabled

	// SYNC - USART Mode select Bit
    // TXSTAbits.SYNC
	SYNC = 0; // asynchrone

	// RCIE - USART Receive Interupt Enable Bit
    // PIE1bits.RCIE
	RCIE = 0; // No receive interrupt

	// RX9 - 9-bit Receive Enable Bit
    // RCSTAbits.RX9
	RX9 = 0; // 8-bit reception

	// CREN - Continuous Receive Enable Bit
    // RCSTAbits.CREN
	CREN = 1; // Continuos receive
}



// Blocking function
void putchar(char c)
{
    // Wait until transmission buffer is clear
    // PIR1bits.TXIF
    while( ! TXIF );

    // Write transmission buffer
    TXREG = c;
}


// Blocking function
int getchar(char* c)
{
    // RCSTAbits.OEERR
	if (OERR)
	{
		//IN CASE OF ERROR RESET CREN
        // RCSTAbits.CREN
        CREN=0;
		CREN=1;
	}

    // Wait reception interrupt
    // PIR1bits.RCIF
	while( ! RCIF );

	*c = RCREG;
	return 1;
}



void wait()
{
    // Variable parameters
    unsigned int delay = 5000;
	unsigned int a = 0;
	unsigned int i=0;
	for(i=0;i<delay;i++)
	{
		while(a < 10)
		{
			a = a+1;
		}
		a=0;
	}
}



void main(void)
{
	uart_init();
}
