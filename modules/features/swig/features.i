%module features
%{
    #define SWIG_FILE_WITH_INIT
    #include "ImgFeatures.h"
    #include "VecFeatures.h"
%}

%include <exception.i>
%import(module="rftk.asserts") "asserts.i"
%import(module="rftk.buffers") "buffers.i"

%include "ImgFeatures.h"
