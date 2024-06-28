#include <variant>
#include <iostream>
#include <chrono>

#include "interpreter.hpp"
#include "lox_callable.hpp"
#include "token.hpp"
#include "error.hpp"
#include "stmt.hpp"


namespace lox{

    auto to_string_object(Object o) -> std::string{
        if(std::holds_alternative<bool>(o)){
            return std::get<bool>(o) ? "true" : "false";
        }
        if(std::holds_alternative<std::string>(o)){
            return std::get<std::string>(o);
        }

        if(std::holds_alternative<double>(o)){
            auto d = std::get<double>(o);
            if(d == static_cast<int>(d)){
                return std::to_string(static_cast<int>(d));
            }
            return std::to_string(d);
        }

        if(std::holds_alternative<Lox_Callable>(o)){
            auto function = std::get<Lox_Callable>(o);
            if(std::holds_alternative<Lox_Function>(function)){
                 auto lox_function = std::get<Lox_Function>(function);
                 auto declaration = std::any_cast<Box<Function>>(lox_function.m_declaration);
                 return "<Fn " + declaration->m_name.get_lexume() + ">";
            }

            if(std::holds_alternative<Func>(function)){
                auto func = std::get<Func>(function);
                return "<Fn " + func.name + ">";
            }

        }

        return "nil";
    }

    auto Interpreter::interpret(std::vector<Stmt> statements) -> void{
        try{
            for(auto statement: statements){
                execute(statement);
            }
        }catch(Runtime_Error& err){
            runtime_error(err);
        }
    }

    auto Interpreter::execute(Stmt stmt) -> void{
         std::visit(*this,stmt);
    }

    auto Interpreter::evaluate(Expr expr) -> Object{
         return std::visit(*this,expr);
    }
    auto Interpreter::operator()(Expr_Monostate /*unused*/) -> Object{
          return {};
    }

    auto Interpreter::operator()(Stmt_Monostate /*unused*/) -> void {}

