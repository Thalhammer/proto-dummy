#include "dummy_code_generator.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/stl_util.h>
#include <google/protobuf/compiler/plugin.pb.h>

#include "options.pb.h"

DummyCodeGenerator::DummyCodeGenerator()
{}

DummyCodeGenerator::~DummyCodeGenerator() 
{}

bool DummyCodeGenerator::Generate(const google::protobuf::FileDescriptor* file, const std::string& parameter, google::protobuf::compiler::GeneratorContext* context, std::string* error)  const
{
	std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(context->Open(file->name() /* You can append a custom file extension here */));
	google::protobuf::io::Printer printer(output.get(), '$');

	std::stringstream ss;

	/* Getting protobuf compiler version */
	google::protobuf::compiler::Version ver;
	context->GetCompilerVersion(&ver);
	ss << "Generated by protoc-sql on protobuf " << ver.major() << "." << ver.minor() << "." << ver.patch()  << "-" << ver.suffix() << std::endl;
	ss << std::endl;
	
	for (int i = 0; i < file->message_type_count(); i++) 
	{
		const google::protobuf::Descriptor *m = file->message_type(i);
		
		ss << "Message " << m->name() << std::endl;
		ss << "\tFullName " << m->full_name() << std::endl;

		for (int f = 0; f < m->field_count(); f++)
		{
			auto fd = m->field(f);
			auto& opts = fd->options().GetExtension(dummy);
			ss << "\t\t" << fd->cpp_type_name() << " " << fd->camelcase_name() << std::endl;
		}
		ss << std::endl;
	}

	for (int i = 0; i < file->service_count(); i++)
	{
		auto service = file->service(i);
		ss << "Service " << service->full_name() << std::endl;
		ss << std::endl;
	}
	printer.PrintRaw(ss.str());
	if (printer.failed()) 
	{
		*error = "DummyCodeGenerator detected write error.";
		return false;
	}
	return true;
}

