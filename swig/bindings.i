%module bindings
%{
void *__gxx_personality_v0;
#include "common/point.h"
#include "common/stroke.h"
#include "paleo/paleo.h"
%}

%include "common/point.h"
%include "common/stroke.h"
%include "paleo/paleo.h"
