#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "list.c"
#include "network.c"

//array to print
static volatile uint16_t pole [320][480];

//unit icon
static uint32_t ikona [16][16] = {1,2,3,4,5,6,7,8,9,0x008000,0x008000,0x008000,13,14,15,16,
								  1,2,3,4,5,6,7,8,0x008000,0x008000,0x008000,12,13,14,15,16,
								  1,2,3,4,0xffa500,0xffa500,7,8,0x008000,0x008000,11,0xffa500,0xffa500,14,15,16,
								  1,2,3,0xffa500,0xb8860b,0xb8860b,0xffa500,0xffa500,0x008000,0xffa500,0xffa500,0xb8860b,0xb8860b,0xffa500,15,16,
								  1,2,0xffa500,0xb8860b,0xb8860b,0xbc8f8f,0xbc8f8f,0xb8860b,0xffa500,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,16,
								  1,0xffa500,0xb8860b,0xb8860b,0xbc8f8f,0xbc8f8f,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,0xffa500,0xb8860b3,0xbc8f8f,0xbc8f8f,0xbc8f8f,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,0xffa500,0xb8860b,0xbc8f8f,0xbc8f8f,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,0xffa500,0xb8860b,0xbc8f8f,0xbc8f8f,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,0xffa500,0xb8860b,0xbc8f8f,0xbc8f8f,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,0xffa500,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,
								  1,2,0xffa500,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,16,
								  1,2,3,0xffa500,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xb8860b,0xffa500,15,16,
								  1,2,3,4,0xffa500,0xffa500,0xffa500,0xffa500,0xffa500,0xffa500,0xffa500,0xffa500,0xffa500,14,15,16,
								  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
								  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
//colour palete
static uint32_t colours[] = {0x000000,0x000080,0x0000FF,0x008000,0x008080,0x008000,0x00FFFF,0x800000
							,0x800080,0x808000,0x808080,0xc0c0c0,0xff0000,0xff00ff,0xffff00,0xffffff};

//draws character to target coordinates
void drawCharacter(char character,int x, int y){	
	for(int i = 0; i < 16; i++){		
		font_bits_t row = font_rom8x16.bits[character * 16 + i];		
		for (int j = 0; j < 8; j++){	
			if (row & (1 << (15 - j)))		
			 pole[y+i][x+j]= 0xFFFF;
		}
			
	}

}
//draws word to target coordinates
void drawWord(char* text,int x, int y){
		for(int i = 0; text[i] != 0;i++){
				drawCharacter(text[i],x+8*i,y);
		}
}
//draws icon target coordinates
void drawIcon(int y,Unit* selUnit){
	if(selUnit->icon != NULL){
		for(int i = 0; i < 16; i++){
			for(int j= 0; j < 16; j++){
				pole[y+i][180+j] = selUnit->icon[j][i];
			}
		}
	}
}
//draws list view
void drawList(UnitList * list, int length,int selected){
	for(int i = 0; i < length; i++){
		if(i == selected){
			drawCharacter('>',35,20+18*i);			
		}
		drawWord(list->units[i]->name,50,20 + 18*i);
		drawIcon(20+18*i,list->units[i]);
	}
}
//decreases selected value
int selDown(int selected,int max_value){
	return ((selected - 1) < 0 ? max_value : (selected - 1));
}

//increases selected value
int selUp(int selected,int max_value){
	return ((selected + 1) > max_value ? 0 : (selected + 1));
}

//draws colour of specified size to target coordinates
void drawColour(int x, int y, int width, int height,uint16_t colour){
		for(int i = 0; i < height;i++){
			for(int j = 0; j < width; j++){
				pole[y+i][x+j] = colour;
			}
		}
}

//gets 16bit colour from 32bit colour
uint16_t get_colour(uint32_t colour32 ){	
	uint8_t b8 = *((uint8_t *)&colour32);
	uint8_t g8 = *((uint8_t *)&colour32+1);
	uint8_t r8 = *((uint8_t *)&colour32+2);	
	uint16_t b5 = b8>>3;
	uint16_t g6 = g8 >>2;
	uint16_t r5 = r8>>3;	
	uint16_t colour =  (g6 << 5) + (r5<<11) + b5;
	return colour;
}
//draws unit menu
void drawBetterMenu(Unit selUnit){
	int x = 240;
	int y = 20;	
	drawWord("Name: ",x,y);
	drawWord(selUnit.name,x+48,y);
	drawWord("Walls: ",x,y+18);
	drawColour(x+56,y+18,30,16,get_colour(selUnit.wall));
	drawWord("Ceiling: ",x,y+36);
	drawColour(x+72,y+36,30,16,get_colour(selUnit.ceiling));
	drawWord("Turn off",x,y+54);
	drawWord("Select colour",x,y+72);
}

//draws unit menu without aditional options
void drawMenu(Unit selUnit){
	int x = 240;
	int y = 20;	
	drawWord("Name: ",x,y);
	drawWord(selUnit.name,x+48,y);
	drawWord("Walls: ",x,y+18);
	drawColour(x+56,y+18,30,16,get_colour(selUnit.wall));
	drawWord("Ceiling: ",x,y+36);
	drawColour(x+72,y+36,30,16,get_colour(selUnit.ceiling));
}

//draws colour palete
void drawPalette(){
	for(int i = 0; i < 16; i++){
		drawColour(240 + 40*(i%4), 120 +40*(i/4), 40, 40,get_colour(colours[i]));
	}
}

// allows to change colour using of selected unit using knobs
// selected - 0 - wall
//			- 1 - ceiling
void changeColour(unsigned char * mem_base,unsigned char *parlcd_mem_base, Unit* selUnit,int selected){
	
	while(1){
		uint32_t knobs;
		uint32_t g_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 25 & 1 ;
		
		knobs = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
				
		*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = knobs;		
		
		//draw colour read from knobs to memory
		drawColour(220 +56,120+18,50,50,get_colour(knobs));
		
		//draw to LCD
		parlcd_write_cmd(parlcd_mem_base, 0x2c);
		for(int i = 0; i < 320; i++){
			for(int j = 0; j < 480; j++){
				parlcd_write_data(parlcd_mem_base, pole[i][j]);
			}
		}		
		
		//checking for button press
		if(g_knob_pres != 0){
			while((*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 25 & 1) == 1){}
			if(selected == 0){//decide which colous is changed
				selUnit->wall = knobs;
			}else if (selected == 1){
				selUnit->ceiling = knobs;
			}
			break;
		}				
	}	
}

//draw edge for palete selection
void drawEdge(int selected){
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 40; j++){
			if((i < 2)|| (i > 37) || (j < 2) || (j > 37)){
				pole[(selected/4)*40+i + 120][(selected%4 )*40 +j +240] = 0xFFFF;
			}
		}
	}
}

