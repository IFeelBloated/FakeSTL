#pragma once
#include "Helpers.hpp"
#include "String.hpp"

template<typename T>
auto lexical_cast(const StandardTemplateLibrary::String &Object);

template<typename T>
auto lexical_cast(const StandardTemplateLibrary::WString &Object);

template<>
auto lexical_cast<long>(const StandardTemplateLibrary::String &Object) {
	return std::strtol(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstol(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long long>(const StandardTemplateLibrary::String &Object) {
	return std::strtoll(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long long>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstoll(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long>(const StandardTemplateLibrary::String &Object) {
	return std::strtoul(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstoul(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long long>(const StandardTemplateLibrary::String &Object) {
	return std::strtoull(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long long>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstoull(Object.Data(), nullptr, 0);
}

template<>
auto lexical_cast<float>(const StandardTemplateLibrary::String &Object) {
	return std::strtof(Object.Data(), nullptr);
}

template<>
auto lexical_cast<float>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstof(Object.Data(), nullptr);
}

template<>
auto lexical_cast<double>(const StandardTemplateLibrary::String &Object) {
	return std::strtod(Object.Data(), nullptr);
}

template<>
auto lexical_cast<double>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstod(Object.Data(), nullptr);
}

template<>
auto lexical_cast<long double>(const StandardTemplateLibrary::String &Object) {
	return std::strtold(Object.Data(), nullptr);
}

template<>
auto lexical_cast<long double>(const StandardTemplateLibrary::WString &Object) {
	return std::wcstold(Object.Data(), nullptr);
}