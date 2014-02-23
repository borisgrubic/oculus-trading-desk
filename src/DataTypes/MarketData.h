#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <stdlib.h>
#include <hdf5.h>
#include <string>

#include "DataType.h"

using namespace std;

struct RetailState {
	int64_t received;

	double bid_1_price;
	int64_t bid_1_volume;
	double bid_2_price;
	int64_t bid_2_volume;
	double bid_3_price;
	int64_t bid_3_volume;
	double bid_4_price;
	int64_t bid_4_volume;
	double bid_5_price;
	int64_t bid_5_volume;

	double ask_1_price;
	int64_t ask_1_volume;
	double ask_2_price;
	int64_t ask_2_volume;
	double ask_3_price;
	int64_t ask_3_volume;
	double ask_4_price;
	int64_t ask_4_volume;
	double ask_5_price;
	int64_t ask_5_volume;
};

struct LastDone {
	int64_t received;

	double price;
	int64_t volume;
	int64_t aggressorSide;
};

struct ExchangeState {
	int64_t received;
	int64_t status;
};

#endif