    auto is_equal(Object const& left, Object const& right){
        auto both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);
        auto both_strings = std::holds_alternative<std::string>(left) and std::holds_alternative<std::string>(right);
        auto both_bools = std::holds_alternative<bool>(left) and std::holds_alternative<bool>(right);
        auto both_nil = std::holds_alternative<std::monostate>(left) and std::holds_alternative<std::monostate>(right);
        if(both_numbers){
            return std::get<double>(left) ==  std::get<double>(right);
        }
        if(both_strings){
            return std::get<std::string>(left) ==  std::get<std::string>(right);
        }
        if(both_bools){
            return std::get<bool>(left) ==  std::get<bool>(right);
        }
        return both_nil;
    }

    auto Interpreter::operator()(Box<Binary>& expr) -> Object{
        auto left = evaluate(expr->m_left);
        auto right = evaluate(expr->m_right);
        using enum Token_Type;
        auto both_numbers = false;
        auto both_strings = false;
        switch(expr->m_operator.get_token_type()){
            case MINUS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) - std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case PLUS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            both_strings = std::holds_alternative<std::string>(left) and std::holds_alternative<std::string>(right);

            if(both_numbers){
                return std::get<double>(left) + std::get<double>(right);
            }

            if(both_strings){
                return std::get<std::string>(left) + std::get<std::string>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers or strings"};

            case EQUAL_EQUAL:
            return is_equal(left, right);

            case BANG_EQUAL:
            return not is_equal(left, right);

            case GREATER:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) > std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case GREATER_EQUAL:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) >= std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
            
            case LESS:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) < std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
    
            case LESS_EQUAL:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) <= std::get<double>(right);
            }
        
            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            case STAR:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);

            if(both_numbers){
                return std::get<double>(left) * std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };
            case SLASH:
            both_numbers = std::holds_alternative<double>(left) and std::holds_alternative<double>(right);
            if(both_numbers){
                return std::get<double>(left) / std::get<double>(right);
            }

            throw Runtime_Error{expr->m_operator, "operands have to be numbers" };

            default:
            return {};
        }
        return {};
    }

    auto Interpreter::operator()(Box<Grouping>& expr) -> Object{
        return evaluate(expr->m_expression);
    }

    auto Interpreter::operator()(Box<Literal>& expr) -> Object{
        return expr->m_value;
    }
   
    auto truthy(Object o){
        if(std::holds_alternative<std::monostate>(o)){
            return false;
        }
        if(std::holds_alternative<bool>(o)){
            return std::get<bool>(o);
        }
        return true;
    }

    auto Interpreter::operator()(Box<Var>& stmt) -> void {
        Object value = evaluate(stmt->m_initializer);
        enviroment->define(stmt->m_name.get_lexume(), value);
    };

    auto Interpreter::operator()(Box<Variable>& expr) -> Object {
        return enviroment->get(expr->m_name);
    };

    auto Interpreter::operator()(Box<Unary>& expr) -> Object{
        auto value = evaluate(expr->m_right);
        switch(expr->m_operator.get_token_type()){
            case Token_Type::MINUS:
                if(not std::holds_alternative<double>(value)){
                    throw Runtime_Error{expr->m_operator, "operand has to be a number" };
                }
                return std::get<double>(value) * -1;
            case Token_Type::BANG:
                return not truthy(value);       
            default:
                throw Runtime_Error{expr->m_operator, "Parser Error" };
        }
    }


    auto Interpreter::operator() (Box<Print>& stmt) -> void{
        Object value = evaluate(stmt->m_expression);
        std::cout << to_string_object(value) << '\n';
    }

    auto Interpreter::operator() (Box<Expression>& stmt) -> void{
        evaluate(stmt->m_expression);
    }

    auto Interpreter::operator()(Box<Assign>& expr) -> Object{
        auto value = evaluate(expr->m_value);
        enviroment->assign(expr->m_name, value);
        return value;
    }

     auto Interpreter::execute_block(std::vector<Stmt> statements, std::shared_ptr<Enviroment> t_enviroment) -> void{
        t_enviroment->get_parent() = enviroment;
        enviroment = t_enviroment;
        try{
            for(auto statement: statements){
                execute(statement);
            }
        }catch (Runtime_Error& err){
            enviroment = enviroment->get_parent();
            throw err;
        }catch(Object& o){
            enviroment = enviroment->get_parent();
            throw o;
        }
        enviroment = enviroment->get_parent();
     }

    auto Interpreter::operator()(Box<Block>& stmt) -> void{
        execute_block(stmt->m_statements,  std::make_shared<Enviroment>());
    }

     auto Interpreter::operator()(Box<If>& stmt) -> void{
        if(truthy(evaluate(stmt->m_condition))){
            execute(stmt->m_thenBranch);
        }else{
            execute(stmt->m_elseBranch);
        }
     }

    auto Interpreter::operator()(Box<Logical>& expr) -> Object{
        auto left = evaluate(expr->m_left);
        if(expr->m_operator.get_token_type()== Token_Type::AND){
            if(!truthy(left)) return left;
        }else {
            if(truthy(left)) return left;
        }
        return evaluate(expr->m_right);
    }

    auto Interpreter::operator()(Box<While>& stmt) -> void{
        while(truthy(evaluate(stmt->m_condition))){
            execute(stmt->m_body);
        }
    }


    auto Interpreter::operator()(Box<Call>& expr) -> Object{
        Object callee = evaluate(expr->m_callee);

        std::vector<Object> arguments;
        for(Expr argument: expr->m_arguments){
            arguments.push_back(evaluate(argument));
        }

        if(not std::holds_alternative<Lox_Callable>(callee)){
            throw Runtime_Error{expr->m_paren, "Can only call functions and classes."};
        }
        auto function = std::get<Lox_Callable>(callee);
        if(auto arity = Arity{}.arity(function); arity !=  arguments.size()){
            throw Runtime_Error(expr->m_paren, "Expected " +  std::to_string(arity) + " arguments but got " + std::to_string(arguments.size()));
        }
        return Caller{*this, arguments}.call(function);
    }

    auto Interpreter::operator()(Box<Function>& stmt) -> void{
        auto function = Lox_Function{stmt, enviroment};
        enviroment->define(stmt->m_name.get_lexume(), function);
    }

    auto Interpreter::operator()(Box<Return>& stmt) -> void{
        throw evaluate(stmt->m_expression);
    }

    Interpreter::Interpreter()
    :enviroment{std::make_shared<Enviroment>()}
    ,globals{enviroment} {
        globals->define("clock", 
        Func{
        [](){
            return 0;
        }
        ,[](auto& interpreter, auto& arguments){
            using namespace std::chrono;
            return static_cast<double>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count());
        }
        ,"clock"
        });
    }

    auto Interpreter::get_enviroment() -> std::shared_ptr<Enviroment>&{
        return enviroment;
    }

    auto Interpreter::get_globals() -> std::shared_ptr<Enviroment>&{
        return globals;
    }
}