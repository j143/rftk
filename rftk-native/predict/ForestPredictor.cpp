#include <cstdio>

#include "assert_util.h"
#include "MatrixBuffer.h"
#include "FeatureTypes.h"
#include "Forest.h"
#include "ForestPredictor.h"


ForestPredictor::ForestPredictor( const Forest& forest )
: mForest(forest)
{}

void ForestPredictor::PredictLeafs(BufferCollection& data,  const int numberOfindices, Int32MatrixBuffer& leafsOut)
{
    ForestPredictLeafs(mForest, data, numberOfindices, leafsOut);
}

void ForestPredictor::PredictYs(BufferCollection& data,  const int numberOfindices, Float32MatrixBuffer& ysOut)
{
    ForestPredictYs(mForest, data, numberOfindices, ysOut);
}

void ForestPredictLeafs(const Forest& forest, BufferCollection& data, const int numberOfindices, Int32MatrixBuffer& leafsOut)
{
    const int numberOfTreesInForest = forest.mTrees.size();
    // Create new results buffer if it's not the right dimensions
    if( leafsOut.GetM() != numberOfindices || leafsOut.GetN() != numberOfTreesInForest )
    {
        leafsOut = Int32MatrixBuffer(numberOfindices, numberOfTreesInForest);
    }

    for(int i=0; i<numberOfindices; i++)
    {
        for(int treeId=0; treeId<numberOfTreesInForest; treeId++)
        {
            int treeDepthOut = 0;
            int leafNodeId = walkTree(forest.mTrees[treeId], 0, data, i, treeDepthOut);
            leafsOut.Set(i, treeId, leafNodeId);
        }
    }
}

void ForestPredictYs(const Forest& forest, BufferCollection& data, const int numberOfindices, Float32MatrixBuffer& ysOut)
{
    // Create new results buffer if it's not the right dimensions
    const int numberOfTreesInForest = forest.mTrees.size();
    const int yDim = forest.mTrees[0].mYs.GetN();
    if( ysOut.GetM() != numberOfindices || ysOut.GetN() != yDim )
    {
        ysOut = Float32MatrixBuffer(numberOfindices, yDim);
    }
    // Reset predictions if the buffer is being reused
    ysOut.Zero();


    // Create a temp buffer for leaf node id (this requires all leaf node ids to be stored in memory)
    // If the number of indices (ie number of rows in x) is to large this might be an issue
    Int32MatrixBuffer leafNodeIds = Int32MatrixBuffer(numberOfindices, forest.mTrees.size());
    ForestPredictLeafs(forest, data, numberOfindices, leafNodeIds);

    float invNumberTrees = 1.0 / static_cast<float>(numberOfTreesInForest);

    for(int i=0; i<numberOfindices; i++)
    {
        for(int treeId=0; treeId<numberOfTreesInForest; treeId++)
        {
            int leafNodeId = leafNodeIds.Get(i, treeId);
            for(int c=0; c<yDim; c++)
            {
                const float delta = forest.mTrees[treeId].mYs.Get(leafNodeId, c) * invNumberTrees;
                const float updatedValue = ysOut.Get(i, c) + delta;
                ysOut.Set(i, c, updatedValue);
            }
        }
    }
}

int nextChild( const Tree& tree, int nodeId, BufferCollection& data, const int index );

int walkTree( const Tree& tree, int nodeId, BufferCollection& data, const int index, int& treeDepthOut )
{
    const int childNodeId = nextChild( tree, nodeId, data, index);
    if(childNodeId == -1)
    {
       return nodeId;
    }
    treeDepthOut++;
    return walkTree(tree, childNodeId, data, index, treeDepthOut);
}

int nextChild( const Tree& tree, int nodeId, BufferCollection& data, const int index )
{
    // First int param is which feature to use
    const int featureType = tree.mIntFeatureParams.Get(nodeId, 0);
    // First float param is the threshold
    const float threshold = tree.mFloatFeatureParams.Get(nodeId, 0);
    bool testResult = false;
    switch( featureType )
    {
        case VEC_FEATURE_AXIS_ALIGNED:
        {
            ASSERT( data.HasFloat32MatrixBuffer(X_FLOAT_DATA) )
            const Float32MatrixBuffer& xs = data.GetFloat32MatrixBuffer(X_FLOAT_DATA);
            int component = tree.mIntFeatureParams.Get(nodeId, 1);
            testResult = xs.Get(index, component) > threshold;
            break;
        }
        case VEC_FEATURE_PROJECTION:
        {
            ASSERT( data.HasFloat32MatrixBuffer(X_FLOAT_DATA) )
            const Float32MatrixBuffer& xs = data.GetFloat32MatrixBuffer(X_FLOAT_DATA);

            float projectionValue = 0.0f;
            const int numberOfComponentsInProjection = tree.mIntFeatureParams.Get(nodeId, 1);
            for(int p=2; p<numberOfComponentsInProjection+2; p++)
            {
                const int componentId = tree.mIntFeatureParams.Get(nodeId, p);
                const float componentProjection = tree.mFloatFeatureParams.Get(nodeId, p);
                projectionValue += xs.Get(index, componentId) * componentProjection;
            }
            testResult = projectionValue > threshold;
            break;

        }
    }
    const int childDirection = testResult ? 0 : 1;
    const int childNodeId = tree.mPath.Get(nodeId, childDirection);
    return childNodeId;
}

