#include "ast.hpp"
#include "core.hpp"
#include <fstream>
#include <sstream>

ASTNode::ASTNode()
{
    ast_id = GlobalUUIDGenerator::generate_uuid();
}


std::ostream& operator<<(std::ostream& os, const VariableType var_type)
{
    if (var_type == VariableType::SCALAR)
    {
        os << "SCALAR";
    }
    else
    {
        os << "TENSOR";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const DataType var_type)
{
    if (var_type == DataType::FLOAT32)
    {
        os << "FLOAT32";
    }
    else
    {
        os << "FLOAT16";
    }

    return os;
}


VariableNode::VariableNode(
    const VariableType vtype, const DataType dtype, const std::string& name
    ) : ASTNode(), vtype(vtype), dtype(dtype), name(name)
{
}


ScalarNode::ScalarNode(
    const VariableType vtype, const DataType dtype, const std::string& name
    ) : VariableNode(vtype, dtype, name)
{
}

void ScalarNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr ScalarNode::create_scalar_node(
    const VariableType vtype, 
    const DataType dtype,
    const std::string& name)
{
    return std::make_shared<ScalarNode>(vtype, dtype, name);   
}


TensorNode::TensorNode(
    const VariableType vtype, 
    const DataType dtype,
    const std::string& name,
    const std::vector<int>& shape
    ) : VariableNode(vtype, dtype, name), shape(shape)
{
}

void TensorNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr TensorNode::create_tensor_node(
    const VariableType vtype, 
    const DataType dtype,
    const std::string& name,
    const std::vector<int>& shape)
{
    return std::make_shared<TensorNode>(vtype, dtype, name, shape);   
}


std::ostream& operator<<(std::ostream& os, const KernelScope scope)
{
    if (scope == KernelScope::GLOBAL)
    {
        os << "GLOBAL";
    }
    else
    {
        os << "DEVICE";
    }

    return os;
}


KernelNode::KernelNode(
    const std::string& name,
    const KernelScope scope, 
    const std::vector<VariableNodePtr>& arguments,
    const VariableNodePtr return_value
    ) : ASTNode(), name(name), scope(scope), 
        arguments(arguments),
        return_value(return_value)
{
}

void KernelNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr KernelNode::create_kernel_node(
    const std::string& name,
    const KernelScope scope, 
    const std::vector<VariableNodePtr>& arguments,
    const VariableNodePtr return_value)
{
    return std::make_shared<KernelNode>(name, scope, arguments, return_value);
}


KernelCallNode::KernelCallNode(
    const KernelNodePtr kernel,
    const std::vector<VariableNodePtr>& arguments
    ) : ASTNode(), kernel(kernel), 
        arguments(arguments)
{
}

void KernelCallNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr KernelCallNode::create_kernelcall_node(
    const KernelNodePtr kernel, 
    const std::vector<VariableNodePtr>& arguments)
{
    return std::make_shared<KernelCallNode>(kernel, arguments);
}


BinaryNode::BinaryNode(const ASTNodePtr lhs, const ASTNodePtr rhs) : ASTNode(), lhs(lhs), rhs(rhs)
{
}


AddNode::AddNode(const ASTNodePtr lhs, const ASTNodePtr rhs) : BinaryNode(lhs, rhs)
{
}

void AddNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr AddNode::create_add_node(const ASTNodePtr lhs, const ASTNodePtr rhs)
{
    return std::make_shared<AddNode>(lhs, rhs);
}


SubNode::SubNode(const ASTNodePtr lhs, const ASTNodePtr rhs) : BinaryNode(lhs, rhs)
{
}

void SubNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr SubNode::create_sub_node(const ASTNodePtr lhs, const ASTNodePtr rhs)
{
    return std::make_shared<SubNode>(lhs, rhs);
}


MulNode::MulNode(const ASTNodePtr lhs, const ASTNodePtr rhs) : BinaryNode(lhs, rhs)
{
}

void MulNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr MulNode::create_mul_node(const ASTNodePtr lhs, const ASTNodePtr rhs)
{
    return std::make_shared<MulNode>(lhs, rhs);
}


DivNode::DivNode(const ASTNodePtr lhs, const ASTNodePtr rhs) : BinaryNode(lhs, rhs)
{
}

void DivNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr DivNode::create_div_node(const ASTNodePtr lhs, const ASTNodePtr rhs)
{
    return std::make_shared<DivNode>(lhs, rhs);
}


UnaryNode::UnaryNode(const ASTNodePtr x) : ASTNode(), x(x)
{
}


SqrtNode::SqrtNode(const ASTNodePtr x) : UnaryNode(x)
{
}

void SqrtNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr SqrtNode::create_sqrt_node(const ASTNodePtr x)
{
    return std::make_shared<SqrtNode>(x);
}


Log2Node::Log2Node(const ASTNodePtr x) : UnaryNode(x)
{
}

void Log2Node::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr Log2Node::create_log2_node(const ASTNodePtr x)
{
    return std::make_shared<Log2Node>(x);
}


Exp2Node::Exp2Node(const ASTNodePtr x) : UnaryNode(x)
{
}

void Exp2Node::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr Exp2Node::create_exp2_node(const ASTNodePtr x)
{
    return std::make_shared<Exp2Node>(x);
}


AssignmentNode::AssignmentNode(const ASTNodePtr trg, const ASTNodePtr src) : ASTNode(), trg(trg), src(src)
{
}

void AssignmentNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr AssignmentNode::create_assignment_node(const ASTNodePtr trg, const ASTNodePtr src)
{
    return std::make_shared<AssignmentNode>(trg, src);
}


AliasNode::AliasNode(const ASTNodePtr src) : ASTNode(), src(src)
{
}

void AliasNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr AliasNode::create_alias_node(const ASTNodePtr src)
{
    return std::make_shared<AliasNode>(src);
}


ReturnNode::ReturnNode(const ASTNodePtr return_value) : ASTNode(), return_value(return_value)
{
}

void ReturnNode::accept(ASTVisitor& visitor)
{
    visitor.apply(*this);
}

ASTNodePtr ReturnNode::create_node(const ASTNodePtr return_value)
{
    return std::make_shared<ReturnNode>(return_value);
}

// printer impl.

ASTPrinter::ASTPrinter()
{
}

void ASTPrinter::save_into_file(const std::string& out_path) const
{
    std::ofstream out(out_path);
    out << ast_as_string;
}

void ASTPrinter::reset()
{
    ast_as_string.clear();
}

void ASTPrinter::apply(KernelNode& node)
{
    std::stringstream ss;

    ss << "-- KernelNode \n";
    ss << "  id:    " << node.ast_id << "\n";
    ss << "  name:  " << node.name << "\n";
    ss << "  scope: " << node.scope << "\n";

    ss << "  args:  ";
    for (auto& arg_ast : node.arguments)
    {
        ss << arg_ast->ast_id << ", ";
    }
    ss << "\n";
    
    if (node.return_value)
    {
        ss << "  ret:   " << node.return_value->ast_id << "\n";
    }
    else
    {
        ss << "  ret:   " << "void" << "\n";
    }

    ss << "  body:  ";
    for (auto& body_ast : node.body)
    {
        ss << body_ast->ast_id << ", ";
    }
    ss << "\n";

    ast_as_string.append(ss.str());


    // recursive call to the ast nodes inside the kernel
    for (auto& arg_ast : node.arguments)
    {
        arg_ast->accept(*this);
    }

    node.return_value->accept(*this);

    for (auto& body_ast : node.body)
    {
        body_ast->accept(*this);
    }
}

void ASTPrinter::apply(KernelCallNode &node)
{
}

void ASTPrinter::apply(ScalarNode &node)
{
}

void ASTPrinter::apply(TensorNode &node)
{
}

void ASTPrinter::apply(AddNode &node)
{
}

void ASTPrinter::apply(SubNode &node)
{
}

void ASTPrinter::apply(MulNode &node)
{
}

void ASTPrinter::apply(DivNode &node)
{
}

void ASTPrinter::apply(SqrtNode &node)
{
}

void ASTPrinter::apply(Log2Node &node)
{
}

void ASTPrinter::apply(Exp2Node &node)
{
}

void ASTPrinter::apply(AssignmentNode &node)
{
}

void ASTPrinter::apply(AliasNode &node)
{
}

void ASTPrinter::apply(ReturnNode &node)
{
}
