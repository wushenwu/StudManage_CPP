#ifndef CONST_H_55AE0822_0214_4fdd_A198_34A03CDD8B64

#define CONST_H_55AE0822_0214_4fdd_A198_34A03CDD8B64

//
const	int MAX_INPUT = 128;

//which table: g_add_table, or g_del_table
const   int ADD     = 1;
const	int DEL	    = 2;
const   int QUERY   = 3;
const   int MODIFY  = 4;

//error process
const   int SUCCESS     = 0x00;
const   int NOT_EXIST   = 0x01;
const   int NO_SPACE    = 0x02;
const   int FAILED      = 0x03;

//地址类型（相对，绝对）
const   int RELATIVE    = 0x00;
const   int ABSOLUTE    = 0x01;

//for memory space statistics
const   int SPLIT   = 8;

#endif

