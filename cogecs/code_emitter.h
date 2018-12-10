#pragma once

#include <ostream>
#include "ast.h"
#include "astvisitor.h"
#include "jitcompiler.h"
#include "builtin.h"

struct Basicx86Emitter : public AstVisitor
{
	void visitPre(const BasicStatement*) {}
	void visitPre(const VarDecl*) {}
	void visitPre(const BasicExpression*) {}
	void visitPre(const Expression*) {}
	void visitPre(const IfStatement*) {}
	void visitPre(const WhileLoop*) {}
	void visitPre(const BlockStatement*) {}
	void visitPre(const LabelStatement*) {}
	void visitPre(const GotoStatement*) {}
	void visitPre(const FunctionCall*) {}
	void visitPre(const FunctionDecl*) {}
	void visitPre(const ReturnStatement*) {}
	void visitPost(const BasicStatement*) {}
	void visitPost(const VarDecl*) {}
	void visitPost(const BasicExpression*) {}
	void visitPost(const Expression* expr) 
	{
		for (const auto& elems : expr->getChilds()) 
		{
			elems->traverse(*this);
		}
	}
	void visitPost(const IfStatement*) {}
	void visitPost(const WhileLoop*) {}
	void visitPost(const BlockStatement*) {}
	void visitPost(const LabelStatement*) {}
	void visitPost(const GotoStatement*) {}
	void visitPost(const FunctionCall* fcall) 
	{
		i_vector.push_function_prolog();
		
		// push params
		for (const auto& param : fcall->parameters) 
		{
			i_vector.push_back({ std::byte(0x68) }); // push
			i_vector.push_back(i_vector.int_to_bytes(std::stoi(param)));
		}

		i_vector.push_back({ std::byte(0xB8) });  // \  mov eax, address of function
		i_vector.push_back(i_vector.get_address(reinterpret_cast<void*>(&builtin_print)));
		i_vector.push_back({ std::byte(0xFF), std::byte(0xD0) }); // call eax
		i_vector.push_back({ std::byte(0x83), std::byte(0xC4), std::byte(0x04) }); // add esp, 4 (clean stack)

		i_vector.push_function_epilog();
	}
	void visitPost(const ReturnStatement*) {}
	void visitPost(const FunctionDecl*) {}

	StatementList getStatements() const { return statements; }
	X86InstrVector getCode() const { return i_vector; }
private:
	StatementList statements;
	X86InstrVector i_vector;

};

auto emitMachineCode(const StatementList& statements)
{
	Basicx86Emitter visitor;

	traverse(statements, visitor);
	
	auto i_vector = visitor.getCode();
	JitCompiler jit(i_vector);
	return jit.compile();
}

