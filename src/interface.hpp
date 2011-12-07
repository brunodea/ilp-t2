#pragma once

#include "StackFrame.hpp"
#include <cstdio>
#include <iostream>
#include <string>
/*
      +-----------------+-----+ parameters
0000h | short eggs      |0002h|
0002h | --- padding --- |0002h|
0004h | int spam        |0004h|
0008h | int arg         |0004h|
      +-----------------+-----+
000Ch | elo estatico    |0004h|
0010h | end. retorno    |0004h|
0014h | elo dinamico    |0004h|
0024h | contexto salvo  |0010h|
0034h | temporarios     |0010h|
      +-----------------+-----+ variaveis
0044h | int var1        |0004h|
0048h | int* ptr1       |0004h|
004Ch +-----------------+-----+
*/

void printTable(StackFrame sFrame);