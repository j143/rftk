%module predict
%{
    #define SWIG_FILE_WITH_INIT
    #include "ForestPredictor.h"
%}

%include <exception.i>
%import(module="rftk.asserts") "asserts.i"
%import(module="rftk.buffers") "buffers.i"
%import(module="rftk.bootstrap") "bootstrap.i"
%import(module="rftk.forest_data") "forest_data.i"
%import(module="rftk.pipeline") "pipeline_external.i"
%import(module="rftk.splitpoints") "splitpoints_external.i"


%include <matrix_features.i>
%include <image_features.i>
%include <classification.i>
%include <regression.i>

%include "ForestPredictor.h"

%template(LinearMatrixClassificationPredictin_f32i32) TemplateForestPredictor< LinearMatrixFeature< DefaultBufferTypes, MatrixBufferTemplate<DefaultBufferTypes::SourceContinuous> >, ClassProbabilityCombiner<float>, DefaultBufferTypes>;
%template(LinearMatrixRegressionPredictin_f32i32) TemplateForestPredictor< LinearMatrixFeature< DefaultBufferTypes, MatrixBufferTemplate<DefaultBufferTypes::SourceContinuous> >, MeanVarianceCombiner<float>, DefaultBufferTypes>;
%template(ScaledDepthDeltaClassificationPredictin_f32i32) TemplateForestPredictor< ScaledDepthDeltaFeature< DefaultBufferTypes >, ClassProbabilityCombiner<float>, DefaultBufferTypes>;
%template(ScaledDepthDeltaRegressionPrediction_f32i32) TemplateForestPredictor< ScaledDepthDeltaFeature< DefaultBufferTypes >, MeanVarianceCombiner<float>, DefaultBufferTypes>;