//allows to change colour of selected unit, by chosing from palete
void colourPicker(unsigned char * mem_base,unsigned char *parlcd_mem_base, Unit* selUnit){
	int selected = 0;
	
	unsigned char last_rgb_knobs_value = 0;
	while(1){
		char rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
		uint32_t g_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 25 & 1 ;
		uint32_t r_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 26 & 1 ;
		
		//check for selection change
		if(last_rgb_knobs_value != rgb_knobs_value){
			if(rgb_knobs_value < last_rgb_knobs_value){
				selected = selDown(selected,15);
			}else{
				selected = selUp(selected,15);
			}
			drawPalette();	
			drawEdge(selected);
		}		
		
		// draw to lcd
		parlcd_write_cmd(parlcd_mem_base, 0x2c);
		for(int i = 0; i < 320; i++){
			for(int j = 0; j < 480; j++){
				parlcd_write_data(parlcd_mem_base, pole[i][j]);
			}
		}
		
		//check for button press
		if(g_knob_pres != 0){
			selUnit->wall = colours[selected];
			selUnit->ceiling = colours[selected];
			break;
		}
		if(r_knob_pres != 0){
			while((*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 26 & 1) == 1){}//wait until not pressed
			break;
		}		
		last_rgb_knobs_value = rgb_knobs_value;
	}
	//clear the palete
	for(int i = 0; i < 160;i++){
		for(int j = 0; j < 160; j++){
			pole[120 +i][240 + j] = 0x0;
		}
	}		
}

