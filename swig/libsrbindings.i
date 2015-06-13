%module libsrbindings
%{
void *__gxx_personality_v0;
#include "common/point.h"
#include "common/stroke.h"
%}

%include "common/point.h"
%include "common/stroke.h"
