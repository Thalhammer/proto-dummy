#include <google/protobuf/compiler/plugin.h>
#include "dummy_code_generator.h"

int main(int argc, char** argv)
{
	DummyCodeGenerator generator;
	return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}

