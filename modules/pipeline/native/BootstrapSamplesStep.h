#pragma once

#include <asserts.h>
#include <bootstrap.h>

#include "BufferCollection.h"
#include "PipelineStepI.h"
#include "UniqueBufferId.h"

// ----------------------------------------------------------------------------
//
// Bootstrap data (sample datapoints without replacement) to determine weights
// and indices of datapoints to include
//
// ----------------------------------------------------------------------------
template <class BufferTypes, class DataMatrixType>
class BootstrapSamplesStep: public PipelineStepI
{
public:
    BootstrapSamplesStep( const BufferId& dataBuffer );
    virtual ~BootstrapSamplesStep();

    virtual PipelineStepI* Clone() const;

    virtual void ProcessStep(   const BufferCollectionStack& readCollection,
                                BufferCollection& writeCollection,
                                boost::mt19937& gen,
                                BufferCollection& extraInfo, int nodeIndex) const;

    // Read only output buffer
    const BufferId IndicesBufferId;
    const BufferId WeightsBufferId;
private:
    const BufferId mDataBufferId;

};


template <class BufferTypes, class DataMatrixType>
BootstrapSamplesStep<BufferTypes, DataMatrixType>::BootstrapSamplesStep(const BufferId& dataBuffer)
: PipelineStepI("BootstrapSamplesStep")
, IndicesBufferId(GetBufferId("IndicesBuffer"))
, WeightsBufferId(GetBufferId("WeightsBuffer"))
, mDataBufferId(dataBuffer)
{}

template <class BufferTypes, class DataMatrixType>
BootstrapSamplesStep<BufferTypes, DataMatrixType>::~BootstrapSamplesStep()
{}

template <class BufferTypes, class DataMatrixType>
PipelineStepI* BootstrapSamplesStep<BufferTypes, DataMatrixType>::Clone() const
{
    BootstrapSamplesStep<BufferTypes, DataMatrixType>* clone = new BootstrapSamplesStep<BufferTypes, DataMatrixType>(*this);
    return clone;
}

template <class BufferTypes, class DataMatrixType>
void BootstrapSamplesStep<BufferTypes, DataMatrixType>::ProcessStep(const BufferCollectionStack& readCollection,
                                                                BufferCollection& writeCollection,
                                                                boost::mt19937& gen,
                                                                BufferCollection& extraInfo, int nodeIndex) const
{
    UNUSED_PARAM(gen);
    UNUSED_PARAM(extraInfo);
    UNUSED_PARAM(nodeIndex);

    const DataMatrixType & buffer
          = readCollection.GetBuffer< DataMatrixType >(mDataBufferId);
    VectorBufferTemplate<typename BufferTypes::Index>& indices
          = writeCollection.GetOrAddBuffer< VectorBufferTemplate<typename BufferTypes::Index> >(IndicesBufferId);
    VectorBufferTemplate<typename BufferTypes::ParamsContinuous>& weights
          = writeCollection.GetOrAddBuffer< VectorBufferTemplate<typename BufferTypes::ParamsContinuous> >(WeightsBufferId);

    const typename BufferTypes::Index numberOfSamples = buffer.GetM();
    weights.Resize(numberOfSamples);
    weights.Zero();

    // This implementation could be much simpler if bootstrap.h was cleaned up
    std::vector<int> counts(numberOfSamples);
    sampleWithReplacement( &counts[0],
                          numberOfSamples,
                          numberOfSamples);

    std::vector<typename BufferTypes::Index> sampledIndices;
    std::vector<typename BufferTypes::Index> oobIndices;
    for(unsigned int i=0; i<counts.size(); i++)
    {
        if( counts[i] > 0 )
        {
            weights.Set(i,static_cast<float>(counts[i]));
            sampledIndices.push_back(i);
        }
        else
        {
            oobIndices.push_back(i);
        }
    }
    indices = VectorBufferTemplate<typename BufferTypes::Index>(&sampledIndices[0], sampledIndices.size());
    
    extraInfo.AddBuffer< VectorBufferTemplate<typename BufferTypes::Index> >(OOB_INDICES,
                         VectorBufferTemplate<typename BufferTypes::Index>(&oobIndices[0], oobIndices.size()));

}