//opens menu for selected unit
void menuEdit(unsigned char * mem_base,unsigned char *parlcd_mem_base, Unit* selUnit){
	int selected = 0;
	char last_rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
	drawBetterMenu(*selUnit);
	
	while(1){
		char rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
		uint32_t r_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 26 & 1 ;
		uint32_t b_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 24 & 1 ;
		
		//chceck for select change
		if(rgb_knobs_value != last_rgb_knobs_value){
			for(int i = 0; i < 70; i++){//clear the arrow
				for(int j = 0; j < 8; j++){
					pole[36+i][224+j] = 0; 
				}
			}
			if(rgb_knobs_value < last_rgb_knobs_value){
				selected = selDown(selected,3);
			}else{
				selected = selUp(selected,3);
			}
		}
		last_rgb_knobs_value = rgb_knobs_value;
		drawCharacter('>',224,38 + 18*selected);
		
		parlcd_write_cmd(parlcd_mem_base, 0x2c);
		for(int i = 0; i < 320; i++){
			for(int j = 0; j < 480; j++){
				parlcd_write_data(parlcd_mem_base, pole[i][j]);
			}
		}
		//check for button press
		if(b_knob_pres != 0){
			if(selected == 2){//turn off lights
				selUnit->wall = 0x0;
				selUnit->ceiling = 0x0;			
			}else if(selected == 3){//choose from palete
				colourPicker(mem_base,parlcd_mem_base,selUnit);	
			}else{//choose own colour
				while((*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 24 & 1) == 1){}
				changeColour(mem_base,parlcd_mem_base,selUnit,selected);
				}
			//refresh menu and set arrow correctly
			drawBetterMenu(*selUnit);
			last_rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
		}
		
		if(r_knob_pres != 0){//exit
			while((*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 26 & 1) == 1){}
			break;
		}
	}	
}

//read icon from static variable to dynamic memory
uint16_t** createIcon(){
	//allocate memory
	uint16_t ** icon = malloc(sizeof(uint16_t*)*16);
	for(int i = 0; i < 16; i++){
		icon[i] = malloc(sizeof(uint16_t) * 16);
	}
	//copy data
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			icon[j][i] = get_colour(ikona[i][j]);
		}
	}
	return icon;
	
}

int main(int argc, char *argv[])
{
	
	//setup lcd
	unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
		exit(1);
	parlcd_hx8357_init(parlcd_mem_base);

	//setup knobs
	unsigned char *mem_base;
	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (mem_base == NULL)
		exit(1);    
  
	//create units
	UnitList * list = initUnitList();
	addUnitItem(list,"Martin",0x0FFF5,0x09,createIcon());
	addUnitItem(list,"Tomas",0x01F5,0x0,NULL);
	addUnitItem(list,"Kamila",0x05,0x10,NULL);
	int selected = 0;
	
	//initialize thread for broadcasting
	pthread_t network;	
	pthread_create(&network,NULL,&startNetwork,list);

	unsigned char last_rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
	while(1){
		char rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
		uint32_t b_knob_pres = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 24 & 1 ;
		
		//check for selection change
		if(rgb_knobs_value != last_rgb_knobs_value){
			if(rgb_knobs_value < last_rgb_knobs_value){
				selected = selDown(selected,list->size-1);
			}else{
				selected = selUp(selected,list->size-1);
			}
		}
		last_rgb_knobs_value = rgb_knobs_value;
		
		//check for button press
		if(b_knob_pres != 0){//draw menu for selected unit
			while((*(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o) >> 24 & 1 )== 1){}//wait for unpress
			menuEdit(mem_base,parlcd_mem_base,list->units[selected]);
			last_rgb_knobs_value = *(volatile unsigned char*)(mem_base + SPILED_REG_KNOBS_8BIT_o)/4;
		}
		//clear memory
		for(int i = 0; i < 320; i++){
			for(int j = 0; j < 480; j++){
				pole[i][j] = 0; 
			}
		}
		//draw list and basic view for selected unit
		drawList(list,list->size,selected);
		drawMenu(*list->units[selected]);
		
		//draw to LCD
		parlcd_write_cmd(parlcd_mem_base, 0x2c);
		for(int i = 0; i < 320; i++){
			for(int j = 0; j < 480; j++){
				parlcd_write_data(parlcd_mem_base, pole[i][j]);
			}
		}		
	}
  return 0;
}
