#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream> 
#include <cctype>

template<typename Container>
auto split(std::string statement, Container tokens) -> std::vector<std::string>{
    auto result =  std::vector<std::string>{};
    auto start = statement.begin();
    auto const cmp = [&](auto const& c){
                    return tokens.contains(c);
                };
    auto current = std::find_if(start, statement.end(),cmp);
    while(current != statement.end()){
        if(start != current){
            result.emplace_back(start, current);
        }
        start = std::next(current);
        current =  std::find_if(start, statement.end(),cmp);
    }

    if(start != current){
        result.emplace_back(start, current);
    }
    return result;
}
struct type_pair{
    std::string type;
    std::string name;
};
struct line{
    std::string m_class;
    std::vector<type_pair> m_types;
};
std::vector<line> generate(std::vector<std::string> input){
    std::vector<line> result(input.size());
    std::transform(input.begin(), input.end(), result.begin(), [](std::string val){
         auto types = std::vector<type_pair>{};
         auto it = std::find(val.begin(), val.end(), ' ');
         auto head = std::string{val.begin(), it};
         it = std::find(it, val.end(), ':');
         val = std::string{it, val.end()};
         auto arr = split(val, std::string(" ,"));
         for(int i = 1; i +1 < arr.size(); i+= 2){
             types.push_back({arr[i], arr[i+1]});
         }
         return line{head, types};
    });
    return result;
}

auto print_class(std::ostream& os, line l){
    os << "    struct " << l.m_class << " {\n";
    os << "        "<< l.m_class << "(";
    for(int i = 0; i < l.m_types.size() -1; i++){
        auto [type, name] = l.m_types[i];
        os << type << " t_" << name << ", ";
    }
    auto [b_type, b_name] = l.m_types.back();
    os << b_type << " t_" << b_name << ");\n";
    for(auto [type, name] : l.m_types){
        os << "        " << type << " m_"<< name << ";\n";
    }
    os << "    };\n\n";
}
auto print_variant_declaration(std::string const& base_name , std::ostream& filehpp,std::vector<line> lines) -> void{
    filehpp << "    using " <<  base_name << " = std::variant<std::monostate\n";
    for(int i = 0; i < lines.size() -1 ; i++){
        filehpp << "                             ,Box<" << lines[i].m_class << ">\n";
    } 

    filehpp << "                             ,Box<"  << lines.back().m_class << ">>;\n";
}

auto print_class_constructor(std::ostream& os, line l) -> void{
    auto header = std::string("    ");
    os << header << l.m_class << "::" << l.m_class << "(";
    for(int i = 0; i < l.m_types.size() -1; i++){
        auto [type, name] = l.m_types[i];
        os << type << " t_" << name << ", ";
    }
    auto [b_type, b_name] = l.m_types.back();
    os << b_type << " t_" << b_name << ")\n";
    if(l.m_types.size() == 1){
        os << header << ":m_" <<b_name<< "{t_" << b_name<< "} {}\n\n";
        return;
    }
    auto [f_type, f_name] = l.m_types[0];
    os << header << ":m_" <<f_name << "{t_" << f_name << "}\n";
    
    for(int i = 1; i < l.m_types.size() -1 ; i++){
        auto [type, name] = l.m_types[i];
        os << header << ",m_"<< name << "{t_" << name<< "}\n";
    }

     os << header << ",m_" <<b_name<< "{t_" << b_name<< "} {}\n\n";
}
auto uppercase(std::string word) -> std::string{
    std::transform(word.begin(), word.end(), word.begin(), toupper);
    return word;
}

auto lowercase(std::string word) -> std::string{
    std::transform(word.begin(), word.end(), word.begin(), tolower);
    return word;
}

auto generate_files(std::string const& output_dir, std::string const& base_name, std::vector<std::string> str_input) -> void{
    auto filehpp = std::ofstream{output_dir + "/" + lowercase(base_name) + ".hpp"};
    std::vector<line> lines = generate(str_input);

    filehpp <<  "#ifndef LOX_" + uppercase(base_name) + "_HPP\n"
                "#define LOX_" + uppercase(base_name) + "_HPP\n"
                "#include \"../token.hpp\"\n"
                "#include \"box.hpp\"\n"
                "\n"
                "#include <memory>\n"
                "#include <variant>\n"
                "\n"
                "namespace lox {\n";

    for(auto [m_class, types]: lines){
        filehpp << "    class "<< m_class << ";\n";
    }
    print_variant_declaration(base_name, filehpp, lines);

    std::for_each(lines.begin(), lines.end(), [&](auto const& l){
        print_class(filehpp, l);
    });

    filehpp <<  "};\n"
                "#endif";
    
    auto filecpp = std::ofstream{output_dir + "/" + lowercase(base_name) + ".cpp"};
    filecpp << "#include \"" + lowercase(base_name) + ".hpp\"\n\n";
    filecpp << "namespace lox{\n";
    
    std::for_each(lines.begin(), lines.end(), [&](auto const& l){
        print_class_constructor(filecpp, l);
    });

    filecpp << "};";
}

auto main(int argc, char** argv) -> int {
    auto const output_dir = std::string(argv[1]);
    generate_files(output_dir, "Expr",std::vector<std::string>{
        "Binary : Expr left, Token operator, Expr right",
        "Grouping : Expr expression",
        "Literal : Object value",
        "Unary : Token operator, Expr right"
    });
}