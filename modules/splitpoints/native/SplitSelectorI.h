#pragma once

#include "BufferCollection.h"
#include "SplitSelectorInfo.h"

template <class BufferTypes>
class SplitSelectorI
{
  public:
    virtual ~SplitSelectorI() {}
    virtual SplitSelectorInfo<BufferTypes> ProcessSplits(const BufferCollectionStack& bufferCollectionStack, int depth) const=0;
    virtual SplitSelectorI<BufferTypes>* Clone() const=0;
};