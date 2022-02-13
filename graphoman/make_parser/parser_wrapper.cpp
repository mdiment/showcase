#include <ruby.h>
#include "rice/rice.hpp"
#include "rice/stl.hpp"

#include "parser/utils.cpp"
#include "parser/parser.hpp"
#include "parser/parser.cpp"

using namespace Rice;

extern "C"
void Init_parser_wrapper()
{
    Data_Type<ICalculatable> rb_cICalculatable = 
        define_class<ICalculatable>("ICalculatable")
            .define_method("calculate", &ICalculatable::calculate);

    Data_Type<Parser> rb_cParser =
        define_class<Parser>("Parser")
            .define_constructor(Constructor<Parser>())
            .define_method("make_f", &Parser::make_f);
}
