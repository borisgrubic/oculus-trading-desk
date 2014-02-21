#ifndef MD_ARCHIVE_DATA_READER_H
#define MD_ARCHIVE_DATA_READER_H

#include <hdf5.h>
#include <string>

#include "DataTypes/MarketData.h"

using namespace std;

class MDArchiveDataReader {
    hid_t mFile;
    hid_t mDataset;
    hid_t mSpace;
    hid_t mMemoryType;
    string mFileName;
    RetailState* mRetailStates;
    LastDone* mLastDones;
    ExchangeState* mExchangeStates;

public:
    MDArchiveDataReader(string fileName);
    ~MDArchiveDataReader();

    int getRetailStateCnt();
    int getLastDoneCnt();
    int getExchangeStateCnt();

    RetailState* getRetailStates();

    LastDone* getLastDones();

    ExchangeState* getExchangeStates();
};

#endif