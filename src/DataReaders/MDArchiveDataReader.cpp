#include <stdlib.h>
#include "MDArchiveDataReader.h"

MDArchiveDataReader::MDArchiveDataReader(string fileName)
: mFileName(fileName),
  mRetailStates(NULL),
  mLastDones(NULL),
  mExchangeStates(NULL)
{
    mFile = H5Fopen (fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
}

MDArchiveDataReader::~MDArchiveDataReader()
{
    H5Fclose (mFile);
    if(mRetailStates != NULL)
        delete [] mRetailStates;

    if(mLastDones != NULL)
        delete [] mLastDones;

    if(mExchangeStates != NULL)
        delete [] mExchangeStates;
}

int MDArchiveDataReader::getRetailStateCnt()
{
    mDataset = H5Dopen (mFile, "RetailStates", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);
   
    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);

    H5Dclose (mDataset);
    H5Sclose (mSpace);
    return dims[0]; 
}

RetailState* MDArchiveDataReader::getRetailStates()
{ 
    if(mRetailStates != NULL)
        return mRetailStates;

    mDataset = H5Dopen (mFile, "RetailStates", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);
    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);

    mRetailStates = new RetailState[dims[0]];

    mMemoryType = H5Tcreate (H5T_COMPOUND, sizeof (RetailState));

    H5Tinsert (mMemoryType, "received",
                        HOFFSET (RetailState, received), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "bid_1_price", 
                        HOFFSET (RetailState, bid_1_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "bid_1_volume", 
                        HOFFSET (RetailState, bid_1_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "bid_2_price", 
                        HOFFSET (RetailState, bid_2_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "bid_2_volume", 
                        HOFFSET (RetailState, bid_2_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "bid_3_price", 
                        HOFFSET (RetailState, bid_3_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "bid_3_volume", 
                        HOFFSET (RetailState, bid_3_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "bid_4_price", 
                        HOFFSET (RetailState, bid_4_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "bid_4_volume", 
                        HOFFSET (RetailState, bid_4_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "bid_5_price", 
                        HOFFSET (RetailState, bid_5_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "bid_5_volume", 
                        HOFFSET (RetailState, bid_5_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "ask_1_price", 
                        HOFFSET (RetailState, ask_1_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "ask_1_volume", 
                        HOFFSET (RetailState, ask_1_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "ask_2_price", 
                        HOFFSET (RetailState, ask_2_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "ask_2_volume", 
                        HOFFSET (RetailState, ask_2_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "ask_3_price", 
                        HOFFSET (RetailState, ask_3_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "ask_3_volume", 
                        HOFFSET (RetailState, ask_3_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "ask_4_price", 
                        HOFFSET (RetailState, ask_4_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "ask_4_volume", 
                        HOFFSET (RetailState, ask_4_volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "ask_5_price", 
                        HOFFSET (RetailState, ask_5_price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "ask_5_volume", 
                        HOFFSET (RetailState, ask_5_volume), H5T_NATIVE_LONG);

    H5Dread (mDataset, mMemoryType, H5S_ALL, H5S_ALL, H5P_DEFAULT, mRetailStates);

    H5Dclose (mDataset);
    H5Sclose (mSpace);
    H5Tclose (mMemoryType);

    return mRetailStates;
}

int MDArchiveDataReader::getLastDoneCnt()
{ 
    mDataset = H5Dopen (mFile, "LastDones", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);

    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);

    H5Dclose (mDataset);
    H5Sclose (mSpace);
    return dims[0]; 
}

LastDone* MDArchiveDataReader::getLastDones()
{ 
    if(mLastDones != NULL)
        return mLastDones;
    mDataset = H5Dopen (mFile, "LastDones", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);
    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);

    mLastDones = new LastDone[dims[0]];

    mMemoryType = H5Tcreate (H5T_COMPOUND, sizeof (LastDone));

    H5Tinsert (mMemoryType, "received",
                        HOFFSET (LastDone, received), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "price", 
                        HOFFSET (LastDone, price), H5T_NATIVE_DOUBLE);
    H5Tinsert (mMemoryType, "volume", 
                        HOFFSET (LastDone, volume), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "aggressorSide", 
                        HOFFSET (LastDone, aggressorSide), H5T_NATIVE_LONG);

    H5Dread (mDataset, mMemoryType, H5S_ALL, H5S_ALL, H5P_DEFAULT, mLastDones);

    H5Dclose (mDataset);
    H5Sclose (mSpace);
    H5Tclose (mMemoryType);

    return mLastDones;
}

int MDArchiveDataReader::getExchangeStateCnt()
{
    mDataset = H5Dopen (mFile, "ExchangeStates", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);
    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);
    H5Dclose (mDataset);
    H5Sclose (mSpace);
    return dims[0]; 
}

ExchangeState* MDArchiveDataReader::getExchangeStates()
{ 
    if(mExchangeStates != NULL)
        return mExchangeStates;
    mDataset = H5Dopen (mFile, "ExchangeStates", H5P_DEFAULT);
    mSpace = H5Dget_space (mDataset);
    hsize_t dims[1];
    H5Sget_simple_extent_dims (mSpace, dims, NULL);

    mExchangeStates = new ExchangeState[dims[0]];

    mMemoryType = H5Tcreate (H5T_COMPOUND, sizeof (ExchangeState));

    H5Tinsert (mMemoryType, "received",
                        HOFFSET (ExchangeState, received), H5T_NATIVE_LONG);
    H5Tinsert (mMemoryType, "status", 
                        HOFFSET (ExchangeState, status), H5T_NATIVE_LONG);

    H5Dread (mDataset, mMemoryType, H5S_ALL, H5S_ALL, H5P_DEFAULT, mExchangeStates);

    H5Dclose (mDataset);
    H5Sclose (mSpace);
    H5Tclose (mMemoryType);

    return mExchangeStates;
}
