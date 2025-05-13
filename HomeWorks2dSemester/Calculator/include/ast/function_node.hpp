#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H

#include "expression_node.hpp"
#include <memory>
#include <string>

class function_node : public expression_node {
public:
    function_node(const std::string& func_name, std::shared_ptr<expression_node> arg);
    double evaluate() const override;

private:
    std::string func_name_;
    std::shared_ptr<expression_node> arg_;
};

#endif // FUNCTION_NODE_H