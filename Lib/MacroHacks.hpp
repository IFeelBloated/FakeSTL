#pragma once
#include "Helpers.hpp"
#include "TypeTraits.hpp"

#define self(ClassMember, Initialization) StandardTemplateLibrary::RemoveCV_t<StandardTemplateLibrary::RemoveReference_t<decltype(Initialization)>> ClassMember = Initialization
#define Begin begin
#define End end