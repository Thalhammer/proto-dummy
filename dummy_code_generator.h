#pragma once
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>

class DummyCodeGenerator : public google::protobuf::compiler::CodeGenerator {
	public:
		DummyCodeGenerator();
		virtual ~DummyCodeGenerator();
		virtual bool Generate(
			const google::protobuf::FileDescriptor* file,
			const std::string& parameter,
			google::protobuf::compiler::GeneratorContext* context,
			std::string* error) const;
};