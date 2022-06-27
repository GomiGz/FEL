#include <stdlib.h>
#include <stdint.h>

//structure representing one unit
typedef struct {
	char* name;
	uint32_t wall;
	uint32_t ceiling;
	uint16_t** icon;
}Unit;
//structure representing list of units
typedef struct{
	Unit** units;
	int size;
	int capacity;	
	}UnitList;


//initialization of the list
UnitList* initUnitList(){
	UnitList * list = malloc(sizeof(UnitList));
	list->units = malloc(sizeof(Unit*) * 3);
	list->size = 0;
	list ->capacity = 3;
	return list;	
}

/*
adding Unit to the list
list - list to be changed
n - name of the unit
w - wall colour
c - ceiling colour
ikona - unit icon
*/
void addUnitItem(UnitList* list,char * n, uint32_t w, uint32_t c, uint16_t** ikona){
	if(list->capacity == list->size){//resizeing
		list->units = realloc(list->units,sizeof(Unit*)*list->capacity * 2);
		list->capacity *= 2;
	}
	Unit* unit = malloc(sizeof(Unit));
	unit->name = n;
	unit->wall = w;
	unit->ceiling = c;
	unit->icon = ikona;	
	list->units[list->size] = unit;
	list->size++;
}

/*
deleting Unit from list
list - list to be changed
name - name of the unit to be removed
*/
void deleteUnit(UnitList* list, char* name){
	if(list->size != 0){
		Unit** newUnits;
		if(list->size *3 <= list->capacity){ //resizeing
			newUnits = malloc(sizeof(Unit*) * list->capacity/2);
			list->capacity /=2;
		}else{
			newUnits = malloc(sizeof(Unit*) * list->capacity);
		}
		int idx = 0;
		for(int i = 0; i < list->size;i++){//copying all but selected units to new array
			if(strcmp(name,list->units[i]->name) != 0){
				newUnits[idx] = list->units[i];
				idx++;
			}		
		}
		free(list->units);
		list->units = newUnits;
		list->size--;	
	}
}
