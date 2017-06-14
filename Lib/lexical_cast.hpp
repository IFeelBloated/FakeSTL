#pragma once
#include "Helpers.hpp"
#include "String.hpp"

template<typename T>
auto lexical_cast(const StandardTemplateLibrary::String &SomeString);

template<typename T>
auto lexical_cast(const StandardTemplateLibrary::WString &SomeString);

template<>
auto lexical_cast<long>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtol(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstol(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long long>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtoll(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<long long>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstoll(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtoul(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstoul(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long long>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtoull(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<unsigned long long>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstoull(SomeString.Data(), nullptr, 0);
}

template<>
auto lexical_cast<float>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtof(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<float>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstof(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<double>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtod(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<double>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstod(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<long double>(const StandardTemplateLibrary::String &SomeString) {
	return std::strtold(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<long double>(const StandardTemplateLibrary::WString &SomeString) {
	return std::wcstold(SomeString.Data(), nullptr);
}

template<>
auto lexical_cast<int>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<int>(Value);
}

template<>
auto lexical_cast<int>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<int>(Value);
}

template<>
auto lexical_cast<short>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<short>(Value);
}

template<>
auto lexical_cast<short>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<short>(Value);
}

template<>
auto lexical_cast<signed char>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<signed char>(Value);
}

template<>
auto lexical_cast<signed char>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<long>(SomeString);
	return static_cast<signed char>(Value);
}

template<>
auto lexical_cast<unsigned int>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned int>(Value);
}

template<>
auto lexical_cast<unsigned int>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned int>(Value);
}

template<>
auto lexical_cast<unsigned short>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned short>(Value);
}

template<>
auto lexical_cast<unsigned short>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned short>(Value);
}

template<>
auto lexical_cast<unsigned char>(const StandardTemplateLibrary::String &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned char>(Value);
}

template<>
auto lexical_cast<unsigned char>(const StandardTemplateLibrary::WString &SomeString) {
	auto Value = lexical_cast<unsigned long>(SomeString);
	return static_cast<unsigned char>(Value);
}