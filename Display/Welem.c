//  ppro
//	Welem.c
//  Created by David del Val on 30/09/2019
//
//

#include "Welem.h"
struct _Welem {
	Wtype t;
	void* dat;
	int wid;
};
Welem* we_ini(Wtype t, void* data){
	Welem* w= calloc(1,sizeof(Welem));
	if(!w)return NULL;
	w->dat=data;
	return w;
}
void we_free(Welem* w){
	/*
	 Free according to the type stored
	 */
}
Canvas* we_render(Welem*w){
	
	return NULL;
}
Welem* we_copy(Welem* d){
	return NULL;
}

