#pragma once

#include "tools.h"
#include "ast.h"

TEST(codegen, test1)
{
	testProgram<AstCloner>("var a;",
	{
		makeNode(VarDecl(0, "a"))
	});
}

TEST(codegen, test2)
{
	testProgram<AstCloner>("a = 1;",
	{
		makeNode(Expression(0,{ 
			makeNode(BasicExpression(0,"a")), 
			makeNode(BasicExpression(0, "=")),
			makeNode(BasicExpression(0, "1"))}))
	});
}

TEST(codegen, test3)
{
	testProgram<AstCloner>("a = 1; a = a - 1;", 
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"a")), makeNode(BasicExpression(0,"=")), makeNode(BasicExpression(0, "1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"a")), 
								makeNode(BasicExpression(0,"=")), 
								makeNode(BasicExpression(0,"a")), 
								makeNode(BasicExpression(0,"-")),
								makeNode(BasicExpression(0,"1")) }))
	});
}

TEST(codegen, test4)
{
	
	testProgram<AstCloner>("{}",
	{
		makeNode(BlockStatement(0))
	});
}

TEST(codegen, test5)
{
		
	testProgram<AstCloner>("if(1) {}",
	{
		makeNode(IfStatement(0, 
                             Expression(0,{ makeNode(BasicExpression(0,"1"))}),
                             { makeNode(BlockStatement(0)) }))
	});

}

TEST(codegen, test6)
{

	testProgram<AstCloner>("var a; if(1) {} var b;",
	{
		makeNode(VarDecl(0, "a")),
		makeNode(IfStatement(0,
				 Expression(0,{ makeNode(BasicExpression(0,"1"))}),
				 { makeNode(BlockStatement(0)) })),
				 makeNode(VarDecl(0, "b"))
	});

}

TEST(codegen, test7)
{
	auto ast = {
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(VarDecl(0, "temp__1")),
		makeNode(Expression(0,
		{ makeNode(BasicExpression(0, "temp__1")),
		makeNode(BasicExpression(0, "=")),
		makeNode(BasicExpression(0,"1")) })),
		makeNode(IfStatement(0,
		Expression(0,{ makeNode(BasicExpression(0, "!")),makeNode(BasicExpression(0, "temp__1")) }),
		{ makeNode(GotoStatement(0, "label__2")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
		makeNode(LabelStatement(0, "label__2")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	};
	testProgram<CFGFlattener>("if(1) {}", ast);
}

TEST(codegen, test8)
{
	testProgram<CFGFlattener>("while(1) {}",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(VarDecl(0, "temp__1")),
		makeNode(LabelStatement(0, "label__2")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"temp__1")), makeNode(BasicExpression(0, "=")), makeNode(BasicExpression(0, "1"))})),
		makeNode(IfStatement(0,
		Expression(0,{ makeNode(BasicExpression(0,"!")), makeNode(BasicExpression(0, "temp__1")) }),
		{ makeNode(GotoStatement(0, "label__3")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
		makeNode(GotoStatement(0, "label__2")),
		makeNode(LabelStatement(0, "label__3")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}

TEST(codegen, test9)
{
	testProgram<CFGFlattener>("{1;}",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}

TEST(codegen, test10)
{
	testProgram<CFGFlattener>("{{1;}}",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}

TEST(codegen, test11)
{
	testProgram<CFGFlattener>("{1;{2;}}",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "2")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}

TEST(codegen, test12)
{
	testProgram<CFGFlattener>("{label_0:{2;}}",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(LabelStatement(0,{ "label_0" })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "2")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}

TEST(codegen, test13)
{	
	testProgram<CFGFlattener>("if (1) {2; if(3) {4;} 5; } 6;",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(VarDecl(0, "temp__3")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "temp__3")), makeNode(BasicExpression(0, "=")), makeNode(BasicExpression(0,"1")) })),
		makeNode(IfStatement(0,
				Expression(0,{ makeNode(BasicExpression(0, "!")), makeNode(BasicExpression(0,"temp__3")) }),
				{ makeNode(GotoStatement(0, "label__4")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "2")) })),
		makeNode(VarDecl(0, "temp__1")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"temp__1")), makeNode(BasicExpression(0,"=")), makeNode(BasicExpression(0,"3")) })),
		makeNode(IfStatement(0,
				Expression(0,{ makeNode(BasicExpression(0, "!")), makeNode(BasicExpression(0, "temp__1")) }),
				{ makeNode(GotoStatement(0, "label__2")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "4")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
		makeNode(LabelStatement(0, "label__2")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "5")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
		makeNode(LabelStatement(0, "label__4")),
		makeNode(Expression(0,{ makeNode(BasicExpression(0, "6")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),

	});
}

TEST(codegen, test14)
{
	testProgram<CFGFlattener>("1+1;1-1;1*1;1/1;!1;1==1;1!=1;1<2;1<=1;2>1;",
	{
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__alloc__")) })),		
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"+")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"-")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"*")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"/")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"!")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"==")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"!=")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"<")), makeNode(BasicExpression(0,"2")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"1")), makeNode(BasicExpression(0,"<=")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"2")), makeNode(BasicExpression(0,">")), makeNode(BasicExpression(0,"1")) })),
		makeNode(Expression(0,{ makeNode(BasicExpression(0,"__dealloc__")) })),
	});
}
