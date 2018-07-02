/*
 * sysclk.c
 *
 * Created: 7/19/2015 2:34:40 PM
 *  Author: Daniel
 */ 

#include "sysclk.h"

extern void InitSysClk(void)
{
	/* PLL Enable (Page 78)
	To enable the PLL, the following procedure must be followed:
	1. Enable reference clock source.
	2. Set the multiplication factor and select the clock reference for the PLL.
	3. Wait until the clock reference source is stable.
	4. Enable the PLL.
	*/
	
	//(App note AVR1003) Unlock clock changes (CCP), Set to 32MHz, DFLL to internal 32KHz ref and enable		
	//Website assistance: 
	//http://morf.lv/modules.php?name=tutorials&lasit=28
	//http://www.avrfreaks.net/forum/setting-pll-and-external-clock
	
	//CPU Clock, 32MHz OSC Source, CPU Clock at 32MHz with PLL at x4 - NOT USED BECAUSE OF USB
	#pragma region	
	/*
	//CLKSys = 32MHz OSC Settings
	OSC.CTRL |= OSC_RC32MEN_bm;
		
	//wait for osc to stabilize
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
						
	OSC.DFLLCTRL |= OSC_RC32MCREF0_bm;
	
	OSC.DFLLCTRL |= DFLL_ENABLE_bm;
	
	//PLLSRC to 32MHz, Unlock PLL changes, Scale to x1
	OSC.PLLCTRL = (OSC.PLLCTRL & ~OSC_PLLSRC_gm) | OSC_PLLSRC_RC32M_gc;
	
	//32MHz OSC / 4 = 8MHz SYSCLK. 8MHz SYSCLK * 4 (PLL MULT) = 32MHz SYSCLK
	OSC.PLLCTRL = (OSC.PLLCTRL & ~OSC_PLLFAC_gm) | (OSC_PLLFAC_gm & 0x04);
		
	//enable PLL
	OSC.CTRL |= OSC_PLLEN_bm;
	
	while(!(OSC.STATUS & OSC_PLLRDY_bm));
	
	//Set clock to PLL
	CCP = CCP_IOREG_gc;
	
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
	*/
	#pragma endregion
	
	//CPU Clock, 2MHz OSC Source, CPU Clock at 31MHz with PLL to x31
	#pragma region
	//CLKSys = 2MHz OSC Settings
	OSC.CTRL |= OSC_RC2MEN_bm;
	
	//wait for osc to stabilize
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));

	//Create a temporary variable to store the CLK config. This is needed because performing the bit operations takes longer than four clock cycles.		
	//OSC Freq (2MHz), CLKSys = 500kHz (4 times slower than the OSC input). No division.
	uint8_t temp = (CLK.PSCTRL & ~CLK_PSADIV_gm) | CLK_PSADIV0_bm;
	
	CCP = CCP_IOREG_gc;

	CLK.PSCTRL = temp;
				
	OSC.DFLLCTRL |= DFLL_ENABLE_bm;
	
	//PLLSRC to 2MHz, CLKSys = 1MHz
	OSC.PLLCTRL = (OSC.PLLCTRL & ~OSC_PLLSRC_gm) | OSC_PLLSRC_RC2M_gc;
	
	//1MHz SYSCLK * 1 (PLL MULT) = SYSCLK
	OSC.PLLCTRL = (OSC.PLLCTRL & ~OSC_PLLFAC_gm) | (OSC_PLLFAC_gm & 0x1F);
	
	//enable PLL
	OSC.CTRL |= OSC_PLLEN_bm;
	
	while(!(OSC.STATUS & OSC_PLLRDY_bm));
	
	//Set clock to PLL
	CCP = CCP_IOREG_gc;
	
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
	#pragma endregion

	//USB Clock, 32MHz OSC Source, 32MHz OSC locked to SOF
	#pragma region
	//Enable = 32MHz OSC
	OSC.CTRL |= OSC_RC32MEN_bm;
		
	//wait for osc to stabilize
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));

	//Set DFLL to USB SOF
	OSC.DFLLCTRL = OSC_RC32MCREF_USBSOF_gc;

	//Put dummy value into DFLL cal registers: 0x2340 found in sysclk.c of ASF code.
	DFLLRC32M.CALA = 0x40;
	DFLLRC32M.CALB = 0x23;

	//Enable the DFLL
	OSC.DFLLCTRL = DFLL_ENABLE_bm;
	
	//set USB clock source to 32MHz OSC
	CLK.USBCTRL = CLK_USBSRC_RC32M_gc;
	#pragma endregion
}